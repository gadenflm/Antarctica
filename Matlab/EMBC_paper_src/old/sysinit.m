function  sysinit(  )

format long
addpath(genpath('.\include\'))

%define :
global total_x; total_x=[0:0.01:20];
global total_y; total_y=[-1:0.01:1];
global time; time=0; 

global temperature; 
temperature=310.15;   %% 310.15 Kelvins

global Boltzmann_constant; 
% Boltzmann_constant =1.38e-23;
Boltzmann_constant =1 ;

water_viscosity=7 ;  % 37 tem
global  blood_viscosity ;
blood_viscosity = 4.5 * water_viscosity;   %Pa^-3


%Figure init:
% 
% figure('name','The velocity field of laminar flow','TAG','flow_v_field');

end

