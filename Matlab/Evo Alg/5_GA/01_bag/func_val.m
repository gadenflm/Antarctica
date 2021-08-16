function y = func_val( x )
   % volume=[3,7,7,8,9,8,1,6,10,1,6,10,2,6,2,8,4,2,10,8];
    %value=[4,4,2,6,3,10,6,4,3,2,10,1,1,5,9,10,7,4,10,1];
    volume=ceil(10*rand(1, 100));
    value=ceil(10*rand(1, 100));
    capacity=60;
    punishment=6;
    
    val_total=value*x;
    volu_total=volume*x;
    
    if volu_total>capacity
        y=val_total-punishment*(volu_total-capacity);
    else
        y=val_total;
    end

    
end

