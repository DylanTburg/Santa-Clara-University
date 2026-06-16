function [timeUpdate, TxPowdB, SNRdB] = PowerControlFunc(f, v, l)
%
% Generate a baseband channel for a power control demo
%
    maxTimeSamp = 1e6;                        % Maximum number of time samples
    cFreq = f;                                % carrier frequency of the signal
    SampTime = 1e-6;                          % 1 micro second sampling time.
    velocity = v;                             % velocity of the transmitter in m/s
    c = 3e8;                                  % speed of light in meters/s
    fdMax = cFreq/c*velocity;                 % Maximum Doppler frequency
    timeVector = [1:maxTimeSamp]*SampTime;    % the time vector for the fading
    MaxDist = SampTime*maxTimeSamp*velocity;  % Maximum distance traveled
    nTaps = 1;                                % assume a 1-tap channel (flat fading)
    
    %
    % Now get an instantiation of a Rayleigh-fading channel for these parameters.
    %
    
    Latency = l;                            % Latency is 5 milliseconds to update power from the command.
    
    % For now we will assume there is the same Latency to measure the power and
    % to receive the update from the receiver and adjust the transmitted power.
    
    Fading=Jakes(timeVector,nTaps,fdMax);
    DesSNRdB=10;
    
    %
    % Desired SNR in dB is 10. We want the received power (abs(Fading).^2) just 
    % hovering around 10 dB. We will adjust the transmitted power from the received
    % power in increments of 2 dB.  Noise power just to make life easier is assumed 
    % to be 1 in the same units as power. So the received power is converted
    % to SNR
    
    Delay = Latency/SampTime ;                  % Number of samples to wait until the trasmitted power is updated
    TxPowdB(1)= 10;                             % dB (Initial power);
    FadPowerdB = 10*log10(abs(Fading).^2);
    randDelay = ceil(100*rand);                 % random delay between updating the Tx power and 
    
    % received power estimate.
    
    PowContToTx =0;
    NumTxUpdate = 1;
    timeUpdate=[];
    for ii=1:maxTimeSamp
      SNRdB(ii) = TxPowdB(NumTxUpdate) + FadPowerdB(ii);
      % Power control at the Tx based on feedback from Rx
       if mod(ii+randDelay,Delay)==0;
          NumTxUpdate=NumTxUpdate+1;
          TxPowdB(NumTxUpdate)=TxPowdB(NumTxUpdate-1) + PowContToTx;
          timeUpdate=[timeUpdate ii*SampTime];
       end
      if mod(ii,Delay)==0 % Measure the SNR 
    
    % If SNR is too low, crank it up 2 dB
    % If too high, go down 2 dB.

         if (SNRdB(ii)> DesSNRdB)
             PowContToTx = -2;
         else
             PowContToTx = 2;
         end
       end
    end
    
    
end