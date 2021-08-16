clc
clear

background=double(imread('background.jpg'))/255;
foreground=double(imread('foreground.jpg'))/255;

maskb=zeros(size(background,1),size(background,2));
maskf=zeros(size(foreground,1),size(foreground,2)); 

 maskb(50:175,651:950)=1;
 maskf(25:150,1:300)=1;

 imshow(maskf);
 figure;
 imshow(maskb);
 
% F(:,:,1) = poisson_gray( background(:,:,1), foreground(:,:,1),maskb, maskf);
% F(:,:,2) = poisson_gray( background(:,:,2), foreground(:,:,2),maskb, maskf);
% F(:,:,3) = poisson_gray( background(:,:,3), foreground(:,:,3), maskb, maskf);
% 
% subplot(131),imshow(foreground,[]);
% subplot(132),imshow(background,[]);
% subplot(133), imshow(F,[]);

