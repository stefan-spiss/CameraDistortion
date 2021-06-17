project('CameraDistortion', 'cpp', default_options : ['cpp_std=c++17', 'optimization=2', 'warning_level=3'])
opencv_dep = dependency('opencv4')
inc = include_directories('include')

src_distort = ['src/mainDistortImage.cpp', 'src/CameraDistortion.cpp']
executable('distort', include_directories : inc, sources : src_distort, dependencies : opencv_dep)
