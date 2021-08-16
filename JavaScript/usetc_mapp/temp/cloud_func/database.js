/**
 *  K_gadenflm 2021年4月19日21:47:24
 */

/**
 * 数据库item格式表
 * @type {Object}
 * @_id        {Obj}    数据库检索标识 
 * @createdAt  {Date}   创建日期（）
 * @updatedAt  {Date}   上传日期（最后一次）
 *
 * !!云端顺序按 字典排序(升序) 顺序  （自己测的，不一定准）
 */
const _dB = {

    /**
     * 测试用数据库
     * @type {Object}
     */
    test_dB_item: {
        test_str_var: 'This is a string variable',
        test_num_var: 0,
        test_bool_var: false,
        test_date_var: new Date(undefined),
        test_obj_var: new Object(undefined),
        test_arr_var: new Array(),
        test_id_var: new Array(),
    },

    /**
     * 用户自身数据库，存储逻辑关系
     * @type {Object}
     */
    user: {
        name: "unknown",
        open_id: "unknown",
        user_resume_info_id: null,
        is_admin: false,
        follow_event_id: new Array(),   //收藏的event
        join_event_id:new Array(),      //关注后确定参加的event
    },

    /**
     * 用户简历相关信息，数据库
     * @type {Object}
     */
    user_resume_info: {

        //必要信息：
        name: "unknown", //姓名 (输入框)
        sex: 0, // 0 unknown, 1 male, 2 female (选择框)
        birth_date: null, // undefined --> 1970-01-01T00:00:00.000Z (选择框)
        hometown: "unknown", //籍贯 (选择框)
        phone_num: "unknown", //电话 (输入框)
        email: "unknown", // 邮件 (输入框)
        edu_background: "unknown", //最高学历 (选择框)

        //个人信息补充（选填）：
        personal_info_optional: {
            en_name: "unknown", //英语名 (输入框)
            graduate_date: null, //毕业时间 (选择框)
            politics_sta: "unknown", //政治面貌 (选择框)
            cur_location: "unknown", //现居住地 (选择框)
            nationality: "unknown", //民族 (选择框) （56个名族+国外)
            mariage: "unknown", //婚姻状况(选择框) （未婚、已婚、离异、保密)
            height: 0, //身高(cm) (输入框)
            weight: 0, //体重(kg) (输入框)
        },

        //教育经历（选填）:
        /*Array of {
            start_date: new Date, // 入学时间 (选择框)
            end_date: new Date, // 毕业时间 (选择框)
            sch_name: "unknown", //学校
            academy:"unknown", // 学院 (选填)
            major: "unknown", //专业 (选填)
            degree: "unknown", //学位 (选填)
            research_target: "unknown", //研究方向 (选填)
        }*/
        edu_exp: new Array(),

        //实习 or 工作 经历（选填）:
        /*Array of {
            start_date: new Date, // 入职时间 (选择框)
            end_date: new Date, // 离职时间 (选择框)
            company_name: "unknown", //工作单位
            job:"unknown", // 职位
            wage: "unknown", //薪资（税前） (选填)
            intro: "unknown", //工作描述 (选填)
        }*/
        internship_exp: new Array(),

        //项目经历（选填）:
        /*Array of {
            start_date: new Date, // 项目开始时间 (选择框)
            end_date: new Date, // 项目结束时间 (选择框)
            project_name: "unknown", //项目名称
            work_name: "unknown", //职务 (选填)
            intro: "unknown", //项目简述 (选填)
        }*/
        project_exp: new Array(),

        //奖励信息（选填）:
        /*Array of {
            date: new Date,  // 获奖时间 (选择框)
            title: "unknown", //奖励名称
            intro: "unknown", //奖励描述 (选填)
        }*/
        award_info: new Array(),

        //自我评价（选填）:
        self_assessment: "unknown", //多少字以内

        //技能爱好（选填）:
        /*Array of {
            name:"unknown",  技能名称
            proficiency："unknown", 熟练度(选择框) (了解、熟悉、精通...)
        }*/
        skill_info: new Array(),

        //其他信息（选填）:
        /*Array of {
            title: "unknown", //信息名称
            intro: "unknown", //信息描述 (选填)
        }*/
        other_info: new Array(),
    },

    /**
     * 招聘、活动、比赛、考试等事件信息的数据库
     * @type {Object}
     */
    event: {
        type: 0, //0:未指定 1:招聘 2:活动 3:比赛 4:考试 5:其他
        name: "unknown",
        title: "unknown",
        start_time: null,
        end_time: null,
        /*Array of {
            subtitle: new String,
            data: new String,
            card_visiable: new Boolean,
        }*/
        info: new Array(),
        info_num: 0,
        priority: 0, //0:未指定 1:强制置顶 2、3、4....:优先级依次顺延
        uploaded_user_open_id:null,//上传者的user_open_id
    },

}

module.exports = _dB;