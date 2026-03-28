syms x
f = (exp(-3*x)+10*sin(x+1))/(cos(x+1))
df1 = diff(f,x)
g = (exp(-2 *x))/(1+exp(-2*x))
df2 = diff(g,x)

h = -4*x^2
If3 = int(h,x,[-2,2])
