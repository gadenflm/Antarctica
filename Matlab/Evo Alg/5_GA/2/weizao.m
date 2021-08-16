clc
clear 
close all

%parameter:

range_max=2;
range_min=0;
num_gen=20;
maxnum_ite=30;

eta=2;


% 1.init:
figure(1)
x_ran=[range_min:0.01:range_max];
[y1_ran,y2_ran]=func_val(x_ran);
plot(y1_ran,y2_ran,'r.'); hold on;
xlabel('f1=x^2');ylabel('f2=(x-2)^2');

x_ran=[range_min:0.1:range_max];
[y1_ran,y2_ran]=func_val(x_ran);
plot(y1_ran,y2_ran,'bo'); hold on;
xlabel('f1=x^2');ylabel('f2=(x-2)^2');