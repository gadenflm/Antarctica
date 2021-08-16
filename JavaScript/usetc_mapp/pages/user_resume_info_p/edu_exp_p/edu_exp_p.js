const cloud_func = require('../../../util/cloud_func');
const _app = getApp();

const i18n = require('../../bean/i18n.js');
const bean = i18n.edu_exp_p;

const obj_init = {
    start_date: null, // 入学时间 (选择框)
    end_date: null, // 毕业时间 (选择框)
    sch_name: "unknown", //学校
    academy: "unknown", // 学院 (选填)
    major: "unknown", //专业 (选填)
    degree: "unknown", //学位 (选填)
    research_target: "unknown", //研究方向 (选填)
};
Page({
    data: {
        show_array: new Array(),
        array_index: -1,
        narParams: { ..._app.data.narParams },
        ...bean,
    },

    onLoad: function() {
        console.log(_app.data);
        tt.setNavigationBarTitle({
            title: '教育信息',
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
        let temp = [..._app.data.user_resume_info.edu_exp];
        let temp_show_array = new Array();
        for (i of temp) {
            temp_show_array.push({
                data: i,
            })
        };
        this.setData({
            show_array: temp_show_array,
        });
    },
    onUnload: function() {
        // 页面销毁时执行
        this.updata_info();
        console.log('页面销毁');
    },
    /**
     * @description 添加一个空的教育信息(finish)
     */
    add_information: function() {
        this.data.array_index = this.data.show_array.length;
        tt.navigateTo({
            url: './edu_exp_input/edu_exp_input'
        })
    },
    /**
     * @description 设置显示的参数
     * @param {object} event 
     */
    choose_para: function(event) {
        let temp = [...this.data.show_array];
        temp[event.target.dataset.index]["data"][event.target.dataset.name] = event.detail.value;
        this.setData({
            show_array: temp,
        });
    },
    updata_info: function() {
        let temp = new Array();
        for (i of this.data.show_array) {
            temp.push(i.data);
        }
        _app.data.user_resume_info.edu_exp = [...temp];
        cloud_func.update_user_resume_info(_app);
    },
    show_msg: function(event) {
        console.log(event);
        this.data.array_index = event.currentTarget.dataset.index;
        tt.navigateTo({
            url: './edu_exp_input/edu_exp_input'
        })
    }
});