/**
 * K_gadeflm 2021年5月2日14:37:46
 */

/**
 * 输入event_id和待更新信息，更新云端数据库信息 
 * @Author K_gadenflm
 * @Date   2021-05-02
 */
const _update_event = async (params) => {
    const {
        event_id,
        update_data,
    } = params;

    try {
        const event_db = lark.db.table('event');
        let event_item = await event_db.where({ _id: ObjectId(event_id) }).findOne();
        let count = 0;
        for (key in update_data) {
            if (key in event_item) {
                event_item[key] = update_data[key];
                count++;
            }
        }
        await event_db.save(event_item);
        return {
            is_succ: true,
            msg: event_item,
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'update_event 函数内部执行失败！',
        }
    }

}

module.exports = _update_event;