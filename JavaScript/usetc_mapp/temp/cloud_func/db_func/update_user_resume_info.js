/**
 * K_gadenflm 2021年4月16日16:29:25
 */

/**
 * 根据open_id检索对应用户，上传用户信息
 * @Author K_gadenflm
 * @Date   2021-04-16
 */
const _update_user_resume_info_func = async (params) => {
    const {
        open_id,
        user_resume_info,
    } = params;

    try {
        const user_db = lark.db.table('user');
        const user_resume_info_db = lark.db.table('user_resume_info');
        const user_list = await user_db.where({ 'open_id': open_id }).find();
        if (user_list.length == 0) {
            return {
                is_succ: false,
                msg: '未检索到该用户！',
            }
        } else if (user_list.length == 1) {
            const user_item = user_list[0];
            const user_resume_info_item = await user_resume_info_db.where({ '_id': user_item.user_resume_info_id }).findOne();
            let count=0;
            for (let key in user_resume_info) {
                if (key in user_resume_info_item) {
                    user_resume_info_item[key] = user_resume_info[key];
                    count++;
                }
            }
            await user_resume_info_db.save(user_resume_info_item);
            return {
                is_succ: true,
                msg: '已上传用户信息键值对数:'+count ,
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
            msg: 'update_user_resume_info 函数内部执行失败！',
        }
    }

}

module.exports = _update_user_resume_info_func;