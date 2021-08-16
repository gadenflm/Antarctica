clc
clear 
close all

%parameter:
%gen_len=20;
gen_len=100;
num_gen=100;
p_mutation=0.05;

max_nochange=100;

% 1.init:
figure(1)
gene_pa=[];
gene_son=[];
for i=1:num_gen
    gene_pa=[gene_pa;gene_B_class(i,gen_len)];
    gene_son=[gene_son;gene_B_class(i,gen_len)];
end

flag_num=0;
ite_num=0;
global_best=-500;
best_gene=[];

while(flag_num<max_nochange)

    ite_num=ite_num+1;   
    flag_num=flag_num+1;
    for i=1:2:num_gen
       [gene_son(i).data,gene_son(i+1).data]=gene_pa(i).cowith(gene_pa(i+1));  
    end
    
    for i=1:num_gen
        gene_son(i).mutation(p_mutation);   
    end

    val_total=zeros(2*num_gen,2);
    
    for i=1:num_gen
        val_total(i,1)=func_val(gene_pa(i).data);
        val_total(i,2)=i;
        val_total(num_gen+i,1)=func_val(gene_son(i).data);
        val_total(num_gen+i,2)=num_gen+i;  
    end
    
    val_sort=sortrows(val_total,-1);  %-1:descend
    
    if (global_best< val_sort(1,1))
        flag_num=0;
        global_best= val_sort(1,1);
        if (val_sort(1,2) >100)
            best_gene=gene_son(val_sort(1,2)-num_gen).data;
        else
            best_gene=gene_pa(val_sort(1,2)).data;
        end
    end
    
    gene_temp=zeros(gen_len,num_gen);
    for i=1:num_gen
         if (val_sort(i,2) >100)
             gene_temp(:,i)=gene_son(val_sort(i,2)-num_gen).data;
         else
             gene_temp(:,i)=gene_pa(val_sort(i,2)).data;
         end
    end
    
    for i=1:num_gen
        gene_pa(i).data=gene_temp(:,i);
    end
    plot(ite_num,global_best,'ro');hold on;
end

best_gene'
global_best
ite_num-100