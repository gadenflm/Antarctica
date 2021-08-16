// const zh =require('./zh');
Page({
    data: {
        
    },

    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '我的',
            success (res) {
                console.log(`${res}`);
            },
            fail (res) {
                console.log(`setNavigationBarTitle 调用失败`);
            }
        });
    },

    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
})