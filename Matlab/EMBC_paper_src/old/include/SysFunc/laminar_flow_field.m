function  field_data = laminar_flow_field(x_axis,y_axis)

r=y_axis;
R=1;
C=1;

% global  blood_viscosity ;
blood_viscosity_0=1.61;
blood_viscosity_1=2.21;

velocity_profile =  (1/( 4*blood_viscosity_0 ))* C * ( R^2 - r.^2 );


blood_v_field=zeros( length(y_axis),length(x_axis) );
y=zeros(1, length(x_axis) );
for i = 1 : length ( x_axis )
    blood_viscosity =1.91+0.05*randn();
%     blood_viscosity =blood_viscosity_0 + ( blood_viscosity_1-blood_viscosity_0) *(i/ length ( x_axis ) );
    y(i)=blood_viscosity;
    velocity_profile =  (1/ (4*blood_viscosity) )* C * ( R^2 - r.^2 );

    blood_v_field( :, i ) = velocity_profile ;
end

field_data = blood_v_field;

 figure(5)
 plot (x_axis,y,'r.');
end

