### 1.目录一（存放待编译源码和对应编译指导文件）：
./applicaitons/sample/camera/apps

apps/src/hello_tvt.c


```
#include <stdio.h>
int main(int argc, char **argv){
    printf ("Hello Tvt!\n");
    return 0;
}
```



apps/BUILD.gn

```
import("//build/lite/config/component/lite_component.gni")
lite_component("hello-OHOS") {
  features = [ ":helloworld" ]
}
executable("helloworld") {
  output_name = "helloworld"
  sources = [ "src/hello_tvt.c" ]  <!-- 这行定位执行文件 -->
  include_dirs = []				<!-- 编译参数 -->
  defines = []					<!-- 编译参数 -->
  cflags_c = []				    <!-- 编译参数 -->
  ldflags = []					<!-- 编译参数 -->
}
```


### 2.修改文件（添加新组件）：
build/lite/components/applications.json 中添加：

```
{
      "component": "hello_world_app",
      "description": "Communication related samples.",
      "optional": "true",
      "dirs": [
        "applications/sample/camera/apps"
      ],
      "targets": [
        "//applications/sample/camera/apps:hello-OHOS"
      ],
      "rom": "",
      "ram": "",
      "output": [],
      "adapted_kernel": [ "liteos_a" ],
      "features": [],
      "deps": {
        "components": [],
        "third_party": []
      }
    },
```


### 3.修改文件（修改单板配置文件）：
vendor/hisilicon/hispark_taurus/config.json 在"subsystem": "applications"的 components中添加：

```
{ "component": "hello_world_app", "features":[] },
```


### 4.编译

