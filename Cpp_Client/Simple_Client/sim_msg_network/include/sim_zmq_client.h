#ifndef _SIM_ZMQ_CLIENT_T_H
#define _SIM_ZMQ_CLIENT_T_H

#include "base_zmq_client.h"
#include "SimpleAtsMsg_types.h"

class sim_zmq_client:public base_zmq_client
{
public:
	sim_zmq_client(){};
	~sim_zmq_client(){};

	std::list<SimpleMsg::SimpleAtsMsg> m_SimpleAtsMsg;

private:
	
	virtual void process_derived_msgc(char *strr, uLong outlen) override;
};


#endif