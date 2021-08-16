//this 对应swap_test这个对象，使用function构造.

function swap_test(par1,par2){
	this.par2=par1;
	this.par1=par2;
	return this;
}

var var_test=new swap_test(14,15);
// console.log(var_test.constructor);

//高级结构： Set Map WeakMap

//set:
let set_test = new Set();

//map:
let map_test = new Map();

//WeakMap:
let weakmap_test = new WeakMap();




