const information = require('./information');
const sys_func = require('../../util/sys_func');
const cloud_func =require('../../util/cloud_func');
const i18n = require('../bean/i18n.js');
const bean = i18n.admin_main_p;
const _app = getApp();
Page({
    data: {
        TAG: "管理员后台",
        isTab:{
            isAdminTab :false,
            isEventTab :true,
        },

        user_name:"",
        image_src_user_index:0,
        user_find_array:new Array(),
        user_index:-1,

        event_name:"",
        image_src_event_index:0,
        event_array:new Array(),
        event_index:-1,

        image_scr_array:information.list,
        narParams:{..._app.data.narParams},
        ...bean,
    },
    onLoad: function() {
        this.init();
        console.log(this.data);
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    onShow: function() {
        this.refresh();
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
    getNameArray:async function(){
        if(this.data.user_find_array.length==0){
            const user_name =this.data.user_name;
            let user_find_array = await cloud_func.name2user_list(user_name);
            this.data.user_find_array =[...user_find_array];
            if(this.data.user_find_array.length==0){
                tt.showToast({
                    title: '不存在该用户',
                    icon: 'none',
                    duration:1500,
                })
            }else{
                this.data.image_src_user_index=1;
                this.refresh();
                console.log(this.data);
            }
        }else{
            this.data.user_name ="";
            this.data.image_src_user_index=0;
            this.data.user_find_array =new Array();
            this.refresh();
        }
    },
    getEventArray:async function(){
        // if(this.data.image_src_event_index==0){
            const event_name =this.data.event_name;
            console.log('event_name:'+event_name);
            let event_find_array =await cloud_func.name2search_list(event_name);
            if(event_find_array.length==0){
                tt.showToast({
                    title: '不存在该事件',
                    icon: 'none',
                    duration:1500,
                })
            }else{
                console.log(event_find_array);
                this.data.event_array =[...event_find_array.item_list];
        //         this.data.image_src_event_index=1;
            }
        // }else{
        //     this.data.event_name ="";
        //     await this.getMyEventData();
        //     this.data.image_src_event_index=0;
        // }
        this.refresh();
    },
    choose_para:function(event){
        this.data[event.target.dataset.name] =event.detail.value;
        // console.log(event);
    },
    // choose_which_user:function(event){
    //     console.log(event);
    //     const {
    //         name,
    //         phone_num,
    //         open_id,
    //     }=event.target.dataset.user;
    //     const show_content =name+'('+phone_num+')';
    //     this.data.user_find_array =[];
    //     this.refresh();
    //     tt.showModal({
    //         title: "确实设置当前用户为admin？",
    //         content: show_content,
    //         confirmText:"确认",
    //         cancelText:"拒绝",
    //         success (res) {
    //             if (res.confirm) {
    //                 cloud_func.open_id2set_is_admin(open_id,true);
    //                 console.log('confirm, continued');
    //             } else if (res.cancel) {
    //                 cloud_func.open_id2set_is_admin(open_id,false);
    //                 console.log('cancel, cold')
    //             } else {
    //                 console.log('what happend')
    //                 // what happend?
    //             }
    //         },
    //         fail (res) {
    //             console.log(`showModal调用失败`);
    //         }
    //     });
    // },
    change_admin:async function(event){
        console.log(event);
        let user ={...this.data.user_find_array[event.target.dataset.user]};
        let is_admin =event.detail.value;
        user.is_admin =is_admin;
        this.data.user_find_array[event.target.dataset.user] =user;
        this.refresh();
        tt.showLoading({
            title: bean.init_uploading,
            mask:true,
        });
        await cloud_func.open_id2set_is_admin(user.open_id,is_admin);
        tt.hideLoading({});
    },
    choose_which_event:function(event){
        this.data.event_index =event.target.dataset.eventindex;
        // console.log(this.data.event_index);
        tt.navigateTo({
            url: './admin_event/admin_event'
        })
        console.log(event.target.dataset)
    },
    goto_add_event:function(){
        this.data.event_index = this.data.event_array.length;
        // console.log(this.data.event_index);
        tt.navigateTo({
            url: './admin_event/admin_event'
        })
    },
    goto_tab:function(event){
        let temp_tab ={...this.data.isTab};
        console.log(temp_tab);
        for(let key in temp_tab){
            if(key == event.target.dataset.tab){
                temp_tab[key] =true;
            }else{
                temp_tab[key] =false;
            }
        }
        this.data.isTab ={...temp_tab};
        this.refresh();
    }
})