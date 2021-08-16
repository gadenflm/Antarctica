const information = require('./information');
const sys_func = require('../../../util/sys_func');
const cloud_func =require('../../../util/cloud_func');
const i18n = require('../../bean/i18n.js');
const bean = i18n.admin_event;
const _app = getApp();
const obj_init= {
    type: 0, //0:未指定 1:招聘 2:活动 3:比赛 4:考试 5:其他
    name: "unknown",
    title: "unknown",
    start_time: null,
    end_time: null,
    /*Array of {
        subtitle: new String,
        data: new String,
        card_visiable: new Boolean,
    }*/
    info: new Array(),
    info_num: 0,
    priority: 0, //0:未指定 1:强制置顶 2、3、4....:优先级依次顺延
    uploaded_user_open_id:_app.data.user_info.open_id,//上传者的user_id
};
const subInfo_init ={
    subtitle: "unknown",
    data: "unknown",
    card_visiable: false,
};
Page({
    data: {
        TAG: "admin_event",
        open_id:"",

        _id:"",
        
        is_new:true,
        list:information.list,
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
    init:function(){
        const cur_pages = getCurrentPages();
        const temp = { ...cur_pages[cur_pages.length - 2].data };
        // console.log(temp);
        if (temp.event_index == temp.event_array.length) {
            this.setData({ ...obj_init });
            this.data.is_new =true;
        } else {
            this.setData(temp.event_array[temp.event_index]);
            this.data.is_new =false;
        }
    },
    choose_para: function(event) {
        let temp = { ...this.data};
        temp[event.target.dataset.name] = event.detail.value;
        sys_func.deliver_obj_val(temp,this.data);
        this.refresh();
    },
    array_choose_para(event){
        // console.log(event);
        let temp =[...this.data.info];
        temp[event.target.dataset.index][event.target.dataset.name] =event.detail.value;
        this.data.info =[...temp];
        this.refresh();
    },
    array_choose_para_picker(event){
        // console.log(event);
        let temp =[...this.data.info];
        temp[event.target.dataset.index][event.target.dataset.name] =event.detail.value==1;
        this.data.info =[...temp];
        this.refresh();
    },
    /**
     *  缺少上传
     * @param {object} event 
     */
    formSubmit:async function(event){
        console.log(event);
        let temp ={...obj_init};
        sys_func.deliver_obj_val(this.data,temp);
        console.log(this.data);
        const {
            type,
            name,
            title,
            start_time,
            end_time,
            info,
            priority,
            uploaded_user_open_id,
        } =this.data;
        const event_sep ={ 
            type:type,
            name:name,
            title:title,
            start_time:start_time,
            end_time:end_time,
            info:info,
            info_num:info.length,
            priority:priority,
            uploaded_user_open_id:uploaded_user_open_id,
        }
        // console.log(event_sep);
        if(this.data.is_new){            
            const database_event =await cloud_func.create_new_event(event_sep);
            this.update_info(database_event);
        }else{
            const database_event =await cloud_func.update_event(this.data._id,event_sep);
            console.log(database_event);
            this.update_info(database_event);
        }
        tt.navigateBack({
            delta: 1
        })
    },
    update_info: function(database_event) {
        const cur_pages = getCurrentPages();
        let temp = cur_pages[cur_pages.length - 2].data;
        const event_index =temp.event_index;
        temp.event_array[event_index] ={...database_event};
    },
    refresh:function(){
        const temp ={...this.data};
        this.setData(temp);
    },
    add_sub_info:function(event){
        // console.log(event);
        let temp =[...this.data.info];
        temp.push({...subInfo_init});
        this.data.info =[...temp];
        this.refresh();
    },
    delete_event:async function(){
        if(this.data.is_new){
            tt.navigateBack({
                delta: 1
            })
        }else{
            // console.log(this.data._id);
            await cloud_func.delete_event(this.data._id);
            //删除上一页的数据
            const cur_pages = getCurrentPages();
            let temp = cur_pages[cur_pages.length - 2].data;
            const event_index =temp.event_index;
            temp.event_array.splice(event_index,1);
            tt.navigateBack({
                delta: 1
            })
        }
    }
})