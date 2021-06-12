# wobj
_Simple Header-only C library to parse .obj 3D models and build a kdtree to speedup triangle search_

wobj.h was created during my third year of studies at AIV (accademia italiana videogiochi)
Since i'm working on a simple raytracer that utilizes said library to load objs, and since the need of at least a simple acceleration structure to execute ray-triangle intersection is huge, I decided to brush off some dust from my pure C skills and extend wobj.h with wobj_kdtree.h (and wobj_utils.h but that's there just to tidy up things, for now).

Feel free to use them however you want and blame me whenever you find something disturbing in the code :) 
#### Unit Test
You can find all the tests for the library inside quad_test.h
