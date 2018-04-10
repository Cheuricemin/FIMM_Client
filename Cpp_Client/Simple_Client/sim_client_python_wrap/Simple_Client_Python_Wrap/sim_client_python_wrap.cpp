// vnctpmd.cpp : 定义 DLL 应用程序的导出函数。
//

#include "sim_client_python_wrap.h"

///-------------------------------------------------------------------------------------
///从Python对象到C++类型转换用的函数
///-------------------------------------------------------------------------------------

void getInt(dict d, string key, int *value)
{
	if (d.has_key(key))		//检查字典中是否存在该键值
	{
		object o = d[key];	//获取该键值
		extract<int> x(o);	//创建提取器
		if (x.check())		//如果可以提取
		{
			*value = x();	//对目标整数指针赋值
		}
	}
};

void getDouble(dict d, string key, double *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<double> x(o);
		if (x.check())
		{
			*value = x();
		}
	}
};

void getStr(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			//对字符串指针赋值必须使用strcpy_s, vs2013使用strcpy编译通不过
			//+1应该是因为C++字符串的结尾符号？不是特别确定，不加这个1会出错
#ifdef _MSC_VER //WIN32
			strcpy_s(value, strlen(buffer) + 1, buffer);
#elif __GNUC__
			strncpy(value, buffer, strlen(buffer) + 1);
#endif
		}
	}
};

void getChar(dict d, string key, char *value)
{
	if (d.has_key(key))
	{
		object o = d[key];
		extract<string> x(o);
		if (x.check())
		{
			string s = x();
			const char *buffer = s.c_str();
			*value = *buffer;
		}
	}
};


///-------------------------------------------------------------------------------------
///C++的回调函数将数据保存到队列中
///-------------------------------------------------------------------------------------



//void Sim_Client_API::OnRtnForQuoteRsp(CThostFtdcForQuoteRspField *pForQuoteRsp)
//{
//	Task task = Task();
//	task.task_name = ONRTNFORQUOTERSP;
//
//	if (pForQuoteRsp)
//	{
//		task.task_data = *pForQuoteRsp;
//	}
//	else
//	{
//		CThostFtdcForQuoteRspField empty_data = CThostFtdcForQuoteRspField();
//		memset(&empty_data, 0, sizeof(empty_data));
//		task.task_data = empty_data;
//	}
//	this->task_queue.push(task);
//};

///-------------------------------------------------------------------------------------
///工作线程从C++lib中取出数据，转化为python对象
///-------------------------------------------------------------------------------------
void Sim_Client_API::get_packet_from_cpp_lib()
{
	
	std::list<SimpleMsg::SimpleAtsMsg> m_SimpleAtsMsg;
	std::list<AtsGeneral::LogMsg> m_LogMsg;
	std::list<AtsGeneral::OrderMsg> m_OrderMsg;
	std::list<AtsGeneral::ExecMsg> m_ExecMsg;
	std::map<std::string, AtsGeneral::FeedSourceMsg> m_FeedSource;
	std::map<std::string, AtsGeneral::ConnectionMsg> m_ConnectionMsg;
	auto zmq_cli = sim_client_manager::get_instance().get_zmq_client();

	while (1)
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

		{
			std::lock_guard<std::mutex> lock(zmq_cli->m_mutex);
			m_SimpleAtsMsg.swap(zmq_cli->m_SimpleAtsMsg);
			m_LogMsg.swap(zmq_cli->m_LogMsg);
			m_OrderMsg.swap(zmq_cli->m_OrderMsg);
			m_ExecMsg.swap(zmq_cli->m_ExecMsg);
			m_FeedSource.swap(zmq_cli->m_FeedSource);
			m_ConnectionMsg.swap(zmq_cli->m_ConnectionMsg);
		}

		for (auto it : m_SimpleAtsMsg)
		{
			Task task = Task();
			task.task_name = SIMPLEATSMSG;
			task.task_data = it;
			this->task_queue.push(task);
		}

		for (auto it : m_LogMsg)
		{
			Task task = Task();
			task.task_name = LOGMSG;
			task.task_data = it;
			this->task_queue.push(task);
		}

		for (auto it : m_OrderMsg)
		{
			Task task = Task();
			task.task_name = ORDERMSG;
			task.task_data = it;
			this->task_queue.push(task);
		}

		for (auto it : m_ExecMsg)
		{
			Task task = Task();
			task.task_name = EXECMSG;
			task.task_data = it;
			this->task_queue.push(task);
		}

		for (auto it : m_FeedSource)
		{
			Task task = Task();
			task.task_name = FEEDSOURCEMSG;
			task.task_data = it.second;
			this->task_queue.push(task);
		}

		for (auto it : m_ConnectionMsg)
		{
			Task task = Task();
			task.task_name = CONNECTIONMSG;
			task.task_data = it.second;
			this->task_queue.push(task);
		}
	}
}

