classdef gene_D_class < handle
    
    properties(Access=private)
    end
    
    properties(Access=public)
        index;
        data;
    end
    
    methods
        
        function self=gene_D_class(in,r_min,r_max)
            self.index=in;
            self.data=(r_max-r_min)*rand()+r_min;
        end
        
        function [son_1_data,son_2_data]= cowith(self,cp,eta_c)
            u=rand();
            if (u<0.5)
                beta=(2*u)*(1/(eta_c+1));
            else
                beta=(2*(1-u))*(1/(eta_c+1));
            end
            son_1_data=0.5*((1+beta)*self.data+(1-beta)*cp.data);
            son_2_data=0.5*((1-beta)*self.data+(1+beta)*cp.data);
        end
        
        function mutation(self,eta_m,r_min,r_max)
            u=rand();
            p1=(self.data-r_min)/(r_max-r_min);
            p2=(r_max-self.data)/(r_max-r_min);
            if (u<0.5)
                p=((2*u)+(1-2*u)*(1-p1).^eta_m).^(1/(eta_m+1))-1;
            else
                p=1-((2*(1-u))+2*(u-0.5)*(1-p2).^(eta_m+1)).^(1/(eta_m+1));
            end
            self.data=self.data+p*(r_max-r_min);
        end
        
    end
    
end

