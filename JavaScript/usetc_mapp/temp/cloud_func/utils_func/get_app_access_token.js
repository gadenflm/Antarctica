/**
 * K_gadenflm 2021年4月14日22:40:31
 */

/**
 * 获取 app_access_token（企业自建应用） 
 * @Author K_gadenflm
 * @Date   2021-04-14
 * @return {msg.data.app_access_token 	API访问token
 *          msg.data.expire				过期时间
 *         }   
 */
const _get_app_access_token = async () => {
    const res = await axios.post('https://open.feishu.cn/open-apis/auth/v3/app_access_token/internal/', {
        "app_id": "cli_a0ec086d75f8500d",
        "app_secret": "J8F3GoOQ82MwhsSCVAl9YcZBueXbjGNL",
    });
    if (res.status == 200) {
        return {
            is_succ: true,
            msg: {
                data:res.data,
            }
        }
    } else {
        return {
            is_succ: false,
            msg:'服务器发送 http 请求失败',
        }
    }

}

module.exports = _get_app_access_token;