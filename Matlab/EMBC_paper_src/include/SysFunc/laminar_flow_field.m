function  field_data = laminar_flow_field(x_axis,y_axis,bl_visco)

global flow_velocity;
global blood_viscosity_mean;
r=y_axis.*1e-9;
R=r(1);

%% C calculate:

C= ( flow_velocity ) /( (1/ (4*blood_viscosity_mean)) * (R ^2) );
%  velocity_profile =  (1/( 4*blood_viscosity_mean))* C * ( R^2 - r.^2 );
%  plot(r,velocity_profile);

%%
blood_v_field=zeros( length(y_axis),length(x_axis) );

for i = 1 : length ( x_axis )
    blood_viscosity = bl_visco(i);
    velocity_profile =  (1/ (4*blood_viscosity) )* C * ( R^2 - r.^2 );
    blood_v_field( :, i ) = velocity_profile ;
end

field_data = blood_v_field;

end

