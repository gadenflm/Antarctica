clc 
clear
close all

load('coordinates.mat');
node=coordinates;

num=length(node);
link=zeros(num);

for i=1:num
    for j=1:num   
        link(i,j)=sqrt((node(i,1)-node(j,1)).^2+(node(i,2)-node(j,2)).^2);
    end
end


