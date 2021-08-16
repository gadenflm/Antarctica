const sys_func= require('../../util/sys_func.js');
const _app=getApp();
Page({
    data:{
        narParams:{..._app.data.narParams},
    },

    onLoad:function(){
        console.log(_app.data);
    }
})
/**
 * 1、navbar存在问题88
 * 2、指导语
 * 3、先第一，后第二会出问题88
 * 4、js内的没有加入zh
 * 5、还差一个问题不记得
 */

/**
 * 1、其他信息的信息描述，为大框
 * 2、奖励填写，左边Label少了字符串，保存信息少了Label(finish)
 * 3、奖励描述、项目简介、其他信息的信息描述、工作经历描述改为大框
 * 4、保存信息、删除信息 样式不一样(finish)
 * 5、工作经历的信息顺序，描述在下面(finish)
 * 6、input的高度，会因为中文和英文的改变而改变
 * 7、narbar 滚轮，会上去的问题
 * 8、
 */