///-------------------------------------------------------------------------------------
///工作线程从队列中取出数据，转化为python对象后，进行推送
///-------------------------------------------------------------------------------------

void Sim_Client_API::processTask()
{
	
	while (1)
	{
		Task task = this->task_queue.wait_and_pop();
		
		switch (task.task_name)
		{
		case FEEDSOURCEMSG:
		{
							  this->process_FeedSourceMsg(task);
							  break;
		}

		case CONNECTIONMSG:
		{
							  this->process_ConnectionMsg(task);
							  break;
		}

		case LOGMSG:
		{
					   this->process_LogMsg(task);
					   break;
		}

		case ORDERMSG:
		{
						 this->process_OrderMsg(task);
						 break;
		}

		case EXECMSG:
		{
						this->process_ExecMsg(task);
						break;
		}

		case SIMPLEATSMSG:
		{
							 this->process_SimpleAtsMsg(task);
							 break;
		}


		};
	}
};

void Sim_Client_API::process_FeedSourceMsg(Task task)
{
	PyLock lock;
	dict data;
	
	AtsGeneral::FeedSourceMsg task_data = any_cast<AtsGeneral::FeedSourceMsg>(task.task_data);
	//std::cout << task_data.Name << " " << task_data.Status << " " << task_data.Activated << std::endl;
	data["Name"] = task_data.Name;
	data["Status"] = (int)task_data.Status;
	data["Activated"] = task_data.Activated;

	this->onRtn_FeedSourceMsg(data);
}

void Sim_Client_API::process_ConnectionMsg(Task task)
{
	PyLock lock;
	dict data;
	
	AtsGeneral::ConnectionMsg task_data = any_cast<AtsGeneral::ConnectionMsg>(task.task_data);
	//std::cout << task_data.Name << " " << task_data.Status << std::endl;
	data["Name"] = task_data.Name;
	data["Status"] = (int)task_data.Status;
	data["TradingAllowed"] = task_data.TradingAllowed;
	data["Ack"] = task_data.Ack;
	data["Can"] = task_data.Can;
	data["Exe"] = task_data.Exe;
	data["Mod"] = task_data.Mod;
	data["NAck"] = task_data.NAck;
	data["New"] = task_data.New;
	data["Rej"] = task_data.Rej;

	if (m_con_map.find(task_data.Name) == m_con_map.end())
	{
		m_con_map.insert(task_data.Name);
		sim_client_manager::get_instance().m_sim_client->ConnectionConnect(task_data.Name,true);
	}

	this->onRtn_ConnectionMsg(data);
}

void Sim_Client_API::process_LogMsg(Task task)
{
	PyLock lock;
	dict data;
	AtsGeneral::LogMsg task_data = any_cast<AtsGeneral::LogMsg>(task.task_data);
	data["TimeStamp"] = task_data.TimeStamp;
	data["Message"] = task_data.Message;
	this->onRtn_LogMsg(data);
}

void Sim_Client_API::process_OrderMsg(Task task)
{
	PyLock lock;
	dict data;

	AtsGeneral::OrderMsg task_data = any_cast<AtsGeneral::OrderMsg>(task.task_data);
	data["Id"] = task_data.Id;
	data["Ack_Cancel_Ratio"] = task_data.Ack_Cancel_Ratio;
	data["Action"] = task_data.Action;
	data["Active"] = task_data.Active;
	data["BookQty"] = task_data.BookQty;
	data["Code"] = task_data.Code;

	data["ExecPrice"] = task_data.ExecPrice;
	data["ExecQty"] = task_data.ExecQty;
	data["LastReason"] = task_data.LastReason;
	data["OpenClose"] = task_data.OpenClose;
	data["Portfolio"] = task_data.Portfolio;
	data["Price"] = task_data.Price;
	data["Quantity"] = task_data.Quantity;
	data["Status"] = task_data.Status;

	data["TimeStamp"] = task_data.TimeStamp;
	data["TradingType"] = task_data.TradingType;
	data["Way"] = task_data.Way;

	this->onRtn_OrderMsg(data);
}

