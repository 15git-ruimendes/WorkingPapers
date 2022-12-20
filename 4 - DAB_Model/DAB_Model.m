# DAB Model SPS

pkg load signal
clf

function out = intstep(Signal,Step,L)
  i = 0;
  out = zeros(size(Signal));
  while (i < length(Signal))
    i = i +1;
    if (i > 1)
      out(i) = out(i-1) + 1/L*Signal(i)*Step/2;
    else
      out(i) = 0;
    endif
  endwhile
  out = out - mean(out);
  return
endfunction


step    = 0.000001
time    = 0:step:0.1;
V1      = 3000;
V2      = 750;
IOut_N  = 150; # Nominal power = 150*750 = 112k500W;
n       = 1200/750;
fsw     = 2500;
phi     = pi/2;
L       = 5e-3;
IL      = 0;

V1T = V1*square(2*pi*fsw*time);
V2T = n*V2*square(2*pi*fsw*time + phi);
VL      = V1*square(2*pi*fsw*time) - n*V2*square(2*pi*fsw*time + phi);

IL      = intstep(VL,step,L);


figure(1);
subplot(3,1,1);
plot(time,V1T);
hold on;
plot(time,V2T);
hold off;
subplot(3,1,2);
plot(time,IL);
subplot(3,1,3);
plot(time,VL);

figure(2);
plot(time,VL.*IL);


phi = -2*pi:0.25:pi*2;
j = 0;
length(phi);
Power = zeros(size(phi));
while j < length(phi)
  j = j+1

  VL      = V1*square(2*pi*fsw*time) - n*V2*square(2*pi*fsw*time + phi(j));
  IL      = intstep(VL,step,L);

   Po = (IL.*VL);
   Power(j) = mean(Po);
endwhile

figure(3)
plot(phi,Power);


L = 0:0.00005:5e-3;
j = 0;
length(L);
Power = zeros(size(L));
phi = pi/2;
while j < length(L)
  j = j+1

  VL      = V1*square(2*pi*fsw*time) - n*V2*square(2*pi*fsw*time + phi);
  IL      = intstep(VL,step,L(j));

   Po = (IL.*VL);
   Power(j) = mean(Po);
endwhile

figure(4)
plot(L,Power);
