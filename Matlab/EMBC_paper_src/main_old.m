clc
clear
close all

sysinit();
global total_x;
global total_y;
global time;

endtime=8;

flow_v_field=field(  total_x, total_y , laminar_flow_field( total_x,total_y ) );
% flow_v_field.visualize('flow_v_field');


% p1=particle(1,0,1,0.1);
% p2=particle( 1,0.4 ,1,0.1);
% p3=particle(0.6,0,1,0.1);
% p4=particle( 1,-0.7 ,1,0.1);

%     flow_v_field.visualize('flow_v_field');
%     p1.visualize('flow_v_field');
%     p2.visualize('flow_v_field');
%     p3.visualize('flow_v_field');
%     p4.visualize('flow_v_field');
% pause (5);

% while (time < endtime);
%     
%     fig_clear();
%     
%     p1.loc_x=p1.loc_x+0.07+0.05*(rand()-0.5);
%     p2.loc_x=p2.loc_x+0.05+0.05*(rand()-0.5);
%     p3.loc_x=p3.loc_x+0.07+0.05*(rand()-0.5);
%     p4.loc_x=p4.loc_x+0.035+0.05*(rand()-0.5);
%     
% %     p1.loc_y=p1.loc_y+0.05;
%     
%     flow_v_field.visualize('flow_v_field');
%     p1.visualize('flow_v_field');
%     p2.visualize('flow_v_field');
%     p3.visualize('flow_v_field');
%     p4.visualize('flow_v_field');
%     time=time+0.1;
%     pause(0.1);
%     
% end