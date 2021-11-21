# 表(List)

## 连续空间存储

最常用例子:数组, 此时各操作时间复杂度如下：

* 遍历(PrintList): O(n)
* 调用第K个元素(FindKth):O(1)
* 插入(Insert):O(n)
* 删除(Delete):O(n)

## 链表(Linked List)

* 遍历(PrintList): O(n)
* 调用第K个元素(FindKth):O(k)
* 插入(Insert):O(1)
* 删除(Delete):O(1)

## 双链表(doubly linked list)和循环链表

在数据结构中附加一个域，是他包含指向前一个单元的指针。增加了空间的需求，同时增加了插入和删除的开销。但是可以方便倒序扫描，同时简化了删除操作的输入。

## 基数排序