void Sim_Client_API::process_ExecMsg(Task task)
{
	PyLock lock;
	dict data;

	AtsGeneral::ExecMsg task_data = any_cast<AtsGeneral::ExecMsg>(task.task_data);
	data["Id"] = task_data.Id;
	data["Code"] = task_data.Code;
	data["OpenClose"] = task_data.OpenClose;
	data["Portfolio"] = task_data.Portfolio;
	data["Price"] = task_data.Price;
	data["Quantity"] = task_data.Quantity;
	data["TradingType"] = task_data.TradingType;
	data["Way"] = task_data.Way;
	data["Reference"] = task_data.Reference;
	data["Time"] = task_data.Time;

	this->onRtn_ExecMsg(data);
}

void create_AtsMsg(SimpleMsg::SimpleAtsMsg &task_data,dict &data)
{
	dict ats;
	ats["Name"] = task_data.Ats.Name;
	ats["AutoStatus"] = task_data.Ats.AutoStatus;
	ats["YesterdayPositionType"] = task_data.Ats.YesterdayPositionType;
	ats["YesterdayPnlBary"] = task_data.Ats.YesterdayPnlBary;
	ats["TodayPnlBary"] = task_data.Ats.TodayPnlBary;
	ats["YesterdayPnlMid"] = task_data.Ats.YesterdayPnlMid;
	ats["TodayPnlMid"] = task_data.Ats.TodayPnlMid;
	ats["YesterdayPnlLast"] = task_data.Ats.YesterdayPnlLast;
	ats["TodayPnlLast"] = task_data.Ats.TodayPnlLast;
	ats["FeesExchange"] = task_data.Ats.FeesExchange;
	ats["FeesBroker"] = task_data.Ats.FeesBroker;
	ats["TotalPnl"] = task_data.Ats.TotalPnl;

	data["Ats"] = ats;
}

