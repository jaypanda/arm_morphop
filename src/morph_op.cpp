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
#include <cstring>
#include <fstream>
#include <iostream>
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
using namespace std;

#ifdef _OPENMP
#include <omp.h>
#endif

#include "morph_op.h"

/* If SSE4.2 is available, use the specific processor instructions */
#ifdef __SSE4_2__ 
#include <nmmintrin.h> 

void erode( uchar *input, uchar *output, int width, int height, int pad){
  int step = width+2*pad;
	const int offs[] = {-1, +1, 0, -step, +step};

	int i, j, ind_o, ind_i;
  __m128i m;
#ifdef _OPENMP                                                                             
#pragma omp parallel shared (input, output, width, height, pad, step, struct_el_size, offs ) private (i,j,m,ind_o,ind_i)
	{
#pragma omp for		
#endif
  for( i = pad ; i < height+pad ; ++i ){
		for( j = pad ; j < width+pad-16 ; j+=16 ){
			ind_o = ((i-pad)*width)+j-pad;
			ind_i = (i*step) + j;
			// unaligned loads
			m = _mm_loadu_si128((const __m128i*)(input + ind_i + offs[0]));
			m = _mm_min_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[1])));
      //aligned loads
			m = _mm_min_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[2])));
			m = _mm_min_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[3])));
			m = _mm_min_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[4])));
			// saving result in output image
	    uchar *temp_out = new uchar[16];
			_mm_store_si128((__m128i*)temp_out, m);
			memcpy(output + ind_o, temp_out, min(16, width-j));
		}
	}
#ifdef _OPENMP
  }
#endif
}

void dilate( uchar *input, uchar *output, int width, int height, int pad){
  int step = width+2*pad;
	const int offs[] = {-1, +1, 0, -step, +step};

	int i, j, ind_o, ind_i;
  __m128i m;
#ifdef _OPENMP                                                                             
#pragma omp parallel shared (input, output, width, height, pad, step, struct_el_size, offs ) private (i,j,m,ind_o,ind_i)
	{
#pragma omp for		
#endif
  for( i = pad ; i < height+pad ; ++i ){
		for( j = pad ; j < width+2*pad-16 ; j+=16 ){
			ind_o = ((i-pad)*width)+j-pad;
			ind_i = (i*step) + j;
			// unaligned loads
			m = _mm_loadu_si128((const __m128i*)(input + ind_i + offs[0]));
			m = _mm_max_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[1])));
      // aligned loads
			m = _mm_max_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[2])));
			m = _mm_max_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[3])));
			m = _mm_max_epu8(m, _mm_loadu_si128((const __m128i*)(input + ind_i + offs[4])));
			// saving result in output image
	    uchar *temp_out = new uchar[16];
			_mm_store_si128((__m128i*)temp_out, m);
			memcpy(output + ind_o, temp_out, min(16, width-j));
		}
	}
#ifdef _OPENMP
  }
#endif

}
#endif

#ifndef __SSE4_2__
#warning "SSE4.2 NOT USED FOR COMPUTING MORPHOLOGICAL OPEN. Consider adding -msse4!" 

void erode( uchar *input, uchar *output, int width, int height, int pad){
  int step = width+2*pad;
	int struct_el_size = 5;
	const int offs[] = {-1, +1, 0, -step, +step};

	int i, j, k, ind_o, ind_i;
	uchar minVal;
#ifdef _OPENMP                                                                             
#pragma omp parallel shared (input, output, width, height, pad, step, struct_el_size, offs ) private (i,j,k,minVal,ind_o,ind_i)
	{
#pragma omp for		
#endif
  for( i = pad ; i < height+pad ; ++i ){
		for( j = pad ; j < width+pad ; ++j ){
			minVal = 255;
			ind_o = ((i-pad)*width)+j-pad;
			ind_i = (i*step) + j;
			for( k = 0 ; k < struct_el_size ; ++k ){
				minVal = min(minVal, input[ind_i + offs[k]]);
			}
			output[ ind_o ] = minVal;
		}
	}
#ifdef _OPENMP
	}
#endif
}
 
void dilate( uchar *input, uchar *output, int width, int height, int pad){
  int step = width+2*pad;
	int struct_el_size = 5;
	const int offs[] = {-1, +1, 0, -step, +step};

	int i, j, k, ind_o, ind_i;
	uchar maxVal;
#ifdef _OPENMP                                                                             
#pragma omp parallel shared (input, output, width, height, pad, step, struct_el_size, offs ) private (i,j,k,maxVal,ind_o,ind_i)
	{
#pragma omp for		
#endif
  for( i = pad ; i < height+pad ; ++i ){
		for( j = pad ; j < width+pad ; ++j ){
			maxVal = 0;
			ind_o = ((i-pad)*width)+j-pad;
			ind_i = (i*step) + j;
			for( k = 0 ; k < struct_el_size ; ++k ){
				maxVal = max(maxVal, input[ind_i + offs[k]]);
			}
			output[ ind_o ] = maxVal;
		}
	}
#ifdef _OPENMP
	}
#endif
}

#endif

uchar* pad_image ( uchar *img, int width, int height, int pad ){
	int padw = width+2*pad;
	int padh = height+2*pad;
	int total = padw*padh;
	uchar* img_padded = new uchar[total];
	for( int i = 0 ; i < height ; ++i ){
		memcpy( img_padded + (padw*(i+pad))+pad, img + width*i, width );
		memset(img_padded+(i+pad)*padw, img[width*i], pad);
		memset(img_padded+(i+pad)*padw+padw-pad-1, img[width*i+width-1], pad);
	}
	for( int i = 0 ; i < pad ; ++i ){
		memcpy( img_padded + i , img_padded+(pad*padw), padw );
		memcpy( img_padded + ((padh-i-2)*padw), img_padded+ ((padh-pad)*padw) , padw );
	}
	return img_padded;
}

void open_op( uchar *input, uchar *output, int width, int height, int levels){
	int pad = 4;
	uchar* input_padded = pad_image( input, width, height, pad );
//	ofstream tempf( "temp.raw", ios::out | ios::binary );
//	tempf.write((char*)input_padded, (width+2*pad)*(height+2*pad));
//	tempf.close();
//	cout << "image padded by " << pad << endl;

	while (levels--){
		erode( input_padded, output, width, height, pad );
	}
	dilate( input_padded, output, width, height, pad );
}

void close_op( uchar *input, uchar *output, int width, int height, int levels){
	int pad = 4;
	uchar* input_padded = pad_image( input, width, height, pad );

	while (levels--){
		dilate( input_padded, output, width, height, pad );
	}
	erode( input_padded, output, width, height, pad );
}



/*-------------------------------------------*/                                            
/* Threaded versions, if OpenMP is available */                                            
#ifdef _OPENMP                                                                             

#endif
