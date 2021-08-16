classdef Mapclass < handle

	properties(Access=public)
        num;
        link;
        node;
        node_set;
		pheromone;
		
	end
	
	methods
		function self=Mapclass()
            load('coordinates.mat');
			self.node=coordinates;
           
            self.num=length(self.node);
            self.link=zeros(self.num);

            for i=1:self.num
                for j=1:self.num   
                    self.link(i,j)=sqrt((self.node(i,1)-self.node(j,1)).^2+(self.node(i,2)-self.node(j,2)).^2);
                end
            end
            
            temp=rand(self.num);
            self.pheromone=triu(temp,0)+triu(temp,1)';
            
            node_set=[];
            for i=1:self.num
                self.node_set=[self.node_set;i];
            end
            
            figure(1);
            for i=1:self.num
                plot (self.node(i,1),self.node(i,2),'ro');hold on;
            end
            
        end	
		
        
		function evaporate(self,p_eva)
            self.pheromone=self.pheromone.*(1-p_eva);
		end
		
		function update(self,ant)
            
		    for i=1:(length(ant.path)-1)
               self.pheromone(ant.path(i),ant.path(i+1))=self.pheromone(ant.path(i),ant.path(i+1))+ ant.delta_ph(ant.path(i),ant.path(i+1));
               self.pheromone(ant.path(i+1),ant.path(i))=self.pheromone(ant.path(i+1),ant.path(i))+ ant.delta_ph(ant.path(i),ant.path(i+1));
            end
            
        end
		
        
        function show(self)     % plot line based on concentration of pheromone
            
            figure(2);
            clf;
            max_ph=max(max( self.pheromone));
            min_ph=min(min( self.pheromone));
            temp=(max_ph-min_ph);
            
            for i=1:self.num-1
                for j=i+1:self.num
                    linewid=0.01+4.99*((self.pheromone(i,j)-min_ph)/temp);
                    plot([self.node(i,1),self.node(j,1)],[self.node(i,2),self.node(j,2)],'b:','LineWidth',linewid);hold on;
                end
            end
            
        end
        
	end

	
end
