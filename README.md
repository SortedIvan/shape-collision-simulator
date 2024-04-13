# shape-collision-simulator

This repository is based on the simulation of collision, using an algorithm implementation of the <b>Seperating Axis Theorem (SAT) </b>.

The implementation is done entirely with C++, where the rendering and graphics are done with SFML, which is a multi-media library based on OpenGL

<h4> How does SAT work?</h4>
The seperating axis theorem is mostly used in games for collision detection. It is based on finding an axis between two objects that seperates them, indicating that they are not colliding.

SAT only works for convex shapes, it does not work correctly for concave shapes.

![algo board](https://github.com/SortedIvan/shape-collision-simulator/assets/62967263/00d57293-c8bf-4aeb-8429-5592772e3e8f)

The basis of the Seperating Axis Theorem is to project all of the vertices of an object onto selected axis, checking whether they produce any overlaps.

The below image demonstrates projecting the vertices of two triangles onto the y axis. This can be done by taking the dot product of the vertex and the selected axis (the y axis in this case)

![algo board (1)](https://github.com/SortedIvan/shape-collision-simulator/assets/62967263/38137025-4804-4738-b715-990f9e5f3d6a)

Then, we take the minimum and maximum values and check for any overlaps. This shows us whether the shapes are colliding on that particular axis. 
In the image shown below, we can see that they collide on they y-axis, however, they don't collide on the x-axis. 

![algo board (2)](https://github.com/SortedIvan/shape-collision-simulator/assets/62967263/540b15a1-ad46-4b44-9522-b8097816037e)

Thus, in order to check all axis, we first calculate the normals of the shapes's edges and project the vertices onto the normals. 
This means that for a triangle, we project onto 3 axis - the normals of the triangle's edges. 

SAT has an early stop strategy, in the sense that once a seperating axis is found, there is no need to further continue and we can safely conclude that the shapes are indeed not colliding.
