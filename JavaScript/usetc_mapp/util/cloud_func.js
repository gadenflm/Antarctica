/**
 * K_gadenflm 2021年4月15日00:10:52 
 */
const sys_func = require('./sys_func.js');

const _cloud_func = {

    /**
     * 用户数据初始化结合云函数部分 （异步）(时间较长)
     * @Author K_gadenflm
     * @Date   2021-04-16
     * @param  {[String]}   login_code [用户辨识]
     * @param  {[Object]}   _app [指向 app]
     * @return {[type]}            [description]
     */
    user_init: async function(login_code, _app, res, rej) {
        const params = {
            login_code: login_code,
        };
        // return new Promise((resolve, reject) => {
        let task = tt.request({
            url: 'https://fsqv7t.api.larkfn.com/user_init',
            method: 'POST', // or GET or PUT or DELETE 
            dataType: 'json',
            responseType: 'text', // or arraybuffer
            data: params,
            header: {
                'content-type': 'application/json'
            },
            success: function(result) {
                if (result.data.is_succ) {
                    const _temp_msg = { ...result.data.msg };

                    _app.data.is_new_user = _temp_msg.is_new_user;
                    _app.data.user_info = _temp_msg.user_info;
                    _app.data.user_key_info = _temp_msg.user_key_info;
                    _app.data.is_admin = _temp_msg.is_admin;
                    _app.data.event_id = _temp_msg.event_id;

                    let temp_user_resume_info = { ..._temp_msg.user_resume_info };
                    delete temp_user_resume_info._id;
                    delete temp_user_resume_info.createdAt;
                    delete temp_user_resume_info.updatedAt;
                    _app.data.user_resume_info = temp_user_resume_info;
                    _app.data.monitor.user_has_inited = true;
                    console.log('用户初始化完毕！');
                    res(true);
                } else {
                    console.log('user_init cloud返回：失败，用户初始化失败！');
                    rej(false);
                }
            },
            fail: function({ errMsg }) {
                console.log('user_init http 请求发送失败:', errMsg);
                rej(false);
            },
        });
        // });
    },

    /**
     * [更新用户简历信息]（异步）
     * @Author K_gadenflm
     * @Date   2021-04-16
     * @param  {[Obj]}   _app [getApp()]
     */
    update_user_resume_info: async function(_app) {
        if (_app.data.monitor.user_has_inited) {
            const params = {
                open_id: _app.data.user_key_info.open_id,
                user_resume_info: _app.data.user_resume_info,
            };

            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/update_user_resume_info',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        // const _temp_msg = { ...result.data.msg };
                        // console.log(_temp_msg);
                        console.log('更新用户数据库完毕！');
                    } else {
                        console.log('update_user_resume_info cloud返回：失败，更新用户数据库信息失败！');
                    }
                },
                fail: function({ errMsg }) {
                    console.log('update_user_resume_info http 请求发送失败:', errMsg);
                },
            });

        } else {
            console.log('更新用户数据库信息失败，用户初始化未完成！');
        }
    },

    /**
     * 输入name，在user数据库中查找到对应信息并返回 Array of {name,phone_num,open_id}
     * examples: let user_list = await cloud_func. name2user_list('巫昆仑');
     * @Author K_gadenflm
     * @Date   2021-04-22
     */
    name2user_list: async function(name) {
        const params = {
            name: name,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/name2user_list',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(_temp_msg.user_list);
                    } else {
                        console.log('name2user_list cloud返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('name2user_list http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },

    /**
     * 传入open_id,is_admin,修改对应user的属性
     * @Author K_gadenflm
     * @Date   2021-04-29
     */
    open_id2set_is_admin: async function(open_id, is_admin) {
        const params = {
            open_id: open_id,
            is_admin: is_admin,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/open_id2set_is_admin',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        resolve(true);
                    } else {
                        console.log('open_id2set_is_admin cloud返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('open_id2set_is_admin http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });

    },

    /**
     * 输入user_open_id，得到该用户上传的event. 返回格式 Array of event
     * @Author K_gadenflm
     * @Date   2021-05-02
     */
    user_id2evnet_list: async function(_app) {
        const params = {
            user_id: _app.data.user_key_info.open_id,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/user_id2event_list',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(_temp_msg.event_list);
                    } else {
                        console.log('user_id2evnet_list cloud返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('user_id2evnet_list http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },

    /**
     * [在数据库中新添加一个event]
     *  event_data:Object  (refer to database.js)
     * @Author K_gadenflm
     * @Date   2021-05-07
     */
    create_new_event: async function(event_data) {
        const params = {
            _db_name: 'event',
            _item_data: event_data,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/create_new_single_item',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(true);
                    } else {
                        console.log('create_new_event cloud返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('create_new_event http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },

    /**
     * [输入event_id和待更新信息，更新云端数据库信息]
     * @Author K_gadenflm
     * @Date   2021-05-07
     * @param   event_data:Object  (refer to database.js)
     */
    update_event: async function(event_id, event_data) {
        const params = {
            event_id: event_id,
            update_data: event_data,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/update_event',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(_temp_msg);
                    } else {
                        console.log('update_event cloud返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('update_event http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },

    /**
     * [输入event_id，删除信息]
     * @Author K_gadenflm
     * @Date   2021-05-07
     * @param   event_id: ObjectId of event 
     */
    delete_event: async function(event_id) {
        const params = {
            event_id: event_id,
        };
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/delete_event',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(_temp_msg);
                    } else {
                        console.log('delete_event 返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('delete_event http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },

    /**
     * 未测试！！！！！！
     * 输入 name: 检索内容。 返回list
     * @Author K_gadenflm
     * @Date   2021-05-08
     * @return {[type]}   [description]
     */
    name2search_list: async function(value) {
        const params = {
            db_name: "event",
            value: value,
        }
        return new Promise((resolve, reject) => {
            let task = tt.request({
                url: 'https://fsqv7t.api.larkfn.com/name2search_list',
                method: 'POST', // or GET or PUT or DELETE 
                dataType: 'json',
                responseType: 'text', // or arraybuffer
                data: params,
                header: {
                    'content-type': 'application/json'
                },
                success: function(result) {
                    if (result.data.is_succ) {
                        const _temp_msg = { ...result.data.msg };
                        resolve(_temp_msg);
                    } else {
                        console.log('name2search_list 返回：失败！');
                        reject(false);
                    }
                },
                fail: function({ errMsg }) {
                    console.log('name2search_list http 请求发送失败:', errMsg);
                    reject(false);
                },
            });
        });
    },
}

module.exports = _cloud_func;