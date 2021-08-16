const information = require('./information');
const cloud_func = require('../../../util/cloud_func');
const i18n = require('../../bean/i18n.js');
const bean = i18n.internship_exp_p;

const _app = getApp();
const obj_init = {
    start_date: new Date, // 入职时间 (选择框)
    end_date: new Date, // 离职时间 (选择框)
    company_name: "unknown", //工作单位
    job: "unknown", // 职位
    wage: "unknown", //薪资（税前） (选填)
    intro: "unknown", //工作描述 (选填)
};
Page({
    data: {
        list: information.list,
        show_array: new Array(),
        array_index: -1,
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '实习工作经历',
        });
        this.getData();
    },
    onShow: function() {
        const temp = this.data.show_array;
        this.setData({
            show_array: temp,
        })
    },
    onUnload: function() {
        // 页面销毁时执行
        this.updata_info();
        console.log('页面销毁');
    },
    getData: function() {
        let temp = [..._app.data.user_resume_info.internship_exp];
        let temp_show_array = new Array();
        for (i of temp) {
            temp_show_array.push({
                data: i,
            })
        }
        this.setData({
            show_array: temp_show_array,
        });
    },
    updata_info: function() {
        let temp = new Array();
        for (i of this.data.show_array) {
            temp.push(i.data);
        }
        _app.data.user_resume_info.internship_exp = [...temp];
        cloud_func.update_user_resume_info(_app);
    },
    add_information: function() {
        this.data.array_index = this.data.show_array.length;
        console.log(11111);
        tt.navigateTo({
            url: './internship_exp_input/internship_exp_input'
        })
    },
    choose_para: function(event) {
        let temp = [...this.data.show_array];
        temp[event.target.dataset.index]["data"][event.target.dataset.name] = event.detail.value;
        this.setData({
            show_array: temp,
        });
    },
    show_msg: function(event) {
        this.data.array_index = event.currentTarget.dataset.index;
        tt.navigateTo({
            url: './internship_exp_input/internship_exp_input'
        })
    }
})