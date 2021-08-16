clc 
clear all
close all

num_iteration=0;
%parameter:
num_ant=30;
p_eva=0.3;
p_d_ph=10;
beta=2;
%initial:


map=Mapclass();
%map.show();

ant=[];
for i=1:num_ant
    ant=[ant;Antclass(i,map.num)];
end

len_min=30*sqrt(2);
path_min=[];
flag_num=0;
while (flag_num<100)
    
    num_iteration=num_iteration+1;
    flag_num=flag_num+1;
    
    for k=1:num_ant
        ant(k).init(map.num);
        ant(k).move(map,beta);
        ant(k).cal_length(map);
        
        if  (ant(k).return_len()<len_min) 
            flag_num=0;
            len_min=ant(k).return_len()
            path_min=ant(k).return_path();
        end
        
        ant(k).cal_d_ph(p_d_ph);
    end
    
    
    map.evaporate(p_eva);

    for k=1:num_ant
        map.update(ant(k));
    end
    
   % pause(0.05);
    %map.show()
    num_iteration=num_iteration+1;
end

map.show();
figure(1)
hold on;

for i=1 :  length(path_min)-1
      plot([map.node(path_min(i),1),map.node(path_min(i+1),1)],[map.node(path_min(i),2),map.node(path_min(i+1),2)],'b-','LineWidth',2);hold on;
end