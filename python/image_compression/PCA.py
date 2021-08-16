import matplotlib.pylab as pylab
import matplotlib.image as im
import numpy as np

def pca(matrix,num_k):
    num_row,num_column=matrix.shape
    std_mat=matrix-np.ones((num_row,num_column))*np.mean(matrix)    #矩阵的标准化
    cov_mat=np.dot(np.transpose(std_mat),std_mat)
    (cha_val,cha_vec)=np.linalg.eig(cov_mat)                       #特征值与特征向量
    cha_index=np.argsort(cha_val)
    vecindex=cha_index[:-(num_k+1):-1]
    feature=cha_vec[:,vecindex]
    new_mat=np.dot(std_mat,feature)
    return (np.dot(new_mat,np.transpose(feature))+np.mean(matrix))


def to_grey(img_rgb):
    img_r,img_g,img_b = img_rgb[:, :, 0],img_rgb[:, :, 1],img_rgb[:, :, 2]
    img_grey=0.2989*img_r+0.5870*img_g+0.1140*img_b
    return img_grey


img_file=im.imread('butterfly.bmp')
img_rgb=np.array(img_file)
(num_row,num_column,num_channal) = img_rgb.shape
img_grey=to_grey(img_rgb)

pylab.figure(1)
pylab.imshow(img_grey,cmap="gray")
pylab.axis('off')

pylab.figure(2)
pylab.imshow(np.real(pca(img_grey,15)),cmap="gray")
pylab.axis('off')

pylab.show()

