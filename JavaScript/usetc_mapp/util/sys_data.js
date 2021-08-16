/**
 * K_gadenflm    2021年4月4日20:20:54
 */

/**
 * [sys_data 整体模块，存储系统data]
 * @type {Object}
 */
let sys_data = {

    /**
     * [_app_data_obj App.data的信息]
     * @type {Object}
     */
    _app_data: {
        // const var:
        _app_id: "cli_a0ec086d75f8500d",
        // parameter:
        is_new_user: new Boolean(undefined), // 该用户是否是新用户
        is_admin: new Boolean(undefined), // 该用户是否是管理员 
        // object_data:
        // 收藏的event对应数据库id
        //
        system_info: {
            version:"unknown",
            SDKVersion:"unknown",
            windowWidth:0,
            windowHeight:0,
            pixelRatio:0,
            statusBarHeight:0,
            language:"zh_CH",
            fontSizeSetting:0,
            safeArea:{},
            navigationBarSafeArea:{
                left:0,
                right:264,
                top:20,
                bottom:64,
                width:264,
                height:44
            },
        },

        //
        narParams:{
            systitle:{
                height:44,
                left:0,
                top:20,
            },
            isShowTitle:true,
            isShowBackButton:true,
            enableBack:true,
        },

        event_id: {},

        cookie_data: {},

        user_info: {},

        user_key_info: {
            access_token: new String(undefined), //user_access_token，用于获取用户资源
            expires_in: -1, //user_access_token的过期时间(s)
            open_id: new String(undefined), //用户在应用中的唯一标识  巫昆仑：ou_dba14199594a8e0c1714d377056b5e3e
            refresh_token: new String(undefined), //刷新用户 access_token 时使用的 token，过期时间为30天
            session_key: new String(undefined), //会话密钥
            tenant_key: new String(undefined), //企业标识（UESTC)
            union_id: new String(undefined), //多个应用中的唯一标识
        },
        /**
         * 用户简历信息
         * @type {Object}
         */
        user_resume_info: {
            name: "unknown", //姓名 (输入框)
            sex: 0, // 0 unknown, 1 male, 2 female (选择框)
            birth_date: null, // undefined --> 1970-01-01T00:00:00.000Z (选择框)
            hometown: "unknown", //籍贯 (选择框)
            phone_num: "unknown", //电话 (输入框)
            email: "unknown", // 邮件 (输入框)
            edu_background: "unknown", //最高学历 (选择框)

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
            edu_exp: new Array(),
            internship_exp: new Array(),
            project_exp: new Array(),
            award_info: new Array(),
            self_assessment: "unknown",
            skill_info: new Array(),
            other_info: new Array(),
        },
    },

    /**
     * 监视器 data的信息
     * @type {Object}
     */
    _monitor_data: {
        user_has_inited: false, // 用户是否被初始化
    },

    /**
     * [_cookie_data 存储本地缓存的信息]
     * @type {Object}
     */
    _cookie_data: {
        cookie_test: "this is a string variable for cookie test",
    },


}

module.exports = sys_data;