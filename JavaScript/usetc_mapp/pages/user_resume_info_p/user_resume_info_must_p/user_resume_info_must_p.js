const information = require('./information');
const cloud_func = require('../../../util/cloud_func');
const sys_func =require('../../../util/sys_func');

const i18n = require('../../bean/i18n.js');
const bean = i18n.user_resume_info_must_p;

const _app = getApp();
const obj_must = {
    name: "unknown", //姓名 (输入框)
    sex: 0, // 0 unknown, 1 male, 2 female (选择框)
    birth_date: null, // undefined --> 1970-01-01T00:00:00.000Z (选择框)
    hometown: "unknown", //籍贯 (选择框)
    phone_num: "unknown", //电话 (输入框)
    email: "unknown", // 邮件 (输入框)
    edu_background: "unknown", //最高学历 (选择框)
}
Page({
    data: {
        list: information.list,
        isWriteAllMust: false,
        isShowMust: true,
        isShowOther: false,
        height: 40,

        temp_user_resume_info: {
            //必要信息：
            name: "unknown", //姓名 (输入框)
            sex: 0, // 0 unknown, 1 male, 2 female (选择框)
            birth_date: null, // undefined --> 1970-01-01T00:00:00.000Z (选择框)
            hometown: "unknown", //籍贯 (选择框)
            phone_num: "unknown", //电话 (输入框)
            email: "unknown", // 邮件 (输入框)
            edu_background: "unknown", //最高学历 (选择框)
        },
        //个人信息补充（选填）：
        temp_personal_info_optional: {
            en_name: "unknown", //英语名 (输入框)
            graduate_date: null, //毕业时间 (选择框)
            politics_sta: "unknown", //政治面貌 (选择框)
            cur_location: "unknown", //现居住地 (选择框)
            nationality: "unknown", //民族 (选择框) （56个名族+国外)
            mariage: "unknown", //婚姻状况(选择框) （未婚、已婚、离异、保密)
            height: 0, //身高(cm) (输入框)
            weight: 0, //体重(kg) (输入框)
        },
        narParams: { ..._app.data.narParams },
        ...bean,
    },
    /** 
     * 修改title
     * 加载数据
     */
    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '必填信息',
        });
        this.getData();
        this.setData({
            narParams:{
                enableBack:false,
            }
        })
    },
    /**
     * 获得_app数据
     */
    getData: function() {
        console.log(_app.data);
        const {
            name,
            sex,
            birth_date,
            hometown,
            phone_num,
            email,
            edu_background,
        } = _app.data.user_resume_info;

        const {
            en_name,
            graduate_date,
            politics_sta,
            cur_location,
            nationality,
            mariage,
            height,
            weight,
        } = _app.data.user_resume_info.personal_info_optional;

        this.setData({
            temp_user_resume_info: {
                //必要信息：
                name: name, //姓名 (输入框)
                sex: sex, // 0 unknown, 1 male, 2 female (选择框)
                birth_date: birth_date, // undefined --> 1970-01-01T00:00:00.000Z (选择框)
                hometown: hometown, //籍贯 (选择框)
                phone_num: phone_num, //电话 (输入框)
                email: email, // 邮件 (输入框)
                edu_background: edu_background, //最高学历 (选择框)
            },
            //个人信息补充（选填）：
            temp_personal_info_optional: {
                en_name: en_name, //英语名 (输入框)
                graduate_date: graduate_date, //毕业时间 (选择框)
                politics_sta: politics_sta, //政治面貌 (选择框)
                cur_location: cur_location, //现居住地 (选择框)
                nationality: nationality, //民族 (选择框) （56个名族+国外)
                mariage: mariage, //婚姻状况(选择框) （未婚、已婚、离异、保密)
                height: height, //身高(cm) (输入框)
                weight: weight, //体重(kg) (输入框)
            },
        });
        console.log(_app.data.user_resume_info.personal_info_optional);

    },
    onUnload: function() {
        // // 页面销毁时执行
        // console.log('页面销毁');
        // if(this.isFinishMust()){
        //     this.formSubmit();
        // }else{
        //     // tt.redirectTo({
        //     //     url: './user_resume_info_must_p',
        //     //     success (res) {
        //     //         tt.showToast({
        //     //             title: '请完成必要信息的填写',
        //     //             icon: 'none',
        //     //             duration: 1500,
        //     //         })
        //     //     },
        //     //     fail (res) {
        //     //         console.log(`redirectTo failure`);
        //     //     }
        //     // })
        //     const temp =getCurrentPages();
        //     // temp[temp.length-2].
        //     console.log(temp);
        // }
    },
    choose_para: function(event) {
        let temp = { ...this.data.temp_user_resume_info };
        temp[event.target.dataset.name] = event.detail.value;
        console.log(temp);
        this.setData({
            temp_user_resume_info: temp,
        });
    },
    choose_person_para: function(event) {
        let temp = { ...this.data.temp_personal_info_optional };
        temp[event.target.dataset.name] = event.detail.value;
        console.log(temp);
        this.setData({
            temp_personal_info_optional: temp,
        });
    },
    formSubmit: function() {
        const temp = this.data.temp_user_resume_info;
        // _app.data.user_resume_info = { ...temp };
        const temp_other = this.data.temp_personal_info_optional;
        // _app.data.user_resume_info.personal_info_optional = { ...temp_other };

        sys_func.deliver_obj_val(temp,_app.data.user_resume_info);
        sys_func.deliver_obj_val(temp_other,_app.data.user_resume_info.personal_info_optional);

        cloud_func.update_user_resume_info(_app);
    },
    isFinishMust: function() {
        let flag = true;
        for (let key in this.data.temp_user_resume_info) {
            if (key in obj_must) {
                if (this.data.temp_user_resume_info[key] == obj_must[key]) {
                    flag = false;
                    break;
                }
            }
        }
        return flag;
    },
    show_must_box_fun: function() {
        this.setData({
            isShowMust: true,
            isShowOther: false,
        });
    },
    show_other_box_fun: function() {
        if (this.isFinishMust()) {
            this.setData({
                isShowMust: false,
                isShowOther: true,
            })
        } else {
            this.show_please_complete();
        }
    },
    show_please_complete: function() {
        tt.showToast({
            title: '请完成必要信息的填写',
            icon: 'none',
            duration: 1500,
        })
    },
    backTo: function() {
        console.log("重载了backTo");
        if (this.isFinishMust()) {
            this.formSubmit();
            tt.navigateBack({
                delta: 1
            });
        } else {
            this.show_please_complete();
        }
    }
})