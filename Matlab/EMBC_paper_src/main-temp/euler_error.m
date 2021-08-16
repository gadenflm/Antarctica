clc
clear all
close all
sysinit;

%% global variable
global total_x;
global total_y;
global pa_radius;
global pa_mass;

%%
% bl_visco=blood_viscosity_field(total_x,total_y);
% save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
load('blood_viscosity_field.mat');
% func_plot1('blood_viscosity_field',total_x,bl_visco);

% laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

pa_test=particle(100,0,pa_radius,pa_mass);

[i,j]=xy2ij(pa_test.loc_x,pa_test.loc_y);

cur_vis=bl_visco(j);
cur_flow_v=laminar_flow_f(i,j);
gamma=6*pi*cur_vis*(pa_radius*1e-9);

t_true=[0:0.0001:0.03];
t=[0:0.0005:0.03];
VD_true=(-cur_flow_v).*exp( t_true.*(-gamma/pa_mass))+cur_flow_v;
VD(1)=0;
for i=2:length(t);  
  VD(i)=VD(i-1)+0.0005*((gamma*cur_flow_v/pa_mass)-(gamma/pa_mass*VD(i-1)) );
end
plot(t_true,VD_true,'k'); hold on;
plot(t,VD,'k--'); 

 legend('Velocity-Continuous','Velocity-Discrete');
xlabel('Time   (s)');
ylabel('Velocity  (m/s)');
title('Paticle Velocity_D')
set(gca,'linewidth',2,'fontsize',18,'fontname','Times');


%%
