function y = func_1( x )
    volume=[3,7,7,8,9,8,1,6,10,1,6,10,2,6,2,8,4,2,10,8];
    value=[4,4,2,6,3,10,6,4,3,2,10,1,1,5,9,10,7,4,10,1];
    capacity=50;
    punishment=15;
    
    val_total=x*value';
    volu_total=x*volume';
    
    if volu_total>capacity
        %y=val_total-punishment*(volu_total-capacity);
        y=-100;
    else
        y=val_total;
    end

    
end

