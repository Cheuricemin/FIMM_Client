#ifndef _BASE_ZMQ_CLIENT_T_H
#define _BASE_ZMQ_CLIENT_T_H

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <zmq.h>                                                                                       
#include <string>
#include <thread>
#include <zlib.h>
#include <mutex>

#include "AtsType_types.h"
#include "AtsGeneral_types.h"
#ifndef __linux__
//#pragma comment(lib,"ws2_32.lib")
#endif


class base_zmq_client
{
public:
	base_zmq_client(){};
	virtual ~base_zmq_client(){
		zmq_close(requester);
		zmq_term(context);
	};

	void init(std::string ip, int port);
	void send(){};
	void rcv();
	bool connect();
	int gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
	int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);
	
	std::string last_msg_type;
	std::map<std::string, AtsGeneral::FeedSourceMsg> m_FeedSourceMsg;
	std::map<std::string, AtsGeneral::ConnectionMsg> m_ConnectionMsg;
	std::list<AtsGeneral::LogMsg> m_LogMsg;
	std::list<AtsGeneral::OrderMsg> m_OrderMsg;
	std::list<AtsGeneral::ExecMsg> m_ExecMsg;

	std::mutex m_mutex;

private:
	void process_thrift_msgc(char *, uLong outlen);
	virtual void process_derived_msgc(char *strr, uLong outlen) = 0;
	void *context = nullptr;
	void *requester = nullptr;
	
	std::string srv_ip;
	int srv_port = -1;
	bool is_init = false;
	std::thread m_thread;
};


#endif