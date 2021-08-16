clc
clear
close all
sysinit();
global total_x;
global total_y;
global time;

endtime=48;

flow_v_field=field(  total_x, total_y , laminar_flow_field( total_x,total_y ) );

flow_v_field.visualize('flow_v_field');

t=  time+0.01:0.1:endtime+0.01;
x=  0:0.1:20;

D=40e-4;

vp_test=flow_v_field.data(101,:);


vp=vp_test;

C= ( sqrt (4* pi * D * t(1) ) )*(1/ exp (-( (x(1)-vp(1)*t(1) ).^2/ (4 * D *t(1)) ) ) );
co=[];

for i = 1 : length(x)
    for j = 1 : length (t)
        co(i,j) = ( C / sqrt (4* pi * D * t(j) ) ) * exp (-( (x(i)-vp(i)*t(j) ).^2/ (4 * D *t(j)) ) );
    end
end

x3=[];
y3=[];
z3=[];
for i = 1 : length (t) 
   [ z3(i),index]=max(co(:,i));
   y3(i)=t(i);
%       x3(i)=x*co(:,i)/length(x);
   x3(i)=x(index);
end

fi=figure('name','concentration(x,t)');
imagesc (x,t,co) ; hold on ;
set_fig_2(fi);
hold on;
for i = 1 : length (t) 
   plot(x3(i),y3(i),'w.'); hold on;
end

fi2=figure(3);
for i = 1: length (t)
    x2(:,i)=x;
end

for i = 1: length (x)
    t2(i,:)=t;
end
color=x2.*t2;
s=surf(x2,t2,co) ;
s.FaceAlpha=0.5;
s.EdgeColor = 'none';
colormap jet
colorbar('location','EastOutside')

xlabel('location_x');
ylabel('time');
zlabel('concentration');

ax =gca;
ax.XAxisLocation ='origin';
ax.YAxisLocation ='origin';
ax.YDir='normal';
ax.LineWidth=1.5;
ax.FontSize=13;
ax.Box='off';

hold on;
for i = 1 : length (t) 
   plot3(x3(i),y3(i),z3(i),'w.'); hold on;
end