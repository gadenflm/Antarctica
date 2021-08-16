function F = poisson_gray( background, foreground, maskb,maskf )
[h,w] = size(background);
n = w*h;
A = sparse(n,n);
f = zeros(n,1); %% Uknown Vector
fx = find(maskb > 0);  %% Unknown Position
bx = find(maskb == 0);  %% Known Position
q = zeros(n,1);
q(fx) = 1;
% Build Laplace Matrix
I = diag(sparse(q));
A = -4*I;
A = A+circshift(I,[0 h])+circshift(I,[0 -h]); % X Comp
A = A+circshift(I,[0 1])+circshift(I,[0 -1]); % Y Comp
A = A+speye(n)-I;
b = zeros(n,1);
b(bx) = background(bx);
% Compute Laplace Foreground
lapforeground = circshift(foreground,[1 0])+circshift(foreground,[-1 0]);
lapforeground = lapforeground+circshift(foreground,[0 1])+circshift(foreground,[0 -1]);
lapforeground = lapforeground-4*foreground;
b(fx) = lapforeground(logical(maskf));
% Solve Linear System Ax = b
x = A\b;
F = reshape(x,[h w]);
