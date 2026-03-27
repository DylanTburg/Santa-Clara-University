C1 = 1e-6;
C2 = 1.5e-6;
C3 = 5e-6;
C = 1e-7;
R1 = 600;
R2 = 800;
R3 = 1000;
Vs=2;
Ceq = 1/(1/C1+1/C2+1/C3);
V1 = Vs *(Ceq/C1)
V2 = Vs *(Ceq/C2)
V3 = Vs *(Ceq/C3)
t_R1 = C * R1
t_R2 = C * R2
t_R3 = C * R3

