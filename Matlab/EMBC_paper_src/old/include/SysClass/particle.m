classdef particle <handle
    
    %% public parameters:
    properties (Access=public)
        
        loc_x;     %float:location of x axis
        loc_y;     %float:location of y axis
        mass;       %float:mass of particle
        radius;      %float:radius of particle
        

    end
    
    %% private parameters:
    properties (Access=private)
    end
    
    methods
        
        %% init:
        function self=particle(loc_x,loc_y,mass,radius)
            self.loc_x=loc_x;
            self.loc_y=loc_y;
            self.mass=mass;
            self.radius=radius;

        end
        
        %% visual:
        function visualize(self,fig_tag)
            fi=findobj('TAG',fig_tag);
            %          plot(self.loc_x,self.loc_y,'w.','MarkerSize',80); hold on
            
            para = [ (self.loc_x-self.radius) , (self.loc_y-self.radius), 2*self.radius , 2*self.radius];
            circle=rectangle('Position', para, 'Curvature', [1 1],'FaceColor','w','EdgeColor','w');
            
        end
        
    end
    
end

