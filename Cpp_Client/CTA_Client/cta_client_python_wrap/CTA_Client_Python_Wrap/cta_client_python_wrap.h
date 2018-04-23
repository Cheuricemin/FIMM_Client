//说明部分

//系统
#ifndef __linux__
#include "stdafx.h"
#endif
#include <string>
#include <queue>
#include <set>
//Boost
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/module.hpp>	//python封装
#include <boost/python/def.hpp>		//python封装
#include <boost/python/dict.hpp>	//python封装
#include <boost/python/object.hpp>	//python封装
#include <boost/python.hpp>			//python封装
#include <boost/thread.hpp>			//任务队列的线程功能
#include <boost/bind.hpp>			//任务队列的线程功能
#include <boost/any.hpp>			//任务队列的任务实现
#include <boost/locale.hpp>			//字符集转换
#include "cta_client_manager.h"
//命名空间
using namespace std;
using namespace boost::python;
using namespace boost;

//常量
#define FEEDSOURCEMSG 1
#define CONNECTIONMSG 2
#define LOGMSG 3
#define ORDERMSG 4
#define EXECMSG 5
#define CTAATSMSG 6



///-------------------------------------------------------------------------------------
///API中的部分组件
///-------------------------------------------------------------------------------------

//GIL全局锁简化获取用，
//用于帮助C++线程获得GIL锁，从而防止python崩溃
class PyLock
{
private:
	PyGILState_STATE gil_state;

public:
	//在某个函数方法中创建该对象时，获得GIL锁
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}

	//在某个函数完成后销毁该对象时，解放GIL锁
	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};


//任务结构体
struct Task
{
	int task_name;		//回调函数名称对应的常量
	any task_data;		//数据结构体
	any task_error;		//错误结构体
	int task_id;		//请求id
	bool task_last;		//是否为最后返回
};


///线程安全的队列
template<typename Data>

class ConcurrentQueue
{
private:
	std::queue<Data> the_queue;								//标准库队列
	mutable boost::mutex the_mutex;							//boost互斥锁
	boost::condition_variable the_condition_variable;			//boost条件变量

public:

	//存入新的任务
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);				//获取互斥锁
		the_queue.push(data);							//向队列中存入数据
		lock.unlock();									//释放锁
		the_condition_variable.notify_one();			//通知正在阻塞等待的线程
	}


	//检查队列是否为空
	bool empty() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return the_queue.empty();
	}

	//取出
	Data wait_and_pop()
	{
		boost::mutex::scoped_lock lock(the_mutex);

		while (the_queue.empty())						//当队列为空时
		{
			the_condition_variable.wait(lock);			//等待条件变量通知
		}

		Data popped_value = the_queue.front();			//获取队列中的最后一个任务
		the_queue.pop();								//删除该任务
		//cout << "size of queue:" << the_queue.size() << endl;
		return popped_value;							//返回该任务
	}

};


//从字典中获取某个建值对应的整数，并赋值到请求结构体对象的值上
void getInt(dict d, string key, int* value);


//从字典中获取某个建值对应的浮点数，并赋值到请求结构体对象的值上
void getDouble(dict d, string key, double* value);


//从字典中获取某个建值对应的字符，并赋值到请求结构体对象的值上
void getChar(dict d, string key, char* value);


//从字典中获取某个建值对应的字符串，并赋值到请求结构体对象的值上
void getStr(dict d, string key, char* value);


///-------------------------------------------------------------------------------------
///C++ SPI的回调函数方法实现
///-------------------------------------------------------------------------------------

//API的继承实现
class CTA_Client_API
{
private:
	boost::thread *task_thread;				//工作线程指针（向python中推送数据）
	boost::thread *sim_lib_thread;				//工作线程指针2（从C++lib里取数据包）
	ConcurrentQueue<Task> task_queue;	//任务队列

	std::set<std::string> m_con_map;
public:
	CTA_Client_API()
	{

		function0<void> f2 = boost::bind(&CTA_Client_API::get_packet_from_cpp_lib, this);
		boost::thread t2(f2);
		this->sim_lib_thread = &t2;

		function0<void> f = boost::bind(&CTA_Client_API::processTask, this);
		boost::thread t(f);
		this->task_thread = &t;

		
	};

	~CTA_Client_API()
	{
	};

	//-------------------------------------------------------------------------------------
	//task：任务
	//-------------------------------------------------------------------------------------
	void get_packet_from_cpp_lib();

	void processTask();

	void process_FeedSourceMsg(Task task);

	void process_ConnectionMsg(Task task);

	void process_LogMsg(Task task);

	void process_OrderMsg(Task task);

	void process_ExecMsg(Task task);

	void process_CTAAtsMsg(Task task);

	//-------------------------------------------------------------------------------------
	//data：回调函数的数据字典
	//error：回调函数的错误字典
	//id：请求id
	//last：是否为最后返回
	//i：整数
	//-------------------------------------------------------------------------------------
	virtual void onRtn_FeedSourceMsg(dict data) {};
	virtual void onRtn_ConnectionMsg(dict data) {};
	virtual void onRtn_LogMsg(dict data) {};
	virtual void onRtn_OrderMsg(dict data) {};
	virtual void onRtn_ExecMsg(dict data) {};
	virtual void onRtn_CTAAtsMsg(dict data) {};

	//-------------------------------------------------------------------------------------
	//req:主动函数的请求字典
	//-------------------------------------------------------------------------------------

	void connect_ats_srv(std::string ip, int port,int zmq_port);//connect srv

	int CreateAutomaton(std::string automatonName, std::string underlyingName, std::string feedsourcesStr, std::string connectionsStr, std::string stocks);

	long long HeartBeat();

	void CloseAll();

	bool CancelOrder(int id);

	void CancelAllOrder();

	bool CreateManualOrder(double price, int way, int quantity, std::string atsName, std::string atsInstrumentCode, int tradingtype, int orderrestriction, int openclose);

	void active_ats_all_time(std::string atsName);

	void StartAutomaton(std::string atsName);
	void StopAutomaton(std::string atsNamme);
	void Start(std::string atsName);
	void Stop(std::string atsName);

};
