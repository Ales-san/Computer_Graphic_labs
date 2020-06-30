#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "PNM_ex.h"
#include "Functs.h"
#include "geom.h"
#include <stdlib.h>

using namespace std;


int main(int argc, char *argv[]) {

	if (argc != 10 && argc != 9) {
		cout << "Wrong number of arguments!";
		return 1;
	}

	FILE *fin = fopen(argv[1], "rb");

	if (!fin) {
		cout << "Input file error!\n";
		cout << argv[1];
		return 1;
	}

	PNM_File file;
	try {
		file = PNM_File(fin);
	}
	catch (exception err) {
		fclose(fin);
		cout << err.what();
		return 1;
	}
	fclose(fin);

	draw_line(file, atoi(argv[3]), atof(argv[4]), Point(atof(argv[6]), atof(argv[5])), Point(atof(argv[8]), atof(argv[7])), (argc == 10 ? atof(argv[9]) : 2.2));

	FILE *fout = fopen(argv[2], "wb");

	if (!fout) {
		cout << "Output file error!";
		return 1;
	}
	
	fprintf(fout, "P%c\n%d %d\n%d\n", file.header.format, file.header.width, file.header.height, file.header.depth);
	fwrite(&file.data[0], sizeof(unsigned char), file.size, fout);
	fclose(fout);
	return 0;
}