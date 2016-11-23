# arm_morphop
1) morphological operations - open/close

2) sse and openmp implementations

3) toggle flags in makefile.inc


INSTALL: make

test: ./run.sh <image raw file path> <width> <height> <output raw file path> <num levels>

example 
	./run.sh data/test_image1.raw 2550 3300 data/output.raw 1
