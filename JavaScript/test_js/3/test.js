
// 测试apply 给一个对象（函数也是对象）传递数组参数
function apply_test_func(par1,par2,par3){
	return [par1,par2,par3];
}

//第一个空对象为null，直接跳过，后面一个数组按次序依次传递
//数组也是对象！！！！！！！！！！！！！！！！！！ 编号为0,1,2
arr_test=apply_test_func.apply({},['par1','par2','par3','par4']); 
// console.log ('type:'+typeof (arr_test)+'  value'+arr_test); 

// 对象作为对象的参数
function test_func(par_obj)
{
	for (i in par_obj){
		console.log (par_obj[i]);
	}
}
// test_func(arr_test);

var test_obj={
	par1:'obj_par1',
	par2:'obj_par2',
	par3:'obj_par3',
}
//Object.keys 获取对象中的元素名 item_name
// for(var key of Object.keys(test_obj))
// {
// 	console.log(key);
// }

//Object.entries 获取对象中的元素名及值，用数组对象封装 [item_name,item_value]
/*for(var key of Object.entries(test_obj))
{
	console.log(key);//得到三个数组
}
*/

// in 判断左边是否是右边对象的 item_name
if ('par1' in test_obj){
	// console.log ('par1 is in the test_obj');
}

if ('obj_par1' in test_obj){
	console.log ('par1:obj_par1 is in the test_obj');
}

//直接复制对象会导致地址传递,克隆对象可以采取：

var test_obj_s_copy={}; //申明空对象
for (var key in test_obj){
	test_obj_s_copy[key]=test_obj[key];
}
// console.log (test_obj_s_copy);
// 
//或者使用 var test_obj_s_copy=Object.assign({},test_obj); 
//Object.assign  作用是把函数参数中的对象合并后返回.

//!!!!!!!最常用的克隆对象方式!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
var test_obj_s_copy_s_copy={...test_obj_s_copy}; 
// console.log (test_obj_s_copy_s_copy);

