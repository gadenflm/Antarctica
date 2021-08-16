function [ i,j ] = xy2ij( x , y )
    global total_x;
    global total_y;
    
    intx=round(x);
    inty=round(y);
    if (intx<total_x(1)) intx=total_x(1); end
    if (intx>total_x(length(total_x))) intx=total_x(length(total_x)); end    
    
    if (inty>total_y(1)) inty=total_y(1); end
    if (inty<total_y(length(total_y))) inty=total_y(length(total_y)); end
    
    i=find(total_y==inty);
    j=find(total_x==intx);

end

