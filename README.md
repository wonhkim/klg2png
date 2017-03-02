# README #

Tool for converting ```klg``` format log file into a sequence of RGB and depth PNG images in local directories.

### Hardware setup

Tested with ```Ubuntu 14.04``` and ```OpenCV 2.4.13```.

### Compilation

```
#!shell

mkdir build
cd build
cmake ../src
make
```

### Usage

```
./klg2png <input_klg_file> <output_rgb_dir> <output_depth_dir> <width> <height>
```
