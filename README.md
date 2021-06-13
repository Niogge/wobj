# wobj
_Simple Header-only C library to parse .obj 3D models and build a kdtree to speedup triangle search_

wobj.h was created during my third year of studies at AIV (accademia italiana videogiochi).
<br>
Since i'm working on a simple raytracer that utilizes said library to load objs, and since the need of at least a simple acceleration structure to execute ray-triangle intersection is huge, I decided to brush off some dust from my pure C skills and extend wobj.h with wobj_kdtree.h (and wobj_utils.h but that's there just to tidy up things, for now).

Feel free to use them however you want and blame me whenever you find something disturbing in the code :) 

#### Theory stuff
Doing raytracing by bruteforce can be a pain.<br>
For each ray you must iterate for every single triangle in the scene, and that can bring to exponential execution times.<br>
BUT, if we sort our triangles in a binary tree, searching for intersections can be way faster (just think about the time cost of looking up elements in a binary tree)<br>
Here binary space partitions come to our help! we recursively split the space into smaller boxes, so instead of looking for a triangle in a set of N (large N usually) we look for a box that intersect with our ray until we found a leaf, and only then we iterate over the triangles inside that leaf. a smaller fraction of the large number of the triangles we had before. <br>
Since we are ~~lazy~~ smart programmers, we use the power of math to get this thing done even faster:<br><br>
AABB. (I'm not going to bother you on why Axis Aligned Bounding Boxes are easier than just boxes)<br><br>
And, since we are using a [KDTree](https://en.wikipedia.org/wiki/K-d_tree), during iteration, we only need to check if we hit the box one axis at the time.<br>
In this [repository](https://github.com/Niogge/one-weekend-raytracing "raytracer") (which is just a deviation from [raytracing in one weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html "Ray Tracing in One Weekend") ) you can find a usage inside [model.cpp](https://github.com/Niogge/one-weekend-raytracing/blob/main/src/Hittables/Model.cpp). The "test.png" you find there is a 2080x1170 png where I rendered a lowpoly version of the Stanford Teapot. Without kdtree it took me 10-20 minutes for a 400xsomething version.<br> With kdtree it took a little over 5 minutes for the 2080x1170 version (keep in mind that this is raytraced on the CPU) so the benefit is tangible.
<br>
A possible improvement could be to pick a better logic to split the AABBs, probably SAH, but even with a ~~lazy~~ simple "split the box in half and whatever" is a great improvement from not having one. 

 
#### Unit Test
You can find all the tests for the library inside quad_test.h
