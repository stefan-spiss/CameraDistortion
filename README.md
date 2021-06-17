# CameraDistortion

Small project that implements different camera models with different distortions and which allows to apply the distortions to images.

## Camera Models

### Radial and Tangential Distortion

This camera model implements the radial and tangential distortion as proposed in the [Matlab camera calibration tool box by Bouguet](http://www.vision.caltech.edu/bouguetj/calib_doc/). The pinhole camera models in [OpenCV](https://docs.opencv.org/4.5.2/d9/d0c/group__calib3d.html) and in [Matlab](https://www.mathworks.com/help/vision/ug/camera-calibration.html) follow a similar model.

Explanations about radial and tangential distortion can be found in [[1]](#1).

### Angular Distortion

This camera model implements the angular distortion as it is used in the fisheye models available in the [Matlab camera calibration tool box by Bouguet](http://www.vision.caltech.edu/bouguetj/calib_doc/) and in [OpenCV](https://docs.opencv.org/4.5.2/db/d58/group__calib3d__fisheye.html).

A equidistant projection model and a fisheye distortion similar to the one proposed in [[2]](#2) is applied. The equidistant projection is given by equation (3) in [[2]](#2) and the fisheye distortion follows equation (6) in the same work with k1=1 (already captured by focal distance f).

### Omnidirectional Mei Rives Distortion

This camera model is the same as implemented in the omnidirectional camera model in [OpenCV](https://docs.opencv.org/4.5.2/d3/ddc/group__ccalib.html) and was introduced in [[3]](#3). 

## Building and Running

### Dependencies and Requirements

- C++ compiler
- [meson](https://mesonbuild.com/) or [CMake](http://www.cmake.org/)
- [ninja](https://ninja-build.org/) or [make](https://en.wikipedia.org/wiki/Make_(software))
- [OpenCV 4](https://opencv.org/) C++ library

### Build instructions

Both build systems, **meson** and **cmake** are supported.

#### Meson

1. Move to project root directory
2. Run meson to initialize build directory: `meson build`
3. Build library and tools with ninja: `ninja -C build`

#### CMake

1. Move to project root directory
2. Create build directory and enter it: `mkdir build && cd build`
3. Run cmake: `cmake .. && cd ../`
4. Build library and tools with make: `make -C build`

### Run

In project root directory run: `./build/distort GridForDistortionEffectsScale1_5.png`

### Changing distortion parameters

Currently the distortion parameters for the different models need to be changed by hand in the source file `src/mainDistortImage.cpp`. There are parameters available for all three models.

## References

<a id="1">[1]</a> 
Janne Heikkila and Olli Silven. A four-step camera calibration procedure with implicit image correction. In Proceedings of IEEE computer society conference on computer vision and pattern recognition, pages 1106–1112, 1997.

<a id="2">[2]</a> 
Juho Kannala and Sami S Brandt. A generic camera model and calibration method for conventional, wide-angle, and fish-eye lenses. IEEE Transactions on Pattern Analysis and Machine Intelligence, 28(8):1335–1340, 2006.


<a id="3">[3]</a> 
Christopher Mei and Patrick Rives. Single View Point Omnidirectional Camera Calibration from Planar Grids. In Proceedings 2007 IEEE International Conference on Robotics and Automation, pages 3945–3950, 2007.
