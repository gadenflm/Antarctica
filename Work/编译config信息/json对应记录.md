# 鸿蒙编译配置文件config.json中子系统对应记录 
---
```
      {
        "subsystem": "aafwk",
        "components": [
          { "component": "ability", "features":[ "enable_ohos_appexecfwk_feature_ability = true" ] }
        ]
      },
```
用户程序框架子系统，为开发者提供的一套开发OpenHarmony应用的框架

---
```
      {
        "subsystem": "applications",
        "components": [
          { "component": "camera_sample_app", "features":[] },
          { "component": "camera_screensaver_app", "features":[] }
        ]
      },
```
####应用
camera_sample_app，可以对应媒体子系统


---
```
      {
        "subsystem": "hiviewdfx",
        "components": [
          { "component": "hilog", "features":[] }
        ]
      },
```
DFX（Design for X）子系统是为了提升质量属性软件设计，目前包含的内容主要有：DFR（Design for Reliability，可靠性）和DFT（Design for Testability，可测试性）特性。

+ hilog_lite部件，包含轻量和小型系统的流水日志能力

---
```      
      {
        "subsystem": "distributed_schedule",
        "components": [
          { "component": "system_ability_manager", "features":[] },
          { "component": "foundation", "features":[] },
          { "component": "distributed_schedule", "features":[] }
        ]
      },
```
####分布式任务调度子系统
分布式任务调度模块负责跨设备组件管理，提供访问和控制远程组件的能力，支持分布式场景下的应用协同。

+ system_ability_manager 系统服务框架实现
+ distributed_schedule 分布式任务调度实现
+ foundation foundation进程实现

---
```
      {
        "subsystem": "security",
        "components": [
          { "component": "hichainsdk", "features":[] },
          { "component": "permission", "features":[] },
          { "component": "app_verify", "features":[] }
        ]
      },
```

安全子系统,提供样例给开发者展示如何去使用已有的安全机制来提升系统的安全能力，包括安全启动、应用权限管理、IPC通信鉴权、HUKS、HiChain、应用签名验签。

---
```      
      {
        "subsystem": "multimedia",
        "components": [
          { "component": "media_service", "features":[] }
        ]
      },
```
####媒体子系统
存放媒体子系统的源码信息，旨在为多媒体应用开发者开发者提供统一的开发接口。
要求（C++11以上）,需要提前加载内核及相关驱动。

配置合适的配置文件，可以参考applications/sample/camera/media下配置文件，如果适配其他sensor可在开源社区中求助。用户使用时将配置文件放入到开发板/storage/data目录，开发者通过该配置文件可以去适配sensor及分辨率、帧率等能力。

---
```      
      {
        "subsystem": "graphic",
        "components": [
          { "component": "graphic_utils", "features":[] },
          { "component": "graphic_hals", "features":[] },
          { "component": "ui", "features":[ "enable_graphic_font = true" ] },
          { "component": "surface", "features":[] },
          { "component": "wms", "features":[] }
        ]
      },
```
图形子系统，主要包括UI组件、布局、动画、字体、输入事件、窗口管理、渲染绘制等模块，构建基于轻量OS的应用框架，满足硬件资源较小的物联网设备的OpenHarmony系统应用开发。

---
```      
      {
        "subsystem": "sensors",
        "components": [
          { "component": "sensor_lite", "features":[] }
        ]
      },
```
####泛Sensor子系统
提供如下功能：Sensor列表查询、Sensor启停、Sensor订阅/去订阅、设置数据上报模式等

---
```      
      {
        "subsystem": "startup", 
        "components": [
          { "component": "syspara", "features":[] },
          { "component": "bootstrap", "features":[] },
          { "component": "init", "features":[] },
          { "component": "appspawn", "features":[] }
        ]
      },
```
####启动恢复子系统
启动恢复负责在内核启动之后到应用启动之前的系统关键进程和服务的启动过程。
+ init组件 负责处理从内核加载第一个用户态进程开始，到第一个应用程序启动之间的系统服务进程启动过程。
+ appspawn应用孵化器组件 负责接受应用程序框架的命令孵化应用进程，设置其对应权限，并调用应用程序框架的入口。
+ bootstrap启动引导组件 提供了各服务和功能的启动入口标识。在SAMGR启动时，会调用boostrap标识的入口函数，并启动系统服务。
+ syspara系统属性组件 负责提供获取与设置操作系统相关的系统属性

