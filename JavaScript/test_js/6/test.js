// 所有变量定义定义
let a;
let b;

// object:
function student_obj(stu_num,name){
	this.name =name;
	this.stu_num= stu_num;
	return this;
}
let stu=[]; // arr of obj
stu[0]=new student_obj('01','张三');


function sort_stu_num_func(stu){

	sort(stu.stu_num);
	let asd = {

	}
	return asd;
}

copied_obj={...obj_name};

//class:
class student_class {
	num=2;
	constructor(){
		this.num=1;
	}
	myquc_func(){

	}
}

let stu121=new student_class();
console.log(stu121.num);

