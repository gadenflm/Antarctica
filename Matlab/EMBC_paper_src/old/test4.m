clc
clear
close all
sysinit();
global total_x;
global total_y;
global time;

endtime=8;

flow_v_field=field(  total_x, total_y , laminar_flow_field( total_x,total_y ) );
noise(1:length(total_y),1:length(total_x))=0;

for i=1:length(total_y)
    for j=1:length(total_x)
        noise(i,j)=randn()*0.001*cos(rand()*2*pi);
    end
end

snr=10*log10(flow_v_field.data./abs(noise));




% fi=figure();
% s=surf (total_x,total_y , snr) ; 
% s.FaceAlpha=0.5;
% s.EdgeColor = 'none';
% colormap hsv
%                   
% colorbar('location','EastOutside')
% 
% xlabel('location x');
% ylabel('location y');
% zlabel('SNR(dB)');
% 
% ax =gca;
% ax.XAxisLocation ='origin';
% ax.YAxisLocation ='origin';
% ax.YDir='normal';
% ax.LineWidth=1.5;
% ax.FontSize=13;
% ax.Box='off';

fi2=figure();
imagesc (total_x,total_y , snr) ; 
set_fig(fi2);
% set_fig(fi);



sta_out=zeros(length(total_y),3);

for i=1:length(total_y)
    
    sum_g=0;
    sum_r=0;
    for j=1:length(total_x)
        
        if (snr(i,j)>=20)
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

legend('SNR<20','SNR>=20');

fi4=figure();
plot (total_y,  sta_out(:,3));hold on;

legend('SNR >20 rate');