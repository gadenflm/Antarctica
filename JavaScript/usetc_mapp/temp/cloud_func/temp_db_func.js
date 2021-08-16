/**
 * K_gadenflm 2021年4月8日10:55:55
 */
const _dB = require('./database.js');

/**
 * 数据库API测试函数
 * @Author K_gadenflm
 * @Date   2021-04-08
 * @param  {[optional]}   params  [optional]
 * @param  {[optional]}   context [optional]
 * @return {[optional]}           [optional]
 */
const _template_db_func = async (params, context) => {
    const _test_tb = lark.db.table('test_dB');
    const _db_log_tb= lark.db.table('_dbLog');

    let _test_tb_item = { ..._dB.test_dB_item };
   // await _test_tb.save(_test_tb_item);
   
    /*const res = await axios({
        url: '',
        method: '',
        // baseURL 将自动加在 url 前面，除非 url 是一个绝对 URL。
        // 它可以通过设置一个 baseURL 便于为 axios 实例的方法传递相对 URL
        baseURL: 'https://some-domain.com/api/',

        //headers: {'X-Requested-With': 'XMLHttpRequest'},
        headers: {},

        // params 是即将与请求一起发送的 URL 参数
        //params:{},

        data: {},

        // timeout: 1000,

        // onUploadProgress 允许为上传处理进度事件,Do whatever you want with the native progress event
        // onUploadProgress: function(progressEvent) {
        // },

        // onDownloadProgress 允许为下载处理进度事件,对原生进度事件的处理
        // onDownloadProgress: function(progressEvent) {
        // },
    });
*/

    return {}
}


module.exports = _template_db_func;