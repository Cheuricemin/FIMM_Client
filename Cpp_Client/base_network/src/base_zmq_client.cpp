#include "base_zmq_client.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TZlibTransport.h>
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

/*使用zlib实现gzip格式数据的压缩和解压,http://blog.csdn.net/chary8088/article/details/48047835*/
/* Uncompress gzip data */
/* zdata 数据 nzdata 原数据长度 data 解压后数据 ndata 解压后长度 */
/*Bytef 就是 unsigned char，uLong就是 unsigned long，这些别名的设置都在zconf.h文件里面*/

int base_zmq_client::gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata)
{
	int err = 0;
	z_stream d_stream = { 0 }; /* decompression stream */
	static char dummy_head[2] = 
	{
		0x8 + 0x7 * 0x10,
		(((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
	};
	d_stream.zalloc = NULL;
	d_stream.zfree = NULL;
	d_stream.opaque = NULL;
	d_stream.next_in = zdata;
	d_stream.avail_in = 0;
	d_stream.next_out = data;
	//只有设置为MAX_WBITS + 16才能在解压带header和trailer的文本
	if (inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) 
		return -1;

	while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) 
	{
		d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
		if ((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) 
			break;
		if (err != Z_OK) 
		{
			if (err == Z_DATA_ERROR)
			{
				d_stream.next_in = (Bytef*)dummy_head;
				d_stream.avail_in = sizeof(dummy_head);
				if ((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
				{
					return -1;
				}
			}
			else return -1;
		}
	}
	if (inflateEnd(&d_stream) != Z_OK) 
		return -1;
	*ndata = d_stream.total_out;
	return 0;
}

int base_zmq_client::gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata)
{
	z_stream c_stream;
	int err = 0;

	if (data && ndata > 0)
	{
		c_stream.zalloc = NULL;
		c_stream.zfree = NULL;
		c_stream.opaque = NULL;
		//只有设置为MAX_WBITS + 16才能在在压缩文本中带header和trailer
		if (deflateInit2(&c_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
			MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY) != Z_OK)
			return -1;
		c_stream.next_in = data;
		c_stream.avail_in = ndata;
		c_stream.next_out = zdata;
		c_stream.avail_out = *nzdata;

		while (c_stream.avail_in != 0 && c_stream.total_out < *nzdata)
		{
			if (deflate(&c_stream, Z_NO_FLUSH) != Z_OK)
				return -1;
		}

		if (c_stream.avail_in != 0)
			return c_stream.avail_in;

		for (;;)
		{
			if ((err = deflate(&c_stream, Z_FINISH)) == Z_STREAM_END)
				break;
			if (err != Z_OK)
				return -1;
		}
		if (deflateEnd(&c_stream) != Z_OK)
			return -1;
		*nzdata = c_stream.total_out;
		return 0;
	}
	return -1;
}

void base_zmq_client::init(std::string ip, int port)
{
	is_init = true;
	
	srv_ip = ip;
	srv_port = port;
}

void base_zmq_client::rcv()
{
	char *strr = (char *)malloc(32 * 1024);
	while (1)
	{
		zmq_msg_t reply;
		zmq_msg_init(&reply);
		zmq_msg_recv(&reply, requester, 0);

		void *ptr = zmq_msg_data(&reply);

		int size = zmq_msg_size(&reply);
		if (size > 0)
		{
			int more;
			size_t more_size = sizeof(more);
			zmq_getsockopt(requester, ZMQ_RCVMORE, &more, &more_size);
			if (more)
			{
				last_msg_type.resize(size+1);
				memcpy((void *)last_msg_type.data(), zmq_msg_data(&reply), size);
				last_msg_type[size] = '\0';

				std::cout << last_msg_type << std::endl;
			}
			else
			{
				char *zmq_buf = (char *)malloc(size + 1);
				memcpy(zmq_buf, zmq_msg_data(&reply), size);
				zmq_buf[size] = '\0';
				uLong outlen = 32 * 1024 - 1;
				int res = gzdecompress((Byte*)zmq_buf, size, (Byte*)strr, &outlen);

				if (res != 0)
				{
					std::cout << "gzdecompress fail" << std::endl;
					free(zmq_buf);
					continue;
				}
				strr[outlen] = 0;

				process_thrift_msgc(strr,outlen);
				
			}
			zmq_msg_close(&reply);
		}

	}

	free(strr);
}

void base_zmq_client::process_thrift_msgc(char *strr, uLong outlen)
{
	if (strcmp(last_msg_type.data(), "FeedSource") == 0)
	{
		AtsGeneral::FeedSourceMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol>compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		m_FeedSource[msg.Name] =msg ;

	}
	else if (strcmp(last_msg_type.data(), "Connection") == 0)
	{
		AtsGeneral::ConnectionMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol> compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		m_ConnectionMsg[msg.Name]=(msg);
	}
	else if (strcmp(last_msg_type.data(), "Logs") == 0)
	{
		AtsGeneral::LogMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol> compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_LogMsg.size() > 10000)
		{
			m_LogMsg.clear();
		}
		m_LogMsg.push_back(msg);
	}
	else if (strcmp(last_msg_type.data(), "Orders") == 0)
	{
		AtsGeneral::OrderMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol> compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_OrderMsg.size() > 10000)
		{
			m_OrderMsg.clear();
		}
		m_OrderMsg.push_back(msg);
	}
	else if (strcmp(last_msg_type.data(), "Execs") == 0)
	{
		AtsGeneral::ExecMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol> compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_ExecMsg.size() > 10000)
		{
			m_ExecMsg.clear();
		}
		m_ExecMsg.push_back(msg);
	}
	else
		process_derived_msgc(strr, outlen);
}

bool base_zmq_client::connect()
{
	try
	{
		if (is_init == false)
			return false;

		int ret;
		context = zmq_ctx_new();
		requester = zmq_socket(context, ZMQ_SUB);

		std::string srv_str = "tcp://" + srv_ip + ":" + boost::lexical_cast<std::string>(srv_port);

		ret = zmq_connect(requester, srv_str.data());

		ret = zmq_setsockopt(requester, ZMQ_SUBSCRIBE, "", 0);

		std::thread t1(std::bind(&base_zmq_client::rcv, this));
		m_thread.swap(t1);
		return true;
	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return false;
}



