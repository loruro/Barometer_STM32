clearvars
close all
clc

load('measurements.mat');
figureN = 1;
g = 9.80665;
M = 0.0289644;
K = 273.15;
R = 8.3144598;

%% Kapitol - up - Pressure
figure(figureN);
figureN = figureN + 1;

height = linspace(0,45,length(upkapitol(:,1)))';
pressureAproxMPL = (double(upkapitol(1,1))/1000)*exp(-(M*g*height)./((double(upkapitol(:,3))/1000+K)*R));
pressureAproxBMP = (double(upkapitol(1,2))/1000)*exp(-(M*g*height)./((double(upkapitol(:,3))/1000+K)*R));

plot(height, double(upkapitol(:,1)) / 1000, 'b');
hold on;
plot(height, double(upkapitol(:,2)) / 1000, 'k');
plot(height, pressureAproxMPL,'r');
plot(height, pressureAproxBMP,'g');

grid;
title('Kapitol - up - Pressure');
xlabel('Height [m]');
ylabel('Pressure [hPa]');
legend('MPL115A1 - measurement','BMP180 - measurement','MPL115A1 - approximation','BMP180 - approximation');

%% Kapitol - up - Temperature
figure(figureN);
figureN = figureN + 1;
plot(height, double(upkapitol(:,3)) / 1000);
grid;
title('Kapitol - up - Temperature');
xlabel('Height [m]');
ylabel('Temperature [°C]');

%% Kapitol - down - Pressure
figure(figureN);
figureN = figureN + 1;

height = linspace(45,0,length(downkapitol(:,1)))';
pressureAproxMPL = (double(downkapitol(end,1))/1000)*exp(-(M*g*height)./((double(downkapitol(:,3))/1000+K)*R));
pressureAproxBMP = (double(downkapitol(end,2))/1000)*exp(-(M*g*height)./((double(downkapitol(:,3))/1000+K)*R));

plot(height, double(downkapitol(:,1)) / 1000, 'b');
hold on;
plot(height, double(downkapitol(:,2)) / 1000, 'k');
plot(height, pressureAproxMPL,'r');
plot(height, pressureAproxBMP,'g');

grid;
title('Kapitol - down - Pressure');
xlabel('Height [m]');
ylabel('Pressure [hPa]');
legend('MPL115A1 - measurement','BMP180 - measurement','MPL115A1 - approximation','BMP180 - approximation');

%% Kapitol - down - Temperature
figure(figureN);
figureN = figureN + 1;
plot(height, double(downkapitol(:,3)) / 1000);
grid;
title('Kapitol - down - Temperature');
xlabel('Height [m]');
ylabel('Temperature [°C]');

%% B5 - up - Pressure
figure(figureN);
figureN = figureN + 1;

height = linspace(0,33,length(upb5(:,1)))';
pressureAproxMPL = (double(upb5(1,1))/1000)*exp(-(M*g*height)./((double(upb5(:,3))/1000+K)*R));
pressureAproxBMP = (double(upb5(1,2))/1000)*exp(-(M*g*height)./((double(upb5(:,3))/1000+K)*R));

plot(height, double(upb5(:,1)) / 1000, 'b');
hold on;
plot(height, double(upb5(:,2)) / 1000, 'k');
plot(height, pressureAproxMPL,'r');
plot(height, pressureAproxBMP,'g');

grid;
title('B5 - up - Pressure');
xlabel('Height [m]');
ylabel('Pressure [hPa]');
legend('MPL115A1 - measurement','BMP180 - measurement','MPL115A1 - approximation','BMP180 - approximation');

%% B5 - up - Temperature
figure(figureN);
figureN = figureN + 1;
plot(height, double(upb5(:,3)) / 1000);
grid;
title('B5 - up - Temperature');
xlabel('Height [m]');
ylabel('Temperature [°C]');

%% B5 - down - Pressure
figure(figureN);
figureN = figureN + 1;

height = linspace(33,0,length(downb5(:,1)))';
pressureAproxMPL = (double(downb5(end,1))/1000)*exp(-(M*g*height)./((double(downb5(:,3))/1000+K)*R));
pressureAproxBMP = (double(downb5(end,2))/1000)*exp(-(M*g*height)./((double(downb5(:,3))/1000+K)*R));

plot(height, double(downb5(:,1)) / 1000, 'b');
hold on;
plot(height, double(downb5(:,2)) / 1000, 'k');
plot(height, pressureAproxMPL,'r');
plot(height, pressureAproxBMP,'g');

grid;
title('B5 - down - Pressure');
xlabel('Height [m]');
ylabel('Pressure [hPa]');
legend('MPL115A1 - measurement','BMP180 - measurement','MPL115A1 - approximation','BMP180 - approximation');

%% B5 - down - Temperature
figure(figureN);
figureN = figureN + 1;
plot(height, double(downb5(:,3)) / 1000);
grid;
title('B5 - down - Temperature');
xlabel('Height [m]');
ylabel('Temperature [°C]');

%% Histogram - MPL115A1 - Pressure
figure(figureN);
figureN = figureN + 1;
hist(double(oss0(:,1))/1000, 100);
grid;
title('Histogram - MPL115A1 - Pressure');
xlabel('Pressure [hPa]');

%% Histogram - BMP180 - Pressure - supersampling: 0
figure(figureN);
figureN = figureN + 1;
hist(double(oss0(:,2))/1000, 100);
grid;
title('Histogram - BMP180 - Pressure - supersampling: 0');
xlabel('Pressure [hPa]');

%% Histogram - BMP180 - Pressure - supersampling: 1
figure(figureN);
figureN = figureN + 1;
hist(double(oss1(:,2))/1000, 100);
grid;
title('Histogram - BMP180 - Pressure - supersampling: 1');
xlabel('Pressure [hPa]');

%% Histogram - BMP180 - Pressure - supersampling: 2
figure(figureN);
figureN = figureN + 1;
hist(double(oss2(:,2))/1000, 100);
grid;
title('Histogram - BMP180 - Pressure - supersampling: 2');
xlabel('Pressure [hPa]');

%% Histogram - BMP180 - Pressure - supersampling: 3
figure(figureN);
figureN = figureN + 1;
hist(double(oss3(:,2))/1000, 100);
grid;
title('Histogram - BMP180 - Pressure - supersampling: 3');
xlabel('Pressure [hPa]');

%% Histogram - BMP180 - Temperature
figure(figureN);
figureN = figureN + 1;
hist(double(oss3(:,3))/1000, 100);
grid;
title('Histogram - BMP180 - Temperature');
xlabel('Temperature [°C]');