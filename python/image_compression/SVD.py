import matplotlib.pylab as pylab
import matplotlib.image as im
import numpy as np

def svd(matrix,rate):
    u, sigma, v = np.linalg.svd(matrix[:, :])
    m = len(u)
    n = len(v)
    a = np.zeros((m, n))

    count = (int)(sum(sigma))
    curSum = 0
    k = 0
    while curSum <= count * rate:
        uk = u[:, k].reshape(m, 1)
        vk = v[k].reshape(1, n)
        a += sigma[k] * np.dot(uk, vk)
        curSum += sigma[k]
        k += 1
    a[a < 0] = 0
    a[a > 255] = 255
    return np.rint(a).astype("uint8")


img_file=im.imread('butterfly.bmp')
img_rgb=np.array(img_file)
rate=0.8

img_r, img_g, img_b = img_rgb[:, :, 0], img_rgb[:, :, 1], img_rgb[:, :, 2]
img_r_svd=svd(img_r,rate)
img_g_svd=svd(img_g,rate)
img_b_svd=svd(img_b,rate)
img_rgb_svd = np.stack((img_r_svd, img_g_svd, img_b_svd), 2)

pylab.figure(1)
pylab.imshow(img_rgb)
pylab.axis('off')

pylab.figure(2)
pylab.imshow(img_rgb_svd)
pylab.axis('off')

pylab.show()

