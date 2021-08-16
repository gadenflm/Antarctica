from PyQt5.QtWidgets import *
from PyQt5.QtCore import QTimer
from PyQt5 import QtWidgets,QtGui

import sys
import os
import math
import serial
import serial.tools.list_ports
import numpy as np

from src.include import Ui_Form
from src.include import fig_plot
from src.include.ClsDemo.micls import MotorImageryClassifier

class main_win(QMainWindow, Ui_Form):
    openfile_dic=''

    def __init__(self):
        super(main_win, self).__init__()
        self.setupUi(self)
        self.setWindowTitle("EEG脑电——小车控制系统")
        self.init()

    def init(self):
        #parameters:
        self.plot_timer=QTimer(self)
        self.run_timer=QTimer(self)
        self.len_seq=0

        #enable:
        self.btn_start.setEnabled(0)
        self.btn_stop.setEnabled(0)

        self.fig_init()
        self.micls_init()
        #initial interaction:
        self.plot_timer.timeout.connect(self.timer_plot)
        self.run_timer.timeout.connect(self.timer_run)

        self.btn_openfile.clicked.connect(self.open_file)
        self.btn_refre_com.clicked.connect(self.com_refre)
        self.btn_com_connect.clicked.connect(self.connect_com)
        self.btn_start.clicked.connect(self.func_start)
        self.btn_stop.clicked.connect(self.func_stop)
        self.btn_debug.clicked.connect(self.debug_func)
        self.btn_0.clicked.connect(self.func_0)
        self.btn_1.clicked.connect(self.func_1)
        self.btn_2.clicked.connect(self.func_2)

    def micls_init(self):
        self.classifier = MotorImageryClassifier(root='../src/include/ClsDemo')
        seq_init = '0001020100'
        self.classifier.seq2eeg(seq_init,'../Data/seq_test',"../Data/")

    def fig_change(self,act):

        if (act == 1):
            self.lbl_pic_up.setPixmap(self.png_up_light)
            self.lbl_pic_left.setPixmap(self.png_left_dark)
            self.lbl_pic_right.setPixmap(self.png_right_dark)
            self.lbl_pic_car.setPixmap(self.png_car)
        elif (act == 2):
            self.lbl_pic_up.setPixmap(self.png_up_dark)
            self.lbl_pic_left.setPixmap(self.png_left_light)
            self.lbl_pic_right.setPixmap(self.png_right_dark)
            self.lbl_pic_car.setPixmap(self.png_car)
        elif (act == 4):
            self.lbl_pic_up.setPixmap(self.png_up_dark)
            self.lbl_pic_left.setPixmap(self.png_left_dark)
            self.lbl_pic_right.setPixmap(self.png_right_light)
            self.lbl_pic_car.setPixmap(self.png_car)
        else:
            self.lbl_pic_up.setPixmap(self.png_up_dark)
            self.lbl_pic_left.setPixmap(self.png_left_dark)
            self.lbl_pic_right.setPixmap(self.png_right_dark)
            self.lbl_pic_car.setPixmap(self.png_car)

    def car_move(self,act):
        if (act==1):
            temp_hex=bytes.fromhex('A55A04B2B6AA')  #前进
        elif (act==2):
            temp_hex = bytes.fromhex('A55A04B4B8AA')  #左转
        elif (act==4):
            temp_hex = bytes.fromhex('A55A04B6BAAA') #右转
        elif (act==5):
            temp_hex = bytes.fromhex('A55A04B8BCAA')  #后退
        else:
            temp_hex = bytes.fromhex('A55A04B5B9AA')  #停止
        try:
            self.car_dev.write(temp_hex)
        except OSError:
            print ("hex send error")

    def car_dev_connect(self):
        try:
            self.car_dev = serial.Serial('com6', 9600, timeout=0.5)
        except OSError:
            print ("car device connect error")

    def com_refre(self):
        try:
            port_list = list(serial.tools.list_ports.comports())
        except OSError:
            print('com_refre error')
        for i in range(0, len(port_list)):
            self.combox_com.addItem(" %s " % (port_list[i]))
      #  self.btn_com_connect.setEnabled(1)


    def connect_com(self):
        self.car_dev_connect()

    def timer_run(self):
        if (self.seq_index<self.len_seq):
            self.run_timer_index = self.run_timer_index + 1
            if(self.run_timer_index % 10 == 0):
                self.seq_index+=1
            self.com_send(self.seq_pred[self.seq_index])
        else:
            self.car_move(3)
         #   if (self.plot_timer.isActive()):
         #       self.plot_timer.stop()
            pass

    def func_start(self):
       self.plot_timer_index=0
       self.plot_timer.start(100)
       self.run_timer_index = 0
       self.seq_index = 0
       self.run_timer.start(100)

    def func_stop(self):
        self.plot_timer.stop()
        self.run_timer.stop()

    def com_send(self,flag):
        if (flag=='0'):
            self.car_move(1)
            self.fig_change(1)
        if (flag == '1'):
            self.car_move(2)
            self.fig_change(2)
        if (flag == '2'):
            self.car_move(4)
            self.fig_change(4)
        if (flag == '!'):
            self.car_move(3)
            self.fig_change(3)

    def debug_func(self):
        png_up_light = QtGui.QPixmap('../Img/up_light.png')
        self.lbl_pic_up.setPixmap(png_up_light)

    def func_0(self):
        self.car_move(1)

    def func_1(self):
        self.car_move(2)

    def func_2(self):
        self.car_move(3)

    def fig_init(self):
        #initial lbl_pic (up left right):
        self.png_up_light = QtGui.QPixmap('../Img/up_light.png')
        self.png_left_light = QtGui.QPixmap('../Img/left_light.png')
        self.png_right_light = QtGui.QPixmap('../Img/right_light.png')
        self.png_up_dark=QtGui.QPixmap('../Img/up_dark.png')
        self.png_left_dark = QtGui.QPixmap('../Img/left_dark.png')
        self.png_right_dark = QtGui.QPixmap('../Img/right_dark.png')
        self.png_car=QtGui.QPixmap('../Img/car.png')
        self.lbl_pic_up.setPixmap(self.png_up_dark)
        self.lbl_pic_left.setPixmap(self.png_left_dark)
        self.lbl_pic_right.setPixmap(self.png_right_dark)
        self.lbl_pic_car.setPixmap(self.png_car)

        #initial group box for figure plot:
        self.fig_eeg = fig_plot(width=30, height=30, dpi=100)
        self.gridlayout = QGridLayout(self.groupBox_data)
        self.gridlayout.addWidget(self.fig_eeg,0,1)

    def open_file(self):
        #function for open BCI_data file
        self.micls_init()
        fileName, fileType = QtWidgets.QFileDialog.getOpenFileName(self, "选取文件", os.getcwd(),"EEG Data Files(*.npy)")
        self.openfile_dic=fileName
        try:
            temp_data = self.classifier.load_seq(fileName)
        except OSError:
            print('load_seq error')
        self.seq_pred = ''
        temp_arr=[]
        for i in range(self.classifier.len_seq):
            temp_ret = self.classifier.get_next()
            ret = self.classifier.predict(temp_ret)
            temp_arr.append(temp_ret)
            self.seq_pred += str(ret)

        self.seq_pred +='!!!!!'
        self.len_seq=len(temp_arr)
        rand_arr_01 = 0.001 * (np.random.random(size=[32, len(temp_arr) * 1000]) - 0.5)

        rand_arr =  rand_arr_01
        temp_arr=np.array(temp_arr).reshape(32,len(temp_arr)*1000)

        temp_arr=temp_arr+rand_arr
        self.eeg_data=temp_arr
        self.plot_data(self.eeg_data, [0, 1, 2], 0, 500)
       # print('seq_pred:', self.seq_pred)
        self.btn_start.setEnabled(1)

    def timer_plot(self):
        if (self.plot_timer_index<=self.len_seq*10-6):
            self.plot_timer_index = self.plot_timer_index + 1
            self.plot_data(self.eeg_data,[0,1,2,3,4,5,6,7],self.plot_timer_index*100,(self.plot_timer_index+5)*100)
        else:
            pass
         #   self.timer_plot.stop()

    def plot_data(self,data,channal,s_point,e_point):

        x_axis=list(range(s_point,e_point))
        self.fig_eeg.axes1.clear()
        self.fig_eeg.axes2.clear()
        self.fig_eeg.axes3.clear()
        self.fig_eeg.axes4.clear()
        self.fig_eeg.axes5.clear()
        self.fig_eeg.axes6.clear()
        self.fig_eeg.axes7.clear()
        self.fig_eeg.axes8.clear()
        self.fig_eeg.axes1.plot(x_axis,data[channal[0],s_point:e_point],'r')
        self.fig_eeg.axes2.plot(x_axis,data[channal[1],s_point:e_point],'g')
        self.fig_eeg.axes3.plot(x_axis,data[channal[2],s_point:e_point],'b')
        self.fig_eeg.axes4.plot(x_axis,data[3,s_point:e_point],'y')
        self.fig_eeg.axes5.plot(x_axis,data[4,s_point:e_point],'r')
        self.fig_eeg.axes6.plot(x_axis,data[5,s_point:e_point],'b')
        self.fig_eeg.axes7.plot(x_axis,data[6,s_point:e_point],'y')
        self.fig_eeg.axes8.plot(x_axis,data[7,s_point:e_point],'g')
        self.fig_eeg.draw()

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    win = main_win()
    win.show()
    sys.exit(app.exec_())