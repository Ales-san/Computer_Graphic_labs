#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#include "PNM_ex.h"
#include "Functs.h"
#include <stdlib.h>

using namespace std;


int main(int argc, char *argv[]) {

	if (argc != 4) {
		cerr << "Wrong number of arguments!";
		return 1;
	}

	FILE *fin = fopen(argv[1], "rb");

	if (!fin) {
		cerr << "Input file error!\n";
		cerr << argv[1];
		return 1;
	}

	PNM_File file;
	try {
		file = PNM_File(fin);
	}
	catch (exception err) {
		fclose(fin);
		cerr << err.what();
		return 1;
	}
	fclose(fin);

	cas(file, atof(argv[3]));

	FILE *fout = fopen(argv[2], "wb");

	if (!fout) {
		cerr << "Output file error!";
		return 1;
	}

	file.write(fout);
	return 0;
}