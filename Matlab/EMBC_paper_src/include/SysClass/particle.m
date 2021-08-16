classdef particle <handle
    
    %% public parameters:
    properties (Access=public)
        
        loc_x;     %float:location of x axis
        loc_y;     %float:location of y axis
        mass;       %float:mass of particle
        radius;      %float:radius of particle
        v_x;   %float:velocity of X axis
        v_y;   %float:velocity of Y axis
        v_xmax;
        stop_bool;
        
        v_d;   %float:velocity_drag
        v_b;   %float:velocity_brownian 
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

            self.v_x=0;
            self.v_y=0;
            self.v_xmax=0;
            
            self.v_b=0;  
            self.v_d=0;   
            self.stop_bool=false;
        end
        
        
    end
    
end

