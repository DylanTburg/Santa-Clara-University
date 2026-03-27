R1 = 1.2e+3;
R2 = 2.2e+3;
R3 = 2.7e+3;
R4 = 3.3e+3;
R5 = 3.9e+3;
R6 = 3.9e+3;
Vs = 10;
syms V3 V4 V2 I2 I3; 
R2_3_4 = 1/(1/R3 + 1/R2 + 1/R4);
Req= R2_3_4+R1;
I1=Vs/Req
V1= I1 * R1
V2 = Vs-V1
V3 = Vs-V1
V4 = Vs-V1
I2 = V2/R2
I3 = V3/R3
I4 = V4/R4
