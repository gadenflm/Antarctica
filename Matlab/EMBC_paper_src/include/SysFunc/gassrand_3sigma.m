function [ outrand ] = gassrand_3sigma( mean,sta_deviation )
    outrand=mean+randn()*sta_deviation;
    
    while ( ( (mean-3*sta_deviation)>outrand ) || ( ( (mean+3*sta_deviation)<outrand) ) )
        outrand=mean+randn()*sta_deviation;
    end
end

