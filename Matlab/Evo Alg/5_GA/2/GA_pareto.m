clc
clear 
close all

%parameter:

range_max=4;
range_min=-2;
num_gen=20;
maxnum_ite=30;

eta=2;


% 1.init:
figure(1)
x_ran=[range_min:0.01:range_max];
[y1_ran,y2_ran]=func_val(x_ran);
plot(y1_ran,y2_ran,'r.'); hold on;
xlabel('f1=x^2');ylabel('f2=(x-2)^2');

gene_pa=[];
gene_son=[];
for i=1:num_gen
    gene_pa=[gene_pa;gene_D_class(i, range_min, range_max)];
    gene_son=[gene_son;gene_D_class(i,range_min,range_max)];
end

ite_num=0;

while(ite_num<maxnum_ite)
    ite_num=ite_num+1;   
    for i=1:2:num_gen
       [gene_son(i).data,gene_son(i+1).data]=gene_pa(i).cowith(gene_pa(i+1),eta);  
    end
    
    for i=1:num_gen
        gene_son(i).mutation(eta,range_min,range_max);   
    end
    
    gene_total=[];
    
    for i=1:num_gen
        gene_total(2*i-1)=gene_pa(i).data;
        gene_total(2*i)=gene_son(i).data;
    end
    [value_total(1,:),value_total(2,:)]=func_val(gene_total);
    
   [front_num,maxf_num] = NDSort(value_total',gene_total',inf);
   
   for i=1:maxf_num
       sort_num(i,1)=front_num(i);
       sort_num(i,2)=i;
   end
   num_sorted=sortrows(sort_num,1);

   for i=1:num_gen
 	  gene_pa(i).data=gene_total(num_sorted(i,2));
   end
   
    figure(1)
    clf
    plot(y1_ran,y2_ran,'r.'); hold on;
    [val1,val2]=func_val(gene_total(num_sorted(1:20,2)));
    plot(val1,val2,'bo'); hold on;
    xlabel('f1=x^2');ylabel('f2=(x-2)^2');
    pause(0.1)
end
