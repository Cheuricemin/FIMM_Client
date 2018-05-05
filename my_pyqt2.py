from __future__ import (absolute_import, division, print_function,
                        unicode_literals)
import sys
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QApplication,QCheckBox,QGridLayout,QGroupBox,QMenu,QPushButton,QRadioButton,QVBoxLayout,QWidget
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

import datetime
import os.path
import pandas as pd
import sys
import backtrader as bt

import sqlite3
import pymysql

from WindPy import w
from pandas import Series
from pandas import DataFrame
import glob
import os, time, random




class MyTable(QTableWidget):
    future1 = "T1806"
    future2 = "T1809"
    future3 = "T1812"
    future4 = "TF1806"
    future5 = "TF1809"
    future6 = "TF1812"
    def __init__(self,parent=None):
        super(MyTable,self).__init__(parent)
        self.setWindowTitle("T1")
        self.setWindowIcon(QIcon("C:\\Users\\cheucemin\\Desktop\\butterfly.png"))
        #self.resize(100,500)
        self.setColumnCount(13)
        self.setRowCount(6)

        #设置表格有13列，7行
        for i in range(13):
            self.setRowHeight(i,20)
            self.setColumnWidth(i, 50)
        for j in range(2):
            self.setColumnWidth(j,15)
        self.setColumnWidth(12,15)
        #设置第行高度为100px，列宽度为150px
        #self.resizeRowsToContents() 这是设置根据内容调整
        self.table()
        update_data_thread = UpdateData()
        update_data_thread.update_date.connect(self.update_item_data)
        update_data_thread.start()
        self.show()


    def table(self):
        self.setItem(0,2,QTableWidgetItem(MyTable.future1))
        self.setItem(1,2,QTableWidgetItem(MyTable.future2))
        self.setItem(2,2,QTableWidgetItem(MyTable.future3))
        self.setItem(3,2,QTableWidgetItem(MyTable.future4))
        self.setItem(4,2,QTableWidgetItem(MyTable.future5))
        self.setItem(5,2,QTableWidgetItem(MyTable.future6))
        #添加表格的文字内容（第二列）

        self.setHorizontalHeaderLabels(["","","Name","Bid Qt","Bid","Ask","Ask Qt","Last","Close","%","Volumn","Position",""])
        self.verticalHeader().setVisible(False)#隐藏纵向的表头
        self.setAlternatingRowColors(True)
        self.setShowGrid(True)


        for i in range(0, 7):
            qb = QCheckBox()
            self.setCellWidget(i, 0, qb)
        for i in range(0,7):
               qb = QCheckBox()
               self.setCellWidget(i, 1, qb)
        for i in range(0, 7):
            qb = QCheckBox()
            self.setCellWidget(i, 12, qb)

    def update_item_data(self,data):

         data1 = data.split()
         for i in range(6):
            self.setItem(i, 3, QTableWidgetItem(data1[0+ i * 7])) #写入BitQt
            self.setItem(i, 4, QTableWidgetItem(data1[1 + i * 7])) #Bid
            self.setItem(i, 5, QTableWidgetItem(data1[2 + i * 7])) #Ask
            self.setItem(i, 6, QTableWidgetItem(data1[3 + i * 7]))#AskQt
            self.setItem(i, 7, QTableWidgetItem(data1[4 + i * 7]))#Last
            self.setItem(i, 8, QTableWidgetItem(data1[5 + i * 7]))#Close
            self.setItem(i, 10, QTableWidgetItem(data1[6 + i * 7]))#Volumn

