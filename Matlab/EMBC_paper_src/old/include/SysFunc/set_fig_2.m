function set_fig_2( fi )

    ax =gca;
    ax.XAxisLocation ='origin';
    ax.YAxisLocation ='origin';
    ax.YDir='normal';
    ax.XColor=[0,0,0];
    ax.YColor=[0,0,0];
    ax.LineWidth=1.5;
    ax.FontSize=13;
    ax.Box='off';
    colormap jet
    colorbar('location','EastOutside')
    
end

