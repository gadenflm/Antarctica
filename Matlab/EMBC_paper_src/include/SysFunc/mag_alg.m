function  mag_alg( pa )
    global force_m;
    global particle_num;
    global vx_max_g;
    
%     for pa_i=1:particle_num
%         if (pa(pa_i).v_x > pa(pa_i).v_xmax) 
%             pa(pa_i).v_xmax=pa(pa_i).v_x;
%         end
%         if (pa(pa_i).v_x > vx_max_g) 
%             vx_max_g=pa(pa_i).v_x;
%         end
%     end 
%      
    pa_force_m=zeros(particle_num,1);
    for pa_i=1:particle_num
        if (pa(pa_i).loc_y>0)
             pa_force_m(pa_i)=exp( abs(pa(pa_i).loc_y)/5000 );
        else
             if (pa(pa_i).loc_y<0 )
                 pa_force_m(pa_i)=-exp(abs(pa(pa_i).loc_y)/5000) ; 
             end
        end
    end
     force_m=-( mean( pa_force_m) ) *1e-13;
end

