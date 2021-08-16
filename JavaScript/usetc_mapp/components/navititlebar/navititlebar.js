Component({
    properties:{
        params:{
            type:Object,
            value:{
                systitle:{
                    height:44,
                    left:16,
                    top:20,
                },
                isShowTitle:true,
                isShowBackButton:true,
                enableBack:true,
            },
            observer: function(newVal, oldVal) {
                this.setData(newVal);
            }
        },
        titleText:{
            type:String,
            value:"标题",
            observer: function(newVal, oldVal) {
                this.setData(newVal);
            }
        }
    },
    data:{
        systitle:{
            height:44,
            left:16,
            top:20,
        },
        isShowTitle:true,
        titleText:"标题",
        isShowBackButton:true,
        enableBack:true,
    },

    methods:{
        backTo:function(){
            if(this.data.enableBack){
                tt.navigateBack({
                    delta: 1
                });
            }else{
                var myEventDetail = {
                    is_succ:false,
                    "msg":"enableBack is false",
                } // detail对象，提供给事件监听函数
                var myEventOption = {} // 触发事件的选项
                this.triggerEvent(
                'myevent',
                myEventDetail,
                myEventOption
                )
            }
        }
    },
    
})