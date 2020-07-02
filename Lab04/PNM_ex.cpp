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

PNM_File::PNM_File(std::vector<PNM_File> &files) {
	header = files[0].header;
	header.format = '6';
	size = files[0].size * 3;
	data.resize(size);
	int w = header.width;
	int h = header.height;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			for (int k = 0; k < 3; k++) {
				data[i * w * 3 + j * 3 + k] = files[k].data[i * w + j];
			}
		}
	}
}

PNM_File::PNM_File(PNM_File &file, int ch) {
	header = file.header;
	header.format = '5';
	size = file.size / 3;
	data.resize(size);
	int w = header.width;
	int h = header.height;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			data[i * w + j] = file.data[i * w * 3 + j * 3 + ch];
		}
	}
}

void PNM_File::write(FILE *fout) {
	fprintf(fout, "P%c\n%d %d\n%d\n", header.format, header.width, header.height, header.depth);
	fwrite(&data[0], sizeof(unsigned char), size, fout);
	fclose(fout);
}