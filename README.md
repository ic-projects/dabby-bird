# C Project Extension - Flappy Bird
## Instructions to Build OpenCV and Link to Project

1. `cd opencv`
2. `mkdir build`
3. `cd build`
4. `cmake -D CMAKE_BUILD_TYPE=Release ..`
5. `make -j[n]` where `[n]` is the number of threads to use.
6. `cd ../..`
7. `cmake .`
8. `cmake-gui`
9. Set the `OpenCV_DIR` parameter to point to the `opencv` folder.
10. Click `Configure` and `Generate` and then close.
11. `make`
