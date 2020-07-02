#include "PNM_ex.h"

PNM_Header::PNM_Header() {};

PNM_Header::PNM_Header(FILE *fin) {
	char checker;
	int check = fscanf(fin, "%c%c\n%d %d\n%d\n", &checker, &format, &width, &height, &depth);
	if (check != 5 || checker != 'P' || format != '5' && format != '6'
		|| width <= 0 || height <= 0 || depth != 255) {
		throw std::exception("Invalid data format!\n");
	}
}

PNM_File::PNM_File() {};

PNM_File::PNM_File(FILE *fin) {
	header = PNM_Header(fin);

	if (header.format == '5') {
		size = header.width * header.height;
	}
	else {
		size = header.width * header.height * 3;
	}
	data.resize(size);
	int check = fread(&data[0], sizeof(unsigned char), size, fin);
	if (check != size) {
		throw std::exception("Invalid data format: width or\and height are wrong!\n");
	}
}

void PNM_File::write(FILE *fout) {
	fprintf(fout, "P%c\n%d %d\n%d\n", header.format, header.width, header.height, header.depth);
	fwrite(&data[0], sizeof(unsigned char), size, fout);
	fclose(fout);
}