% ves_network.m

%--------------------------Vessel System 1-------------------------------

clc
clear

% Layer 1
ipd_t=0.1; % intercapillary distance is 0.1mm

% Peritumoral vessel system
xp=-5:ipd_t:5; 
yp=-5:ipd_t:5;

xp_bd=3; % Boundary
yp_bd=3;

for i=1:length(xp)
    if xp(i)<=-xp_bd | xp(i)>=xp_bd
        plot([xp(i) xp(i)], [-5 5], 'k-')
        xlabel('x (mm)')
        ylabel('y (mm)')
        hold on
    else
        plot([xp(i) xp(i)], [-5 -yp_bd], 'k-')
        xlabel('x (mm)')
        ylabel('y (mm)')
        hold on
        plot([xp(i) xp(i)], [yp_bd 5], 'k-')
        hold on
    end
end

for i=1:length(yp)
    if yp(i)<=-yp_bd | yp(i)>=yp_bd
        plot([-5 5], [yp(i) yp(i)], 'k-')
        hold on
    else
        plot([-5 -xp_bd], [yp(i) yp(i)], 'k-')
        hold on
        plot([xp_bd 5], [yp(i) yp(i)], 'k-')
        hold on
    end
end

% Tumor angiogenesis
xt=-xp_bd:ipd_t:xp_bd;
yt=-yp_bd:ipd_t:yp_bd;

% Strength profile of Invasion Percolation lattice
length_IP=length(xt);
strength=ones(length_IP+2,length_IP+2);
strength(2:length_IP+1,2:length_IP+1)=rand(length_IP,length_IP);

% Step 1
% Nodes
x_node=-xp_bd;
y_node=-yp_bd;
% plot(x_node,y_node, 'k.')
% hold on

node_ves=[x_node y_node];
D_node=size(node_ves);

% Edges
edge_ves=[];
D_edge=size(edge_ves);

% Step 2
[Y I]=min([strength(round((y_node+yp_bd)/ipd_t)+2,round((x_node+xp_bd)/ipd_t)+1),...
           strength(round((y_node+yp_bd)/ipd_t)+2,round((x_node+xp_bd)/ipd_t)+3), ...
           strength(round((y_node+yp_bd)/ipd_t)+1,round((x_node+xp_bd)/ipd_t)+2),...
           strength(round((y_node+yp_bd)/ipd_t)+3,round((x_node+xp_bd)/ipd_t)+2)]); % Invasion site
if I==1
    plot([x_node-ipd_t,x_node],[y_node,y_node],'k-')
    hold on
    
    % Nodes
    node_ves=[node_ves;x_node-ipd_t y_node];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;x_node-ipd_t/2 y_node];
    D_edge=size(edge_ves); 
    
elseif I==2
    plot([x_node+ipd_t,x_node],[y_node,y_node],'k-')
    hold on
    
    node_ves=[node_ves;x_node+ipd_t y_node];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;x_node+ipd_t/2 y_node];
    D_edge=size(edge_ves);
    
elseif I==3
    plot([x_node,x_node],[y_node-ipd_t,y_node],'k-')
    hold on
    
    node_ves=[node_ves;x_node y_node-ipd_t];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;x_node y_node-ipd_t/2];
    D_edge=size(edge_ves);
    
else
    plot([x_node,x_node],[y_node+ipd_t,y_node],'k-')
    hold on
    
    node_ves=[node_ves;x_node y_node+ipd_t];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;x_node y_node+ipd_t/2];
    D_edge=size(edge_ves);
end

% Step K
occup=0.85;

K=ceil(occup*length_IP*(length_IP-1)*2);

while D_node(1)<=K

    D_node(1)-K;
    %D_node(1)-K
for i=1:D_node(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge(1)
                if round(100*(node_ves(i,1)-ipd_t/2-edge_ves(k,1)))==0 & round(100*(node_ves(i,2)-edge_ves(k,2)))==0 % Avoiding nonzero residual
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(round((node_ves(i,2)+yp_bd)/ipd_t)+2,round((node_ves(i,1)+xp_bd)/ipd_t)+1);
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge(1)
                if round(100*(node_ves(i,1)+ipd_t/2-edge_ves(k,1)))==0 & round(100*(node_ves(i,2)-edge_ves(k,2)))==0
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(round((node_ves(i,2)+yp_bd)/ipd_t)+2,round((node_ves(i,1)+xp_bd)/ipd_t)+3);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge(1)
                if round(100*(node_ves(i,1)-edge_ves(k,1)))==0 & round(100*(node_ves(i,2)-ipd_t/2-edge_ves(k,2)))==0
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(round((node_ves(i,2)+yp_bd)/ipd_t)+1,round((node_ves(i,1)+xp_bd)/ipd_t)+2);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge(1)
                if round(100*(node_ves(i,1)-edge_ves(k,1)))==0 & round(100*(node_ves(i,2)+ipd_t/2-edge_ves(k,2)))==0
                    def_cell(i,j)=1; % Excluding the existing edge
                    k=D_edge(1)+1;
                else
                    def_cell(i,j)=strength(round((node_ves(i,2)+yp_bd)/ipd_t)+3,round((node_ves(i,1)+xp_bd)/ipd_t)+2);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell);
[min_def_cell I_col]=min(Y);

if I_col==1
    plot([node_ves(I_row(I_col),1)-ipd_t,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)-ipd_t node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)-ipd_t/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==2
    plot([node_ves(I_row(I_col),1)+ipd_t,node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2),node_ves(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1)+ipd_t node_ves(I_row(I_col),2)];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1)+ipd_t/2 node_ves(I_row(I_col),2)];
    D_edge=size(edge_ves);
    
