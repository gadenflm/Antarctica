classdef field
    
    %% public parameters:
    properties (Access=public)
        data;
        x_axis;
        y_axis;
    end
    
    %% private parameters:
    properties (Access=private)
    end
    
    methods
        
        %% init:
        function self=field(x_axis,y_axis,data)
            self.x_axis=x_axis;
            self.y_axis=y_axis;
            self.data=data;
        end
        
        %% action on particle:
        function act(self,pa)
            
            global time;
            global blood_viscosity ;
            global Boltzmann_constant;
            global temperature;
            
            Gamma=6*pi*blood_viscosity*pa.radius;
            
           % pa.mass * diff( pa.velocity )/diff( time ) = Gamma *( self.data() - pa.velocity ) + 2 * Gamma *  Boltzmann_constant * temperature  * delta_func()

        end
        
        %% visual:
        function visualize(self,fig_tag)
            fi=findobj('TAG',fig_tag);
            imagesc (self.x_axis,self.y_axis,self.data) ; hold on ;
            set_fig(fi);
        end
        
    end
    
end

