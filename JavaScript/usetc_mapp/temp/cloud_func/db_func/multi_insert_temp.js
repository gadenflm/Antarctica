
/**
 * K_gadenflm 2021年5月2日13:11:28
 */

const create_new_single_item= require('./create_new_single_item.js');
const _dB = require('./database.js');

/**
 * 插入多个初始化元素到指定数据库，并指定其中部分信息。
 * @Author K_gadenflm
 * @Date   2021-05-02
 */
const _multi_insert_temp=async (params) => {
	const {
		num,
		item_data,
	} = params;
	for (let i = 0 ; i<num ; i++ ){
		let temp_item={
			_db_name:'event',
        	_item_data:item_data,
		};

		console.log( await create_new_single_item(temp_item));

	}
}
module.exports=_multi_insert_temp;