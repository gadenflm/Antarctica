/**
 *  K_gadenflm 2021年4月15日23:36:43
 */
const create_new_single_item_func = require('./create_new_single_item');
const get_app_access_token_func = require('./get_app_access_token');
const login_code2user_key_func = require('./login_code2user_key');
const user_key2user_info_func = require('./user_key2user_info');

/**
 * 用户初试化 云函数
 * @Author K_gadenflm
 * @Date   2021-04-14
 * @param  { login_code  小程序调用后，作为用户标识
 *         }  
 * @return {[optional]}  is_succ 反应是否初始化成功,  msg 为初始化信息
 */

const _user_init_func = async (params) => {
    const {
        login_code,
    } = params;

    let temp_ret = new Object(undefined); //存放云函数返回obj的中间变量

    // 获取 app_access_token
    try {
        temp_ret = await get_app_access_token_func();
        if (temp_ret.is_succ) {
            const app_access_token = temp_ret.msg.data.app_access_token;
            const app_access_token_expire = temp_ret.msg.data.expire;

            // 获取 服务器用户标识信息
            try {
                temp_ret = await login_code2user_key_func({
                    login_code: login_code,
                    app_access_token: app_access_token,
                });
                if (temp_ret.is_succ) {
                    const user_key_info = temp_ret.msg.data;
                    const open_id = user_key_info.open_id;
                    const user_access_token = user_key_info.access_token;

                    // 检索云端信息
                    try {
                        temp_ret = await user_key2user_info_func({
                            user_access_token: user_access_token,
                        });
                        if (temp_ret.is_succ) {
                            const user_info = temp_ret.msg.data.data;
                            const name = user_info.name;
                            // 维护用户数据库
                            try {
                                // 根据用户唯一标识，检索用户信息
                                const user_db = lark.db.table('user');
                                const user_resume_info_db = lark.db.table('user_resume_info');
                                const user_list = await user_db.where({ 'open_id': open_id }).find();

                                if (user_list.length == 0) {
                                    // 创建数据库信息
                                    // 用户简历信息
                                    const user_resume_info_ret = await create_new_single_item_func({
                                        _db_name: 'user_resume_info',
                                        _item_data: {
                                            name: name,
                                        },
                                    });
                                    //用户逻辑信息
                                    const user_ret = await create_new_single_item_func({
                                        _db_name: 'user',
                                        _item_data: {
                                            open_id: open_id,
                                            name: name,
                                            user_resume_info_id: user_resume_info_ret.msg._id,
                                        },
                                    });
                                    return {
                                        is_succ: true,
                                        msg: {
                                            is_new_user: true,
                                            is_admin:user_ret.msg.is_admin,
                                            event_id:user_ret.msg.event_id,
                                            user_key_info: user_key_info,
                                            user_info: user_info,
                                            user_resume_info: user_resume_info_ret.msg,
                                        },
                                    }
                                } else if (user_list.length == 1) {
                                    // 老用户，读取数据库，根据 user_access_token过期时间戳 维护user_access_token
                                    const user_item = user_list[0];
                                    const user_resume_info_item = await user_resume_info_db.where({ '_id':user_item.user_resume_info_id }).findOne();
                                    return {
                                        is_succ: true,
                                        msg: {
                                            is_new_user: false,
                                            is_admin:user_item.is_admin,
                                            event_id:user_item.event_id,
                                            user_key_info: user_key_info,
                                            user_info: user_info,
                                            user_resume_info: user_resume_info_item,
                                        },
                                    }
                                } else {
                                    return {
                                        is_succ: false,
                                        msg: '检索到重复用户！',
                                    }
                                }

                            } catch (err) {
                                return {
                                    is_succ: false,
                                    msg: '用户数据维护失败！',
                                }
                            }

                        } else {
                            return {
                                is_succ: false,
                                msg: 'user_key2user_info 失败',
                            }
                        }
                    } catch (err) {
                        return {
                            is_succ: false,
                            msg: 'user_key2user_info 失败（调用本地云函数失败过程 ERROR）',
                        }
                    }

                } else {
                    return {
                        is_succ: false,
                        msg: '获取user_key失败!',
                    }
                }
            } catch (err) {
                return {
                    is_succ: false,
                    msg: '获取user_key失败!（调用本地云函数失败过程 ERROR）',
                }
            }

        } else {
            return {
                is_succ: false,
                msg: '获取 app_access_token 失败！',
            }
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: '获取 app_access_token 失败！（调用本地云函数失败过程 ERROR）',
        }
    }


}

module.exports = _user_init_func;