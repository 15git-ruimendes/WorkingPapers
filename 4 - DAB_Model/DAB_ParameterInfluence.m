# DAB Model

pkg load signal
close all

# Z type integrator function

function out = intstep(Signal,Step,L)
  i = 0;
  out = zeros(size(Signal));
  while (i < length(Signal))
    i = i +1;
    if (i > 1)
      out(i) = out(i-1) + 1/L*Signal(i)*Step;
    else
      out(i) = 0;
    endif
  endwhile
  out = out - mean(out);
  return
endfunction

# Static Parameters

V1   = 3000;
V2   = 750;
fsw  = 7500;
n    = 1500/300;

Step = 1e-5;
Time = 0:Step:1e-3;
NoSteps = length(Time);
# Non-Static Parameters

VL  = zeros(size(Time));
L   = 1e-6:(1e-5 - 1e-6)/50:1e-5;
Phi = -2*pi:(4*pi)/50:2*pi;
Power = zeros(length(L),length(L));

for i = 1:length(Phi)
  for j = 1:length(L)
    VL = V1*square(2*pi*fsw*Time) - n*V2*square(2*pi*fsw*Time + Phi(i));
    IL = intstep(VL,Step,L(j));
    Po = (IL.*VL);
    Power(i,j) = mean(Po);
  endfor
  disp("Next Phi")
  i
endfor

figure(1)
surf(L,Phi,Power)
title('Power Transfer Surface @7500Hz Commutation and 1500/300 Transformer ratio')
xlabel('Transformer Inductance')
ylabel('Phase Shift Angle')
zlabel('Mean Power Transfer')
figure(2)
contour(L,Phi,Power)
title('Power Transfer Surface @7500Hz Commutation and 1500/300 Transformer ratio')
xlabel('Transformer Inductance')
ylabel('Phase Shift Angle')
zlabel('Mean Power Transfer')

Phi = pi/2;
fsw = 2000:(2500-2000)/50:2500;
L   = 1e-6:(1e-5 - 1e-6)/50:1e-5;
Power = zeros(length(L),length(L));

for i = 1:length(fsw)
  for j = 1:length(L)
    VL = V1*square(2*pi*fsw(i)*Time) - n*V2*square(2*pi*fsw(i)*Time + Phi);
    IL = intstep(VL,Step,L(j));
    Po = (IL.*VL);
    Power(i,j) = mean(Po);
  endfor
  disp("Next Phi")
  i
endfor

figure(3)
surf(L,fsw,Power)
title('Power Transfer Surface @PI/2 Phase Shift and 1500/300 Transformer ratio')
xlabel('Transformer Inductance')
ylabel('Switching Frequency')
zlabel('Mean Power Transfer')

figure(4)
contour(L,fsw,Power)
title('Power Transfer Surface @PI/2 Phase Shift and 1500/300 Transformer ratio')
xlabel('Transformer Inductance')
ylabel('Switching Frequency')
zlabel('Mean Power Transfer')


Time = 0:1e-6:5e-2;
VL = V1*square(2*pi*2500*Time) - n*V2*square(2*pi*2500*Time + pi/2);
IL = intstep(VL,1e-6,1e-3);
Po = (IL.*VL);
figure(5)
subplot(2,1,1);hold on;
plot(Time,VL);
plot(Time,IL);
subplot(2,1,2);
plot(Time,Po);












