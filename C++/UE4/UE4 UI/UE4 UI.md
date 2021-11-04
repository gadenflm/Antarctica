# UE4 UI系统

UE3 前使用的UI解决方案名为 HUD， UE4 引入 UMG， 此处只介绍 UMG。

## UMG 基础知识

__Unreal Motion Graphics 虚幻4的界面编辑系统__

* 编辑界面布局
* 编辑界面动画
* UI 蓝图中交互处理

__widget 控件__

__Slot 插槽__

除了根节点，每个控件都会有插槽,负责子控件与父控件的摆放关系, 需要测试对不用长宽比屏幕分辨率的适配性

关键属性:

* Z Order(z 轴顺序), 一般下方控件的层级 > 上方控件的层级, CanvasPanel 是例外
__这个属性会影响渲染的先后顺序，违反常规顺序改动层级可能会导致性能大幅度占用_
* Anchors(锚点), 决定子控件在父控件的相对位置, __预设中的双向拉伸可以铺满占用整个父控件__

__Visibility 可见性__

与一般UI 可见性有区别，此处有5种，分别对应 可显示和可点击

* Visible: 显示并可以接受点击
* Collapsed: 不显示并折叠, 不接受点击事件。
...

## 常用控件

过多，不记录，对应官方文档使用。

## UI 动画

添加后，出现动画轨。与Flash类似，可以添加关键帧。

## 事件驱动更新

* 功能/函数绑定 
* 属性绑定
* 事件驱动更新(更推荐)

功能/函数绑定和属性绑定，都是逐帧tick判定，对性能消耗较大，大致原理即每帧检查对应函数或对应属性。适合小和简单的系统。

事件驱动，遇到特定事件(如按下f键)再触发更新。适合更加复杂。

## 示例

演示，在FPS demo中加入血量，弹药，准星。

新建--> 在Designer 中加入控件并对应设置(中央一个 image, 下方两个 Text) --> Graph 中设计蓝图 --> 在蓝图 Event BeginPlay 中添加 Create 过程，使得游戏载入时Create对应UI

* Event Pre Construct 为编译器构建完成时触发的事件
* Event Construct 为运行时触发

演示二，制作主界面 开始游戏 和 退出游戏

## UMG 与 Slate

Slate 是UE 中 真正的 UI 框架。 UMG 只是对 Slate 的封装。
用 Slate 编写界面布局相对较麻烦。

* SWidget： Slate 层控件UI 的基类
	- `ComputeDesiredSize()` 计算负责空间的大小
	- `ArrangeChildren()` 父节点如何分配区域中的子项排列, __递归实现，自上而下计算节点，直到所有子空间都分配完毕__
	- `OnPaint()` 负责外观如何显示和渲染，__在所有可见控件上迭代生成绘制元素列表(每帧都会生成)，这个元素列表被渲染线程处理消耗__
	- `Event handlers()` Slate 在特定时机在控件上调用这些函数
	- `...`

## 性能 与 优化

(User Widget)对应一个用户界面，维护一个树状结构(Widget Tree), 渲染时 Widget Tre会被遍历两遍。 

根据可见性会影响遍历，因此只需要显示不需要接受交互事件的控件建议设为 Not-Hit-Testable(多数控件默认为Visible，此时既显示也接受交互)。

会递归 UI 层级，尽量压缩深度，保证递归次数减少。同时尽量合并图集,可使用UE官方Paper2D或三方工具，批量渲染以减少 `DrawCall`

__Invalidation Box, 让子集缓存Slate Tick数据, 不需要每帧进行计算。当某个 Child Widget 的渲染信息发生变化时，通知 InvalidationBox 更新缓存信息__

__Retainer Box, 让子集每隔几帧渲染一次，分摊渲染资源消耗。 只能RetainerBox 嵌套 Invalidation Box， 不能反过来。 Retainer Box 会额外占用显存！__

