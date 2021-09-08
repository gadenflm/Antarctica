### sh->bash

build_release.sh
platform/ambarella/build.sh 
driver/Sdkfunc/libsdk/build.sh
burn_img/ambarella/build.sh
burn_img/ambarella/script/*.sh


### 修改shell 从dash->bash
`sudo dpkg-reconfigure dash`

选择no

### 编译so(libsdk_func)时报错，找不到libasound.so 
解决：找到编译amba_sdk时得到的libasound.so.2.0.0(1Mb)，复制到`tvt_build/output/E4H_4M/cv28m_cashewnut/library`

