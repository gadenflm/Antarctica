/**
 * K_gadenflm 2021年4月22日20:20:40
 */

/**
 * 输入name，在user表中查找到对应信息并返回 Array of {name,phone_num,is_admin,open_id}
 * @Author K_gadenflm
 * @Date   2021-04-22
 */
const _name2user_list = async (params) => {
    const {
        name,
    } = params;

    try {
        const user_db = lark.db.table('user');
        const user_list = await user_db.where({ name: String(name) })
            .populate({
                table: 'user_resume_info',
                ref: 'user_resume_info_id',
            }).find();

        let user_array = [];
        for (let user_item of user_list) {
            user_array.push({
                name: user_item.name,
                phone_num: user_item.user_resume_info_id.phone_num,
                is_admin: user_item.is_admin,
                open_id: user_item.open_id
            })
        }
        return {
            is_succ: true,
            msg: {
                user_list: user_array,
            }
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'name2user_list 函数内部执行失败！',
        }
    }
}

module.exports = _name2user_list;