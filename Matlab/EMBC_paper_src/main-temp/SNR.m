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
bl_visco=blood_viscosity_field(total_x,total_y);
%  save('.\include\MatData\blood_viscosity_field.mat','bl_visco');
% load('blood_viscosity_field.mat');
func_plot1('blood_viscosity_field',total_x,bl_visco);

laminar_flow_f= laminar_flow_field( total_x,total_y ,bl_visco);
% save('.\include\MatData\laminar_flow_field.mat','laminar_flow_f','-v7.3');
% load('laminar_flow_field.mat');
% func_plot3('flow_velocity_field',total_x,total_y,laminar_flow_f);

%%
snr=[];
n=zeros(1,length(total_y));

for j=1:length(total_x)
    for i=1:length(total_y)
        
        alpha=rand()*2*pi;
        theta=rand()*2*pi;
        force_b=randn()*4e-13;
        n(i)=n(i)+delta_t*( (cos(alpha)*sin(theta)*force_b/pa_mass)-(gamma/pa_mass*n(i)) );
        snr(i,j)=10*log10( (laminar_flow_f(i,j))/abs(n(i)) );
    end
end


fi2=figure();
func_plot2('snr',total_x,total_y,snr);


sta_out=zeros(length(total_y),3);

for i=1:length(total_y)
    
    sum_g=0;
    sum_r=0;
    for j=1:length(total_x)
        
        if (snr(i,j)>=10)
            sum_g=sum_g+1;
        else
            sum_r=sum_r+1;
        end
    end
    sta_out(i,1)=sum_r;
    sta_out(i,2)=sum_g;
    sta_out(i,3)=sum_g/(sum_g+sum_r);
    %% 
    
end

fi3=figure();
plot (total_y,  sta_out(:,1),'r.');hold on;
plot (total_y,  sta_out(:,2),'b.');hold on;

legend('SNR<10','SNR>=10');

fi4=figure();
plot (total_y,  sta_out(:,3));hold on;

legend('SNR >10 rate');




