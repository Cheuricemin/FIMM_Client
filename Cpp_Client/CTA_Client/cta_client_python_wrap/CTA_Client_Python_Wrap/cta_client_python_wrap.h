//˵������

//ϵͳ
#ifndef __linux__
#include "stdafx.h"
#endif
#include <string>
#include <queue>
#include <set>
//Boost
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python/module.hpp>	//python��װ
#include <boost/python/def.hpp>		//python��װ
#include <boost/python/dict.hpp>	//python��װ
#include <boost/python/object.hpp>	//python��װ
#include <boost/python.hpp>			//python��װ
#include <boost/thread.hpp>			//������е��̹߳���
#include <boost/bind.hpp>			//������е��̹߳���
#include <boost/any.hpp>			//������е�����ʵ��
#include <boost/locale.hpp>			//�ַ���ת��
#include "cta_client_manager.h"
//�����ռ�
using namespace std;
using namespace boost::python;
using namespace boost;

//����
#define FEEDSOURCEMSG 1
#define CONNECTIONMSG 2
#define LOGMSG 3
#define ORDERMSG 4
#define EXECMSG 5
#define CTAATSMSG 6



///-------------------------------------------------------------------------------------
///API�еĲ������
///-------------------------------------------------------------------------------------

//GILȫ�����򻯻�ȡ�ã�
//���ڰ���C++�̻߳��GIL�����Ӷ���ֹpython����
class PyLock
{
private:
	PyGILState_STATE gil_state;

public:
	//��ĳ�����������д����ö���ʱ�����GIL��
	PyLock()
	{
		gil_state = PyGILState_Ensure();
	}

	//��ĳ��������ɺ����ٸö���ʱ�����GIL��
	~PyLock()
	{
		PyGILState_Release(gil_state);
	}
};


//����ṹ��
struct Task
{
	int task_name;		//�ص��������ƶ�Ӧ�ĳ���
	any task_data;		//���ݽṹ��
	any task_error;		//����ṹ��
	int task_id;		//����id
	bool task_last;		//�Ƿ�Ϊ��󷵻�
};


///�̰߳�ȫ�Ķ���
template<typename Data>

class ConcurrentQueue
{
private:
	std::queue<Data> the_queue;								//��׼�����
	mutable boost::mutex the_mutex;							//boost������
	boost::condition_variable the_condition_variable;			//boost��������

public:

	//�����µ�����
	void push(Data const& data)
	{
		boost::mutex::scoped_lock lock(the_mutex);				//��ȡ������
		the_queue.push(data);							//������д�������
		lock.unlock();									//�ͷ���
		the_condition_variable.notify_one();			//֪ͨ���������ȴ����߳�
	}


	//�������Ƿ�Ϊ��
	bool empty() const
	{
		boost::mutex::scoped_lock lock(the_mutex);
		return the_queue.empty();
	}

	//ȡ��
	Data wait_and_pop()
	{
		boost::mutex::scoped_lock lock(the_mutex);

		while (the_queue.empty())						//������Ϊ��ʱ
		{
			the_condition_variable.wait(lock);			//�ȴ���������֪ͨ
		}

		Data popped_value = the_queue.front();			//��ȡ�����е����һ������
		the_queue.pop();								//ɾ��������
		//cout << "size of queue:" << the_queue.size() << endl;
		return popped_value;							//���ظ�����
	}

};


//���ֵ��л�ȡĳ����ֵ��Ӧ������������ֵ������ṹ������ֵ��
void getInt(dict d, string key, int* value);


//���ֵ��л�ȡĳ����ֵ��Ӧ�ĸ�����������ֵ������ṹ������ֵ��
void getDouble(dict d, string key, double* value);


//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ�������ֵ������ṹ������ֵ��
void getChar(dict d, string key, char* value);


//���ֵ��л�ȡĳ����ֵ��Ӧ���ַ���������ֵ������ṹ������ֵ��
void getStr(dict d, string key, char* value);


///-------------------------------------------------------------------------------------
///C++ SPI�Ļص���������ʵ��
///-------------------------------------------------------------------------------------

//API�ļ̳�ʵ��
class CTA_Client_API
{
private:
	boost::thread *task_thread;				//�����߳�ָ�루��python���������ݣ�
	boost::thread *sim_lib_thread;				//�����߳�ָ��2����C++lib��ȡ���ݰ���
	ConcurrentQueue<Task> task_queue;	//�������

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
	//task������
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
	//data���ص������������ֵ�
	//error���ص������Ĵ����ֵ�
	//id������id
	//last���Ƿ�Ϊ��󷵻�
	//i������
	//-------------------------------------------------------------------------------------
	virtual void onRtn_FeedSourceMsg(dict data) {};
	virtual void onRtn_ConnectionMsg(dict data) {};
	virtual void onRtn_LogMsg(dict data) {};
	virtual void onRtn_OrderMsg(dict data) {};
	virtual void onRtn_ExecMsg(dict data) {};
	virtual void onRtn_CTAAtsMsg(dict data) {};

	//-------------------------------------------------------------------------------------
	//req:���������������ֵ�
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
