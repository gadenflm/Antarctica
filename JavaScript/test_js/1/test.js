//对象定义:
var object_name={
	num_var:0,  			
	str_var:'Hello World!',
	bool_var:true,
	item_func_name:function (para_name){
	},
};
//调用测试: 字符串+数字！
//console.log (object_name.str_var+object_name.num_var);
//for i in 循环:
/*for (i in object_name){
	console.log('item_name:'+i+'  item_value:'+object_name[i])
}
*/

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!
直接把一个对象赋给另一个对象
赋的是对象地址，而不是对象的值
两对象是相互关联的
*/ 
object_2=object_name;
object_2.str_var='Goodbye World!';
// console.log(object_name.str_var);


//常量定义:
const _const_var='This is a constant variable';


//元素字符串名测试：
var item_name_string='str_var';
// console.log (object_name[item_name_string]);


//除0 与 infinity: 
//console.log (-5/0)


//数组测试:
var array_var=[];
array_var=['string',3.14,false,null,'for test'];
array_var[4]=null;
array_var[1]++;
//数组输出 + for i of 循环 + typeof + .length:
//!!!!!!for_of_ 只能用于数组
/*for (var i of array_var){
	console.log(typeof(i)+' '+i);
}
console.log('The length of Array : '+array_var.length);
*/


//函数定义，if，=== or ==
function func_name(para_name){
	if (para_name===1) console.log('==='); else console.log('==');
}
// func_name(true);


// for (var i=0;i<n;i++){} // while(true) {} // do {} while(true); 
// for、while、do-while完全同C

