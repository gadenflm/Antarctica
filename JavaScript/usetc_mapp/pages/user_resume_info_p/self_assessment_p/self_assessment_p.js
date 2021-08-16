const cloud_func = require('../../../util/cloud_func');
const i18n = require('../../bean/i18n.js');
const bean = i18n.self_assessment;
const _app = getApp();
Page({
    data: {
        self_assessment: "unknown",
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '自我评价',
        });
        this.getData();
        console.log('页面注册');
    },
    onUnload: function() {
        // 页面销毁时执行
        this.formSubmit();
        console.log('页面销毁');
    },
    getData: function() {
        let temp = _app.data.user_resume_info.self_assessment;
        this.setData({
            self_assessment: temp,
        });
    },
    reset_information: function() {
        this.setData({
            self_assessment: ""
        })
    },
    formSubmit: function() {
        _app.data.user_resume_info.self_assessment = this.data.self_assessment;
        cloud_func.update_user_resume_info(_app);
    },
    onTextInput: function(event) {
        const temp = event.detail.value;
        this.setData({
            self_assessment: temp,
        })
    }
})