void create_TrdInstrumentMsg(SimpleMsg::TrdInstrumentMsg &task_data, boost::python::list &data)
{
	dict trd;
	trd["AtsName"] = task_data.AtsName;

	dict atsinstr;
	{
		atsinstr["Code"] = task_data.AtsInstrument.Code;
		atsinstr["ExchangeFees"] = task_data.AtsInstrument.ExchangeFees;
		atsinstr["BrokerFees"] = task_data.AtsInstrument.BrokerFees;
		atsinstr["YesterdayPnlBarycenter"] = task_data.AtsInstrument.YesterdayPnlBarycenter;
		atsinstr["TodayPnlBarycenter"] = task_data.AtsInstrument.TodayPnlBarycenter;
		atsinstr["SizeToSend"] = task_data.AtsInstrument.SizeToSend;
		atsinstr["InstrumentClassName"] = task_data.AtsInstrument.InstrumentClassName;

		{
			dict positon;
			positon["Instrument"] = task_data.AtsInstrument.Position.Instrument;
			positon["Portfolio"] = task_data.AtsInstrument.Position.Portfolio;
			positon["YesterdayPosition"] = task_data.AtsInstrument.Position.YesterdayPosition;
			positon["TodayPosition"] = task_data.AtsInstrument.Position.TodayPosition;
			positon["TotalPosition"] = task_data.AtsInstrument.Position.TotalPosition;

			positon["TodayBuyPosition"] = task_data.AtsInstrument.Position.TodayBuyPosition;
			positon["TodayBuyPrice"] = task_data.AtsInstrument.Position.TodayBuyPrice;
			positon["TodaySellPosition"] = task_data.AtsInstrument.Position.TodaySellPosition;
			positon["TodaySellPrice"] = task_data.AtsInstrument.Position.TodaySellPrice;
			positon["YesterdayPrice"] = task_data.AtsInstrument.Position.YesterdayPrice;

			positon["YesterdayPriceLocal"] = task_data.AtsInstrument.Position.YesterdayPriceLocal;
			positon["YesterdayPriceExternal"] = task_data.AtsInstrument.Position.YesterdayPriceExternal;
			positon["YesterdayPositionLocal"] = task_data.AtsInstrument.Position.YesterdayPositionLocal;
			positon["YesterdayPositionManual"] = task_data.AtsInstrument.Position.YesterdayPositionManual;
			positon["YesterdayPositionExternal"] = task_data.AtsInstrument.Position.YesterdayPositionExternal;

			positon["UseManualPosition"] = task_data.AtsInstrument.Position.UseManualPosition;
			positon["YstPositionType"] = task_data.AtsInstrument.Position.YstPositionType;
			positon["YstPriceType"] = task_data.AtsInstrument.Position.YstPriceType;
			positon["Connection"] = task_data.AtsInstrument.Position.Connection;
			positon["TodayPurPosition"] = task_data.AtsInstrument.Position.TodayPurPosition;

			positon["TodayRedPosition"] = task_data.AtsInstrument.Position.TodayRedPosition;

			atsinstr["Position"] = positon;
		}

		{
			dict FeedItem;
			boost::python::list BidDepths, AskDepths, BidQtys, AskQtys;
			FeedItem["Code"] = task_data.AtsInstrument.FeedItem.Code;
			FeedItem["FeedSourceName"] = task_data.AtsInstrument.FeedItem.FeedSourceName;
			FeedItem["Bid"] = task_data.AtsInstrument.FeedItem.Bid;
			FeedItem["Ask"] = task_data.AtsInstrument.FeedItem.Ask;
			FeedItem["BidQuantity"] = task_data.AtsInstrument.FeedItem.BidQuantity;

			FeedItem["AskQuantity"] = task_data.AtsInstrument.FeedItem.AskQuantity;
			FeedItem["Last"] = task_data.AtsInstrument.FeedItem.Last;
			FeedItem["LastQuantity"] = task_data.AtsInstrument.FeedItem.LastQuantity;
			FeedItem["LastOrClose"] = task_data.AtsInstrument.FeedItem.LastOrClose;
			FeedItem["Mid"] = task_data.AtsInstrument.FeedItem.Mid;

			FeedItem["Close"] = task_data.AtsInstrument.FeedItem.Close;
			FeedItem["Settlement"] = task_data.AtsInstrument.FeedItem.Settlement;
			FeedItem["UpperLimit"] = task_data.AtsInstrument.FeedItem.UpperLimit;
			FeedItem["LowerLimit"] = task_data.AtsInstrument.FeedItem.LowerLimit;
			FeedItem["Perf"] = task_data.AtsInstrument.FeedItem.Perf;

			FeedItem["DailyVolume"] = task_data.AtsInstrument.FeedItem.DailyVolume;
			FeedItem["isBiddAskActive"] = task_data.AtsInstrument.FeedItem.isBiddAskActive;
			FeedItem["isSuspended"] = task_data.AtsInstrument.FeedItem.isSuspended;
			FeedItem["MaxDepth"] = task_data.AtsInstrument.FeedItem.MaxDepth;

			for (auto &it : task_data.AtsInstrument.FeedItem.AskDepths)
			{
				AskDepths.append(it);
			}
			for (auto &it : task_data.AtsInstrument.FeedItem.BidDepths)
			{
				BidDepths.append(it);
			}
			for (auto &it : task_data.AtsInstrument.FeedItem.AskQtys)
			{
				AskQtys.append(it);
			}
			for (auto &it : task_data.AtsInstrument.FeedItem.BidQtys)
			{
				BidQtys.append(it);
			}

			FeedItem["AskDepths"] = AskDepths;
			FeedItem["BidDepths"] = BidDepths;
			FeedItem["AskQtys"] = AskQtys;
			FeedItem["BidQtys"] = BidQtys;
			atsinstr["FeedItem"] = FeedItem;
		}
	}
	trd["AtsInstrument"] = atsinstr;

	data.append(trd);
}

