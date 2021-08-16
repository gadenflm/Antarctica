/**
 * K_gadenflm 2021年5月2日16:19:46
 * /

/**
 * ！！！！！！！！！！！！！！！！！！！未上线 未测试！
 * 添加用户与event 之间的关系。 输入 open_id, event_id, type, 增加用户数据库对应 Array of event.
 * @Author K_gadenflm
 * @Date   2021-05-02
 */
const _add_user2event = async (params) => {
    const {
        user_open_id,
        event_id,
        type, // 0: follow , 1:join
    } = params;

    try {
        const user_db = lark.db.table('user');
        const event_db = lark.db.table('event');
        const user_list = await user_db.where({ 'open_id': user_open_id }).find();
        if (user_list.length == 0) {
            return {
                is_succ: false,
                msg: '未检索到该用户！',
            }
        } else if (user_list.length == 1) {
            let user_item = user_list[0];
            if (type == 0) {
                user_item.follow_event_id.push(event_id);
                await user_db.save(user_item);
                return {
                    is_succ: true,
                    msg: 'user follow event',
                }
            } else if (type == 1) {
                user_item.join_event_id.push(event_id);
                await user_db.save(user_item);
                return {
                    is_succ: true,
                    msg: 'user join event',
                }
            } else {
                return {
                    is_succ: false,
                    msg: '错误！type取值必须为0 or 1!',
                }
            }

        } else {
            return {
                is_succ: false,
                msg: '检索到重复用户！',
            }
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'add_user2event 函数内部执行失败！',
        }
    }

}

module.exports = _add_user2event;