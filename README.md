# arm_morphop
morphological operations - open/close
implemented sse and openmp implementations
toggle flags in makefile.inc


INSTALL: make

test: ./run.sh <image raw file path> <width> <height> <output raw file path> <num levels>

example 
	./run.sh data/test_image1.raw 2550 3300 data/output.raw 1
