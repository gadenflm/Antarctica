import numpy as np
from scipy.io import loadmat

def get_mean(data): #平均数
    num_row,num_column=data.shape
    mean_data=np.zeros(num_row)
    for i in range(0,num_row):
        mean_data[i]=np.mean(data[i])
    return mean_data

def get_var(data): #方差
    num_row,num_column=data.shape
    var_data=np.zeros(num_row)
    for i in range(0,num_row):
        var_data[i]=np.var(data[i])
    return var_data

def get_fft(data): #功率谱
    num_row,num_column=data.shape
    fft_data=np.zeros((num_row,num_column))
    for i in range(0,num_row):
        fft_data[i]=20*np.log10(np.abs((np.fft.fft(data[i])))**2)
    return fft_data

mat_file_path= '../Data/block1.mat'
mat_data=loadmat(mat_file_path)
data=mat_data['data']


