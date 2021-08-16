function  func_plot2( namestr,x,y,data )

fi=figure('Name','namestr');

imagesc (x,y,data) ;
ax =gca;
ax.XAxisLocation ='origin';
ax.YAxisLocation ='origin';
ax.YDir='normal';
ax.XColor=[1,1,1];
ax.YColor=[0,0,0];
ax.LineWidth=2;
ax.FontSize=20;
ax.Box='off';
colormap hsv
colorbar('location','EastOutside');

end

