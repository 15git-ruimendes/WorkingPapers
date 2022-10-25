# DAB Parameter Determination
# Determination of leackage inductance as
# a function of switching frequency and
# maximum power.

clf
close all

MaxmiumPower = 56000;
VoltageIn = 3000;
VoltageOut = 750;

TransformerGain = 0.25;
SwitchingFrequency = 500:3000/200:3500;
Inductance = zeros(length(SwitchingFrequency));

for j = 1:length(SwitchingFrequency)
   Inductance(j) = TransformerGain*VoltageIn*VoltageOut/(MaxmiumPower*8*SwitchingFrequency(j));
endfor


figure(1);
plot(SwitchingFrequency,Inductance);
xlabel('Switching Frequency');
title('Leackage Inductance Calculation');






