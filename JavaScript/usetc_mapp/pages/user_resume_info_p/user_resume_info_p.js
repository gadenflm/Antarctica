const information = require('./information');
const sys_func = require('../../util/sys_func.js');

const i18n = require('../bean/i18n.js');
const bean = i18n.user_resume_info_p;

const _app = getApp();
Page({
    data: {
        TAG: "简历",
        completeness: undefined,
        user_resume_info_list: information.list,
        narParams:{..._app.data.narParams},
        ...bean,
    },

    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '简历',
        });
        this.isCompleteInfo();
    },
    isCompleteInfo: function() {
        let percent = sys_func.analyse_user_info_proportion(_app);
        console.log(percent);
        this.setData({
            completeness: percent,
        })
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    button_fun: function(res) {
        let url = res.currentTarget.dataset.url;
        // console.log(url);
        tt.navigateTo({
            url: url,
        })
    }
})