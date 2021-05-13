# -*- coding: utf-8 -*-
n = 200
N = 0

points = []
x = n+1
y = 1
for i in range(1,n+1):
    points.append((x,y))
    points.append((-x,y))
    points.append((x,-y))
    points.append((-x,-y))
    x -= 1
    y += n+2-i

print(len(points))
for i in range(len(points)):
    print("{} {}".format(points[i][0], points[i][1]))