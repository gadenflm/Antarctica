const cloud_func = require('../../../util/cloud_func');
const _app = getApp();
const i18n = require('../../bean/i18n.js');
const bean = i18n.project_exp_p;

const obj_init = {
    start_date: null, // 项目开始时间 (选择框)
    end_date: null, // 项目结束时间 (选择框)
    project_name: "unknown", //项目名称
    work_name: "unknown", //职务 (选填)
    intro: "unknown", //项目简述 (选填)
};
Page({
    data: {
        show_array: new Array(),
        array_index: -1,
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '项目信息',
        });
        this.getData();
    },
    onShow: function() {
        const temp = this.data.show_array;
        this.setData({
            show_array: temp,
        })
    },
    getData: function() {
        let temp_edu_exp = [..._app.data.user_resume_info.project_exp];
        let temp_show_array = new Array();
        for (i of temp_edu_exp) {
            temp_show_array.push({
                data: i,
            })
        }
        this.setData({
            show_array: temp_show_array,
        });
    },
    onUnload: function() {
        // 页面销毁时执行
        this.updata_info();
        console.log('页面销毁');
    },
    add_information: function() {
        this.data.array_index = this.data.show_array.length;
        tt.navigateTo({
            url: './project_exp_input/project_exp_input'
        })
    },
    choose_para: function(event) {
        let temp = [...this.data.show_array];
        temp[event.target.dataset.index]["data"][event.target.dataset.name] = event.detail.value;
        this.setData({
            show_array: temp,
        });
    },
    updata_info: function() {
        let temp_edu_exp = new Array();
        for (i of this.data.show_array) {
            temp_edu_exp.push(i.data);
        }
        _app.data.user_resume_info.project_exp = [...temp_edu_exp];
        cloud_func.update_user_resume_info(_app);
    },
    show_msg: function(event) {
        console.log(event);
        this.data.array_index = event.currentTarget.dataset.index;
        tt.navigateTo({
            url: './project_exp_input/project_exp_input'
        })
    }
})