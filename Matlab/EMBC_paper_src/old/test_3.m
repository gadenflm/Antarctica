clc

x_axis=[0:0.01:20];
y_axis=[-1:0.01:1];
r=y_axis;
R=1;
C=1;


% global  blood_viscosity ;
blood_viscosity_0=1.61;
blood_viscosity_1=2.21;

velocity_profile =  (1/ 4*blood_viscosity_0 )* C * ( R^2 - r.^2 );


blood_v_field=zeros( length(y_axis),length(x_axis) );
y=zeros(1, length(x_axis) );
err=zeros(1, length(x_axis) );

for i = 1 : length ( x_axis )
    blood_viscosity =blood_viscosity_0 + ( blood_viscosity_1-blood_viscosity_0) *(i/ length ( x_axis ) );
    y(i)=blood_viscosity;
    velocity_profile =  (1/ 4*blood_viscosity )* C * ( R^2 - r.^2 );

    blood_v_field( :, i ) = velocity_profile ;
  
    if i < (length ( x_axis )/4)  err(i)=abs(rand()*rand())/8;
    
    else    if i < (length ( x_axis )/2)  err(i)=abs(rand()*rand())/7;
       
        else if i < (length ( x_axis )/(1.5))  err(i)=abs(rand()*rand())/6;
        
            else err(i)=abs(rand()*rand())/5;
            end
        end
    
    
    end
end

field_data = blood_v_field;

k = 2000;
result = sin(pi/2:pi:(pi/2+k*pi));

 figure(5)
 plot (x_axis,y.*(1+result.*err),'r');
 
 
