const sys_fun = require('../../../../util/sys_func');

const i18n = require('../../../bean/i18n.js');
const bean = i18n.other_info_input;

const _app = getApp();
const obj_init = {
    title: "unknown", //信息名称
    intro: "unknown", //信息描述 (选填)
};
const obj_must = {
    title: "unknown", //信息名称
};
Page({
    data: {
        is_disable: false,
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '填写其他信息',
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
        // sys_fun.deliver_obj_val(this.data,temp.show_array[temp.array_index].data);
        let temp_data = { ...obj_init };
        sys_fun.deliver_obj_val(this.data, temp_data);
        temp.show_array[temp.array_index] = {
            data: temp_data,
        }
        console.log(temp)
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
    /**
     * @description 设置显示的参数
     * @param {object} event 
     */
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