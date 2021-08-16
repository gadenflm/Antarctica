function [ out_data ] = func_duigou( in_data,sta,sto,Am)

    data=in_data;
    len=length( data);
    
    mult=ones(len,1);
   
    start=round(sta*len);
    stop=round(sto*len);
    
    size=stop-start;
    i=start;
%     while (i<=stop)
%         i=i+1;
%         x=(i-start)/size*3+0.3025;
%         y=x+1/x;
%         mult(i)=1-(1-y/(0.3025+1/0.3025))*Am;
%     end
    
   while (i<=stop)
       i=i+1;
       x=(i-start)/size+0.62;
       y=x+1/x;
       mult(i)=1-(1-y/(0.62+1/0.62))*Am;
   end
    length(mult)
    data= data.*mult;
    
%     figure;
%     plot(data(:,1),mult); 
%     title('multiply distribution');
%     
    out_data=data;
end

