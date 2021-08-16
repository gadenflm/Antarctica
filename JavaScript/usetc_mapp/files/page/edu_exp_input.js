const information =require('./information');
const _app = getApp();
Page({
    data: {
        list :information.list,
    },

    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '',
        });
        this.getData();
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    /**
     * @description 获取数据(finish) 
    */
     getData:function (){
        
    },
})