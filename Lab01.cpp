#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class PNM_Header {
public:
	char format;
	int width;
	int height;
	int depth;

	PNM_Header() {};

	PNM_Header(FILE *fin) {
		char checker;
		int check = fscanf(fin, "%c%c\n%d %d\n%d\n", &checker, &format, &width, &height, &depth);
		if (check != 5 || checker != 'P' || format != '5' && format != '6' 
				|| width <= 0 || height <= 0 || depth != 255) {
			throw exception("Invalid data format!\n");
		}
	}
};

class PNM_File {
public:
	PNM_Header header;
	vector <unsigned char> data;
	
	int size = 0;
	PNM_File() {}
	PNM_File(FILE *fin) {
		header = PNM_Header(fin);
		
		if (header.format == '5') {
			size = header.width * header.height;
		} else {
			size = header.width * header.height * 3;
		}
		data.resize(size);
		int check = fread(&data[0], sizeof(unsigned char), size, fin);
		if (check != size) {
			throw exception("Invalid data format: width or\and height are wrong!\n");
		}
	}
};

void inverse(PNM_File &file) {
	for (int i = 0; i < file.size; i++) {
		file.data[i] = char(file.header.depth - file.data[i]);
	}
}

void vertical_reflection(PNM_File &file) {
	int h = file.header.height, w = file.header.width;
	if (file.header.format == '6') {
		w *= 3;
	}
	for (int i = 0; i < h / 2; i++) {
		for (int j = 0; j < w; j++) {
			std::swap(file.data[w * i + j],
				file.data[w * (h - i - 1) + j]);
		}
	}
}

void horisontal_reflection(PNM_File &file) {
	int h = file.header.height, w = file.header.width, factor = 1;
	if (file.header.format == '6') {
		factor = 3;
	}
	for (int k = 0; k < factor; k++) {
		for (int i = 0; i < w / 2; i++) {
			for (int j = 0; j < h; j++) {
				std::swap(file.data[(w * j + i) * factor + k],
					file.data[(w * (j + 1) - i - 1) * factor + k]);
			}
		}
	}

}

void rotate_counter_clockwise(PNM_File &file) {
	vector <unsigned char> ndata;
	ndata.resize(file.size);
	int h = file.header.height, w = file.header.width, factor = 1;
	if (file.header.format == '6') {
		factor = 3;
	}
	for (int k = 0; k < factor; k++) {
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				ndata[(h * i + j) * factor + k] = file.data[(w * (j + 1) - i - 1) * factor + k];
			}
		}
	}
	file.data = ndata;
	std::swap(file.header.height, file.header.width);
}

void rotate_clockwise(PNM_File &file) {
	rotate_counter_clockwise(file);
	horisontal_reflection(file);
	vertical_reflection(file);
}

int main(int argc, char *argv[]) {

	if (argc != 4) {
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
	} catch (exception err) {
		cout << err.what();
		return 1;
	}
	std::fclose(fin);
	switch (argv[3][0] - '0') {
	case 0:
		inverse(file);
		break;
	case 1:
		horisontal_reflection(file);
		break;
	case 2:
		vertical_reflection(file);
		break;
	case 3:
		rotate_clockwise(file);
		break;
	case 4:
		rotate_counter_clockwise(file);
		break;
	default:
		cout << "Wrong operation!";
		return 1;
	}

	FILE *fout = fopen(argv[2], "wb");

	if (!fout) {
		cout << "Output file error!";
		return 1;
	}
	fprintf(fout, "P%c\n%d %d\n%d\n", file.header.format, file.header.width, file.header.height, file.header.depth);
	std::fwrite(&file.data[0], sizeof(unsigned char), file.size, fout);
	std::fclose(fout);
	return 0;
}
