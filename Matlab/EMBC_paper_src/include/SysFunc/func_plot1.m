function  func_plot1( namestr,x,data )

fi=figure('Name',namestr);
plot (x, data,'k') ;

if strcmp(namestr , 'blood_viscosity_field')
    xlabel('Vessel X Axis   (¦Ìm)');
    ylabel('Blood Viscosity  (Pa/s)');
    title('Blood Viscosity')
    set(gca,'linewidth',2,'fontsize',18,'fontname','Times');
    xlim([-50,2000]);
end

if strcmp(namestr , 'VD')
    xlabel('Time   (s)');
    ylabel('Velocity  (m/s)');
    title('Paticle Velocity_D')
    set(gca,'linewidth',2,'fontsize',18,'fontname','Times');
end
end

