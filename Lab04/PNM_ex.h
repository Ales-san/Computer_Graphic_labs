#pragma once
#include <vector>
#include <string>


class PNM_Header {
public:
	char format;
	int width;
	int height;
	int depth;

	PNM_Header();

	PNM_Header(FILE *fin);
};

class PNM_File {
public:
	PNM_Header header;
	std::vector <unsigned char> data;

	int size = 0;
	PNM_File();
	PNM_File(FILE *fin);
	PNM_File(std::vector<PNM_File>&);
	PNM_File(PNM_File&, int);
	void write(FILE *fout);
};
