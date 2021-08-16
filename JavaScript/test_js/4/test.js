var test_arr=[0,1,2,3,4];

//数组功能测试 pop push:

//pop返回抽取最后一个值
//
//shift 同 pop，从头抽取，返回值！
test_var=test_arr.pop();
// console.log(test_arr);

//push 返回压进去最后一个值后的整体对象的长度 还是一个对象（没有数组的预定义的一些功能函数！！！！！！！！！！！！！！！！！！！！！！！！！！！！）
var test_var=test_arr.push(8,7,6,5);
//console.log(test_var);

//unshift 同 push ，从头压入，返回长度
test_var=test_arr.unshift(8,7,6,5);
//console.log(test_var);


//concat 连接一些两个数组，返回数组（有预定义了！！！！！）
test_var=test_arr.concat(test_arr);
// console.log(test_var);

// reverse ，将数组逆序（第一个元素和最后一个互换......）
test_arr.reverse();
 // console.log(test_arr);


//indexOf lastIndexOf 查找！！注意大小写！！ 返回位置 
// console.log (test_arr.indexOf(8));
// console.log (test_arr.lastIndexOf(8));

//includes 返回bool
// console.log(test_arr.includes(0));

//splice(par1,par2) 从par1 开始截取，长度为par2.截取数组
var arr_test=[0,1,2,3,4,5,6];
console.log(arr_test.splice(2,4));


//函数可以被定义在调用后。
test_func();
function test_func(){
	// console.log('test_func can be defined after call it.');
}

//find 搜索 ， 返回值参数为真时元素item
var find_test=[
	{id:12,name:'a'},
	{id:13,name:'b'},
	{id:12,name:'c'},
	{id:34,name:'c46'}
]


var gg=find_test.find(function(par){//这是对对象的引用
	return par.name=='c46';
});

// console.log (gg);
// console.log (gg.id);

//同理 有findindx !!
//array.findindex(function(){return ()});