void Sim_Client_API::process_SimpleAtsMsg(Task task)
{
	PyLock lock;
	dict data;
	boost::python::list fut, opt;

	SimpleMsg::SimpleAtsMsg task_data = any_cast<SimpleMsg::SimpleAtsMsg>(task.task_data);
	data["KeepOrders"] = task_data.KeepOrders;
	data["ForceOpen"] = task_data.ForceOpen;
	data["FeedsourcesStr"] = task_data.FeedsourcesStr;
	data["ConnectionsStr"] = task_data.ConnectionsStr;

	create_AtsMsg(task_data, data);

	for (auto &it : task_data.AllFutures)
	{
		create_TrdInstrumentMsg(it, fut);
	}
	data["AllFutures"] = fut;

	for (auto &it : task_data.AllOptions)
	{
		create_TrdInstrumentMsg(it, opt);
	}
	data["AllOptions"] = opt;

	this->onRtn_ExecMsg(data);

}



///-------------------------------------------------------------------------------------
///主动函数
///-------------------------------------------------------------------------------------

void Sim_Client_API::connect_ats_srv(std::string ip, int port, int zmq_port)
{
	sim_client_manager::get_instance().connect_ats_srv(ip, port, zmq_port);
};

int Sim_Client_API::CreateAutomaton(std::string automatonName, std::string underlyingName, std::string feedsourcesStr, std::string connectionsStr, std::string stocks)
{
	SimpleMsg::SimpleAtsMsg msg;
	sim_client_manager::get_instance().m_sim_client->CreateAutomaton(msg,automatonName, underlyingName, feedsourcesStr, connectionsStr, stocks);
	if (msg.Ats.Name.size() == 0)
		return -1;
	return 1;
}

long long Sim_Client_API::HeartBeat()
{
	long long res = sim_client_manager::get_instance().m_sim_client->HeartBeat();
	return res;
}

void Sim_Client_API::CloseAll()
{
	sim_client_manager::get_instance().m_sim_client->CloseAll();
}

bool Sim_Client_API::CancelOrder(int id)
{
	bool res = sim_client_manager::get_instance().m_sim_client->CancelOrder(id);
	return res;
}

void Sim_Client_API::CancelAllOrder()
{
	sim_client_manager::get_instance().m_sim_client->CancelAllOrder();
}

bool Sim_Client_API::CreateManualOrder(double price, int way, int quantity, std::string atsName, std::string atsInstrumentCode, int tradingtype, 
	int orderrestriction, int openclose)
{
	bool res = sim_client_manager::get_instance().CreateManualOrder(price,way,quantity,atsName,
		atsInstrumentCode,tradingtype,orderrestriction,openclose);
	return res;
}

void Sim_Client_API::active_ats_all_time(std::string atsName)
{
	sim_client_manager::get_instance().active_ats_all_time(atsName);
}

void Sim_Client_API::StartAutomaton(std::string atsName)
{
	sim_client_manager::get_instance().m_sim_client->StartAutomaton(atsName);
}

void Sim_Client_API::StopAutomaton(std::string atsName)
{
	sim_client_manager::get_instance().m_sim_client->StopAutomaton(atsName);
}

void Sim_Client_API::Start(std::string atsName)
{
	sim_client_manager::get_instance().m_sim_client->Start(atsName);
}

void Sim_Client_API::Stop(std::string atsName)
{
	sim_client_manager::get_instance().m_sim_client->Stop(atsName);
}


///-------------------------------------------------------------------------------------
///Boost.Python封装
///-------------------------------------------------------------------------------------

struct Sim_Client_API_Wrap : Sim_Client_API, wrapper < Sim_Client_API >
{
	//以下的try...catch...可以实现捕捉python环境中错误的功能，防止C++直接出现原因未知的崩溃
	/*
	virtual void onFrontConnected()
	{
		
		try
		{
			this->get_override("onFrontConnected")();
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onFrontDisconnected(int i)
	{
		try
		{
			this->get_override("onFrontDisconnected")(i);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onHeartBeatWarning(int i)
	{
		try
		{
			this->get_override("onHeartBeatWarning")(i);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspError(dict data, int id, bool last)
	{
		try
		{
			this->get_override("onRspError")(data, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUserLogin(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUserLogin")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUserLogout(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUserLogout")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspSubMarketData(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspSubMarketData")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUnSubMarketData(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUnSubMarketData")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspSubForQuoteRsp(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspSubForQuoteRsp")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRspUnSubForQuoteRsp(dict data, dict error, int id, bool last)
	{
		try
		{
			this->get_override("onRspUnSubForQuoteRsp")(data, error, id, last);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtnDepthMarketData(dict data)
	{
		try
		{
			this->get_override("onRtnDepthMarketData")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtnForQuoteRsp(dict data)
	{
		try
		{
			this->get_override("onRtnForQuoteRsp")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};*/

