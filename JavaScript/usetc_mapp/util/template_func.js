/**
 * K_gadenflm 2021年4月5日10:46:36
 */


/**
 * 部分功能函数模板(测试)
 * @type {Object}
 */
const _temp_func = {
    /**
     * 云函数调用测试
     * @Author K_gadenflm
     * @Date   2021-04-05
     * @param  {string}   func_name 调用函数名
     * @param  {optional}   params   http请求的data，cloud_func的输入参数
     * @param  {optional}   ret_data  返回的http body
     * @return {bool}     is_succ true or false
     */
    cloud_test_func: function(func_name, params, ret_data) {

       tt.cloud.run(func_name, params)
            .then(data => {
                ret_data = { ...data };
                console.log('cloud func 返回 http body：', ret_data);
                return true;
            })
            .catch(error => {
                console.log('fail', error);
                return false;
            });
    },

    /**
     * http发送请求测试 (http request 模板)
     * @Author K_gadenflm
     * @Date   2021-04-05
     * @param  
     * @return {[]}  
     */
    http_request_test_func: async function() {
        let task = tt.request({
            url: 'https://fsqv7t.api.larkfn.com/test_func',
            method: 'POST', // or GET or PUT or DELETE 
            dataType: 'json',
            responseType: 'text', // or arraybuffer
            data: {
                test_string: 'hello world!',
                test_number: 20210405,
            },
            header: {
                'content-type': 'application/json'
            },
            success: function(result) {
                console.log('http 请求发送成功！', result);
            },
            fail: function({ errMsg }) {
                is_succ = false;
                console.log('http 请求发送失败:', errMsg);
            },
            complete(result) {
                console.log("complete:", result)
            }
        });

        /*        if (someReason) {
                    task.abort();
                }*/
    },

}
module.exports = _temp_func;