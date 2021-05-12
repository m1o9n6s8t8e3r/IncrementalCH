import numpy as np

def on_line(x, y, m ,b):
    if (y == m * x + b):
        return True;
    else:
        return False;

N = 300
low = -3000
high = 3000
lines = set()
vertical = set()
points = np.zeros((N, 2), dtype=int)
i = 0
while (i < N):
    x = np.random.randint(low, high=high)
    y = np.random.randint(low, high=high)
    colinear = False
    for line in lines:
        m, b = line
        if on_line(x, y, m, b):
            colinear = True;
            break;
    for X in vertical:
        if (x == X):
            colinear = True;
            break;
    if colinear:
        continue;
    for j in range(i):
        if (x == points[j][0]):
            vertical.add(y)
            continue;
        m = (y - points[j][1]) / (x - points[j][0])
        # y = mx + b
        b = y - m * x
        lines.add((m, b))
    points[i][0] = x
    points[i][1] = y
    i += 1
print(N)
for i in range(N):
    print("{} {}".format(points[i][0], points[i][1]))