elseif I_col==3
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)-ipd_t,node_ves(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-ipd_t];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)-ipd_t/2];
    D_edge=size(edge_ves);
    
else
    plot([node_ves(I_row(I_col),1),node_ves(I_row(I_col),1)],[node_ves(I_row(I_col),2)+ipd_t,node_ves(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves=[node_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+ipd_t];
    D_node=size(node_ves);
    
    % Edges
    edge_ves=[edge_ves;node_ves(I_row(I_col),1) node_ves(I_row(I_col),2)+ipd_t/2];
    D_edge=size(edge_ves);
end
end

% Additional links
i=1;
while i<=D_node(1)
    if round(100*(node_ves(i,1)+xp_bd))==0 | round(100*(node_ves(i,1)-xp_bd))==0 | round(100*(node_ves(i,2)+yp_bd))==0 | round(100*(node_ves(i,2)-yp_bd))==0
        i=i+1;
    else
        j=1;
        count_inflow=0;
        while j<=D_edge(1)
            if (round(100*(node_ves(i,1)-ipd_t/2-edge_ves(j,1)))==0 & round(100*(node_ves(i,2)-edge_ves(j,2)))==0) ...
                    | (round(100*(node_ves(i,1)-edge_ves(j,1)))==0 & round(100*(node_ves(i,2)-ipd_t/2-edge_ves(j,2)))==0)
                j=D_edge(1)+1;
                count_inflow=1;
            else
                j=j+1;
            end
        end
        
        if count_inflow==0;
           if rand<1/2
              edge_ves=[edge_ves; node_ves(i,1)-ipd_t/2 node_ves(i,2)];
              node_ves=[node_ves; node_ves(i,1)-ipd_t node_ves(i,2)];
              plot([node_ves(i,1) node_ves(i,1)-ipd_t],[node_ves(i,2) node_ves(i,2)],'k-')
              hold on
           else
              edge_ves=[edge_ves; node_ves(i,1) node_ves(i,2)-ipd_t/2];
              node_ves=[node_ves; node_ves(i,1) node_ves(i,2)-ipd_t]; 
              plot([node_ves(i,1) node_ves(i,1)],[node_ves(i,2) node_ves(i,2)-ipd_t],'k-')
              hold on
           end
        end
        
        j=1;
        count_outflow=0;
        while j<=D_edge(1)
            if (round(100*(node_ves(i,1)+ipd_t/2-edge_ves(j,1)))==0 & round(100*(node_ves(i,2)-edge_ves(j,2)))==0) ...
                    | (round(100*(node_ves(i,1)-edge_ves(j,1)))==0 & round(100*(node_ves(i,2)+ipd_t/2-edge_ves(j,2)))==0)
                j=D_edge(1)+1;
                count_outflow=1;
            else
                j=j+1;
            end
        end
        
        if count_outflow==0;
           if rand<1/2
              edge_ves=[edge_ves; node_ves(i,1)+ipd_t/2 node_ves(i,2)];
              node_ves=[node_ves; node_ves(i,1)+ipd_t node_ves(i,2)];
              plot([node_ves(i,1) node_ves(i,1)+ipd_t],[node_ves(i,2) node_ves(i,2)],'k-')
              hold on
           else
              edge_ves=[edge_ves; node_ves(i,1) node_ves(i,2)+ipd_t/2];
              node_ves=[node_ves; node_ves(i,1) node_ves(i,2)+ipd_t]; 
              plot([node_ves(i,1) node_ves(i,1)],[node_ves(i,2) node_ves(i,2)+ipd_t],'k-')
              hold on
           end
        end
        
        D_node=size(node_ves)
        D_edge=size(edge_ves);
        
        i=i+1
    end
end

% Removing edges in the inner layer
xp_bd2=2; % Boundary
yp_bd2=2;

i=1;
while i<=D_edge(1)
    if (abs(edge_ves(i,1))<xp_bd2) & (abs(edge_ves(i,2))<yp_bd2)
        plot([edge_ves(i,1)-ipd_t/2 edge_ves(i,1)+ipd_t/2], [edge_ves(i,2) edge_ves(i,2)], 'w-')
        hold on
        plot([edge_ves(i,1) edge_ves(i,1)], [edge_ves(i,2)-ipd_t/2 edge_ves(i,2)+ipd_t/2], 'w-')
        hold on
        edge_ves(i,:)=[];
        D_edge=size(edge_ves);
    else
        i=i+1;
    end
end

i=1
while i<=D_node(1)
    if (abs(node_ves(i,1))<xp_bd2) & (abs(node_ves(i,2))<yp_bd2)
        node_ves(i,:)=[];
        D_node=size(node_ves);
    else
        i=i+1;
    end
end

% Layer 2
% Tumor angiogenesis
xt2=-xp_bd2:ipd_t:xp_bd2;
yt2=-yp_bd2:ipd_t:yp_bd2;

% Strength profile of Invasion Percolation lattice
length_IP2=length(xt2);
strength2=ones(length_IP2+2,length_IP2+2);
strength2(2:length_IP2+1,2:length_IP2+1)=rand(length_IP2,length_IP2);

% Step 1
% Nodes
x_node2=-xp_bd2;
y_node2=-yp_bd2;
% plot(x_node,y_node, 'k.')
% hold on

node_ves2=[x_node2 y_node2];
D_node2=size(node_ves2);

% Edges
edge_ves2=[];
D_edge2=size(edge_ves2);

% Step 2
[Y I]=min([strength2(round((y_node2+yp_bd2)/ipd_t)+2,round((x_node2+xp_bd2)/ipd_t)+1),...
           strength2(round((y_node2+yp_bd2)/ipd_t)+2,round((x_node2+xp_bd2)/ipd_t)+3), ...
           strength2(round((y_node2+yp_bd2)/ipd_t)+1,round((x_node2+xp_bd2)/ipd_t)+2),...
           strength2(round((y_node2+yp_bd2)/ipd_t)+3,round((x_node2+xp_bd2)/ipd_t)+2)]); % Invasion site
if I==1
    plot([x_node2-ipd_t,x_node2],[y_node2,y_node2],'k-')
    hold on
    
    % Nodes
    node_ves2=[node_ves2;x_node2-ipd_t y_node2];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;x_node2-ipd_t/2 y_node2];
    D_edge2=size(edge_ves2); 
    
elseif I==2
    plot([x_node2+ipd_t,x_node2],[y_node2,y_node2],'k-')
    hold on
    
    node_ves2=[node_ves2;x_node2+ipd_t y_node2];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;x_node2+ipd_t/2 y_node2];
    D_edge2=size(edge_ves2);
    
elseif I==3
    plot([x_node2,x_node2],[y_node2-ipd_t,y_node2],'k-')
    hold on
    
    node_ves2=[node_ves2;x_node2 y_node2-ipd_t];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;x_node2 y_node2-ipd_t/2];
    D_edge2=size(edge_ves2);
    
