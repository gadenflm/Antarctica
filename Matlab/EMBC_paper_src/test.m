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

load('true.mat');
true_data=visco_detc;
load('no_ctrl.mat');
nc_data=visco_detc;
load('with_ctrl.mat');
wc_data=visco_detc;

fi1=figure(1);
plot(total_x,true_data,'--');
hold on;
plot(total_x,nc_data,'-');
hold on;
plot(total_x,wc_data,'-');
hold on;
xlabel('Vessel X Axis  (¦Ìm)');
ylabel('Relative Viscosity');
title('Relative Viscosity')
set(gca,'linewidth',2,'fontsize',18,'fontname','Times');
xlim([50,1950]);

legend ('referential viscosity' , 'without controller' , 'with controller' );

nc_er=abs(nc_data-true_data)./abs(true_data);
wc_er=abs(wc_data-true_data)./abs(true_data);

fi2=figure(2);

plot(total_x,10*log10(nc_er),'r','linewidth',3);
hold on;

plot(total_x,10*log10(wc_er),'b','linewidth',3);


xlabel('Vessel X Axis  (¦Ìm)');
ylabel('10log_(Error)');

set(gca,'linewidth',3,'fontsize',30,'fontname','Times');
xlim([50,1950]);

legend('Error Without Controller','Error With Controller');
