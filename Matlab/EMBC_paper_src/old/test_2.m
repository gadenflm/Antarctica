clc
clear
close all
sysinit();
% global total_x;
% global total_y;
 time=0;

endtime=48;


t=  time+0.01:0.1:endtime+0.01;
x=  0:0.1:20;

v=21/48;

p1_x=zeros(length(t),1);
p2_x=zeros(length(t),1);
p3_x=zeros(length(t),1);
p4_x=zeros(length(t),1);
m_x=zeros(length(t),1);

for i=2:length(t)
    p1_x(i)= p1_x(i-1)+v+0.5*(rand()-0.5);
    p2_x(i)= p2_x(i-1)+v+0.5*(rand()-0.5);
    p3_x(i)= p3_x(i-1)+v+0.5*(rand()-0.5);
    p4_x(i)= p4_x(i-1)+v+0.5*(rand()-0.5);
    m_x(i)=(p1_x(i)+p2_x(i)+p3_x(i)+p4_x(i))/4;
end

p1_x=func_duigou(p1_x,0,0.99,4);
p2_x=func_duigou(p2_x,0,0.99,4);
p3_x=func_duigou(p3_x,0,0.99,4);
p4_x=func_duigou(p4_x,0,0.99,4);
m_x_2=m_x;
m_x=func_duigou(m_x,0,0.99,4);
fi=figure(4)
plot ( t(1:475) , p1_x(1:475)); hold on;
plot ( t (1:475), p2_x(1:475)); hold on;
plot ( t(1:475) , p3_x(1:475)); hold on;
plot ( t(1:475) , p4_x(1:475)); hold on;
plot ( t(1:475) , m_x(1:475)); hold on;

legend('particle1','particle2','particle3','particle4','mean')

fi5=figure(5)
err=abs(v*t'-m_x_2/10);
plot(err(1:475));