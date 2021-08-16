% inv_perc.m

tic
clc
clear

K=700;
length_IP=30;

edge_max=8+3*(length_IP-2)+4*((length_IP-1).^2);
edge_ves=zeros(edge_max,2);
node_ves=zeros(edge_max+1,2);

% Strength profile of Invasion Percolation lattice

strength=ones(length_IP+2,length_IP+2);
strength(2:length_IP+1,2:length_IP+1)=rand(length_IP,length_IP);
%strength =load('strength.txt');

figure(1)
subplot(1,2,1);
imagesc(strength(2:length_IP+1,2:length_IP+1));
colorbar('vert');
hold on
subplot(1,2,2);
imagesc(strength(2:length_IP+1,2:length_IP+1));
colorbar('vert');
hold on

% 1 up         2 down         3 left         4 right
dire=[-1 0;1 0;0 -1;0 1];
edge_exist=zeros(length_IP,length_IP,4);

%node and edge init:
i_node=1;
j_node=1;

% x_node=j_node          y_node=i_node
node_ves=[j_node i_node];
D_node=1;
edge_ves=[];
D_edge=0;
subplot(1,2,1)
plot(i_node,j_node, 'r^','LineWidth', 2,'MarkerSize',8)
hold on
subplot(1,2,2)
plot(i_node,j_node, 'r^','LineWidth', 2,'MarkerSize',8)
hold on

while D_node<=K
    min=1;
    min_i=0;
    min_j=0;
    min_dire=0;
    for i=1:D_node
        i_node=node_ves(i,2);
        j_node=node_ves(i,1);
        
        %1 up:
        if (edge_exist(i_node,j_node,1) == 0)
            tmp=strength(i_node+dire(1,1)+1,j_node+dire(1,2)+1);
            if  tmp< min 
                min=tmp;
                min_i=i_node;
                min_j=j_node;
                min_dire=1;
            end
        end
        %2 down:
        if (edge_exist(i_node,j_node,2) == 0)
            tmp=strength(i_node+dire(2,1)+1,j_node+dire(2,2)+1);
            if  tmp< min 
                min=tmp;
                min_i=i_node;
                min_j=j_node;
                min_dire=2;
            end
        end        
        %3 left:
        if (edge_exist(i_node,j_node,3) == 0)
            tmp=strength(i_node+dire(3,1)+1,j_node+dire(3,2)+1);
            if  tmp< min 
                min=tmp;
                min_i=i_node;
                min_j=j_node;
                min_dire=3;
            end
        end        
        %4 right:
        if (edge_exist(i_node,j_node,4) == 0)
            tmp=strength(i_node+dire(4,1)+1,j_node+dire(4,2)+1);
            if  tmp< min 
                min=tmp;
                min_i=i_node;
                min_j=j_node;
                min_dire=4;
            end
        end
    end
    if (min_dire~=0)
        % node and edge:
        node_ves=[node_ves;min_j+dire(min_dire,2) min_i+dire(min_dire,1)];
        edge_ves=[edge_ves;(2*min_j+dire(min_dire,2))/2 (2*min_i+dire(min_dire,1))/2];
        edge_exist(min_i,min_j,min_dire)=1;

        if min_dire==1 
            tmp_dire=2;
        end
        if min_dire==2 
            tmp_dire=1;
        end
        if min_dire==3 
            tmp_dire=4;
        end
        if min_dire==4 
            tmp_dire=3;
        end
        edge_exist(min_i+dire(min_dire,1),min_j+dire(min_dire,2),tmp_dire)=1;

        D_edge=D_edge+1;
        D_node=D_node+1;


         %plot:   
        subplot(1,2,1)
        plot([min_j+dire(min_dire,2),min_j],[min_i+dire(min_dire,1),min_i],'r-','LineWidth', 2);
        hold on
        subplot(1,2,2)
        plot([min_j+dire(min_dire,2),min_j],[min_i+dire(min_dire,1),min_i],'r-','LineWidth', 2);
        hold on
    end
end

save('2.mat', 'node_ves', 'edge_ves');
% Additional links:
i=1;
while i<=D_node
    i_node=node_ves(i,2);
    j_node=node_ves(i,1);
    
     if (i_node~=1 && i_node~=length_IP && j_node~=1 && j_node~=length_IP)
         new_dire=0;
         
        % 1 up          3 left:
        if (edge_exist(i_node,j_node,1) == 0) &&(edge_exist(i_node,j_node,3) == 0)
            if (rand<0.5)
                new_dire=1;
           else
                new_dire=3;
           end
        end
        
        if (new_dire~=0)
            % node and edge:
            node_ves=[node_ves;j_node+dire(new_dire,2) i_node+dire(new_dire,1)];
            edge_ves=[edge_ves;(2*j_node+dire(new_dire,2))/2 (2*i_node+dire(new_dire,1))/2];
            edge_exist(i_node,j_node,new_dire)=1;

            if (new_dire==1) 
                tmp_dire=2;
            end
            if (new_dire==3) 
                tmp_dire=4;
            end

            edge_exist(i_node+dire(new_dire,1),j_node+dire(new_dire,2),tmp_dire)=1;
            D_edge=D_edge+1;
            D_node=D_node+1;

            %plot:   
            subplot(1,2,2)
            plot([j_node+dire(new_dire,2),j_node],[i_node+dire(new_dire,1),i_node],'k-','LineWidth', 2);
            hold on
            
        end
 
         new_dire=0;
        % 2 down          4 right:
        if (edge_exist(i_node,j_node,2) == 0) &&(edge_exist(i_node,j_node,4) == 0)
            if (rand<0.5)
                new_dire=2;
           else
                new_dire=4;
           end
        end
        
        if (new_dire~=0)
         
            % node and edge:
            node_ves=[node_ves;j_node+dire(new_dire,2) i_node+dire(new_dire,1)];
            edge_ves=[edge_ves;(2*j_node+dire(new_dire,2))/2 (2*i_node+dire(new_dire,1))/2];
            edge_exist(i_node,j_node,new_dire)=1;

            if (new_dire==2) 
                tmp_dire=1;
            end
            if (new_dire==4)
                tmp_dire=3;
            end
            edge_exist(i_node+dire(new_dire,1),j_node+dire(new_dire,2),tmp_dire)=1;
            D_edge=D_edge+1;
            D_node=D_node+1;

            %plot:   
            subplot(1,2,2)
            plot([j_node+dire(new_dire,2),j_node],[i_node+dire(new_dire,1),i_node],'k-','LineWidth', 2);
            hold on
            
        end        
     end
    i=i+1;
end

%arround :
subplot(1,2,2);
for  i=1:(length_IP-1)
    if ( edge_exist(i,1,2)==0 && edge_exist(i+1,1,1)==0 )
        plot([1,1],[i,i+1],'k-','LineWidth', 2)
        hold on    
    end
    
    if ( edge_exist(i,length_IP,2)==0 && edge_exist(i+1,length_IP,1)==0 )
        
        plot([length_IP,length_IP],[i,i+1],'k-','LineWidth', 2)
        hold on    
    end
    
    if ( edge_exist(1,i,4)==0 && edge_exist(1,i+1,3)==0 )
        plot([i,i+1],[1,1],'k-','LineWidth', 2)
        hold on    
    end
    
    if ( edge_exist(length_IP,i,4)==0 && edge_exist(length_IP,i+1,3)==0 )
        plot([i,i+1],[length_IP,length_IP],'k-','LineWidth', 2)
        hold on    
    end
end
occup=D_node/length_IP.^2;

toc
