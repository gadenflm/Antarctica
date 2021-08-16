classdef gene_B_class < handle
    
    properties(Access=private)
        length;
    end
    
    properties(Access=public)
        index;
        data;
    end
    
    methods
        
        function self=gene_B_class(in,len)
            self.index=in;
            self.length=len;
            self.data=round(rand(len,1));
        end
        
        function [son_1_data,son_2_data]= cowith(self,cp)
            
            co_point=unidrnd(self.length-1);
            
            p_1=zeros(self.length,1);
            p_2=zeros(self.length,1);
            
            p_1(1: co_point) =1;   
            p_2(co_point+1:self.length)=1;
            
            son_1_data=(p_1.*self.data) + (p_2.*cp.data);
            son_2_data=(p_2.*self.data) + (p_1.*cp.data);
        end
        
        function mutation(self,p_mu)
            for i=1:self.length
                if (rand()<p_mu)
                    self.data(i)=~self.data(i);
                end
            end
        end
        
    end
    
end

