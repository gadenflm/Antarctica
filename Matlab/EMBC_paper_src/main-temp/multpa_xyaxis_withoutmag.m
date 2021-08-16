clc
clear all
close all
sysinit;

%% global variable
global total_x;
global total_y;
global pa_radius;
global pa_mass;
global time;
global delta_t;
global particle_num;


%% field :viscosity and laminar flow velocity
%  bl_visco=blood_viscosity_field(total_x,total_y);
%  save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
load('blood_viscosity_field.mat');
% func_plot1('blood_viscosity_field',total_x,bl_visco);

% laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

% Langevin equation Discrete analisis
pa_locx=[];
pa_locy=[];
for p_index=1:particle_num
    pa(p_index) =particle(0,0,pa_radius,pa_mass);
    flag=true;
    iter_index=1;
    time=0;
    
    while (flag)
        pa_locx(  iter_index ,p_index)=pa(p_index).loc_x;
        pa_locy(  iter_index ,p_index)=pa(p_index).loc_y;
        
        [i,j]=xy2ij(pa(p_index).loc_x,pa(p_index).loc_y);
        cur_vis=bl_visco(j);
        cur_flow_v=laminar_flow_f(i,j);
        gamma=6*pi*cur_vis*( pa_radius *1e-9);

        force_b=randn()*3e-13;
        theta=rand()*2*pi;
        
        pa(p_index).v_x=pa(p_index).v_x+delta_t*((cos(theta)*force_b/pa_mass)+(gamma*cur_flow_v/pa_mass)-(gamma/pa_mass*pa(p_index).v_x) );
        pa(p_index).v_y=pa(p_index).v_y+delta_t*( (sin(theta)*force_b/pa_mass)-(gamma/pa_mass*pa(p_index).v_y) );

        pa(p_index).loc_x= pa(p_index).loc_x+pa(p_index).v_x*(1e6)*delta_t;
        pa(p_index).loc_y= pa(p_index).loc_y+pa(p_index).v_y*(1e9)*delta_t;
        
        time=time+delta_t;
        iter_index=iter_index+1;

        if ( (pa(p_index).loc_x>=total_x(length(total_x))) ||  (pa(p_index).loc_y<=total_y(length(total_y))) || (pa(p_index).loc_y>=total_y(1)) )
            flag=false;
        end

    end
    
    
end
save('.\include\MatData\pa_loc.mat','pa_locx','pa_locy');  
% load ('pa_loc.mat');


fi=figure();
imagesc (total_x,total_y,laminar_flow_f) ;
ax =gca;
ax.XAxisLocation ='origin';
ax.YAxisLocation ='origin';
ax.YDir='normal';
ax.XColor=[1,1,1];
ax.YColor=[0,0,0];
ax.LineWidth=2;
ax.FontSize=20;
ax.Box='off';
colormap hsv
colorbar('location','EastOutside');

xlabel('Vessel X Axis   (¦Ìm)');
ylabel('Vessel Y Axis   (nm)');
xlim([-50,2000]); hold on 
    
 for j=1:particle_num
     i=2;
     flag=true;
     while(flag)
         plot ( [pa_locx(i-1,j),pa_locx(i,j)],[pa_locy(i-1,j),pa_locy(i,j)],'w','LineWidth',3); hold on 
         i=i+1;
         if (i>length(pa_locx)) flag=false; 
         else if ( (pa_locx(i,j)==0) && (pa_locy(i,j)==0) ) flag=false; end
         end
             
     end
 end
