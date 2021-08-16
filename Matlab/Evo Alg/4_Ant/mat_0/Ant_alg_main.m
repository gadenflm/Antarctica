clc 
clear all
close all

num_iteration=0;
%parameter:
num_ant=30;
p_eva=0.2;
p_d_ph=5;

%initial:


map=Mapclass();
%map.show();


ant=[];
for i=1:num_ant
    ant=[ant;Antclass(i,map.num)];
end

len_min=30*sqrt(2);
while (len_min>=5)
    
    num_iteration=num_iteration+1;
    
    
    for k=1:num_ant
        ant(k).init(map.num);
        ant(k).move(map);
        ant(k).cal_length(map);
        
        if  (ant(k).return_len()<len_min) 
            len_min=ant(k).return_len()
        end
        
        ant(k).cal_d_ph(p_d_ph);
    end
    
    
    map.evaporate(p_eva);

    for k=1:num_ant
        map.update(ant(k));
    end
    
   % pause(0.05);
    map.show()
    num_iteration=num_iteration+1;
end