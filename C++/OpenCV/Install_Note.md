### OpenCV win10 安装记录 (for vs2015)
(vc14 for vs2015; vc15 for vs2017)

1. Downloads from [openCV官网](https://opencv.org/releases.html)
2. run opencv-4.5.2-vc14_vc15.exe

3. add PATH to SystemVariable
D:\gcc_g++\OpenCV\4.5.2\opencv\build\x64\vc14\bin\

4. 注销

### 针对单个VS2015 项目 添加OpenCV环境：

1. Debug X86改为X64

2. 打开项目属性页(Alt + Enter)， VC++目录 栏中的包含目录，
	添加

 * `...opencv\build\include`
 * `...opencv\build\include\opencv2`

库目录，添加

   * `...opencv\build\x64\vc14\lib`

3. 项目属性页(Alt + Enter), 链接器-输入-附加依赖项,添加 (下面二选一)
4. 
   `(1)Debug Mode: opencv_world452d.lib`
   
   `(2)Release Mode: opencv_world452.lib`
   **(452 对应版本4.5.2)**

### 测试代码   
``` cpp

#include <opencv2\opencv.hpp>

int main() {
	cv::Mat src = cv::imread("images\\src.bmp");
	cv::imshow("Output",src);

	cv::waitKey(0);
	return 0;
}
```