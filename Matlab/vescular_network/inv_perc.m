% inv_perc.m
tic
clc
clear

% Strength profile of Invasion Percolation lattice
length_IP=9;
%strength=ones(length_IP+2,length_IP+2);
%strength(2:length_IP+1,2:length_IP+1)=rand(length_IP,length_IP);
%save('strength.txt','strength','-ascii');

strength =load('strength.txt');

figure(1)
% subplot(2,2,1)
% imagesc(strength(2:length_IP+1,2:length_IP+1))
% colorbar('vert')
% hold on
% subplot(2,2,2)
% imagesc(strength(2:length_IP+1,2:length_IP+1))
% colorbar('vert')
% hold on
subplot(1,2,1)
imagesc(strength(2:length_IP+1,2:length_IP+1))
colorbar('vert')
hold on
subplot(1,2,2)
imagesc(strength(2:length_IP+1,2:length_IP+1))
colorbar('vert')
hold on

% Step 1
% Nodes
x_node=1;
y_node=1;
% subplot(2,2,1)
% plot(x_node,y_node, 'r^','LineWidth', 2,'MarkerSize',8)
% hold on
% subplot(2,2,2)
% plot(x_node,y_node, 'r^','LineWidth', 2,'MarkerSize',8)
% hold on
subplot(1,2,1)
plot(x_node,y_node, 'r^','LineWidth', 2,'MarkerSize',8)
hold on
subplot(1,2,2)
plot(x_node,y_node, 'r^','LineWidth', 2,'MarkerSize',8)
hold on

node_ves=[x_node y_node];
D_ves=size(node_ves);
occup=D_ves(1)/length_IP.^2;

% Edges
edge_ves=[];
D_edge=size(edge_ves);

% Step 2
[Y I]=min([strength(y_node+1,x_node),strength(y_node+1,x_node+2),strength(y_node,x_node+1),strength(y_node+2,x_node+1)]); % Invasion site
if I==1
%     subplot(2,2,2)
%     plot([x_node-1,x_node],[y_node,y_node],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([x_node-1,x_node],[y_node,y_node],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([x_node-1,x_node],[y_node,y_node],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;x_node-1 y_node];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;x_node-1/2 y_node];
    D_edge=size(edge_ves); 
    
elseif I==2
%     subplot(2,2,2)
%     plot([x_node+1,x_node],[y_node,y_node],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([x_node+1,x_node],[y_node,y_node],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([x_node+1,x_node],[y_node,y_node],'r-','LineWidth', 2)
    hold on
    
    node_ves=[node_ves;x_node+1 y_node];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;x_node+1/2 y_node];
    D_edge=size(edge_ves);
    
elseif I==3
%     subplot(2,2,2)
%     plot([x_node,x_node],[y_node-1,y_node],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([x_node,x_node],[y_node-1,y_node],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([x_node,x_node],[y_node-1,y_node],'r-','LineWidth', 2)
    hold on
    
    node_ves=[node_ves;x_node y_node-1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;x_node y_node-1/2];
    D_edge=size(edge_ves);
    
else
%     subplot(2,2,2)
%     plot([x_node,x_node],[y_node+1,y_node],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([x_node,x_node],[y_node+1,y_node],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([x_node,x_node],[y_node+1,y_node],'r-','LineWidth', 2)
    hold on
    
    node_ves=[node_ves;x_node y_node+1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;x_node y_node+1/2];
    D_edge=size(edge_ves);
end

% Step K=10

K=10;

while D_node(1)<=K

for i=1:D_node(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)-1/2==edge_ves(k,1) & node_ves(i,2)==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+1,node_ves(i,1));
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)+1/2==edge_ves(k,1) & node_ves(i,2)==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+1,node_ves(i,1)+2);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)==edge_ves(k,1) & node_ves(i,2)-1/2==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2),node_ves(i,1)+1);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)==edge_ves(k,1) & node_ves(i,2)+1/2==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+2,node_ves(i,1)+1);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell);
[min_def_cell I_col]=min(Y);

if I_col==1
%     subplot(2,2,2)
%     plot([node_ves(I_row(I_col),1)-1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1)-1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1)-1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)-1 node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)-1/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==2
%     subplot(2,2,2)
%     plot([node_ves(I_row(I_col),1)+1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1)+1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1)+1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)+1 node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)+1/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==3
%     subplot(2,2,2)
%     plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-1/2];
    D_edge=size(edge_ves);
    
else
%     subplot(2,2,2)
%     plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
%     hold on
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+1/2];
    D_edge=size(edge_ves);
end
end

% Step K=100

K=27;

while D_node(1)<=K

for i=1:D_node(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)-1/2==edge_ves(k,1) & node_ves(i,2)==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+1,node_ves(i,1));
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)+1/2==edge_ves(k,1) & node_ves(i,2)==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+1,node_ves(i,1)+2);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)==edge_ves(k,1) & node_ves(i,2)-1/2==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2),node_ves(i,1)+1);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge(1)
                if node_ves(i,1)==edge_ves(k,1) & node_ves(i,2)+1/2==edge_ves(k,2)
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(node_ves(i,2)+2,node_ves(i,1)+1);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell);
[min_def_cell I_col]=min(Y);

