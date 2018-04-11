#ifndef _CTA_ZMQ_CLIENT_T_H
#define _CTA_ZMQ_CLIENT_T_H

#include "base_zmq_client.h"
#include "CTAAtsMsg_types.h"

class cta_zmq_client:public base_zmq_client
{
public:
	cta_zmq_client(){};
	~cta_zmq_client(){};

	std::list<CTAMsg::CTAAtsMsg> m_CTAAtsMsg;

private:
	
	virtual void process_derived_msgc(char *strr, uLong outlen) override;
};


#endif