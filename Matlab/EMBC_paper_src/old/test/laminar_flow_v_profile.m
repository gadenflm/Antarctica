clc
clear
close all
format long
addpath(genpath('.\include\'))

r=[1:-0.01:-1]';
R=1;

C=1;
water_viscosity=7 ;  % 37 tem
blood_viscosity = 4.5 * water_viscosity;

velocity_profile =  (1/ 4*blood_viscosity )* C * ( R^2 - r.^2 );
figure('name','The velocity profile of laminar flow')
plot ( velocity_profile  , r );

x_axis = [0:0.01:8] ; 
y_axis= [-1:0.01:1];

blood_v_field=zeros( length(y_axis),length(x_axis) );
for i = 1 : length ( x_axis ) 
    blood_v_field( :, i ) = velocity_profile ; 
end

fi= figure('name','The velocity field of laminar flow');

imagesc (x_axis,y_axis, blood_v_field );
set_fig(fi);


