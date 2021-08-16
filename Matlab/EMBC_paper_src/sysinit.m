format long
addpath(genpath('.\include\'))

%% system define :
global total_x; 
total_x=[-50:1:2000];     %¦Ìm

global total_y;
total_y=[5000:-1:-5000]';     %nm

global delta_t;
delta_t=0.0005;        %s

global time;
time=0;

global sampling_interval;
freq=10;                                %Hz
sampling_interval=1/freq;     %s

global particle_num;
particle_num=5;

global force_m;
force_m=0;

global vx_max_g;
vx_max_g=0;


%% parameters define:

global flow_velocity;
flow_velocity=0.5e-3; %m/s

global blood_viscosity_mean;
blood_viscosity_mean=1.8e-3;  % Pa/s

global pa_radius;
pa_radius = 100; %nm

gamma=6*pi*blood_viscosity_mean*( pa_radius* 1e-9 );

global pa_mass;
%  pa_mass=(4/3)*pi*((pa_radius)^3)*5.18*1e-3 %kg
 pa_mass=1e-3*gamma;

global C;  % C= delta_p/L
C= ( flow_velocity ) /( (1/ (4*blood_viscosity_mean)) * ((total_y(1)*(1e-9)) ^2) );
