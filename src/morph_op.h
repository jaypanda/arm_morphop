/*
Author: Jay Guru Panda

This software intends to implement a program performing the morphological ‘open’ operation on an input image,
where an open is an erosion, followed by a dilation.

An erode operation replaces each pixel with the minimum of its Von Neumann neighbourhood, the dilate
operation does the same, but with the maximum. At each boundary, the neigbourhood is truncated to
exclude parts extending beyond the boundaries of the image.

Images are to be input and output as raw, headerless binary gray-scale 8bpp images, in little-endian
scanline order.

Processing parameters will be set on the command line:
width height [levels]
- width: positive integer, up to and including 2 24
- height: positive integer, up to and including 2 24
- levels: number of times to erode before dilating, default=1
width*height must be a multiple of 64.

*/

#ifndef __open_op_h
#define __open_op_h

typedef unsigned int uint32;
typedef unsigned char uchar;

void open_op(uchar *input, uchar* output, int width, int height, int levels); 
void close_op( uchar *input, uchar *output, int width, int height, int levels);
void erode( uchar *input, uchar *output, int width, int height, int step);
void dilate( uchar *input, uchar *output, int width, int height, int step);
uchar* pad_image ( uchar *img, int width, int height, int pad );

//void open_op_sse(uchar *input, uchar* output, int width, int height, int levels); 
//void open_op_avx(uchar *input, uchar* output, int width, int height, int levels); 

#endif
