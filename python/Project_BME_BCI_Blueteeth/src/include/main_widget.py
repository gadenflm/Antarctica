# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'main_widget.ui'
#
# Created by: PyQt5 UI code generator 5.15.1
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(1225, 887)
        self.btn_openfile = QtWidgets.QPushButton(Form)
        self.btn_openfile.setGeometry(QtCore.QRect(60, 320, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_openfile.setFont(font)
        self.btn_openfile.setObjectName("btn_openfile")
        self.groupBox_data = QtWidgets.QGroupBox(Form)
        self.groupBox_data.setEnabled(True)
        self.groupBox_data.setGeometry(QtCore.QRect(40, 20, 961, 281))
        self.groupBox_data.setContextMenuPolicy(QtCore.Qt.DefaultContextMenu)
        self.groupBox_data.setObjectName("groupBox_data")
        self.btn_debug = QtWidgets.QPushButton(Form)
        self.btn_debug.setGeometry(QtCore.QRect(1060, 240, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_debug.setFont(font)
        self.btn_debug.setObjectName("btn_debug")
        self.btn_1 = QtWidgets.QPushButton(Form)
        self.btn_1.setGeometry(QtCore.QRect(1060, 140, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_1.setFont(font)
        self.btn_1.setObjectName("btn_1")
        self.btn_2 = QtWidgets.QPushButton(Form)
        self.btn_2.setGeometry(QtCore.QRect(1060, 190, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_2.setFont(font)
        self.btn_2.setObjectName("btn_2")
        self.btn_0 = QtWidgets.QPushButton(Form)
        self.btn_0.setGeometry(QtCore.QRect(1060, 90, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_0.setFont(font)
        self.btn_0.setObjectName("btn_0")
        self.combox_com = QtWidgets.QComboBox(Form)
        self.combox_com.setGeometry(QtCore.QRect(220, 320, 231, 41))
        font = QtGui.QFont()
        font.setFamily("Adobe Heiti Std")
        font.setPointSize(14)
        self.combox_com.setFont(font)
        self.combox_com.setObjectName("combox_com")
        self.btn_refre_com = QtWidgets.QPushButton(Form)
        self.btn_refre_com.setGeometry(QtCore.QRect(470, 320, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_refre_com.setFont(font)
        self.btn_refre_com.setObjectName("btn_refre_com")
        self.btn_com_connect = QtWidgets.QPushButton(Form)
        self.btn_com_connect.setGeometry(QtCore.QRect(600, 320, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_com_connect.setFont(font)
        self.btn_com_connect.setObjectName("btn_com_connect")
        self.lbl_pic_left = QtWidgets.QLabel(Form)
        self.lbl_pic_left.setGeometry(QtCore.QRect(190, 650, 100, 100))
        self.lbl_pic_left.setObjectName("lbl_pic_left")
        self.lbl_pic_car = QtWidgets.QLabel(Form)
        self.lbl_pic_car.setGeometry(QtCore.QRect(340, 550, 300, 300))
        self.lbl_pic_car.setText("")
        self.lbl_pic_car.setObjectName("lbl_pic_car")
        self.btn_start = QtWidgets.QPushButton(Form)
        self.btn_start.setGeometry(QtCore.QRect(730, 320, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_start.setFont(font)
        self.btn_start.setObjectName("btn_start")
        self.btn_stop = QtWidgets.QPushButton(Form)
        self.btn_stop.setGeometry(QtCore.QRect(860, 320, 121, 41))
        font = QtGui.QFont()
        font.setFamily("微软雅黑")
        font.setPointSize(12)
        self.btn_stop.setFont(font)
        self.btn_stop.setObjectName("btn_stop")
        self.lbl_pic_right = QtWidgets.QLabel(Form)
        self.lbl_pic_right.setGeometry(QtCore.QRect(690, 650, 100, 100))
        self.lbl_pic_right.setObjectName("lbl_pic_right")
        self.lbl_pic_up = QtWidgets.QLabel(Form)
        self.lbl_pic_up.setGeometry(QtCore.QRect(460, 410, 50, 100))
        self.lbl_pic_up.setObjectName("lbl_pic_up")

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.btn_openfile.setText(_translate("Form", "选择脑电数据"))
        self.groupBox_data.setTitle(_translate("Form", "GroupBox"))
        self.btn_debug.setText(_translate("Form", "Debug"))
        self.btn_1.setText(_translate("Form", "分类1"))
        self.btn_2.setText(_translate("Form", "分类2"))
        self.btn_0.setText(_translate("Form", "分类0"))
        self.btn_refre_com.setText(_translate("Form", "刷新"))
        self.btn_com_connect.setText(_translate("Form", "连接"))
        self.lbl_pic_left.setText(_translate("Form", "<html><head/><body><p><br/></p></body></html>"))
        self.btn_start.setText(_translate("Form", "开始"))
        self.btn_stop.setText(_translate("Form", "停止"))
        self.lbl_pic_right.setText(_translate("Form", "<html><head/><body><p><br/></p></body></html>"))
        self.lbl_pic_up.setText(_translate("Form", "<html><head/><body><p><br/></p></body></html>"))