class UpdateData(QThread):
    update_date = pyqtSignal(str)
    def __init__(self):
        super().__init__()
    def run(self):

        future1 = MyTable.future1+".CFE"
        future2 = MyTable.future2+".CFE"
        future3 = MyTable.future3+".CFE"
        future4 = MyTable.future4+".CFE"
        future5 = MyTable.future5+".CFE"
        future6 = MyTable.future6+".CFE"

        while True:
            cnt1 = w.wsq(future1, "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")
            cnt2 = w.wsq(future2, "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")
            cnt3 = w.wsq(future3, "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")
            cnt4 = w.wsq(future4,  "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")
            cnt5 = w.wsq(future5,  "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")
            cnt6 = w.wsq(future6,  "rt_bid1,rt_bsize1,rt_ask1,rt_asize1,rt_latest,rt_last,rt_vol")

            cnt = str(cnt1.Data[0][0])+"\n"+str(int(cnt1.Data[1][0]))+"\n"+str(cnt1.Data[2][0])+"\n"+str(int(cnt1.Data[3][0]))+"\n"+str(cnt1.Data[4][0])+"\n"+str(cnt1.Data[5][0])+"\n"+str(int(cnt1.Data[6][0]))+"\n"+\
                  str(cnt2.Data[0][0])+"\n"+str(int(cnt2.Data[1][0]))+"\n"+str(cnt2.Data[2][0])+"\n"+str(int(cnt2.Data[3][0]))+"\n"+str(cnt2.Data[4][0])+"\n"+str(cnt2.Data[5][0])+"\n"+str(int(cnt2.Data[6][0]))+"\n"+\
                  str(cnt3.Data[0][0])+"\n"+str(int(cnt3.Data[1][0]))+"\n"+str(cnt3.Data[2][0])+"\n"+str(int(cnt3.Data[3][0]))+"\n"+str(cnt3.Data[4][0])+"\n"+str(cnt3.Data[5][0])+"\n"+str(int(cnt3.Data[6][0]))+"\n"+\
                  str(cnt4.Data[0][0])+"\n"+str(int(cnt4.Data[1][0]))+"\n"+str(cnt4.Data[2][0])+"\n"+str(int(cnt4.Data[3][0]))+"\n"+str(cnt4.Data[4][0])+"\n"+str(cnt4.Data[5][0])+"\n"+str(int(cnt4.Data[6][0]))+"\n"+\
                  str(cnt5.Data[0][0])+"\n"+str(int(cnt5.Data[1][0]))+"\n"+str(cnt5.Data[2][0])+"\n"+str(int(cnt5.Data[3][0]))+"\n"+str(cnt5.Data[4][0])+"\n"+str(cnt5.Data[5][0])+"\n"+str(int(cnt5.Data[6][0]))+"\n"+\
                  str(cnt6.Data[0][0])+"\n"+str(int(cnt6.Data[1][0]))+"\n"+str(cnt6.Data[2][0])+"\n"+str(int(cnt6.Data[3][0]))+"\n"+str(cnt6.Data[4][0])+"\n"+str(cnt6.Data[5][0])+"\n"+str(int(cnt6.Data[6][0]))#这些是把所有的信息都发送了

            self.update_date.emit(cnt)
            time.sleep(1)



class Color(QWidget):
    def __init__(self,color,*args,**kwargs):
        super(Color,self).__init__(*args,**kwargs)
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QPalette.Window,QColor(color))
        self.setPalette(palette)

