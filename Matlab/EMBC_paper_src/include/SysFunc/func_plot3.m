function  func_plot3( namestr,x,y,data )

fi=figure('Name',namestr);

s=surf (x,y,data) ; 
s.FaceAlpha=0.5;
s.EdgeColor = 'none';
colormap hsv                  
colorbar('location','EastOutside')

ax =gca;
ax.XAxisLocation ='origin';
ax.YAxisLocation ='origin';
ax.YDir='normal';
ax.LineWidth=2;
ax.FontSize=20;
ax.Box='off';

if strcmp(namestr , 'flow_velocity_field')
    xlabel('Vessel X Axis   (¦Ìm)');
    ylabel('Vessel Y Axis   (nm)');
    zlabel('Laminar flow velocity  (m/s)');
    xlim([-50,2000]);
end

end

