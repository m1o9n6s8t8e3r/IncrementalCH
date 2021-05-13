#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 12 18:33:53 2021

@author: isaac
"""

from scipy.spatial import ConvexHull
import matplotlib.pyplot as plt
import numpy as np


def ViewHull(gridLength, numPoints):
    points = np.random.randint(0, gridLength, size=(numPoints, 2))  # Random points in 2-D
    hull = ConvexHull(points)
    for plot_id in (1, 2):
        fig, ax = plt.subplots(ncols=1, figsize=(5, 3))
        ax.plot(points[:, 0], points[:, 1], '.', color='k')
        if plot_id == 1:
            ax.set_title('Given points')
        else:
            ax.set_title('Convex hull')
            for simplex in hull.simplices:
                ax.plot(points[simplex, 0], points[simplex, 1], 'c')
            ax.plot(points[hull.vertices, 0], points[hull.vertices, 1], 'o', mec='r', color='none', lw=1, markersize=10)
        ax.set_xticks(range(0, gridLength, gridLength // 10))
        ax.set_yticks(range(0, gridLength, gridLength // 10))
    plt.show()
    
#ViewHull(20, 15)
    

gridLength = 20
numPoints = 15;
points = np.random.randint(0, gridLength, size=(numPoints, 2)) # Random points in 2-D

def ViewSeqHull(gridLength, points, curPoint):
    hull = ConvexHull(points[:curPoint])
    for plot_id in (1, 2):
        if (plot_id == 1):
            continue
        fig, ax = plt.subplots(ncols=1, figsize=(5, 3))
        ax.plot(points[:, 0], points[:, 1], '.', color='k')
        ax.plot(points[curPoint, 0], points[curPoint, 1], 'o', mec='r', color='blue', lw=1, markersize=10)
        if plot_id == 1:
            ax.set_title('Given points')
        else:
            ax.set_title('Convex hull')
            for simplex in hull.simplices:
                ax.plot(points[simplex, 0], points[simplex, 1], 'c')
            ax.plot(points[hull.vertices, 0], points[hull.vertices, 1], 'o', mec='r', color='none', lw=1, markersize=10)
        ax.set_xticks(range(0, gridLength, gridLength // 10))
        ax.set_yticks(range(0, gridLength, gridLength // 10))
    plt.show()

for i in range(3,numPoints):
    ViewSeqHull(gridLength, points, i)

def visible2D(v, t):
    p0 = t[0]
    p1 = t[1]
    a = p2[0]-p1[0]
    b = p2[1]-p1[1]
    c = v[0]-p0[0]
    d = v[1]-p0[1]
    crossprod = (a*d) - (b*c)
    return crossprod > 0

ParallelPoints = [
        [8,4],
        [4,8],
        [12,8],
        [8,13],
        [2,2],
        [15,16],
        [14,3]
        ]

def viewFacets(gridLength, points, facets):
    fig, ax = plt.subplots(ncols=1, figsize=(5, 3))
    ax.plot(points[:, 0], points[:, 1], '.', color='k')
    ax.set_xticks(range(0, gridLength, gridLength // 10))
    ax.set_yticks(range(0, gridLength, gridLength // 10))
    plt.show()
    