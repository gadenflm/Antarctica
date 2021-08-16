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
global flow_velocity;
global particle_num;
global C;
global freq;
global force_m;

%% field :viscosity and laminar flow velocity
%  bl_visco=blood_viscosity_field(total_x,total_y);
%  save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
load('blood_viscosity_field.mat');
% func_plot1('blood_viscosity_field',total_x,bl_visco);

% laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

 for p_index=1:particle_num   
    pa(p_index) =particle(0,0,pa_radius,pa_mass);
 end
 
%% Langevin equation Discrete analisis
v_xmax=0;
pa_locx=[];
pa_locy=[];

flag=true;
iter_index=1;
time=0;


while (flag)   
     flag=false;
%      mag_alg(pa);
    force_m=1e-17;
     for p_index=1:particle_num   

        pa_locx(  iter_index ,p_index)=pa(p_index).loc_x;
        pa_locy(  iter_index ,p_index)=pa(p_index).loc_y;
        
        if ( ~pa(p_index).stop_bool)
            [i,j]=xy2ij(pa(p_index).loc_x,pa(p_index).loc_y);
            cur_vis=bl_visco(j);
            cur_flow_v=laminar_flow_f(i,j);
            gamma=6*pi*cur_vis*( pa_radius *1e-9);
           
            force_b=randn()*3e-13;
            theta=rand()*2*pi; 

            pa(p_index).v_x=pa(p_index).v_x+delta_t*((cos(theta)*force_b/pa_mass)+(gamma*cur_flow_v/pa_mass)-(gamma/pa_mass*pa(p_index).v_x) );
%             pa(p_index).v_y=pa(p_index).v_y+delta_t*( (sin(theta)*force_b/pa_mass)+(force_m/pa_mass)-(gamma/pa_mass*pa(p_index).v_y) );
            pa(p_index).v_y=pa(p_index).v_y+delta_t*( (sin(theta)*force_b/pa_mass)-(gamma/pa_mass*pa(p_index).v_y) );
%             pa(p_index).v_y=pa(p_index).v_y+delta_t*( (force_m/pa_mass) );
            
            pa(p_index).loc_x= pa(p_index).loc_x+pa(p_index).v_x*(1e6)*delta_t;
            pa(p_index).loc_y= pa(p_index).loc_y+pa(p_index).v_y*(1e9)*delta_t;

            if ( (pa(p_index).loc_x>=total_x(length(total_x))) ||  (pa(p_index).loc_y<=total_y(length(total_y))) || (pa(p_index).loc_y>=total_y(1)) )
                pa(p_index).stop_bool=true;
            else
                flag=true;
            end
        end
        
     end
     time=time+delta_t;
     iter_index=iter_index+1;
  
end
save('.\include\MatData\pa_loc.mat','pa_locx','pa_locy');  


%%  plot particle location
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
         plot ( [pa_locx(i-1,j),pa_locx(i,j)],[pa_locy(i-1,j),pa_locy(i,j)],'w','LineWidth',5); hold on 
         i=i+1;
         if (i>=length(pa_locx)) flag=false; 
          else if ( (pa_locx(i,j)==pa_locx(i-1,j)) && (pa_locy(i,j)==pa_locy(i-1,j)) ) flag=false; end
         end
             
     end
 end
 
%% 
% visco_detect=[];
% 
% sampling_locx=zeros(1,particle_num);
% sampling_locy=zeros(1,particle_num);
% 
% i_det=0;
% 
% flage=false;
% while (flag)
%     
%     time=0;
%     cur_irange=[];
% end
% v_test=[];
% for p_index = 1: particle_num
%     i=2;
%     while (i<=length(pa_loc))
%         if  (pa_loc(i ,(2*p_index-1 ))~=0)
%             delta_x=pa_loc(i ,(2*p_index-1 ))-pa_loc(i-1 ,(2*p_index-1 ));
%             v_test(i-1,p_index )=delta_x*(1e-6)/delta_t;
%         end
%         i=i+1;
%     end
% end
% 
% for 