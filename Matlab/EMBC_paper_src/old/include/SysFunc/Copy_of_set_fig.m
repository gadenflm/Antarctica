function set_fig( fi )

    fi.Position =[100 350 1600 400];
    ax =gca;
    ax.XAxisLocation ='origin';
    ax.YAxisLocation ='origin';
    ax.YDir='normal';
    ax.XColor=[1,1,1];
    ax.YColor=[0,0,0];
    ax.LineWidth=1.5;
    ax.FontSize=13;
    ax.Box='off';
    colormap jet
    colorbar('location','EastOutside')
    
end

