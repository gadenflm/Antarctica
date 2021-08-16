/**
 *  K_gadenflm 2021年4月14日23:08:04
 */

const sys_data = require('./sys_data.js');
const sys_func = require('./sys_func.js');
const cloud_func = require('./cloud_func.js');
const monitor = require('./monitor.js');

/**
 * 初始化函数集合
 * @type {Object}
 */
const _init_func = {
    /**
     * 用户初始化 (异步)
     * @Author K_gadenflm
     * @Date   2021-04-12
     * @return {[type]}   [待补充]
     */
    user_init_func: async function(_app) {
        return new Promise((resolve, reject) => {
            tt.login({
                success(login_res) {
                    let user_login_code = login_res.code;
                    cloud_func.user_init(user_login_code, _app, resolve, reject);
                },
                fail(login_res) {
                    console.log('tt.login API 调用失败!!!，user未初始化!!');
                    reject(false);
                },
            });
        });
    },

    /**
     * 
     * @param {*} _app 
     * @returns 
     */
    sys_info_init:async function(_app) {

        return new Promise ((resolve,reject)=>{
            tt.getSystemInfo({
                success (res) {
                    for (let key in res) {
                        if (key in _app.data.system_info) {
                            _app.data.system_info[key]=res[key];
                        }
                    }
                    if ('navigationBarSafeArea' in res) {
                        const temp = res.navigationBarSafeArea;
                        let temp_obj = {                        
                            systitle: {
                                height: temp.height,
                                left: temp.left,
                                top: (res.statusBarHeight+1),
                            },
                            isShowTitle:true,
                            titleText:"标题",
                            isShowBackButton:true,
                        };
                        _app.data.narParams={...temp_obj};
                    }
                    console.log('系统信息初始化完毕');
                    resolve(true);
                },
                fail (res) {
                    console.log('系统信息获取失败');
                    reject(false);
                }
            });
        })

    },

    /**
     * 监视器初始化函数
     * @Author K_gadenflm
     * @Date   2021-04-16
     */
    monitor_init: async function(_app) {
        try {
            _app.data.monitor = new monitor({
                data: sys_data._monitor_data,
                
            });
            console.log('监视器初始化完毕');
            return true;
        } catch (err) {
            console.log('监视器初始化失败：', err);
            return false;
        }
    },

    /**
     * 本地缓存初试化函数 (异步)
     * @Author K_gadenflm
     * @Time   2021-04-09
     * @return {[bool]}   [is_succ]
     */
    cookie_init_func: async function(_app) {

        try {
            res = tt.getStorageInfoSync();
            if (res.currentSize === 2) {
                console.log('cookie init 完毕', await this.cookie_save_sync_func(sys_data._cookie_data));
                return true;
            } else {
                console.log('cookie load 完毕', await this.cookie_load_syn_func(sys_data._cookie_data, _app.data.cookie_data));
                return true;
            }
        } catch (error) {
            console.log('getStorageInfoSync API failed:' + error);
            return false;
        };

    },

    /**
     *  本地缓存存储函数（同步） 循环，键值对保存
     * @Author K_gadenflm
     * @Time   2021年4月9日
     * @param  {[obj]}     data [需要存储的data]
     * @return {[bool]}     返回 true ，保存成功。 false 保存失败。
     */
    cookie_save_sync_func: function(save_data) {

        let is_succ = true;
        for (let i in save_data) {
            try {
                tt.setStorageSync(i, save_data[i]);
            } catch (error) {
                is_succ = false;
                console.log('set cookie API失败:' + error);
                break;
            }
        }

        return is_succ;
    },

    /**
     * 本地缓存读取函数（同步） 
     * @Author K_gadenflm
     * @Time   2021年4月4日
     * @param  {[obj]}     load_data_info [提取data的键值对信息。]
     * @param  {[obj]}     loaded_data    [提取获得的data]
     * @return {[bool]}     返回 true ，保存成功。 false 保存失败。
     */
    cookie_load_syn_func: function(load_data_info, loaded_data) {

        let is_succ = true;
        for (let i in load_data_info) {
            try {
                loaded_data[i] = tt.getStorageSync(i);
            } catch (error) {
                is_succ = false;
                console.log('get cookie API失败:' + error);
            }
        }
        return is_succ;
    },

}

module.exports = _init_func;