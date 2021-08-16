clc
clear all
close all

p_num=20;       %number of particles
val_num=100;     %number of product
iteration_num_max=400; 

v0=0.5;

c_1=1;
c_2=1;
w=0.8;

x=[];     %x(i,j,t)  x:location   i: num of index     j:num of dimension     t:num of iteration
v=[];     %v(i,j,t)  v:velocity   i: num of index     j:num of dimension     t:num of iteration
p=[];     %p(i,t)    p:optimal of one particles
g=[];     %g(t)      g:gloabal optimal
y=[];     %y=func(x);


figure(1);
%sum_av=0;
%for ii=1:100
t=0;
for i=1:p_num
    %x(i,:,t+1)=round(rand(val_num,1));
    x(i,:,t+1)=zeros(val_num,1);
    v(i,:,t+1)=(rand(val_num,1)-0.5)*2*v0;
    p(i,:,t+1)=x(i,:,t+1);
    y(i,t+1)=func_2(x(i,:,t+1)); 
end

[max_y(t+1),index]=max(y(:,t+1));
g(:,t+1)=x(index,:,t+1);

max_y_temp=max_y(t+1);
g_temp=g(:,t+1);
plot(t+1,max_y_temp,'ro');hold on;

while (t<=iteration_num_max)
    
    t=t+1;
   % pause(0.05);
    figure(1);
    
    for i=1 : p_num
        for j=1:val_num
            if (rand()<sigmoid(v(i,j,t)))
                x(i,j,t+1)=1;
            else
                x(i,j,t+1)=0;
            end
        end 
        
        y(i,:,t+1)=func_2(x(i,:,t+1));
        
       v(i,:,t+1)=w*v(i,:,t)+c_1*rand()*(p(i,:,t)-x(i,:,t+1))+c_2*rand()*(g(:,t)'-x(i,:,t+1));

        [p_max index]=max(y(i,:));
        p(i,:,t+1)=x(i,:,index);

    end

    [max_y(t+1),index]=max(y(:,t+1));
    
    if (max_y(t+1)>max_y_temp)
        max_y_temp=max_y(t+1);
        g(:,t+1)=x(index,:,t+1);
        g_temp=g(:,t+1);

    else
        max_y(t+1)=max_y_temp;
        g(:,t+1)=g_temp;
    end
    
    plot(t+1,max_y_temp,'ro');hold on;

end

max_y_temp
g_temp'
%sum_av=sum_av+max_y_temp;
%end
%sum_av/100