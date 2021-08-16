clc
clear

p_num=20;       %number of particles
iteration_num_max=100; 
statistics_num=20;
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
func1=@(x1,x2)((x1-1).^2+(x2+4).^2);
func_x1=[-2:0.01:2];
func_x2=[-2:0.01:2];
for i=1:401
    for j=1:401
        func_y(i,j,1)=func1(func_x1(i),func_x2(i));
    end
end

num_convergence=0;
sum_iteration=0;
for iii=1 : statistics_num
    t=0;
    clf
    surf(func_x1,func_x2,func_y);
    hold on

    for i=1 : p_num
        x(i,1,t+1)=(rand()-0.5)*4;
        x(i,2,t+1)=(rand()-0.5)*4;
        y(i,t+1)=func_1(x(i,1,t+1),x(i,2,t+1));
        v(i,1,t+1)=(rand()-0.5)*2*v0;
        v(i,2,t+1)=(rand()-0.5)*2*v0;
        p(i,:,t+1)=x(i,:,t+1);

        plot3 (x(i,1,t+1),x(i,2,t+1),y(i,t+1),'ro')
    end
    [min_y(t+1) index]=min(y(:,t+1));
    g(:,t+1)=x(index,:,t+1);
    plot3 (g(1,t+1),g(2,t+1),min_y(t+1),'g*');

    flag=true;
    min_y_temp=min_y(t+1);
    g_temp=g(:,t+1);
   
    
    while (flag)
        if (var(x(:,1,t+1))+var(x(:,2,t+1))<0.1) 
            flag=false;
            num_convergence= num_convergence+1; 
            sum_iteration=sum_iteration+t-1;
        end
        
        if (t>iteration_num_max)
             flag=false;
        end
        t=t+1;
     %   pause(0.1);

        figure(1);
        clf
        surf(func_x1,func_x2,func_y);
        hold on

        for i=1 : p_num
            x(i,:,t+1)=x(i,:,t)+v(i,:,t);

            y(i,t+1)=func_1(x(i,1,t+1),x(i,2,t+1));

            v(i,:,t+1)=w*v(i,:,t)+c_1*rand()*(p(i,:,t)-x(i,:,t+1))+c_2*rand()*(g(:,t)'-x(i,:,t+1));

            [p_min index]=min(y(i,:));
            p(i,:,t+1)=x(i,:,index);

            plot3 (x(i,1,t+1),x(i,2,t+1),y(i,t+1),'ro')
        end

        [min_y(t+1) index]=min(y(:,t+1));

        if (min_y(t+1)<min_y_temp)
            min_y_temp=min_y(t+1);
            g(:,t+1)=x(index,:,t+1);
            g_temp=g(:,t+1);

        else
            min_y(t+1)=min_y_temp;
            g(:,t+1)=g_temp;
        end
        plot3 (g(1,t+1),g(2,t+1),min_y(t+1),'g*');


    end

end
num_convergence
aver_iteration=(sum_iteration)/num_convergence