#ifndef __CTA_CLIENT_MANAGER_H__
#define __CTA_CLIENT_MANAGER_H__

#include <thread>
#include <functional>
#include "singleton.hpp"
#include "CTAOperation.h"
#include "cta_zmq_client.h"

using namespace CTAMsg;
using namespace AtsGeneral;

class cta_client_manager :public SingletonBase<cta_client_manager>
{
public:
	void connect_ats_srv(std::string ip, int port, int zmq_port);

	bool CreateManualOrder(double price, int way, int quantity,
		std::string atsName, std::string atsInstrumentCode, int tradingtype, int orderrestriction, int openclose);

	void active_ats_all_time(std::string atsName);

	cta_zmq_client *get_zmq_client(){ return &m_zmq_client; }
public:
	CTAOperationClient *m_cta_client;
	cta_zmq_client m_zmq_client;
};


#endif 