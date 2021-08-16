function [out] = func_1( x )

func1=@(x)(1-cos(3*x)*exp(-x));
func2=@(x)(1+cos(3*x)*exp(-x));

if (x<0.8) 
    out= func1(x);
else
    out= func2(x)
end


end

