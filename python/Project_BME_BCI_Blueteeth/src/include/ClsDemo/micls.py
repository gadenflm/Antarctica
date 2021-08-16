import joblib
import os
import mne
import numpy as np


def numpy2mne(eeg_data, eeg_labs=None):
	channels = 32#eeg_data.shape[1]
	# 数据集MNE化
	#################################################################
	print('numpy2mne:', eeg_data.shape)#(2722, 7, 1000)
	eeg_data = eeg_data.reshape(-1,channels,1000)
	
	# 创建一个info结构
	#################################################################
	info = mne.create_info(
		ch_names=channels,#['EEG',],
		ch_types='eeg',#['eeg',],
		sfreq=256
	)

	epochs = mne.EpochsArray(eeg_data, info)
	return epochs

def filter_band(eeg_data, eeg_labs=None, low=5, high=9):
	'''7-30Hz滤波
	'''
	epochs = numpy2mne(eeg_data, eeg_labs)
	epochs = epochs.filter(low, high, fir_design='firwin', skip_by_annotation='edge')
	eeg_data = epochs.get_data()
	return eeg_data, eeg_labs


class MotorImageryClassifier:#简称：micls
	'''
	设置文件根路径root，此路径下包含：
		处理文件：csp.pkl
		分类模型：forest.pkl
	'''
	def __init__(self, root=r'D:\Minecode\py\BMEgame_BCI\src\include\ClsDemo'):
		super(MotorImageryClassifier, self).__init__()

		self.root = root
		self.forest = joblib.load(os.path.join(root, 'forest.pkl'))
		self.csp = joblib.load(os.path.join(root, 'csp.pkl'))

	def preprocess(self, eeg_data):
		eeg_data, _ = filter_band(eeg_data)
		# csp = joblib.load(os.path.join(root, 'csp.pkl'))
		eeg_data = self.csp.transform(eeg_data)
		return eeg_data
	
	@staticmethod
	def load_npy(npy_path='left.npy'):
		eeg_data = np.load(npy_path)
		eeg_data = eeg_data.reshape(1,32,1000)
		return eeg_data

	def predict(self, eeg_data):
		'''
		输入一条脑电数据：1*32*1000即前32导联，1000个点（250Hz，四秒钟）
		返回类别：none=0, left=1, right=2
		'''
		eeg_data = self.preprocess(eeg_data)#预处理
		return self.forest.predict(eeg_data)[0]

	eeg_seq = None
	len_seq = 0

	def load_seq(self, npy_path='seq.npy'):
		eeg_data = np.load(npy_path)
		eeg_data = eeg_data.reshape(-1,32,1000)
		self.eeg_seq = eeg_data
		self.len_seq = self.eeg_seq.shape[0]
		return eeg_data

	index = 0
	def get_next(self):
		eeg = self.eeg_seq[self.index:self.index+1]
		self.index += 1
		return eeg
	
	@staticmethod
	def seq2eeg(seq='012', save_npy='seq', root=r'D:\Minecode\py\BMEgame_BCI\src\include\ClsDemo'):
		'''
		输入：0-2的字符串
		输出：对应的EEG连续脑电信号(n*32*1000)
		'''
		none = np.load(os.path.join(root, 'none.npy'))
		left = np.load(os.path.join(root, 'left.npy'))
		right = np.load(os.path.join(root, 'right.npy'))
		eeg_choices = [none, left, right]

		eeg_data = []
		for i in list(seq):
			i = int(i)
			if i not in [0,1,2]:
				raise EnvironmentError
			eeg_data.append(eeg_choices[i])
		eeg_data = np.concatenate(eeg_data)
		#print('seq2eeg:', eeg_data.shape)
		if isinstance(save_npy, str):
			np.save(save_npy, eeg_data)



# 测试：预测
###################################################################
def test(npy='left.npy'):
	classifier = MotorImageryClassifier(root=root)
	eeg = classifier.load_npy(npy)
	print('Step1-load_npy:', eeg.shape)
	ret = classifier.predict(eeg)
	print('Step3-predict:', ret)






if __name__ == '__main__':
	root=r'D:\MineProjects\Python\BCI_blueth_car\src\include\ClsDemo'

		
	# 测试：拼接
	###################################################################
	seq_init = '002122221111010101022'
	seq_pred = ''

	classifier = MotorImageryClassifier(root=root)
	classifier.seq2eeg(seq_init)
	classifier.load_seq('seq.npy')
	for i in range(classifier.len_seq):
		ret = classifier.predict(classifier.get_next())
		seq_pred += str(ret)
	print('seq_init:', seq_init)
	print('seq_pred:', seq_pred)
	# 002122221111010101022
	# 002122221111010101022



	# 测试：预测
	###################################################################
	# test('none.npy')
	# test('left.npy')
	# test('right.npy')

	'''
	输出：

	Step1-load_npy: (1, 32, 1000) 
	Step3-predict: 0

	Step1-load_npy: (1, 32, 1000)
	Step3-predict: 1

	Step1-load_npy: (1, 32, 1000)
	Step3-predict: 2
	'''


	# 测试
	###################################################################
	# forest = joblib.load(os.path.join(root, 'forest.pkl'))

	# # event_id = dict(none=0, left=1, right=2)
	# # data = preprocess(npy_path='right.npy')#2
	# data = preprocess(npy_path='left.npy')#2
	# # data = preprocess(npy_path='none.npy')#0
	# ret = forest.predict(data)
	# print(ret)
