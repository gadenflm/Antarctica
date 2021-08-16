/**
 * K_gadenflm 2021年4月12日19:58:37
 */
const _dB = require('./database.js');

/**
 * 在数据库中插入一个item
 * @Author K_gadenflm
 * @Date   2021-04-12
 * @param  {_db_name,	   需要插入的数据库的名字.
 *          _item_data,	   对应item的数据.
 *          }   
 * @return {is_succ:bool, msg:string}         
 */
const _create_single_new_item_func = async (params) => {
    const {
        _db_name,
        _item_data,
    } = params;

    let new_item = { ..._dB[_db_name] };
    for (let key_name in _item_data) {
        if (key_name in new_item) {
            new_item[key_name] = _item_data[key_name];
        }
    }

    try {
        const cur_db = lark.db.table(_db_name);
        new_item = await cur_db.save(new_item);
        return {
            is_succ: true,
            msg: new_item,
        }
    } catch (err) {
        console.log('_create_single_new_item_func', err);
        return {
            is_succ: false,
            msg: err,
        }
    }


}

module.exports = _create_single_new_item_func;