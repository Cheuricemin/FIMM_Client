import os
import time
import ctypes
from CTA_Client_Python_Wrap import  *

def print_dict(d):
    return
    """按照键值打印一个字典"""
    for key,value in d.items():
        print(key + ':' + str(value))

class TestCTAApi(CTA_Client_API):
    # 这是行情源消息的回调，是CTA_Client_API里虚函数的重载，下同
	#Name是行情源的名字（和服务端一致），Status为0代表行情源up，1代表down，Activated为bool值，为true代表被激活，具体定义见AtsGeneral.thrift和AtsType.thrift，下同
    def onRtn_FeedSourceMsg(self,data):
        print_dict(data)

    # 这是交易源消息的回调 是CTA_Client_API里虚函数的重载
    def onRtn_ConnectionMsg(self,data):
        print_dict(data)

    # 这是日志消息的回调 是CTA_Client_API里虚函数的重载
    def onRtn_LogMsg(self, data):
        print_dict(data)

    # 这是order消息的回调 是CTA_Client_API里虚函数的重载
    def onRtn_OrderMsg(self, data):
        print_dict(data)

    # 这是成交消息的回调 是CTA_Client_API里虚函数的重载
    def onRtn_ExecMsg(self, data):
        print_dict(data)
		
		
	# 这是处理CTAMsg消息的回调	 是CTA_Client_API里虚函数的重载
    def onRtn_CTAAtsMsg(self, data):
        print(data['Ats']['Name'])
        print(data['AllFutures'][0]['AtsInstrument']['FeedItem'])

if __name__ == '__main__':

    api = TestCTAApi()
    api.connect_ats_srv('localhost',9094,9095)#参数是服务端的ip，thrift的端口和zmq的端口
    api.CreateAutomaton('TFT','TF|T','','CTP','lz_ctp')#创建ats，这几个参数的意思见C#的ini文件
    #api.Start('TFT')#启动对应ats
    time.sleep(100000)