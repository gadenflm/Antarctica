const information =require('./information');
const _app = getApp();
const obj_init ={
    start_date: null, // 入学时间 (选择框)
    end_date: null, // 毕业时间 (选择框)
    sch_name: "unknown", //学校
    academy:"unknown", // 学院 (选填)
    major: "unknown", //专业 (选填)
    degree: "unknown", //学位 (选填)
    research_target: "unknown", //研究方向 (选填)
    };
Page({
    data: {
        list :information.list,
        show_array:new Array(),
        array_index:-1,
    },

    onLoad: function() {
        tt.setNavigationBarTitle({
            title: '教育信息',
        });
        this.getData();

    },
    onShow:function(){
        tt.startPullDownRefresh();
    },
    /**
     * @description 获取数据(finish) 
    */
     getData:function (){
        let temp_edu_exp =[..._app.data.user_resume_info.edu_exp];
        let temp_show_array=new Array();
        for (i of temp_edu_exp) {
            temp_show_array.push({
                data:i,
            })
        }
        this.setData({
            show_array:temp_show_array,
        });
        console.log(this.data.show_array.length);
    },
    onUnload: function() {
        // 页面销毁时执行
        console.log('页面销毁');
    },
    /**
     * @description 添加一个空的教育信息(finish)
     */
    add_information:function () {
        let edu = [...this.data.show_array];
        let newEdu ={
                    data:{...obj_init},
                    is_show:true,
                    };
        edu.push(newEdu);
        this.setData({
            show_array:edu,
        });
        this.data.array_index = this.data.show_array.length-1;
        // this.data.array_index = 1;
        tt.navigateTo({
            url: './edu_exp_input/edu_exp_input'
        })
    },
    formSubmit:function(event){
        let temp =[...this.data.show_array];
        temp[event.target.dataset.index].is_show =false;
        this.setData({
            show_array:temp,
        });
    },
    /**
     * @description 设置显示的参数
     * @param {object} event 
     */
    choose_para:function(event){
        let temp = [...this.data.show_array];
        temp[event.target.dataset.index]["data"][event.target.dataset.name] =event.detail.value;
        this.setData({
            show_array:temp,
        });
    },
    updata_info:function (){
        let temp_edu_exp=new Array();
        for (i of this.data.show_array) {
            temp_edu_exp.push(i.data);
        } 
       _app.data.user_resume_info.edu_exp=[...temp_edu_exp];
    },
});