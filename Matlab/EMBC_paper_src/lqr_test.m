clc
clear
     %x   vx  y   vy
X0=[0 ; 0 ; 1000 ;100 ];

A=[0,1,0,0;
      0,0,0,0;
      0,0,0,1;
      0,0,0,0];
  
B= [0       ,0;
       100   ,0;
       0       ,0;
       0       ,100];
   
% Q:

Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,10  ,0;
      0     ,0      ,0    ,1];
  
R=[0.01 , 0;
      0      , 1];
  
C=eye(4,4);

D=zeros(4,2);

K=lqr(A,B,Q,R);

sys=ss((A-B*K),B,C,D);

[Y,T,X]=initial(sys,X0);

len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi1=figure(1);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 10,1 R 1
hold on; 
xlabel('Time');
ylabel('Y-location');
set(gca,'linewidth',3,'fontsize',20,'fontname','Times');

fi2=figure(2);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 10,1 R 1
hold on; 
xlabel('Time');
ylabel('Y-Velocity');
set(gca,'linewidth',3,'fontsize',20,'fontname','Times');


Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,100 ,0;
      0     ,0      ,0    ,1];
R=[0.01 , 0;
      0      , 1];
K=lqr(A,B,Q,R);
sys=ss((A-B*K),B,C,D);
[Y,T,X]=initial(sys,X0);
len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi=figure(1);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 100,1 R 1
hold on; 
fi2=figure(2);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 100,1 R 1
hold on; 

Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,10 ,0;
      0     ,0      ,0    ,10];
R=[0.01 , 0;
      0      , 1];
K=lqr(A,B,Q,R);
sys=ss((A-B*K),B,C,D);
[Y,T,X]=initial(sys,X0);
len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi=figure(1);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 10,10 R 1
hold on; 
fi2=figure(2);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 10,10 R 1
hold on; 

fi1=figure(1);
legend('Q1,Q2=10,1','Q1,Q2=100,1','Q1,Q2=10,10');
fi2=figure(2);
legend('Q1,Q2=10,1','Q1,Q2=100,1','Q1,Q2=10,10');


Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,100 ,0;
      0     ,0      ,0    ,1];
R=[0.01 , 0;
      0      , 1];
K=lqr(A,B,Q,R);
sys=ss((A-B*K),B,C,D);
[Y,T,X]=initial(sys,X0);
len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi3=figure(3);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 100,1 R 1
hold on; 
xlabel('Time');
ylabel('Y-location');
set(gca,'linewidth',3,'fontsize',20,'fontname','Times');
fi4=figure(4);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 10,1 R 1
hold on; 
xlabel('Time');
ylabel('Y-Velocity');
set(gca,'linewidth',3,'fontsize',20,'fontname','Times');

Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,100 ,0;
      0     ,0      ,0    ,1];
R=[0.01 , 0;
      0      , 10];
K=lqr(A,B,Q,R);
sys=ss((A-B*K),B,C,D);
[Y,T,X]=initial(sys,X0);

len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi3=figure(3);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 100,1 R 10
hold on; 
fi4=figure(4);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 100,1 R 10
hold on; 

Q=[0.01,0      ,0    ,0;
      0     ,0.01 ,0    ,0;
      0     ,0      ,100 ,0;
      0     ,0      ,0    ,1];
R=[0.01 , 0;
      0      , 100];
K=lqr(A,B,Q,R);
sys=ss((A-B*K),B,C,D);
[Y,T,X]=initial(sys,X0);

len=length(X);

noi_x=[1:1:len];
noi=[];
for i=1:len
    noi(i)=randn()*50;
end
noi=noi';

fi3=figure(3);
plot(T,X(:,3)+noi,'linewidth',4);  %Q 100,1 R 100
hold on; 
fi4=figure(4);
plot(T,X(:,4)+noi,'linewidth',4);  %Q 100,1 R 100
hold on; 

fi3=figure(3);
legend('R=1','R=10','R=100');
fi4=figure(4);
legend('R=1','R=10','R=100');