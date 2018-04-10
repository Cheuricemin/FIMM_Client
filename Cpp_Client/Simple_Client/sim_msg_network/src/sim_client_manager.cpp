#include "sim_client_manager.h"
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TBinaryProtocol.h>
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using boost::shared_ptr;

void sim_client_manager::connect_ats_srv(std::string ip, int port,int zmq_port)
{
	if (ip.size() == 0)
		ip = "localhost";

	boost::shared_ptr<TSocket> socket(new TSocket(ip, port));
	boost::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
	boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

	transport->open();

	m_zmq_client.init(ip, zmq_port);
	m_zmq_client.connect();

	m_sim_client = new SimpleAtsOperationClient(protocol);
	m_sim_client->StartPublish();
	//m_sim_client->ConnectionConnect("CTP",false);
}

bool sim_client_manager::CreateManualOrder(double price, int way, int quantity,
	std::string atsName, std::string atsInstrumentCode, int tradingtype, int orderrestriction, int openclose)
{
	AtsType::OrderWay::type ordway = (AtsType::OrderWay::type)way;
	AtsType::OrderRestriction::type restricition = (AtsType::OrderRestriction::type)orderrestriction;
	AtsType::OrderOpenClose::type oc = (AtsType::OrderOpenClose::type)openclose;
	if (m_sim_client!=nullptr)
		return m_sim_client->CreateManualOrder(price, ordway, quantity, atsName, atsInstrumentCode, tradingtype, restricition, oc);

	return false;
}

void sim_client_manager::active_ats_all_time(std::string atsName)
{
	TradingPeriodManagerMsg tm;
	tm.ShiftEodTime = "00:00:00";
	tm.ShiftPriceTime = "00:00:00";
	tm.ShiftEodTimeN = "00:00:00";

	TradingPeriodMsg tmsg;
	tmsg.AutoStopInterval = 0;
	tmsg.StartTime = "00:00:00";
	tmsg.StopTime = "23:59:59";
	tmsg.Phase = (AtsType::TradingPhase::type)1;

	tm.TradingPeriodList.push_back(tmsg);

	m_sim_client->SetTradingPeriodManager(atsName,tm);
}