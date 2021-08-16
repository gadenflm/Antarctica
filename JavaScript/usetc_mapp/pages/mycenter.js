const sys_data = require('../util/sys_data.js');
const sys_func = require('../util/sys_func.js');
const info =require('./information.js');
const i18n = require ( './bean/i18n.js');
const bean = i18n.mycenter;

const _app = getApp();
Page({
    data: {
        image_src:_app.data.user_info.avatar_url,
        nickname_src:_app.data.user_info.nick_name,
        student_number:"000000000000",
        isComplete:Boolean,
        mList:info.list,
        narParams:{..._app.data.narParams},
        ...bean,
    },

    onLoad: function() {
        tt.showLoading({
            title: bean.init_loading,
            mask:true,
        });
        
        const c_page =this;
        Promise.all(sys_func.finished_array).then(() => {
            c_page.getData();
            c_page.isCompleteInfo();
            tt.hideLoading();
            console.log('app_init_finished_array complete!');
        });
        this.setData({
            narParams:{
                isShowBackButton:false,
            }
        })
    },
    onUnload: function() {
        console.log('页面销毁');
    },
    getData:function(){
        const _app =getApp();
        const{
            avatar_url,
            name,
        }=_app.data.user_info;
        this.setData({
            image_src:avatar_url,
            nickname_src:name,
        });
    },
    isCompleteInfo:function(){
        // console.log(this.data.TAG+"获取是否完成基本信息的填写");
        this.setData({
            isCompleteInfo:true,
        })
    },
    button_fun({currentTarget:{dataset:{url}}}){
        tt.navigateTo({
            url,
        })
    },
    feedback_fun:function(){
        tt.showPrompt({
            title: '反馈',
            success (res) {
                if (res.confirm) {
                    console.log('confirm and inputValue is: ' + res.inputValue);
                } else if (res.cancel) {
                    console.log('cancel')
                }
            },
            fail (res) {
                console.log(`showPrompt调用失败`);
            }
        });
    },
    about_fun:function(){
        tt.showToast({
            title: 'v 1.0.1',
            duration: 2000,
            success (res) {
                console.log(`${res}`);
            },
            fail (res) {
                console.log(`showToast 调用失败`);
            }
        });
    },
    load_fun:function(){
        tt.showLoading({
            title: 'load...',
            success (res) {
                console.log(`${res}`);
            },
            fail (res) {
                console.log(`showLoading 调用失败`);
            }
        });
    },
    hide_load_fun:function(){
        tt.hideLoading({
            success (res) {
                console.log(`${res}`);
            },
            fail (res) {
                console.log(`hideLoading 调用失败`);
            }
        });
    },
    show_modal:function(){
        tt.showModal({
            title: 'title',
            content: 'content',
        })
    },
    show_action_sheet:function(){
        tt.showActionSheet({
            itemList: [1,2,3],
            success (res) {
                console.log(`res.tapIndex + 1`);
            },
            fail(res) {
                console.log(`showActionSheet failure`);
            }
        })
    },
    /**
     * test 
     */
    test_js_fun:function(){
        // const list =["博士","硕士","学士"];
        // console.log(list.findIndex(element => element == "硕士"));

        // let temp_edu_exp = _app.data.user_resume_info.edu_exp;
        // if(temp_edu_exp[0]==undefined){
        //     temp_edu_exp = new Array(0);
        // }
        // const newEdu ={
        //     start_date: null, // 入学时间 (选择框)
        //     end_date: null, // 毕业时间 (选择框)
        //     sch_name: "unknown", //学校
        //     academy:"unknown", // 学院 (选填)
        //     major: "unknown", //专业 (选填)
        //     degree: "unknown", //学位 (选填)
        //     research_target: "unknown", //研究方向 (选填)
        //     };
        // temp_edu_exp.push(newEdu);
        // for(var i=0;i<temp_edu_exp.length;i++){
        //     temp_edu_exp[i].isShow =false;
        // }
        // console.log(temp_edu_exp);

        tt.navigateTo({
            url: '../files/testPage/testPage'
        })
    }
})