clear all;
close all;
Vs = 12;
Is1 = 4e-3;
Is2 = 2e-3;
R1 = 1e3;
R2 = 1e3;
R3 = 1e3;
R4 = 1e3;
R5 = 1e3;
R6 = 1e3;
syms Va Vb Vc Vd

eq1 = (Va-Vb)/R1 - Is1 + (Vd-Vc)/R2 + Vd/R5 + Vd/(R3+R6) == 0;
eq2 = (Vb-Va)/R1 + Is2 + (Vc-Vd)/R2 + Vc/R4 == 0;
eq3 = Vc-Vb == Vs;
eq4 = Vd-Va == 2*Vc;

eqns = [eq1, eq2, eq3, eq4];
vars = [Va, Vb, Vc, Vd];

[C, S] = equationsToMatrix(eqns, vars)

V = inv(C) * S