else
    plot([x_node2,x_node2],[y_node2+ipd_t,y_node2],'k-')
    hold on
    
    node_ves2=[node_ves2;x_node2 y_node2+ipd_t];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;x_node2 y_node2+ipd_t/2];
    D_edge2=size(edge_ves2);
end

% Step K
occup=0.7;

K=ceil(occup*length_IP2*(length_IP2-1)*2);

while D_node2(1)<=K

    D_node2(1)-K

for i=1:D_node2(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge2(1)
                if round(100*(node_ves2(i,1)-ipd_t/2-edge_ves2(k,1)))==0 & round(100*(node_ves2(i,2)-edge_ves2(k,2)))==0 % Avoiding nonzero residual
                    def_cell2(i,j)=1; % Excluding the existing edge
                    k=D_edge2(1)+1;
                else
                    def_cell2(i,j)=strength2(round((node_ves2(i,2)+yp_bd2)/ipd_t)+2,round((node_ves2(i,1)+xp_bd2)/ipd_t)+1);
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge2(1)
                if round(100*(node_ves2(i,1)+ipd_t/2-edge_ves2(k,1)))==0 & round(100*(node_ves2(i,2)-edge_ves2(k,2)))==0
                    def_cell2(i,j)=1; % Excluding the existing edge
                    k=D_edge2(1)+1;
                else
                    def_cell2(i,j)=strength2(round((node_ves2(i,2)+yp_bd2)/ipd_t)+2,round((node_ves2(i,1)+xp_bd2)/ipd_t)+3);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge2(1)
                if round(100*(node_ves2(i,1)-edge_ves2(k,1)))==0 & round(100*(node_ves2(i,2)-ipd_t/2-edge_ves2(k,2)))==0
                    def_cell2(i,j)=1; % Excluding the existing edge
                    k=D_edge2(1)+1;
                else
                    def_cell2(i,j)=strength2(round((node_ves2(i,2)+yp_bd2)/ipd_t)+1,round((node_ves2(i,1)+xp_bd2)/ipd_t)+2);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge2(1)
                if round(100*(node_ves2(i,1)-edge_ves2(k,1)))==0 & round(100*(node_ves2(i,2)+ipd_t/2-edge_ves2(k,2)))==0
                    def_cell2(i,j)=1; % Excluding the existing edge
                    k=D_edge2(1)+1;
                else
                    def_cell2(i,j)=strength2(round((node_ves2(i,2)+yp_bd2)/ipd_t)+3,round((node_ves2(i,1)+xp_bd2)/ipd_t)+2);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell2);
[min_def_cell I_col]=min(Y);

