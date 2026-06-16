%
%
% this function calculates a Rayleigh fading model according to a modified
% Jakes formula. See ’Jakes Fading Model Revisited’ by
% Dent, Bottomley and Croft, IEE Electronics Letters, June 24, 1993.
function [Fading] = Jakes(timeVector, nTaps, fdMax);
%
% Input variables: timeVector -- vector of time (in seconds)
% nTaps -- number of independent fading taps
% fdMax -- Maximum Doppler frequency
% Output variable: Fading -- an timeVector x nTaps array of independent
% complex fading values.
%
N0=16;
%
% Get N0 Walsh Hadamard vectors;
%
WHstart = hadamard(N0);
WH = WHstart(:,2:N0);
beta = pi*[1:N0]/N0;
alpha = 2*pi*([1:N0]-0.5)/N0;
freqVector = fdMax*cos(alpha);
theta = 2*pi* rand(1,N0);
Fading = zeros(length(timeVector),nTaps);
for nn=1:N0
Fading = (exp(j*beta(nn))* cos(2*pi*freqVector(nn)*timeVector+theta(nn))).'*WH(nn,1:nTaps) ...
 +Fading;
end;
Fading = Fading*sqrt(2/N0);
