function [ bl_visco ] = blood_viscosity_field(x_axis,y_axis)

global blood_viscosity_mean;
bl_visco = zeros(1,length(x_axis));

visc_start=1.6*1e-3;
visc_stop =2.2*1e-3;
 for i=1:length(x_axis)
           visc_mean=visc_start+(i/length(x_axis))*(visc_stop-visc_start);
           bl_visco(i)=visc_mean+gassrand_3sigma(0,0.25)*5e-5;
%            bl_visco(i)=blood_viscosity_mean+gassrand_3sigma(0,0.25)*5e-5;
           
 end
 

end