if I_col==1
    plot([node_ves2(I_row(I_col),1)-ipd_t,node_ves2(I_row(I_col),1)],[node_ves2(I_row(I_col),2),node_ves2(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves2=[node_ves2;node_ves2(I_row(I_col),1)-ipd_t node_ves2(I_row(I_col),2)];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;node_ves2(I_row(I_col),1)-ipd_t/2 node_ves2(I_row(I_col),2)];
    D_edge2=size(edge_ves2);
    
elseif I_col==2
    plot([node_ves2(I_row(I_col),1)+ipd_t,node_ves2(I_row(I_col),1)],[node_ves2(I_row(I_col),2),node_ves2(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves2=[node_ves2;node_ves2(I_row(I_col),1)+ipd_t node_ves2(I_row(I_col),2)];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;node_ves2(I_row(I_col),1)+ipd_t/2 node_ves2(I_row(I_col),2)];
    D_edge2=size(edge_ves2);
    
elseif I_col==3
    plot([node_ves2(I_row(I_col),1),node_ves2(I_row(I_col),1)],[node_ves2(I_row(I_col),2)-ipd_t,node_ves2(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves2=[node_ves2;node_ves2(I_row(I_col),1) node_ves2(I_row(I_col),2)-ipd_t];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;node_ves2(I_row(I_col),1) node_ves2(I_row(I_col),2)-ipd_t/2];
    D_edge2=size(edge_ves2);
    
else
    plot([node_ves2(I_row(I_col),1),node_ves2(I_row(I_col),1)],[node_ves2(I_row(I_col),2)+ipd_t,node_ves2(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves2=[node_ves2;node_ves2(I_row(I_col),1) node_ves2(I_row(I_col),2)+ipd_t];
    D_node2=size(node_ves2);
    
    % Edges
    edge_ves2=[edge_ves2;node_ves2(I_row(I_col),1) node_ves2(I_row(I_col),2)+ipd_t/2];
    D_edge2=size(edge_ves2);
end
end

% Additional links
i=1;
while i<=D_node2(1)
    if round(100*(node_ves2(i,1)+xp_bd2))==0 | round(100*(node_ves2(i,1)-xp_bd2))==0 | round(100*(node_ves2(i,2)+yp_bd2))==0 | round(100*(node_ves2(i,2)-yp_bd2))==0
        i=i+1;
    else
        j=1;
        count_inflow=0;
        while j<=D_edge2(1)
            if (round(100*(node_ves2(i,1)-ipd_t/2-edge_ves2(j,1)))==0 & round(100*(node_ves2(i,2)-edge_ves2(j,2)))==0) ...
                    | (round(100*(node_ves2(i,1)-edge_ves2(j,1)))==0 & round(100*(node_ves2(i,2)-ipd_t/2-edge_ves2(j,2)))==0)
                j=D_edge2(1)+1;
                count_inflow=1;
            else
                j=j+1;
            end
        end
        
        if count_inflow==0;
           if rand<1/2
              edge_ves2=[edge_ves2; node_ves2(i,1)-ipd_t/2 node_ves2(i,2)];
              node_ves2=[node_ves2; node_ves2(i,1)-ipd_t node_ves2(i,2)];
              plot([node_ves2(i,1) node_ves2(i,1)-ipd_t],[node_ves2(i,2) node_ves2(i,2)],'k-')
              hold on
           else
              edge_ves2=[edge_ves2; node_ves2(i,1) node_ves2(i,2)-ipd_t/2];
              node_ves2=[node_ves2; node_ves2(i,1) node_ves2(i,2)-ipd_t]; 
              plot([node_ves2(i,1) node_ves2(i,1)],[node_ves2(i,2) node_ves2(i,2)-ipd_t],'k-')
              hold on
           end
        end
        
        j=1;
        count_outflow=0;
        while j<=D_edge2(1)
            if (round(100*(node_ves2(i,1)+ipd_t/2-edge_ves2(j,1)))==0 & round(100*(node_ves2(i,2)-edge_ves2(j,2)))==0) ...
                    | (round(100*(node_ves2(i,1)-edge_ves2(j,1)))==0 & round(100*(node_ves2(i,2)+ipd_t/2-edge_ves2(j,2)))==0)
                j=D_edge2(1)+1;
                count_outflow=1;
            else
                j=j+1;
            end
        end
        
        if count_outflow==0;
           if rand<1/2
              edge_ves2=[edge_ves2; node_ves2(i,1)+ipd_t/2 node_ves2(i,2)];
              node_ves2=[node_ves2; node_ves2(i,1)+ipd_t node_ves2(i,2)];
              plot([node_ves2(i,1) node_ves2(i,1)+ipd_t],[node_ves2(i,2) node_ves2(i,2)],'k-')
              hold on
           else
              edge_ves2=[edge_ves2; node_ves2(i,1) node_ves2(i,2)+ipd_t/2];
              node_ves2=[node_ves2; node_ves2(i,1) node_ves2(i,2)+ipd_t]; 
              plot([node_ves2(i,1) node_ves2(i,1)],[node_ves2(i,2) node_ves2(i,2)+ipd_t],'k-')
              hold on
           end
        end
        
        D_node2=size(node_ves2)
        D_edge2=size(edge_ves2);
        
        i=i+1
    end
end

% Removing edges in the inner layer
xp_bd3=1; % Boundary
yp_bd3=1;

i=1;
while i<=D_edge2(1)
    if (abs(edge_ves2(i,1))<xp_bd3) & (abs(edge_ves2(i,2))<yp_bd3)
        plot([edge_ves2(i,1)-ipd_t/2 edge_ves2(i,1)+ipd_t/2], [edge_ves2(i,2) edge_ves2(i,2)], 'w-')
        hold on
        plot([edge_ves2(i,1) edge_ves2(i,1)], [edge_ves2(i,2)-ipd_t/2 edge_ves2(i,2)+ipd_t/2], 'w-')
        hold on
        edge_ves2(i,:)=[];      
        D_edge2=size(edge_ves2);
    else
        i=i+1;
    end
end

i=1;
while i<=D_node2(1)
    if (abs(node_ves2(i,1))<xp_bd3) & (abs(node_ves2(i,2))<yp_bd3)
        node_ves2(i,:)=[];
        D_node2=size(node_ves2);
    else
        i=i+1;
    end
end

% Layer 3
% Tumor angiogenesis
xt3=-xp_bd3:ipd_t:xp_bd3;
yt3=-yp_bd3:ipd_t:yp_bd3;

% Strength profile of Invasion Percolation lattice
length_IP3=length(xt3);
strength3=ones(length_IP3+2,length_IP3+2);
strength3(2:length_IP3+1,2:length_IP3+1)=rand(length_IP3,length_IP3);

% Step 1
% Nodes
x_node3=-xp_bd3;
y_node3=-yp_bd3;
% plot(x_node,y_node, 'k.')
% hold on

node_ves3=[x_node3 y_node3];
D_node3=size(node_ves3);

% Edges
edge_ves3=[];
D_edge3=size(edge_ves3);

% Step 2
[Y I]=min([strength3(round((y_node3+yp_bd3)/ipd_t)+2,round((x_node3+xp_bd3)/ipd_t)+1),...
           strength3(round((y_node3+yp_bd3)/ipd_t)+2,round((x_node3+xp_bd3)/ipd_t)+3), ...
           strength3(round((y_node3+yp_bd3)/ipd_t)+1,round((x_node3+xp_bd3)/ipd_t)+2),...
           strength3(round((y_node3+yp_bd3)/ipd_t)+3,round((x_node3+xp_bd3)/ipd_t)+2)]); % Invasion site
if I==1
    plot([x_node3-ipd_t,x_node3],[y_node3,y_node3],'k-')
    hold on
    
    % Nodes
    node_ves3=[node_ves3;x_node3-ipd_t y_node3];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;x_node3-ipd_t/2 y_node3];
    D_edge3=size(edge_ves3); 
    
elseif I==2
    plot([x_node3+ipd_t,x_node3],[y_node3,y_node3],'k-')
    hold on
    
    node_ves3=[node_ves3;x_node3+ipd_t y_node3];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;x_node3+ipd_t/2 y_node3];
    D_edge3=size(edge_ves3);
    
elseif I==3
    plot([x_node3,x_node3],[y_node3-ipd_t,y_node3],'k-')
    hold on
    
    node_ves3=[node_ves3;x_node3 y_node3-ipd_t];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;x_node3 y_node3-ipd_t/2];
    D_edge3=size(edge_ves3);
    
else
    plot([x_node3,x_node3],[y_node3+ipd_t,y_node3],'k-')
    hold on
    
    node_ves3=[node_ves3;x_node3 y_node3+ipd_t];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;x_node3 y_node3+ipd_t/2];
    D_edge3=size(edge_ves3);
end

% Step K
occup=0.55;

K=ceil(occup*length_IP3*(length_IP3-1)*2);

while D_node3(1)<=K

    D_node3(1)-K

for i=1:D_node3(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge3(1)
                if round(100*(node_ves3(i,1)-ipd_t/2-edge_ves3(k,1)))==0 & round(100*(node_ves3(i,2)-edge_ves3(k,2)))==0 % Avoiding nonzero residual
                    def_cell3(i,j)=1; % Excluding the existing edge
                    k=D_edge3(1)+1;
                else
                    def_cell3(i,j)=strength3(round((node_ves3(i,2)+yp_bd3)/ipd_t)+2,round((node_ves3(i,1)+xp_bd3)/ipd_t)+1);
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge3(1)
                if round(100*(node_ves3(i,1)+ipd_t/2-edge_ves3(k,1)))==0 & round(100*(node_ves3(i,2)-edge_ves3(k,2)))==0
                    def_cell3(i,j)=1; % Excluding the existing edge
                    k=D_edge3(1)+1;
                else
                    def_cell3(i,j)=strength3(round((node_ves3(i,2)+yp_bd3)/ipd_t)+2,round((node_ves3(i,1)+xp_bd3)/ipd_t)+3);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge3(1)
                if round(100*(node_ves3(i,1)-edge_ves3(k,1)))==0 & round(100*(node_ves3(i,2)-ipd_t/2-edge_ves3(k,2)))==0
                    def_cell3(i,j)=1; % Excluding the existing edge
                    k=D_edge3(1)+1;
                else
                    def_cell3(i,j)=strength3(round((node_ves3(i,2)+yp_bd3)/ipd_t)+1,round((node_ves3(i,1)+xp_bd3)/ipd_t)+2);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge3(1)
                if round(100*(node_ves3(i,1)-edge_ves3(k,1)))==0 & round(100*(node_ves3(i,2)+ipd_t/2-edge_ves3(k,2)))==0
                    def_cell3(i,j)=1; % Excluding the existing edge
                    k=D_edge3(1)+1;
                else
                    def_cell3(i,j)=strength3(round((node_ves3(i,2)+yp_bd3)/ipd_t)+3,round((node_ves3(i,1)+xp_bd3)/ipd_t)+2);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell3);
[min_def_cell I_col]=min(Y);

if I_col==1
    plot([node_ves3(I_row(I_col),1)-ipd_t,node_ves3(I_row(I_col),1)],[node_ves3(I_row(I_col),2),node_ves3(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves3=[node_ves3;node_ves3(I_row(I_col),1)-ipd_t node_ves3(I_row(I_col),2)];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;node_ves3(I_row(I_col),1)-ipd_t/2 node_ves3(I_row(I_col),2)];
    D_edge3=size(edge_ves3);
    
elseif I_col==2
    plot([node_ves3(I_row(I_col),1)+ipd_t,node_ves3(I_row(I_col),1)],[node_ves3(I_row(I_col),2),node_ves3(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves3=[node_ves3;node_ves3(I_row(I_col),1)+ipd_t node_ves3(I_row(I_col),2)];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;node_ves3(I_row(I_col),1)+ipd_t/2 node_ves3(I_row(I_col),2)];
    D_edge3=size(edge_ves3);
    
elseif I_col==3
    plot([node_ves3(I_row(I_col),1),node_ves3(I_row(I_col),1)],[node_ves3(I_row(I_col),2)-ipd_t,node_ves3(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves3=[node_ves3;node_ves3(I_row(I_col),1) node_ves3(I_row(I_col),2)-ipd_t];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;node_ves3(I_row(I_col),1) node_ves3(I_row(I_col),2)-ipd_t/2];
    D_edge3=size(edge_ves3);
    
else
    plot([node_ves3(I_row(I_col),1),node_ves3(I_row(I_col),1)],[node_ves3(I_row(I_col),2)+ipd_t,node_ves3(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves3=[node_ves3;node_ves3(I_row(I_col),1) node_ves3(I_row(I_col),2)+ipd_t];
    D_node3=size(node_ves3);
    
    % Edges
    edge_ves3=[edge_ves3;node_ves3(I_row(I_col),1) node_ves3(I_row(I_col),2)+ipd_t/2];
    D_edge3=size(edge_ves3);
end
end

% Additional links
i=1;
while i<=D_node3(1)
    if round(100*(node_ves3(i,1)+xp_bd3))==0 | round(100*(node_ves3(i,1)-xp_bd3))==0 | round(100*(node_ves3(i,2)+yp_bd3))==0 | round(100*(node_ves3(i,2)-yp_bd3))==0
        i=i+1;
    else
        j=1;
        count_inflow=0;
        while j<=D_edge3(1)
            if (round(100*(node_ves3(i,1)-ipd_t/2-edge_ves3(j,1)))==0 & round(100*(node_ves3(i,2)-edge_ves3(j,2)))==0) ...
                    | (round(100*(node_ves3(i,1)-edge_ves3(j,1)))==0 & round(100*(node_ves3(i,2)-ipd_t/2-edge_ves3(j,2)))==0)
                j=D_edge3(1)+1;
                count_inflow=1;
            else
                j=j+1;
            end
        end
        
        if count_inflow==0;
           if rand<1/2
              edge_ves3=[edge_ves3; node_ves3(i,1)-ipd_t/2 node_ves3(i,2)];
              node_ves3=[node_ves3; node_ves3(i,1)-ipd_t node_ves3(i,2)];
              plot([node_ves3(i,1) node_ves3(i,1)-ipd_t],[node_ves3(i,2) node_ves3(i,2)],'k-')
              hold on
           else
              edge_ves3=[edge_ves3; node_ves3(i,1) node_ves3(i,2)-ipd_t/2];
              node_ves3=[node_ves3; node_ves3(i,1) node_ves3(i,2)-ipd_t]; 
              plot([node_ves3(i,1) node_ves3(i,1)],[node_ves3(i,2) node_ves3(i,2)-ipd_t],'k-')
              hold on
           end
        end
        
        j=1;
        count_outflow=0;
        while j<=D_edge3(1)
            if (round(100*(node_ves3(i,1)+ipd_t/2-edge_ves3(j,1)))==0 & round(100*(node_ves3(i,2)-edge_ves3(j,2)))==0) ...
                    | (round(100*(node_ves3(i,1)-edge_ves3(j,1)))==0 & round(100*(node_ves3(i,2)+ipd_t/2-edge_ves3(j,2)))==0)
                j=D_edge3(1)+1;
                count_outflow=1;
            else
                j=j+1;
            end
        end
        
        if count_outflow==0;
           if rand<1/2
              edge_ves3=[edge_ves3; node_ves3(i,1)+ipd_t/2 node_ves3(i,2)];
              node_ves3=[node_ves3; node_ves3(i,1)+ipd_t node_ves3(i,2)];
              plot([node_ves3(i,1) node_ves3(i,1)+ipd_t],[node_ves3(i,2) node_ves3(i,2)],'k-')
              hold on
           else
              edge_ves3=[edge_ves3; node_ves3(i,1) node_ves3(i,2)+ipd_t/2];
              node_ves3=[node_ves3; node_ves3(i,1) node_ves3(i,2)+ipd_t]; 
              plot([node_ves3(i,1) node_ves3(i,1)],[node_ves3(i,2) node_ves3(i,2)+ipd_t],'k-')
              hold on
           end
        end
        
        D_node3=size(node_ves3)
        D_edge3=size(edge_ves3);
        
        i=i+1
    end
end

% Removing edges in the inner layer
xp_bd4=0.5; % Boundary
yp_bd4=0.5;

i=1;
while i<=D_edge3(1)
    if (abs(edge_ves3(i,1))<xp_bd4) & (abs(edge_ves3(i,2))<yp_bd4)
        plot([edge_ves3(i,1)-ipd_t/2 edge_ves3(i,1)+ipd_t/2], [edge_ves3(i,2) edge_ves3(i,2)], 'w-')
        hold on
        plot([edge_ves3(i,1) edge_ves3(i,1)], [edge_ves3(i,2)-ipd_t/2 edge_ves3(i,2)+ipd_t/2], 'w-')
        hold on
        edge_ves3(i,:)=[];      
        D_edge3=size(edge_ves3);
    else
        i=i+1;
    end
end

i=1;
while i<=D_node3(1)
    if (abs(node_ves3(i,1))<xp_bd4) & (abs(node_ves3(i,2))<yp_bd4)
        node_ves3(i,:)=[];
        D_node3=size(node_ves3);
    else
        i=i+1;
    end
end

% Layer 4
% Tumor angiogenesis
xt4=-xp_bd4:ipd_t:xp_bd4;
yt4=-yp_bd4:ipd_t:yp_bd4;

% Strength profile of Invasion Percolation lattice
length_IP4=length(xt4);
strength4=ones(length_IP4+2,length_IP4+2);
strength4(2:length_IP4+1,2:length_IP4+1)=rand(length_IP4,length_IP4);

% Step 1
% Nodes
x_node4=-xp_bd4;
y_node4=-yp_bd4;
% plot(x_node,y_node, 'k.')
% hold on

node_ves4=[x_node4 y_node4];
D_node4=size(node_ves4);

% Edges
edge_ves4=[];
D_edge4=size(edge_ves4);

% Step 2
[Y I]=min([strength4(round((y_node4+yp_bd4)/ipd_t)+2,round((x_node4+xp_bd4)/ipd_t)+1),...
           strength4(round((y_node4+yp_bd4)/ipd_t)+2,round((x_node4+xp_bd4)/ipd_t)+3), ...
           strength4(round((y_node4+yp_bd4)/ipd_t)+1,round((x_node4+xp_bd4)/ipd_t)+2),...
           strength4(round((y_node4+yp_bd4)/ipd_t)+3,round((x_node4+xp_bd4)/ipd_t)+2)]); % Invasion site
if I==1
    plot([x_node4-ipd_t,x_node4],[y_node4,y_node4],'k-')
    hold on
    
    % Nodes
    node_ves4=[node_ves4;x_node4-ipd_t y_node4];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;x_node4-ipd_t/2 y_node4];
    D_edge4=size(edge_ves4); 
    
elseif I==2
    plot([x_node4+ipd_t,x_node4],[y_node4,y_node4],'k-')
    hold on
    
    node_ves4=[node_ves4;x_node4+ipd_t y_node4];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;x_node4+ipd_t/2 y_node4];
    D_edge4=size(edge_ves4);
    
elseif I==3
    plot([x_node4,x_node4],[y_node4-ipd_t,y_node4],'k-')
    hold on
    
    node_ves4=[node_ves4;x_node4 y_node4-ipd_t];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;x_node4 y_node4-ipd_t/2];
    D_edge4=size(edge_ves4);
    
else
    plot([x_node4,x_node4],[y_node4+ipd_t,y_node4],'k-')
    hold on
    
    node_ves4=[node_ves4;x_node4 y_node4+ipd_t];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;x_node4 y_node4+ipd_t/2];
    D_edge4=size(edge_ves4);
end

% Step K
occup=0.4;

K=ceil(occup*length_IP4*(length_IP4-1)*2);

while D_node4(1)<=K

    D_node4(1)-K

for i=1:D_node4(1)
    for j=1:4
        if j==1
            k=1;
            while k<=D_edge4(1)
                if round(100*(node_ves4(i,1)-ipd_t/2-edge_ves4(k,1)))==0 & round(100*(node_ves4(i,2)-edge_ves4(k,2)))==0 % Avoiding nonzero residual
                    def_cell4(i,j)=1; % Excluding the existing edge
                    k=D_edge4(1)+1;
                else
                    def_cell4(i,j)=strength4(round((node_ves4(i,2)+yp_bd4)/ipd_t)+2,round((node_ves4(i,1)+xp_bd4)/ipd_t)+1);
                    k=k+1;
                end
            end
            
        elseif j==2
            k=1;
            while k<=D_edge4(1)
                if round(100*(node_ves4(i,1)+ipd_t/2-edge_ves4(k,1)))==0 & round(100*(node_ves4(i,2)-edge_ves4(k,2)))==0
                    def_cell4(i,j)=1; % Excluding the existing edge
                    k=D_edge4(1)+1;
                else
                    def_cell4(i,j)=strength4(round((node_ves4(i,2)+yp_bd4)/ipd_t)+2,round((node_ves4(i,1)+xp_bd4)/ipd_t)+3);
                    k=k+1;
                end
            end
            
        elseif j==3
            k=1;
            while k<=D_edge4(1)
                if round(100*(node_ves4(i,1)-edge_ves4(k,1)))==0 & round(100*(node_ves4(i,2)-ipd_t/2-edge_ves4(k,2)))==0
                    def_cell4(i,j)=1; % Excluding the existing edge
                    k=D_edge4(1)+1;
                else
                    def_cell4(i,j)=strength4(round((node_ves4(i,2)+yp_bd4)/ipd_t)+1,round((node_ves4(i,1)+xp_bd4)/ipd_t)+2);
                    k=k+1;
                end
             end
            
        else
            k=1;
            while k<=D_edge4(1)
                if round(100*(node_ves4(i,1)-edge_ves4(k,1)))==0 & round(100*(node_ves4(i,2)+ipd_t/2-edge_ves4(k,2)))==0
                    def_cell4(i,j)=1; % Excluding the existing edge
                    k=D_edge4(1)+1;
                else
                    def_cell4(i,j)=strength4(round((node_ves4(i,2)+yp_bd4)/ipd_t)+3,round((node_ves4(i,1)+xp_bd4)/ipd_t)+2);
                    k=k+1;
                end
            end
        end
    end
end

[Y I_row]=min(def_cell4);
[min_def_cell I_col]=min(Y);

if I_col==1
    plot([node_ves4(I_row(I_col),1)-ipd_t,node_ves4(I_row(I_col),1)],[node_ves4(I_row(I_col),2),node_ves4(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves4=[node_ves4;node_ves4(I_row(I_col),1)-ipd_t node_ves4(I_row(I_col),2)];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;node_ves4(I_row(I_col),1)-ipd_t/2 node_ves4(I_row(I_col),2)];
    D_edge4=size(edge_ves4);
    
elseif I_col==2
    plot([node_ves4(I_row(I_col),1)+ipd_t,node_ves4(I_row(I_col),1)],[node_ves4(I_row(I_col),2),node_ves4(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves4=[node_ves4;node_ves4(I_row(I_col),1)+ipd_t node_ves4(I_row(I_col),2)];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;node_ves4(I_row(I_col),1)+ipd_t/2 node_ves4(I_row(I_col),2)];
    D_edge4=size(edge_ves4);
    
elseif I_col==3
    plot([node_ves4(I_row(I_col),1),node_ves4(I_row(I_col),1)],[node_ves4(I_row(I_col),2)-ipd_t,node_ves4(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves4=[node_ves4;node_ves4(I_row(I_col),1) node_ves4(I_row(I_col),2)-ipd_t];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;node_ves4(I_row(I_col),1) node_ves4(I_row(I_col),2)-ipd_t/2];
    D_edge4=size(edge_ves4);
    
else
    plot([node_ves4(I_row(I_col),1),node_ves4(I_row(I_col),1)],[node_ves4(I_row(I_col),2)+ipd_t,node_ves4(I_row(I_col),2)],'k-')
    hold on
    
    % Nodes
    node_ves4=[node_ves4;node_ves4(I_row(I_col),1) node_ves4(I_row(I_col),2)+ipd_t];
    D_node4=size(node_ves4);
    
    % Edges
    edge_ves4=[edge_ves4;node_ves4(I_row(I_col),1) node_ves4(I_row(I_col),2)+ipd_t/2];
    D_edge4=size(edge_ves4);
end
end

% Additional links
i=1;
while i<=D_node4(1)
    if round(100*(node_ves4(i,1)+xp_bd4))==0 | round(100*(node_ves4(i,1)-xp_bd4))==0 | round(100*(node_ves4(i,2)+yp_bd4))==0 | round(100*(node_ves4(i,2)-yp_bd4))==0
        i=i+1;
    else
        j=1;
        count_inflow=0;
        while j<=D_edge4(1)
            if (round(100*(node_ves4(i,1)-ipd_t/2-edge_ves4(j,1)))==0 & round(100*(node_ves4(i,2)-edge_ves4(j,2)))==0) ...
                    | (round(100*(node_ves4(i,1)-edge_ves4(j,1)))==0 & round(100*(node_ves4(i,2)-ipd_t/2-edge_ves4(j,2)))==0)
                j=D_edge4(1)+1;
                count_inflow=1;
            else
                j=j+1;
            end
        end
        
        if count_inflow==0;
           if rand<1/2
              edge_ves4=[edge_ves4; node_ves4(i,1)-ipd_t/2 node_ves4(i,2)];
              node_ves4=[node_ves4; node_ves4(i,1)-ipd_t node_ves4(i,2)];
              plot([node_ves4(i,1) node_ves4(i,1)-ipd_t],[node_ves4(i,2) node_ves4(i,2)],'k-')
              hold on
           else
              edge_ves4=[edge_ves4; node_ves4(i,1) node_ves4(i,2)-ipd_t/2];
              node_ves4=[node_ves4; node_ves4(i,1) node_ves4(i,2)-ipd_t]; 
              plot([node_ves4(i,1) node_ves4(i,1)],[node_ves4(i,2) node_ves4(i,2)-ipd_t],'k-')
              hold on
           end
        end
        
        j=1;
        count_outflow=0;
        while j<=D_edge4(1)
            if (round(100*(node_ves4(i,1)+ipd_t/2-edge_ves4(j,1)))==0 & round(100*(node_ves4(i,2)-edge_ves4(j,2)))==0) ...
                    | (round(100*(node_ves4(i,1)-edge_ves4(j,1)))==0 & round(100*(node_ves4(i,2)+ipd_t/2-edge_ves4(j,2)))==0)
                j=D_edge4(1)+1;
                count_outflow=1;
            else
                j=j+1;
            end
        end
        
        if count_outflow==0;
           if rand<1/2
              edge_ves4=[edge_ves4; node_ves4(i,1)+ipd_t/2 node_ves4(i,2)];
              node_ves4=[node_ves4; node_ves4(i,1)+ipd_t node_ves4(i,2)];
              plot([node_ves4(i,1) node_ves4(i,1)+ipd_t],[node_ves4(i,2) node_ves4(i,2)],'k-')
              hold on
           else
              edge_ves4=[edge_ves4; node_ves4(i,1) node_ves4(i,2)+ipd_t/2];
              node_ves4=[node_ves4; node_ves4(i,1) node_ves4(i,2)+ipd_t]; 
              plot([node_ves4(i,1) node_ves4(i,1)],[node_ves4(i,2) node_ves4(i,2)+ipd_t],'k-')
              hold on
           end
        end
        
        D_node4=size(node_ves4)
        D_edge4=size(edge_ves4);
        
        i=i+1
    end
end

% Tumor

r=0.5;
phi=-pi:pi/180:pi;

plot(r*cos(phi), r*sin(phi), 'b-','LineWidth', 1)

% Boundaries
plot([-xp_bd,-xp_bd],[-yp_bd,yp_bd],'r-','LineWidth', 1)
hold on
plot([-xp_bd,xp_bd],[yp_bd,yp_bd],'r-','LineWidth', 1)
hold on
plot([xp_bd,xp_bd],[-yp_bd,yp_bd],'r-','LineWidth', 1)
hold on
plot([-xp_bd,xp_bd],[-yp_bd,-yp_bd],'r-','LineWidth', 1)
hold on

plot([-xp_bd2,-xp_bd2],[-yp_bd2,yp_bd2],'r-','LineWidth', 1)
hold on
plot([-xp_bd2,xp_bd2],[yp_bd2,yp_bd2],'r-','LineWidth', 1)
hold on
plot([xp_bd2,xp_bd2],[-yp_bd2,yp_bd2],'r-','LineWidth', 1)
hold on
plot([-xp_bd2,xp_bd2],[-yp_bd2,-yp_bd2],'r-','LineWidth', 1)
hold on

plot([-xp_bd3,-xp_bd3],[-yp_bd3,yp_bd3],'r-','LineWidth', 1)
hold on
plot([-xp_bd3,xp_bd3],[yp_bd3,yp_bd3],'r-','LineWidth', 1)
hold on
plot([xp_bd3,xp_bd3],[-yp_bd3,yp_bd3],'r-','LineWidth', 1)
hold on
plot([-xp_bd3,xp_bd3],[-yp_bd3,-yp_bd3],'r-','LineWidth', 1)
hold on

plot([-xp_bd4,-xp_bd4],[-yp_bd4,yp_bd4],'r-','LineWidth', 1)
hold on
plot([-xp_bd4,xp_bd4],[yp_bd4,yp_bd4],'r-','LineWidth', 1)
hold on
plot([xp_bd4,xp_bd4],[-yp_bd4,yp_bd4],'r-','LineWidth', 1)
hold on
plot([-xp_bd4,xp_bd4],[-yp_bd4,-yp_bd4],'r-','LineWidth', 1)
hold on

% savefile = 'ves.mat';
% save(savefile, 'node_ves', 'edge_ves', 'node_ves2', 'edge_ves2', 'node_ves3', 'edge_ves3', 'node_ves4', 'edge_ves4');

