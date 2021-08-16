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

%%
%  bl_visco=blood_viscosity_field(total_x,total_y);
%  save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
load('blood_viscosity_field.mat');
% func_plot1('blood_viscosity_field',total_x,bl_visco);

% laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

%%
figure(1);

pa_test=particle(0,0,pa_radius,pa_mass);
flag=true;
iter_index=1;
while (flag)
    [i,j]=xy2ij(pa_test.loc_x,pa_test.loc_y);
    cur_vis=bl_visco(j);
    cur_flow_v=laminar_flow_f(i,j);
    gamma=6*pi*cur_vis*( pa_radius *1e-9);
    pa_test.v_d=pa_test.v_d+delta_t*((gamma*cur_flow_v/pa_mass)-(gamma/pa_mass*pa_test.v_d) );
    
    alpha=rand()*2*pi;
    theta=rand()*2*pi;
    force_b=randn()*4e-13;
    pa_test.v_b=pa_test.v_b+delta_t*( (cos(alpha)*sin(theta)*force_b/pa_mass)-(gamma/pa_mass*pa_test.v_b) );
    plot (time,pa_test.v_d,'r.');hold on;
    plot (time,pa_test.v_b,'b.');hold on;
    
    pa_test.loc_x= pa_test.loc_x+pa_test.v_d*(1e6)*delta_t;
    pa_test.loc_x= pa_test.loc_x+pa_test.v_b*(1e6)*delta_t;
    time=time+delta_t;
    iter_index=iter_index+1;
    
    if ( pa_test.loc_x>=total_x(length(total_x)) )
        flag=false;
    end
    
end
xlabel('Time   (s)');
ylabel('Velocity  (m/s)');
title('Paticle Velocity');
set(gca,'linewidth',2,'fontsize',18,'fontname','Times');
legend('Velocity_D','Velocity_B');
%%
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

%%
%  bl_visco=blood_viscosity_field(total_x,total_y);
%  save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
load('blood_viscosity_field.mat');
% func_plot1('blood_viscosity_field',total_x,bl_visco);

% laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

%%
figure(1);

pa_test=particle(0,0,pa_radius,pa_mass);
flag=true;
iter_index=1;
while (flag)
    [i,j]=xy2ij(pa_test.loc_x,pa_test.loc_y);
    cur_vis=bl_visco(j);
    cur_flow_v=laminar_flow_f(i,j);
    gamma=6*pi*cur_vis*( pa_radius *1e-9);
    pa_test.v_d=pa_test.v_d+delta_t*((gamma*cur_flow_v/pa_mass)-(gamma/pa_mass*pa_test.v_d) );
    
    alpha=rand()*2*pi;
    theta=rand()*2*pi;
    force_b=randn()*3e-13;
    pa_test.v_b=pa_test.v_b+delta_t*( (cos(alpha)*sin(theta)*force_b/pa_mass)-(gamma/pa_mass*pa_test.v_b) );
    plot (time,pa_test.v_d,'r.');hold on;
    plot (time,pa_test.v_b,'b.');hold on;
    
    pa_test.loc_x= pa_test.loc_x+pa_test.v_d*(1e6)*delta_t;
    pa_test.loc_x= pa_test.loc_x+pa_test.v_b*(1e6)*delta_t;
    time=time+delta_t;
    iter_index=iter_index+1;
    
    if ( pa_test.loc_x>=total_x(length(total_x)) )
        flag=false;
    end
    
end
xlabel('Time   (s)');
ylabel('Velocity  (m/s)');
title('Paticle Velocity');
set(gca,'linewidth',2,'fontsize',18,'fontname','Times');
legend('Velocity_D','Velocity_B');
%%
