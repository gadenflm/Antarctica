const information = require('./information');
const sys_func = require('../../util/sys_func');
const cloud_func =require('../../util/cloud_func');
const i18n = require('../bean/i18n.js');
const bean = i18n.admin_main_p;
const _app = getApp();
Page({
    data: {
        TAG: "管理员后台",
        event_array:new Array(),
        name:"",
        user_find_array:new Array(),

        narParams:{..._app.data.narParams},
        ...bean,
    },
    onLoad: function() {
        // console.log(this.data);
        this.init();
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    init:function(){
        tt.showLoading({
            title: bean.init_loading,
            mask:true,
        });
        let task_array =[];
        task_array.push(this.getMyEventData());
        const c_page =this;
        Promise.all(task_array).then(()=>{
            c_page.refresh();
            // console.log(c_page.data);
            tt.hideLoading();
        });
    },
    getData:function(){
    },
    getMyEventData:async function(){
        let event_array =await cloud_func.user_id2evnet_list(_app);
        // console.log(event_array);
        this.data.event_array =[...event_array];
        return true;
    },
    refresh:function(){
        const temp ={...this.data};
        this.setData(temp);
    },
    getNameArray:async function(event){
        const name =this.data.name;
        let user_find_array = await cloud_func.name2user_list(name);
        this.data.user_find_array =[...user_find_array];
        this.refresh();
        console.log(this.data);
    }
})