#ifndef __SIM_CLIENT_MANAGER_H__
#define __SIM_CLIENT_MANAGER_H__

#include <thread>
#include <functional>
#include "singleton.hpp"
#include "SimpleAtsOperation.h"
#include "sim_zmq_client.h"

using namespace SimpleMsg;
using namespace AtsGeneral;

class sim_client_manager :public SingletonBase<sim_client_manager>
{
public:
	void connect_ats_srv(std::string ip, int port,int zmq_port);
	
	bool CreateManualOrder(double price, int way, int quantity, 
		std::string atsName, std::string atsInstrumentCode, int tradingtype, int orderrestriction, int openclose);

	void active_ats_all_time(std::string atsName);

	sim_zmq_client *get_zmq_client(){ return &m_zmq_client; }
public:
	SimpleAtsOperationClient *m_sim_client=nullptr;
	sim_zmq_client m_zmq_client;
};


#endif 
