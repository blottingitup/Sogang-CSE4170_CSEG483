# CSE4170 and CSEG483 at Sogang University
[![Static Badge](https://img.shields.io/badge/C%2B%2B-blue?logo=c%2B%2B)](https://cppreference.com/w/Main_Page.html)
[![Static Badge](https://img.shields.io/badge/github-freeglut-blue?logo=github)](https://github.com/freeglut/freeglut)
[![Static Badge](https://img.shields.io/badge/github-GLM-orange?logo=github)](https://github.com/g-truc/glm)
[![Static Badge](https://img.shields.io/badge/CUDA%20Toolkit-13.1-brightgreen?logo=nvidia)](https://docs.nvidia.com/cuda/archive/13.1.1/)  

**CSE4170: Compatible with freeglut 3.8.0.**  
  
This repository contains the code submitted for  
**Introduction to Computer Graphics (CSE4170)** and **Introduction to GPU Programming (CSEG483)**.  
The main objective is to enhance the given base code.
Direct copy is not recommended, but taking reference to *whisk up* some code of your own will be just fine.  

---
### CSE4170
**HW1: Polygon Editor Development with GLUT Toolkit**  
* Switch between Standby, Creation, Selection and Animation modes.
* A maximum of 10 polygons can exist at once; polygons must be distinguishable by their colored edges.
* Creation Mode: Press 's' in Standby Mode to enter. Place vertices using SHIFT + LMB and press 'e' to finish; the polygon's centroid will then appear.
* Selection Mode: Click the LMB on a polygon's centroid in Standby Mode. Only one polygon can be selected at once. The selected polygon can be deleted with 'c', moved with the LMB, rotated with the RMB, scaled with the mouse wheel, and flipped with the arrow keys.
* Animation Mode: Press 'a' in Selection Mode to enter. The polygon will rotate and scale simultaneously.  

**HW2: 2D Hierarchical Modeling Transformation with OpenGL**
* Create a program that has hierarchical levels 0~4 using provided objects and some custom objects.
* Level 0: Moved along the x-axis by clicking and dragging the LMB.
* Movement of level 1 and 4 objects must have on/off features.
* Level 1: Orbit counter-clockwise in a diamond shape. Press 'f' to toggle movement.
* Level 2: Orbit counter-clockwise in an ellipse while following a sinusoidal path. It also revolves counter-clockwise.
* Level 3: Orbit counter-clockwise in a circle while pulsing. It also revolves clockwise.
* Level 4: Three objects orbit counter-clockwise in a circle, each orbit having its own orbital radius. It also revolves counter-clockwise. Press 'g' to toggle movement.
* Level 4 objects also have an afterimage effect with a green-to-blue color gradient.

---
### CSEG483
**HW1: 1D Gaussian Filtering**
* Write a kernel function for 1D Gaussian filtering using a grid-stride loop.
* After issuing a dummy call, measure the average execution time of the kernel function over multiple iterations.
* Verify that both the host and kernel functions produce the same results.
* Experiment with variables that affect the overall performance, and provide a summary and analysis of the results.  

**HW2: 1D Gaussian Filtering with Wrap Condition**
* Write a kernel function for 1D Gaussian filtering using the wrap-around condition.
* Also write two additional kernel functions: one using shared memory and the other using a grid-stride loop.
* Use Nsight Compute to analyze register and shared memory usage, achieved warp occupancy and other metrics relative to thread block dimensions.
