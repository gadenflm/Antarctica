/**
 * K_gadenflm 2021年4月29日20:02:38
 */

/**
 * 传入open_id,is_admin,修改对应user的属性
 * @Author K_gadenflm
 * @Date   2021-04-29
 */
const _open_id2set_is_admin = async (params) => {
    const {
        open_id,
        is_admin,
    } = params;

    try {
        const user_db = lark.db.table('user');
        const user_list = await user_db.where({ open_id: open_id }).find();
        if (user_list.length == 0) {
            return {
                is_succ: false,
                msg: '未检索到该用户！',
            }
        } else if (user_list.length == 1) {
            const user_item = user_list[0];
            user_item.is_admin = is_admin;
            await user_db.save(user_item);
            return {
                is_succ: true,
                msd: '更新成功',
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
            msg: 'open_id2set_is_admin 函数内部执行失败！',
        }
    }

}

module.exports = _open_id2set_is_admin;