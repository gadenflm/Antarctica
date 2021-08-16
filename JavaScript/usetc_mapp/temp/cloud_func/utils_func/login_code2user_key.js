/**
 * K_gadenflm 2021年4月15日11:19:24 
 */

/**
 * 用login code 从服务器获得 user 个人辨识信息
 * @Author K_gadenflm
 * @Date   2021-04-15
 * @param  {login_code, app_access_token} 
 * @return {is_succ,  msg}    
 */
const _login_code2user_key = async (params) => {
    const {
        login_code,
        app_access_token,
    } = params;
    const res = await axios.post('https://open.feishu.cn/open-apis/mina/v2/tokenLoginValidate', {
        "token": app_access_token,
        "code": login_code,
    });

    if (res.status == 200) {
        return {
            is_succ: true,
            msg: {
                data: res.data.data,
            }
        }
    } else {
        return {
            is_succ: false,
            msg: '服务器发送 http 请求失败',
        }
    }
}

/*res.data:
   { code: 0,
     msg: 'success',
     data:
      { access_token: 'u-TwhHtp5qT61IL7cxlt1pGd',			//user_access_token，用于获取用户资源
        expires_in: 1618464420,								//user_access_token的过期时间(s)
        open_id: 'ou_dba14199594a8e0c1714d377056b5e3e',		//用户在应用中的唯一标识
        refresh_token: 'ur-BmWELbUQOwDYlvr1XFEebb',			//刷新用户 access_token 时使用的 token，过期时间为30天
        session_key: '9b808c0d3a77c4497f2881d3ace44165',	//会话密钥
        tenant_key: '2ed7ed66594f565b',						//企业标识（UESTC)
        union_id: 'ou_64676bc87ecb22e33bd15c2201d0e1e9' 	//多个应用中的唯一标识
    } 
}*/

module.exports = _login_code2user_key;