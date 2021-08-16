/**
 * K_gadenflm 2021年5月2日15:51:01
 */

/**
 * 输入event_id,删除对应event.
 * @Author K_gadenflm
 * @Date   2021-05-02
 */
const _delete_event = async (params) => {
    const {
        event_id,
    } = params;

    try {
        const event_db = lark.db.table('event');
        const result = await event_db.where({ _id: ObjectId(event_id) }).delete();
        return {
            is_succ: true,
            msg: result ,
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'delete_event 函数内部执行失败！',
        }
    }
}

module.exports = _delete_event;