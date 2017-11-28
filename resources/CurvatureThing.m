clear all
close all
clc

x1 = [0:0.1:2];
x2 = [0,0.09,0.18,0.27,0.36,0.45,0.55,0.65,0.75,0.85,0.96,1.07,1.18,1.29,1.40,1.50,1.60,1.70,1.80,1.90,2];

% x1 = [0:0.1:10];
% x2 = x1*0.9;

k = zeros(size(x1));
d = zeros(size(x1));
theta = zeros(size(x1));
x = zeros(size(x1));
y = zeros(size(x1));

D = 0.1;

for i=2:numel(x1)
    d(i) = (x1(i)-x1(i-1)+x2(i)-x2(i-1))/2
    k(i) = 2/D * (x1(i)-x1(i-1)-x2(i)+x2(i-1))/(x1(i)-x1(i-1)+x2(i)-x2(i-1));
    theta(i) = theta(i-1) + d(i)*k(i)*180/pi;
    x(i) = x(i-1) + sind(theta(i))*d(i);
    y(i) = y(i-1) + cosd(theta(i))*d(i);
end

plot(k)
figure
plot(x,y,'-o')
axis equal