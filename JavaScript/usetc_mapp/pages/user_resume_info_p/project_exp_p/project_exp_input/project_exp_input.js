const sys_fun = require('../../../../util/sys_func');
const _app = getApp();
const i18n = require('../../../bean/i18n.js');
const bean = i18n.project_exp_input;

const obj_init = {
    start_date: null, // 项目开始时间 (选择框)
    end_date: null, // 项目结束时间 (选择框)
    project_name: "unknown", //项目名称
    work_name: "unknown", //职务 (选填)
    intro: "unknown", //项目简述 (选填)
};
const obj_must = {
    start_date: null, // 入学时间 (选择框)
    end_date: null, // 毕业时间 (选择框)
    project_name: "unknown", //学校
};
Page({
    data: {
        is_disable: false,
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '填写项目信息',
        });
        this.getData();
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    getData: function() {
        const cur_pages = getCurrentPages();
        const temp = { ...cur_pages[cur_pages.length - 2].data };

        if (temp.array_index == temp.show_array.length) {
            this.setData({ ...obj_init });
            this.setData({
                is_disable: true,
            })
        } else {
            this.setData(temp.show_array[temp.array_index].data);
        }
    },
    update_info: function() {
        const cur_pages = getCurrentPages();
        const temp = cur_pages[cur_pages.length - 2].data;
        let temp_data = { ...obj_init };
        sys_fun.deliver_obj_val(this.data, temp_data);
        temp.show_array[temp.array_index] = {
            data: temp_data,
        }
    },
    formSubmit: function(event) {
        let flag = true;
        for (let key in this.data) {
            if (key in obj_must) {
                if (this.data[key] == obj_must[key]) {
                    flag = false;
                    break;
                }
            }
        }
        if (flag) {
            this.update_info();
            tt.navigateBack({
                delta: 1
            })
        } else {
            tt.showToast({
                title: '请填完必填信息',
                icon: 'none',
                duration: 1500,
            });
        }
    },
    choose_para: function(event) {
        let temp = { ...this.data };
        temp[event.target.dataset.name] = event.detail.value;
        this.setData(temp);
    },

    delete_edu_info: function() {
        const cur_pages = getCurrentPages();
        const temp = cur_pages[cur_pages.length - 2].data;
        temp.show_array.splice(temp.array_index, 1);
        tt.navigateBack({
            delta: 1
        })
    },
})