if I_col==1
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1)-1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1)-1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)-1 node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)-1/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==2
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1)+1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1)+1,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)+1 node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)+1/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==3
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-1/2];
    D_edge=size(edge_ves);
    
else
    subplot(1,2,1)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    subplot(1,2,2)
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+1,node_ves(I_row(I_col),2)],'r-','LineWidth', 2)
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+1];
    D_node=size(node_ves);
    occup=D_node(1)/length_IP.^2;
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+1/2];
    D_edge=size(edge_ves);
end
end

save('1.mat', 'node_ves', 'edge_ves');

% Additional links
i=1;
while i<=D_node(1)
    if node_ves(i,1)==1 | node_ves(i,1)==length_IP | node_ves(i,2)==1 | node_ves(i,2)==length_IP
        i=i+1;
    else
        j=1;
        count_inflow=0;
        while j<=D_edge(1)
            if (node_ves(i,1)-1/2==edge_ves(j,1) & node_ves(i,2)==edge_ves(j,2)) | (node_ves(i,1)==edge_ves(j,1) & node_ves(i,2)-1/2==edge_ves(j,2))
                j=D_edge(1)+1;
                count_inflow=1;
            else
                j=j+1;
            end
        end
        
        if count_inflow==0;
           if rand<1/2
              edge_ves=[edge_ves; node_ves(i,1)-1/2 node_ves(i,2)];
              node_ves=[node_ves; node_ves(i,1)-1 node_ves(i,2)];
              subplot(1,2,2)
              plot([node_ves(i,1) node_ves(i,1)-1],[node_ves(i,2) node_ves(i,2)],'k-','LineWidth', 2)
              hold on
           else
              edge_ves=[edge_ves; node_ves(i,1) node_ves(i,2)-1/2];
              node_ves=[node_ves; node_ves(i,1) node_ves(i,2)-1]; 
              subplot(1,2,2)
              plot([node_ves(i,1) node_ves(i,1)],[node_ves(i,2) node_ves(i,2)-1],'k-','LineWidth', 2)
              hold on
           end
        end
        
        j=1;
        count_outflow=0;
        while j<=D_edge(1)
            if (node_ves(i,1)+1/2==edge_ves(j,1) & node_ves(i,2)==edge_ves(j,2)) | (node_ves(i,1)==edge_ves(j,1) & node_ves(i,2)+1/2==edge_ves(j,2))
                j=D_edge(1)+1;
                count_outflow=1;
            else
                j=j+1;
            end
        end
        
        if count_outflow==0;
           if rand<1/2
              edge_ves=[edge_ves; node_ves(i,1)+1/2 node_ves(i,2)];
              node_ves=[node_ves; node_ves(i,1)+1 node_ves(i,2)];
              subplot(1,2,2)
              plot([node_ves(i,1) node_ves(i,1)+1],[node_ves(i,2) node_ves(i,2)],'k-','LineWidth', 2)
              hold on
           else
              edge_ves=[edge_ves; node_ves(i,1) node_ves(i,2)+1/2];
              node_ves=[node_ves; node_ves(i,1) node_ves(i,2)+1]; 
              subplot(1,2,2)
              plot([node_ves(i,1) node_ves(i,1)],[node_ves(i,2) node_ves(i,2)+1],'k-','LineWidth', 2)
              hold on
           end
        end
        
        D_node=size(node_ves);
        D_edge=size(edge_ves);
        
        i=i+1;
    end
end

subplot(1,2,2)
for i=1:length_IP-1
    test=0; % no vessel existing
    j=1;
    while j<=D_edge(1)
        if edge_ves(j,1)~=1
            j=j+1;
        elseif round(i+1/2-edge_ves(j,2))~=0
            j=j+1;
        else
            test=1;
            j=D_edge(1)+1;
        end
    end
    if test==0
        plot([1,1],[i,i+1],'k-','LineWidth', 2)
        hold on
    else
    end
        
    test=0; % no vessel existing
    j=1;
    while j<=D_edge(1)
        if edge_ves(j,1)~=length_IP
            j=j+1;
        elseif round(i+1/2-edge_ves(j,2))~=0
            j=j+1;
        else
            test=1;
            j=D_edge(1)+1;
        end
    end
    if test==0
        plot([length_IP,length_IP],[i,i+1],'k-','LineWidth', 2)
        hold on
    else
    end
    
    test=0; % no vessel existing
    j=1;
    while j<=D_edge(1)
        if edge_ves(j,2)~=1
            j=j+1;
        elseif round(i+1/2-edge_ves(j,1))~=0
            j=j+1;
        else
            test=1;
            j=D_edge(1)+1;
        end
    end
    if test==0
        plot([i,i+1],[1,1],'k-','LineWidth', 2)
        hold on
    else
    end
    
    test=0; % no vessel existing
    j=1;
    while j<=D_edge(1)
        if edge_ves(j,2)~=length_IP
            j=j+1;
        elseif round(i+1/2-edge_ves(j,1))~=0
            j=j+1;
        else
            test=1;
            j=D_edge(1)+1;
        end
    end
    if test==0
        plot([i,i+1],[length_IP,length_IP],'k-','LineWidth', 2)
        hold on
    else
    end
end
toc