class Window(QWidget):
    def __init__(self,parent=None):
        super(Window,self).__init__(parent)

        grid = QGridLayout()
        grid.addWidget(self.SpreadParameters(),0,0)
        grid.addWidget(self.SpreadPosition(),1,0)
        grid.addWidget(self.graph(),1,1)
        grid.addWidget(self.MarketData(),0,1)
        grid.setContentsMargins(1, 1, 1, 1)


        layout1 = QHBoxLayout()
        layout1.addLayout(grid)

        self.setLayout(layout1)
        self.setWindowTitle("TFT1")
        self.setWindowIcon(QIcon("C:\\Users\\cheucemin\\Desktop\\butterfly.png"))
        self.resize(2100,800)


    def createExampleGroup0(self):
        groupBox = QGroupBox("Spread Position")
        radio1 = QRadioButton("&Radio pizza")
        radio2 = QRadioButton("&Radio taco")
        radio3 = QRadioButton("Ra&dio burrito")
        qb = QCheckBox("hit")

        radio1.setChecked(False)

        vbox = QVBoxLayout()
        vbox.addWidget(radio1)
        vbox.addWidget(radio2)
        vbox.addWidget(radio3)
        vbox.addWidget(qb)
        vbox.addStretch(1)

        groupBox.setLayout(vbox)
        return groupBox

    def graph(self):
        groupBox2 = QGroupBox("graph")

        layout1 = QHBoxLayout()
        layout2 = QVBoxLayout()

        layout1.setContentsMargins(1, 1, 1, 1)
        layout2.setContentsMargins(0, 0, 0, 0)

        layout1.setSpacing(10)
        # this is to put the margin a distance

        layout2.addWidget(Color('red'))
        layout2.addWidget(Color('yellow'))
        layout2.addWidget(Color('purple'))

        layout1.addWidget(Color('red'))
        layout1.addWidget(Color('green'))
        layout1.addWidget(Color('blue'))
        layout1.addLayout(layout2)

        groupBox2.setLayout(layout1)
        return groupBox2

    def MarketData(self):
        groupBox3 = QGroupBox("Market Data")

        layout1 = QVBoxLayout()
        layout1.setSpacing(10)
        # this is to put the margin a distance


        layout1.addWidget(MyTable())   #把之前写好的table放到这里来
        groupBox3.setLayout(layout1)
        return groupBox3


    def SpreadParameters(self):

        groupBox = QGroupBox("Spread Parameter")#小组名称
        LastKLineTime = QLabel("LastKLineTime")
        KInterval = QLabel("KInterval")
        ConvergeRate = QLabel("ConvergeRate")
        FairSpread = QLabel("FairSpread")
        PosShift = QLabel("PosShift")
        Speed = QLabel("Speed")
        PosConvex = QLabel("PosConvex")
        Pnl = QLabel("Pnl")
        AdjustSpread = QLabel("AdjustSpread")
        YesterdayBary = QLabel("YesterdayBary")
        MinGainContrib = QLabel("MinGainContrib")
        TodayBary = QLabel("TodayBary")
        MinGainHit = QLabel("MinGainHit")
        FeesExch = QLabel("FeesExch")
        MinInterval= QLabel("MinInterval(ms)")
        FeesBroker = QLabel("FeesBroker")
        HitSize = QLabel("HitSize")
        Total = QLabel("Total")
        Ratio = QLabel("Ratio")
        CanManual = QLabel("CanManual")
        SpeedConvergeRate = QLabel("SpeedConvergeRate")
        btn = QPushButton('AUTO OFF', self)


        KIntervalEdit = QLineEdit()
        ConvergeRateEdit= QLineEdit()
        FairSpreadEdit = QLineEdit()
        PosShiftEdit = QLineEdit()
        SpeedEdit = QLineEdit()
        PosConvexEdit = QLineEdit()
        AdjustSpreadEdit = QLineEdit()
        YesterdayBaryEdit = QLineEdit()
        MinGainContribEdit = QLineEdit()
        TodayBaryEdit = QLineEdit()
        MinGainHitEdit = QLineEdit()
        FeesExchEdit = QLineEdit()
        MinIntervalEdit = QLineEdit()
        FeesBrokerEdit = QLineEdit()
        HitSizeEdit = QLineEdit()
        TotalEdit = QLineEdit()
        RatioEdit = QLineEdit()
        SpeedConvergeRateEdit = QLineEdit()

        radio1 = QCheckBox("&IgnoreUpperDown")
        radio2 = QCheckBox("&Show All")


        gbox = QGridLayout()
        gbox.setSpacing(0)
        gbox.addWidget(LastKLineTime,1,0)
        # 设置时间那一块
        dte = QDateTimeEdit()
        dte.setDateTime(QDateTime.currentDateTime())
        dte.setDisplayFormat("yyyy/MM/dd hh:mm:ss")
        dte.setCalendarPopup(True)
        gbox.addWidget(dte,1,1)



        gbox.addWidget(KInterval,2,0)
        gbox.addWidget(KIntervalEdit,2,1)
        gbox.addWidget(ConvergeRate, 2, 2)
        gbox.addWidget(ConvergeRateEdit, 2, 3)

        gbox.addWidget(FairSpread,3,0)
        gbox.addWidget(FairSpreadEdit,3,1)
        gbox.addWidget(PosShift, 3, 2)
        gbox.addWidget(PosShiftEdit, 3, 3)

        gbox.addWidget(Speed, 4, 0)
        gbox.addWidget(SpeedEdit, 4, 1)
        gbox.addWidget(PosConvex, 4, 2)
        gbox.addWidget(PosConvexEdit, 4, 3)

        gbox.addWidget(Pnl,5,0)
        gbox.addWidget(AdjustSpread, 5, 2)
        gbox.addWidget(AdjustSpreadEdit,5,3)

        gbox.addWidget(YesterdayBary, 6, 0)
        gbox.addWidget(YesterdayBaryEdit, 6, 1)
        gbox.addWidget(MinGainContrib,6,2)
        gbox.addWidget(MinGainContribEdit,6,3)

        gbox.addWidget(TodayBary,7,0)
        gbox.addWidget(TodayBaryEdit,7,1)
        gbox.addWidget(MinGainHit, 7, 2)
        gbox.addWidget(MinGainHitEdit, 7, 3)

        gbox.addWidget(FeesExch, 8, 0)
        gbox.addWidget(FeesExchEdit, 8, 1)
        gbox.addWidget(MinInterval, 8, 2)
        gbox.addWidget(MinIntervalEdit, 8, 3)

        gbox.addWidget(FeesBroker, 9, 0)
        gbox.addWidget(FeesBrokerEdit, 9, 1)
        gbox.addWidget(HitSize, 9, 2)
        gbox.addWidget(HitSizeEdit, 9, 3)

        gbox.addWidget(Total, 10, 0)
        gbox.addWidget(TotalEdit, 10, 1)
        gbox.addWidget(Ratio, 10, 2)
        gbox.addWidget(RatioEdit, 10, 3)

        gbox.addWidget(CanManual, 11, 0)
        gbox.addWidget(SpeedConvergeRate, 11, 2)
        gbox.addWidget(SpeedConvergeRateEdit, 11, 3)

        gbox.addWidget(radio1,12,0)
        gbox.addWidget(radio2, 12,1)
        gbox.addWidget(btn, 12, 2)

        layout1 = QVBoxLayout()
        layout1.addLayout(gbox)
        groupBox.setLayout(layout1)
        return groupBox

    def SpreadPosition(self):

        groupBox1 = QGroupBox("Spread Position")
        SpreadPosition = QLabel("SpreadPosition")
        SpreadHit = QLabel("SpreadHit")
        SpreadPosition1 = QLabel("Spread-Position")
        Position1 = QLabel("Position1")
        Position2 = QLabel("Position2")
        MaxPosition = QLabel("MaxPosition")
        SpreadContrib = QLabel("SpreadContrib")
        MinGainContribExit = QLabel("MinGainContribExit")
        radio3 = QCheckBox("&SpreadHit")
        radio4 = QCheckBox("&SpreadContr")
        radio5 = QCheckBox("&ConBuy")
        radio6 = QCheckBox("&ConSell")
        BidHit = QPushButton('BidHit', self)
        AskHit = QPushButton('AskHit', self)
        BidContrib = QPushButton('BidContrib', self)
        AskContrib = QPushButton('AskContrib', self)
        radio7 = QCheckBox("&UserMarket")
        radio8 = QCheckBox("&KeepOrders")
        MaxWaitInterval = QLabel("MaxWaitInterval(s)")
        ContribMinDiff = QLabel("ContribMinDiff")
        ContribSize = QLabel("ContribSize")
        ContribMaxDistance = QLabel("ContribMaxDistance")
        MaxNumberofOrder = QLabel("MaxNumberofOrder")

        SpreadPosition1Edit = QLineEdit()
        SpreadHitask = QLineEdit()
        SpreadHitbid = QLineEdit()
        Postion1Edit = QLineEdit()
        Position2Edit = QLineEdit()
        MaxPositionEdit = QLineEdit()
        SpreadContribask = QLineEdit()
        SpreadContribbid = QLineEdit()
        MinGainContribExitEdit = QLineEdit()
        MaxWaitIntervalEdit = QLineEdit()
        ContribMinDiffEdit = QLineEdit()
        ContribSizeEdit = QLineEdit()
        ContribMaxDistanceEdit = QLineEdit()
        MaxNumberofOrderEdit = QLineEdit()

        gbox1 = QGridLayout()
        gbox1.setSpacing(1)
        gbox1.addWidget(SpreadPosition,0,0)
        gbox1.addWidget(SpreadHit,0,2)

        gbox1.addWidget(SpreadPosition1,1,0)
        gbox1.addWidget(SpreadPosition1Edit,1,1)
        gbox1.addWidget(SpreadHitbid, 1, 2)
        gbox1.addWidget(SpreadHitask,1,3)

        gbox1.addWidget(Position1,2,0)
        gbox1.addWidget(Postion1Edit,2,1)
        gbox1.addWidget(SpreadContrib,2,2)

        gbox1.addWidget(Position2,3,0)
        gbox1.addWidget(Position2Edit,3,1)
        gbox1.addWidget(SpreadContribbid,3,2)
        gbox1.addWidget(SpreadContribask,3,3)


        gbox1.addWidget(MaxPosition,4,0)
        gbox1.addWidget(MaxPositionEdit,4,1)
        gbox1.addWidget(MinGainContribExit, 4, 2)
        gbox1.addWidget(MinGainContribExitEdit, 4, 3)

        gbox1.addWidget(radio3, 5, 0)
        gbox1.addWidget(radio4, 5, 1)
        gbox1.addWidget(MaxWaitInterval, 5, 2)
        gbox1.addWidget(MaxWaitIntervalEdit, 5, 3)

        gbox1.addWidget(radio5, 6, 0)
        gbox1.addWidget(radio6, 6, 1)
        gbox1.addWidget(ContribMinDiff, 6, 2)
        gbox1.addWidget(ContribMinDiffEdit, 6, 3)


        gbox1.addWidget(BidHit, 7, 0)
        gbox1.addWidget(AskHit, 7, 1)
        gbox1.addWidget(ContribSize, 7, 2)
        gbox1.addWidget(ContribSizeEdit, 7, 3)

        gbox1.addWidget(BidContrib, 8, 0)
        gbox1.addWidget(AskContrib, 8, 1)
        gbox1.addWidget(ContribMaxDistance, 8, 2)
        gbox1.addWidget(ContribMaxDistanceEdit, 8, 3)

        gbox1.addWidget(radio7, 9, 0)
        gbox1.addWidget(radio8, 9, 1)
        gbox1.addWidget(MaxNumberofOrder, 9,2)
        gbox1.addWidget(MaxNumberofOrderEdit, 9, 3)

        groupBox1.setLayout(gbox1)
        return groupBox1
    #event part
    def closeEvent(self,event):

            reply = QMessageBox.question(self,'Message',"Are you sure to quit now?",QMessageBox.Yes|
                                         QMessageBox.No, QMessageBox.No)
            #We show a message box with two buttons: Yes and No. The first string appears on the
            #titlebar. The second string is the message text displayed by the dialog.
            if reply == QMessageBox.Yes:
                event.accept()
            else:
                event.ignore()


class MainWindow(QMainWindow):
    def __init__(self,*args,**kwargs):
        super(MainWindow,self).__init__(*args,**kwargs)
        self.setWindowTitle("TFT1")
        self.setWindowIcon(QIcon("C:\\Users\\cheucemin\\Desktop\\butterfly.png"))
        self.setGeometry(50, 50, 1100, 400) #参数分别代表，窗口的位置，宽度和高度

        tabs = QTabWidget()
        tabs.setDocumentMode(True)
        tabs.setTabPosition(QTabWidget.South) #设置tab的地方
        tabs.addTab(Window(),'spread trading')
        tabs.addTab(Color('blue'),'Position')
        self.setCentralWidget(tabs)



if __name__ == '__main__':
    w.start()
    app = QApplication(sys.argv)
    clock = MainWindow()
    clock.show()
    sys.exit(app.exec_())

