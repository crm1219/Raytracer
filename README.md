# Authors
Name: Camelia Mosor

Email: crm782@nau.edu

# Usage
This is a program that uses ray tracing to render plane, sphere, and triangle primitives from a scene file into a pixel grid in a ppm file. To execute this program, run "make" in the program directory, and then run the command "./raycast ppm_width ppm_height input.scene output.ppm", with ppm_width and ppm_height being the width and height in pixels of the output file.

To test, run using the given sample input scene file.

# Known Issues
None to my knowledge.

# Research
To fulfill this project, I first attempted to implement code for triangle rendering based on the algorithm discussed in class, then consulted the website https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/why-are-triangles-useful.html to fix the errors I was facing. Though the implementation was mostly similar to handling intersection with a plane, I did learn that it is important to take into account which direction the vertices of the triangle are ordered in order to determine which way the normal vector should point. The triangle I added to my input scene did not appear at first because my vertices were ordered clockwise and the normal vector needed to be reversed, but I found out the cause of the issue in the process of my research and fixed it. I also learned precisely how to determine if a point was inside a triangle using the cross-product and dot-product of vectors.
