/**
 * K_gadenflm 2021年4月16日12:21:11
 */

class monitor {

    /**
     * 监视器初始化
     * @Author K_gadenflm
     * @Date   2021-04-16
     * data中存放对应 {
     * 	key:value,
     * }
     * 
     * watch:{
     * 	 key:function(new_val,old_val){
     * 	 }
     * }
     */
    constructor(obj) {
        // 判断是否是Obj类型，不是则自动命名成空
        if (this.getBaseType(obj.data) === 'Object') {
            this.$data = obj.data;
        } else {
        	this.$data = {};
        	console.log('monitor data参数不是Object!');
        }

        this.$watch = this.getBaseType(obj.watch) === 'Object' ? obj.watch : {};

        for (let _key in obj.data) {
            this.setData(_key);
        }
    }

    // 获取param的类型
    getBaseType(param) {
        const typeStr = Object.prototype.toString.apply(param);
        return typeStr.slice(8, -1);
    }


    setData(_key) {

    	//修改对象的现有属性（重载 set 过程: 加入判定,值发生改变则调用函数）
        Object.defineProperty(this, _key, {

            get: function() {
                return this.$data[_key];
            },

            set: function(val) {
                const oldVal = this.$data[_key];
                if (oldVal === val) return val;

                // val 值发生变化，call this内部定义的函数
                this.$data[_key] = val;
                this.$watch[_key] && typeof this.$watch[_key] === 'function' && (
                    this.$watch[_key].call(this, val, oldVal)
                );
                return val;
            },
        });
    }
}

module.exports= monitor;