	virtual void onRtn_FeedSourceMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_FeedSourceMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtn_ConnectionMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_ConnectionMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtn_LogMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_LogMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtn_OrderMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_OrderMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtn_ExecMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_ExecMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};

	virtual void onRtn_SimpleAtsMsg(dict data)
	{
		try
		{
			this->get_override("onRtn_SimpleAtsMsg")(data);
		}
		catch (error_already_set const &)
		{
			PyErr_Print();
		}
	};
};


BOOST_PYTHON_MODULE(Simple_Client_Python_Wrap)
{
	PyEval_InitThreads();	//导入时运行，保证先创建GIL

	class_<Sim_Client_API_Wrap, boost::noncopyable>("Sim_Client_API")

		.def("connect_ats_srv", &Sim_Client_API_Wrap::connect_ats_srv)
		.def("CreateAutomaton", &Sim_Client_API_Wrap::CreateAutomaton)

		.def("HeartBeat", &Sim_Client_API_Wrap::HeartBeat)
		.def("CloseAll", &Sim_Client_API_Wrap::CloseAll)

		.def("CancelOrder", &Sim_Client_API_Wrap::CancelOrder)
		.def("CancelAllOrder", &Sim_Client_API_Wrap::CancelAllOrder)

		.def("CreateManualOrder", &Sim_Client_API_Wrap::CreateManualOrder)
		.def("active_ats_all_time", &Sim_Client_API_Wrap::active_ats_all_time)

		.def("Start", &Sim_Client_API_Wrap::Start)
		.def("Stop", &Sim_Client_API_Wrap::Stop)
		.def("StartAutomaton", &Sim_Client_API_Wrap::StartAutomaton)
		.def("StopAutomaton", &Sim_Client_API_Wrap::StopAutomaton)

		/*.def("onFrontConnected", pure_virtual(&Sim_Client_API_Wrap::onFrontConnected))
		.def("onFrontDisconnected", pure_virtual(&Sim_Client_API_Wrap::onFrontDisconnected))
		.def("onHeartBeatWarning", pure_virtual(&Sim_Client_API_Wrap::onHeartBeatWarning))
		.def("onRspError", pure_virtual(&Sim_Client_API_Wrap::onRspError))
		.def("onRspUserLogin", pure_virtual(&Sim_Client_API_Wrap::onRspUserLogin))
		.def("onRspUserLogout", pure_virtual(&Sim_Client_API_Wrap::onRspUserLogout))
		.def("onRspSubMarketData", pure_virtual(&Sim_Client_API_Wrap::onRspSubMarketData))
		.def("onRspUnSubMarketData", pure_virtual(&Sim_Client_API_Wrap::onRspUnSubMarketData))
		.def("onRtnDepthMarketData", pure_virtual(&Sim_Client_API_Wrap::onRtnDepthMarketData))
		.def("onRspSubForQuoteRsp", pure_virtual(&Sim_Client_API_Wrap::onRspSubForQuoteRsp))
		.def("onRspUnSubForQuoteRsp", pure_virtual(&Sim_Client_API_Wrap::onRspUnSubForQuoteRsp))
		.def("onRtnForQuoteRsp", pure_virtual(&Sim_Client_API_Wrap::onRtnForQuoteRsp))*/
		.def("onRtn_FeedSourceMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_FeedSourceMsg))
		.def("onRtn_ConnectionMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_ConnectionMsg))
		.def("onRtn_LogMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_LogMsg))
		.def("onRtn_OrderMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_OrderMsg))
		.def("onRtn_ExecMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_ExecMsg))
		.def("onRtn_SimpleAtsMsg", pure_virtual(&Sim_Client_API_Wrap::onRtn_SimpleAtsMsg))
		;
};
