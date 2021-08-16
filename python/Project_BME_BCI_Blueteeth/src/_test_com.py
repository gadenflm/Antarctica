import serial #导入模块

com6= serial.Serial('com6', 9600, timeout=0.5)

run_hex=bytes.fromhex('A55A04B2B6AA')  #前进
stop_hex=bytes.fromhex('A55A04B5B9AA')  #停止
left_hex=bytes.fromhex('A55A04B4B8AA')  #左转
right_hex=bytes.fromhex('A55A04B6BAAA') #右转
back_hex=bytes.fromhex('A55A04B8BCAA')  #后退
com6.write(stop_hex)

#5A04B2B6AA