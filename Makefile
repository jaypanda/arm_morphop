.SUFFIXES: .cpp .o

include makefile.inc

CC=g++

morphopen: test/main.cpp 
	$(CC) -std=c++11 -o morphopen test/main.cpp src/morph_op.cpp -Iexternal/cxxopts/src/ -Isrc/ $(CFLAGS) $(LDFLAGS)

clean:
	rm morphopen
