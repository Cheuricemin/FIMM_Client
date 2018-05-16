#include "sim_zmq_client.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TZlibTransport.h>

using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

void sim_zmq_client::process_derived_msgc(char *strr, uLong outlen)
{
	if (strcmp(last_msg_type.data(), "IAATS") == 0)
	{
		SimpleMsg::SimpleAtsMsg msg;
		boost::shared_ptr<TMemoryBuffer> membuffer(new TMemoryBuffer());
		boost::shared_ptr<TCompactProtocol>compact_proto(new TCompactProtocol(membuffer));
		membuffer->resetBuffer((uint8_t *)strr, outlen);
		msg.read(compact_proto.get());

		std::lock_guard<std::mutex> lock(m_mutex);
		m_SimpleAtsMsg.push_back(msg);

	}
}