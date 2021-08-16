clc
clear

x_begin=0;
x_end=4;          %0<x<4
p_num=20;       %number of particles
iteration_num_max=100; 
statistics_num=20;
v0=0.5;

c_1=0.01;
c_2=1;
w=0.8;

x=[];     %x(i,j,t)  x:location   i: num of index     j:num of dimension     t:num of iteration
v=[];     %v(i,j,t)  v:velocity   i: num of index     j:num of dimension     t:num of iteration
p=[];     %p(i,t)    p:optimal of one particles
g=[];     %g(t)      g:gloabal optimal

y=[];     %y=func(x);



figure(1);
func_x=[0:0.01:4];
for i=1:401
    func_y(i,1)=func_1(func_x(i));
end

num_convergence=0;
sum_iteration=0;
for iii=1 : statistics_num
    t=0;
    clf
    plot(func_x,func_y,'r.');
    hold on

    for i=1 : p_num
        x(i,t+1)=i*((x_end-x_begin)/(p_num+1));
        y(i,t+1)=func_1(x(i,t+1));
        v(i,t+1)=(rand()-0.5)*2*v0;
        p(i,t+1)=x(i,t+1);

        plot (x(i,t+1),y(i,t+1),'bo')
    end
    [max_y(t+1) g(t+1)]=max(y(:,t+1));
    g(t+1)=x(g(t+1),t+1);
    plot (g(t+1),max_y(t+1),'g*');

    flag=true;
    max_y_temp=max_y(t+1);
    g_temp=g(t+1);
   
    
    while (flag)
        if (var(x(:,t+1))<0.01) 
            flag=false;
            num_convergence= num_convergence+1; 
            sum_iteration=sum_iteration+t-1;
        end
        
        if (t>iteration_num_max)
             flag=false;
        end
        t=t+1;
        pause(0.1);

        figure(1);
        clf
        plot(func_x,func_y,'r.');
        hold on

        for i=1 : p_num
            x(i,t+1)=x(i,t)+v(i,t);
            if (x(i,t+1)<0) x(i,t+1)=0+rand()*0.1;end
            if (x(i,t+1)>4) x(i,t+1)=4-rand()*0.1;end
            y(i,t+1)=func_1(x(i,t+1));

            v(i,t+1)=w*v(i,t)+c_1*rand()*(p(i,t)-x(i,t+1))+c_2*rand()*(g(t)-x(i,t+1));

            [p_max p(i,t+1)]=max(y(i,:));
            p(i,t+1)=x(i,p(i,t+1));

            plot (x(i,t+1),y(i,t+1),'bo')
        end

        [max_y(t+1) g(t+1)]=max(y(:,t+1));
        if (max_y(t+1)>max_y_temp)
            max_y_temp=max_y(t+1);
            g(t+1)=x(g(t+1),t+1);
            g_temp=g(t+1);

        else
            max_y(t+1)=max_y_temp;
            g(t+1)=g_temp;
        end
        plot (g(t+1),max_y(t+1),'g*');

    end

end
num_convergence
aver_iteration=(sum_iteration)/num_convergence