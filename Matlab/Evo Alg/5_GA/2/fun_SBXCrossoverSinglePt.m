function [child]=fun_SBXCrossoverSinglePt(parent,pcross,minRealVal,maxRealVal,eta_c)
[PopSize,Dim]=size(parent);
t=randperm(PopSize);%%%随机打乱个体位置
parent=parent(t,:);%%%随机打乱个体位置
child=parent;

for i=1:PopSize/2
    if rand()>pcross
        continue;
    end
    j=ceil(rand()*Dim);
    ylow=minRealVal(j);
    yup=maxRealVal(j);
    y1=parent(2*i-1,j);
    y2=parent(2*i,j);
    r=rand();
    if r<=0.5                
        beta=(2*r).^(1/(eta_c+1));
    else
        beta=(0.5/(1.0-r)).^(1.0/(eta_c+1.0));                
    end
       child1=0.5*((1+beta)*y1+(1-beta)*y2);
       child2=0.5*((1-beta)*y1+(1+beta)*y2);   
       
       child1=min(max(child1, ylow), yup);       
       child2=min(max(child2, ylow), yup);
       
       child(2*i-1,j)=child1;
       child(2*i,j)=child2;
       child(2*i-1,j+1:Dim)=parent(2*i,j+1:Dim);
       child(2*i,j+1:Dim)=parent(2*i-1,j+1:Dim);
    
end
  
