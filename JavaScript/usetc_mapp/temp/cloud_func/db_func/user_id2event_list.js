/**
 * K_gadenflm 2021��5��2��13:56:14
 */

/**
 * ����user_open_id���õ����û��ϴ���event. ���ظ�ʽ Array of event
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
            msg: 'user_id2event_list �����ڲ�ִ��ʧ�ܣ�',
        }
    }

}

module.exports = _user_id2event_list;