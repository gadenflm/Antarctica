/**
 * K_gadenflm 2021年5月2日13:56:14
 */

/**
 * 输入user_open_id，得到该用户上传的event. 返回格式 Array of event
 * @Author K_gadenflm
 * @Date   2021-05-02
 */
const _user_id2event_list = async (params) => {
    const {
        user_id,
    } = params;

    try {
        const event_db = lark.db.table('event');
        const event_list = await event_db.where({ uploaded_user_open_id: String(user_id) }).find();
        return {
            is_succ: true,
            msg: {
                event_list: event_list,
            }
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'user_id2event_list 函数内部执行失败！',
        }
    }

}

module.exports = _user_id2event_list;