---
```      
      {
        "subsystem": "kernel",
        "components": [
          { "component": "liteos_a", "features":[] }
        ]
      },
```

####内核（必要）
---
```      
      {
        "subsystem": "communication",
        "components": [
          { "component": "ipc", "features":[] },
          { "component": "soft_bus", "features":[] },
          { "component": "wpa_supplicant", "features":[] }
        ]
      },
```
分布式软总线子系统，致力于实现近场设备间统一的分布式通信能力管理，提供不区分链路的设备发现和传输接口。

---
```      
      {
        "subsystem": "ace",
        "components": [
          { "component": "ace_engine_lite", "features":[ "" ] }
        ]
      },
```
JS应用开发框架子系统，提供了一套跨平台的类web应用开发框架，通过Toolkit将开发者编写的HML、CSS和JS文件编译打包成JS Bundle，解析运行JS Bundle，生成native UI View组件树并进行渲染显示。

---
```      
      {
        "subsystem": "drivers",
        "components": [
          { "component": "adapter_uhdf", "features":[] },
          { "component": "peripheral_display", "features":[] },
          { "component": "peripheral_input", "features":[] },
          { "component": "peripheral_sensor", "features":[] },
          { "component": "peripheral_wlan", "features":[] }
        ]
      },
```
#### 驱动子系统
驱动子系统采用C面向对象编程模型构建，通过平台解耦、内核解耦，兼容不同内核，提供了归一化的驱动平台底座，旨在为开发者提供更精准、更高效的开发环境，力求做到一次开发，多系统部署。

+ adapter_uhdf 适配平台差异性的代码，该仓主要存放OpenHarmony驱动子系统内核或用户态驱动框架的代码和编译脚本，提供驱动框架的能力。 （必有）
+ display Display HDI 接口定义及其默认实现，对上层图形服务提供显示驱动能力，包括显示图层的管理、显示内存的管理及硬件图形加速
+ input Input HDI接口定义及其实现，对上层输入系统服务提供操作input设备的驱动能力，包括input设备管理、业务流控制、数据上报等
+ sensor Sensor HDI接口定义与实现，接口主要包括所有Sensor信息查询、Sensor启停、Sensor订阅/去订阅、Sensor参数配置等稳定的接口，简化服务开发 (Sensor子系统配套)
+ wlan WLAN HDI接口定义与实现，包括创建和销毁HAL层和WLAN驱动的通道、获取本设备支持的WLAN特性等



---
```      
      {
        "subsystem": "appexecfwk",
        "components": [
          { "component": "bundle_mgr", "features":[] }
        ]
      },
```
个人理解为组件管理

---
```      
      {
        "subsystem": "global",
        "components": [
          { "component": "global_resource_manager", "features":[] },
          { "component": "global_i18n_kits", "features":[] }
        ]
      },
```
全球化子系统，提供支持多语言、多文化的能力。

---
```      
      {
        "subsystem": "vendor",
        "components": [
          { "component": "hi3516dv300_init", "features":[] },
          { "component": "hardware", "features":[] },
          { "component": "middleware", "features":[] }
        ]
      },
```
目前未知作用

---
```      
      {
        "subsystem": "test",
        "components": [
          { "component": "developer_test", "features":[] },
          { "component": "xts_acts", "features":[] },
          { "component": "xts_tools", "features":[] }
        ]
      },
```
测试子系统，开发者基于系统新增特性可以通过开发者自己开发用例保证，对于系统已有特性的修改，也可通过修改项目中原有的测试用例保证，开发者测试旨在帮助开发者在开发阶段就能开发出高质量代码。

+ developertest  # 开发者测试框架
+ xts # XTS认证套件

---
```      
      {
        "subsystem": "utils",
        "components": [
          { "component": "kv_store", "features":[] },
          { "component": "os_dump", "features":[] }
        ]
      },
```
#### 公共基础 

+ kv_store 键值对存储
+ os_dump Dump系统属性,在bin路径下执行os_dump，即可打印当前系统参数

---
```      
      {
        "subsystem": "ai",
        "components": [
          { "component": "ai_engine", "features":[] }
        ]
      },
```
####AI业务子系统
依赖：

+ distributedschedule_services_samgr_lite
+ startup_init_lite

---
```      
      {
        "subsystem": "pms",
        "components": [
          { "component": "powermanageservice", "features":[ "enable_screensaver = true" ] }
        ]
      }
```
轻量级电源管理子系统