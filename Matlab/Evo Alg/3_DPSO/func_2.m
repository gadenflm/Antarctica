function y = func_2( x )
    volume=ceil(10*rand(1, 100));
    value=ceil(10*rand(1, 100));
    capacity=60;
    punishment=10;
    
    val_total=x*value';
    volu_total=x*volume';
    
    if volu_total>capacity
        y=val_total-punishment*(volu_total-capacity);
    else
        y=val_total;
    end

    
end