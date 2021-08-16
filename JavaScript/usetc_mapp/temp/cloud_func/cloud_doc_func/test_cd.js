const _test_cd = async (params) => {
    let {
        user_access_token
    } = params;
    const auth = 'Bearer ' + String(user_access_token);
    console.log(auth);
    const res = await axios({
        method: 'GET',
        url: 'https://open.feishu.cn/open-apis/drive/explorer/v2/root_folder/meta',
        headers: {
            "Authorization": auth,
        },
    });
    console.log(res);
    /*    if (res.status == 200) {
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
        }*/
}
module.exports = _test_cd;