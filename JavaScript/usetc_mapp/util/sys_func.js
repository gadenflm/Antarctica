/**
 * K_gadenflm   2021年4月12日18:11:33
 */

const sys_data = require('./sys_data.js');
const init_func = require('./init_func.js');
const cloud_func = require('./cloud_func.js');

/**
 * 存储系统使用的函数
 * @type {Object}
 */

const _sys_func = {

    /**
     * 系统初始化函数(同步)
     * @Author K_gadenflm
     * @Date   2021-04-04
     * @param  {[obj]}   _app [App的地址]
     */
    app_init_func: function(_app) {
        tt.cloud.init();

        ///修改对应cloud_func.js

        _app.data = { ...sys_data._app_data };

        // let ret_msg = this.cloud_sync_func('get_app_access_token', new Object(undefined));

        this.finished_array = [];
        this.finished_array.push(init_func.sys_info_init(_app));
        this.finished_array.push(init_func.user_init_func(_app));
        this.finished_array.push(init_func.monitor_init(_app));
        this.finished_array.push(init_func.cookie_init_func(_app));

        // this.test_wkl();
    },

    /**
     * [删除数据库中的固有元素] (未测试)
     * @Author K_gadenflm
     * @Date   2021-04-16
     * @param  {[obj]}   db_item [待删除对象]
     */
    db_item_delete: function(db_item) {

        delete db_item._id;
        delete db_item._createdAt;
        delete db_item._updatedAt;
    },

    /**
     * 自我测试使用函数
     * @Author K_gadenflm
     * @Date   2021-04-05
     */
    test_wkl: async function(_app) {
        // await init_func.sys_info_init(_app);

    },

    /**
     * 传递obj1的对应键值对给obj2.
     * @Author K_gadenflm
     * @Date   2021-04-20
     */
    deliver_obj_val: function(obj1, obj2) {
        for (let key in obj1) {
            if (key in obj2) {
                obj2[key] = obj1[key];
            }
        }
    },


    /**
     * 计算当前用户信息完整度
     * @Author K_gadenflm
     * @Date   2021-04-20
     * @param  {[type]}   _app [description]
     * @return {[type]}        [description]
     */
    analyse_user_info_proportion: function(_app) {

        let i = 0;
        let j = 0;
        const cur_info = { ..._app.data.user_resume_info };
        const based_info = { ...sys_data._app_data.user_resume_info };
        for (let key in cur_info) {
            if (cur_info[key] == based_info[key]) {
                i += 1;
            } else {
                j += 1;
            }
        }
        let temp_str = ((j / (i + j)) * 100).toFixed(1) + '%';
        return temp_str;
    },
}
module.exports = _sys_func;