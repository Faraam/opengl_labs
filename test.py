import math
import matplotlib.pyplot as plt

def solve_r(phi, p):
    sqrtp = math.sqrt(p)
    c = math.cos(phi)
    s = math.sin(phi)
    k = (c*c) * (s*s*s)
    eps = 1e-12
    if abs(k) < eps:
        return sqrtp

    def f(r):
        return (r*r - p)**3 - (r**5) * sqrtp * k

    a = 0
    b = 1
    fa = f(a)
    for _ in range(70):
        m = 0.5 * (a + b)
        fm = f(m)
        if abs(fm) <= eps:
            return 0.5 * (a + b)
        if fa * fm <= 0.0:
            b, fb = m
        else:
            a, fa = m, fm
    return 0

p = 0.1
pcount = 2000
minf, maxf = 0.0, 2.0 * math.pi

x, y = [], []
for i in range(pcount):
    phi = minf + (maxf - minf) * i / (pcount - 1)
    r = solve_r(phi, p)
    print(phi,r)
    x.append(r * math.cos(phi))
    y.append(r * math.sin(phi))

# рисование
plt.figure(figsize=(6,6))
plt.plot(x, y)
plt.axhline(0, color='gray', lw=0.5)
plt.axvline(0, color='gray', lw=0.5)
plt.gca().set_aspect('equal')
plt.title("Полярная кривая")
plt.show()
