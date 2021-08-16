classdef Antclass < handle
	properties(Access=private)
 		index;
        node_set;
	    location;
    end
    
	properties(Access=public)
        delta_ph;
        path_length;
		path; 
        
	end
	
	methods
	
        function self=Antclass(index,node_num)
            self.index=index;
            self.location=floor(rand()*node_num)+1;
            self.path=[];

        end
		
        function func_opt=calc_opt_func(self)
		
        end
        
        function init(self,node_num)
            
            self.location=floor(rand()*node_num)+1;
            self.path=[self.location];
            self.delta_ph=zeros(node_num);
        end
        
        function move(self,map)
            
            while ( length(self.path)< map.num)
                
                clear prob;
                prob(:,1)=setdiff(map.node_set,self.path); %generate probility set    (complete graph!!)
                
            %calculate: 
                sum=0;
                for i=1:length(prob)
                    sum=sum+map.pheromone(self.location,prob(i,1));
                end
                for i=1:length(prob)
                    prob(i,2)=map.pheromone(self.location,prob(i,1))/sum;  
                end
             %add link:   
                prob_sum = cumsum(prob(:,2));     
                target_index = find(prob_sum >= rand()); 
                self.location = prob(target_index(1),1);
                
                self.path=[self.path;self.location];
            end    
        
        end
        
        
        function len= return_len(self)
            len=self.path_length;
        end    
        
        function cal_length(self,map)
            
            sum=0;
            self.path=[self.path;self.path(1)];
            
            for i=1:(length(self.path)-1)
                sum=sum+map.link(self.path(i),self.path(i+1));
            end
            
            self.path_length=sum;
        end
        
        function cal_d_ph(self,p_d_ph)

            for i=1:(length(self.path)-1)
                self.delta_ph(self.path(i),self.path(i+1))=(1/self.path_length)*p_d_ph;
            end

        end
        
      %  function remove_loops(self)  end   ( complete graph!)
	 
	end
	
end