function [out] = func_1( x1,x2 )

func1=@(x1,x2)((x1-1).^2+(x2+4).^2);
punishment=1;

if (x1*x1+x2*x2-4<0)
    out= func1(x1,x2);
else
    out= func1(x1,x2)+punishment*(x1*x1+x2*x2-4);
   % out =1000;
end

end

