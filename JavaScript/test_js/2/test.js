var obj_test={
	str_test:'This is a test String, a item of the parents object',

	//object 嵌套：
	obj_son_test:{
		str_test:'This is also a test String, a item of the son object',
		// 嵌套的this测试：
		show:function (){
		console.log (this.str_test);
		return this
		},
	},

	// 不加function也合法，不推荐
	show(){
		console.log (this.str_test);
	}
}

/*obj_test.show();
which_this_test=obj_test.obj_son_test.show();
which_this_test.show();*/



// function 参数,返回测试:
var test_var='a test var is not changed';
function return_test(para_obj){
	para_obj.str_test='This is changed by return_test_function!!!!';
	this.test_var='a test is changed by return_test_function!!!!';
	return this;
}

//传对象参数传的是地址!!!!!!!!!!! 加不加new都一样!!!!!!!!!!!
var new_obj=new return_test(obj_test);
// obj_test.show();

var not_new=return_test(obj_test);
// obj_test.show();

//this为父的整体？？？？？？？？？？？？？？？？？？？
// console.log(test_var);
// console.log (not_new);



//function 直接返回 object 测试：
function re_obj_test(par_1,par_2,par_3){
	return {
		return_1:par_1,
		return_2:par_2,
		return_3:par_3,
		return_4(){
			console.log('return_1:'+this.return_1+' return_2:'+this.return_2+' return_3:'+this.return_3);
		},
	}
}

var re_test=re_obj_test('par_1','par_2','par_3');
// re_test.return_4();



//JS 为动态语言，可以随时随地在OBJ里添加元素.!!!!!!!!!!!!!!!
re_test.return_new='thie is a new item';
// console.log(re_test.return_new);

//动态语言，同样 delete 可以随时删除元素！！！！！！！！！！！
delete re_test.return_new;
// console.log(re_test.return_new); //OUTPUT: undifined