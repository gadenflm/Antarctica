/**
 * K_gadenflm 2021年4月15日22:12:08 
 */

/**
 * 用user access token 从服务器获得 user_info 信息
 * @Author K_gadenflm
 * @Date   2021-04-15
 * @param  {user_access_token
 *         }   
 * @return {is_succ msg}         
 */
const _user_key2_user_info = async (params) => {
    const {
        user_access_token,
    } = params;
    let head_str = "Bearer " + user_access_token;

    const res = await axios.get('https://open.feishu.cn/open-apis/authen/v1/user_info', {
        headers: {
            Authorization: head_str,
        }
    });
    if (res.status == 200) {
        return {
            is_succ: true,
            msg: {
                data: res.data,
            }
        }
    } else {
        return {
            is_succ: false,
            msg: '服务器发送 http 请求失败',
        }
    }

}

module.exports = _user_key2_user_info;