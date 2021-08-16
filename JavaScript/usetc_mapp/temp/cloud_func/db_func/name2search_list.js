/**
 * K_gadenflm 2021年5月8日10:34:30
 */

/**
 * 输入 数据库名字，name: 检索内容。 返回list
 * @Author K_gadenflm
 * @Date   2021-05-08
 */

/*{
    "db_name":"event",
    "value":"abc"
}*/
const _name2search_list = async (params) => {
    const {
        db_name,
        value,
    } = params;

    const regular=new RegExp(".*"+String(value)+".*");
    // console.log( regular);
    try {
        const cur_db = lark.db.table(db_name);
        const item_list = await cur_db.where({ name: regular }).find();
        return {
            is_succ: true,
            msg: {
                item_list: item_list,
            }
        }
    } catch (err) {
        return {
            is_succ: false,
            msg: 'name2search_list 函数内部执行失败！',
        }
    }
}

module.exports = _name2search_list;