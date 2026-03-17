# SIT

Simple Image Transformer

A collection of image transformations.

## Usage

For the most basic usage, you can just run:

```console
sit ./path/to/image/image -t <transformation1> <transformation2> -o ./path/to/output
```

```console
Simple Image Transformer


sit [OPTIONS] image_file


POSITIONALS:
  image_file TEXT:FILE REQUIRED
                              Path to image

OPTIONS:
  -h,     --help              Print this help message and exit
  -o,     --output TEXT       Path to output resulting image
  -t,     --transform TEXT ... 
                              Transformations to perform in order
          --blur-size INT     Blur size
          --blur-pass INT     Number of blur passes
          --blur-sigma FLOAT  Sigma for gaussian blur
```
