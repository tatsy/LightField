LightField
==========

Light field image viewer with Qt5.

## Installation

You need Qt5 to build this project. With Qt5, you can easily build the project using CMake.

```shell
$ git clone https://github.com/tatsy/LightField.git
$ cmake .
$ cmake --build .
```

## Usage

#### Datasets

This viewer accepts light field images from [Stanford Light Field Archive](http://lightfield.stanford.edu/lfs.html). Please download the rectified datasets at this archive.

#### Load

Run the viewer, push the ```Load``` button, and choose the unzipped folder ```rectified``` (you can change the folder name, but please make sure that the chosen directory contains all the downloaded images).

#### Interface

* Focus and aperture size

With the sliders in the right, you can change the focus and aperture size of the camera.

* View point

By clicking and dragging on the view, you can change the view point.

## Screen shot

![Light field viewer](output/light_field_output.jpg)

## License

* MIT License, Tatsuya Yatagawa (tatsy)
