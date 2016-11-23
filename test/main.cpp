#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "cxxopts.hpp"

#include "morph_op.h"

using namespace std;

int main(int argc, char *argv[]){
	if (argc < 7) {
        std::cerr << "Usage: " << argv[0] << " --input <img file path> --width <img width> --height <img height> [--levels <n levels> --output <img file path>]" << std::endl;
        return 1;
  }
	uchar *input;
	uchar *output;
	
	int width=0, height=0, levels=0;
	string input_file_path="";
	string output_file_path="";
	
	cxxopts::Options options("open_op test", "input image raw file and compute output image with morphological open operation");
	options.add_options()
		("input", "input raw image file path", cxxopts::value<string>())
		("w,width", "width of raw image", cxxopts::value<int>())
		("h,height", "height of raw image", cxxopts::value<int>())
		("l,levels", "num levels for morphological open operation", cxxopts::value<int>())
		("output", "output raw image file path", cxxopts::value<string>());
	options.parse(argc, argv);

	if( options.count("input") > 0 ){
		input_file_path = options["input"].as<string>();
	}else{
		cerr << "input image file path parameter missing" << endl;
	}
	if( options.count("output") > 0 ){
		output_file_path = options["output"].as<string>();
	}else{
		cout << "overwriting input image file..." << endl;
		output_file_path = input_file_path;
	}
	if( options.count("width") > 0 ){
		width = options["width"].as<int>();
	}else{
		cerr << "width parameter missing" << endl;
	}
	if( options.count("height") > 0 ){
		height = options["height"].as<int>();
	}else{
		cerr << "height parameter missing" << endl;
	}
	if( options.count("levels") > 0 ){
		levels = options["levels"].as<int>();
	}else{
		levels = 1;
	}
 
	input = new uchar[width*height];
	output = new uchar[width*height];
	ifstream inpFile(input_file_path, ios::in | ios::binary);
	ofstream outFile(output_file_path, ios::out | ios::binary);
	
	size_t count = width*height;
  inpFile.read((char*)input, count);
	open_op(input, output, width, height, levels);
	outFile.write((char*)output, count);
}
	
	

