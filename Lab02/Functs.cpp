#include <algorithm>
#include <vector>

#include "PNM_ex.h"
#include "geom.h"


//void inverse(PNM_File &file) {
//	for (int i = 0; i < file.size; i++) {
//		file.data[i] = unsigned char(file.header.depth - file.data[i]);
//	}
//}
//
//void vertical_reflection(PNM_File &file) {
//	int h = file.header.height, w = file.header.width;
//	if (file.header.format == '6') {
//		w *= 3;
//	}
//	for (int i = 0; i < h / 2; i++) {
//		for (int j = 0; j < w; j++) {
//			std::swap(file.data[w * i + j],
//				file.data[w * (h - i - 1) + j]);
//		}
//	}
//}
//
//void horisontal_reflection(PNM_File &file) {
//	int h = file.header.height, w = file.header.width, factor = 1;
//	if (file.header.format == '6') {
//		factor = 3;
//	}
//	for (int k = 0; k < factor; k++) {
//		for (int i = 0; i < w / 2; i++) {
//			for (int j = 0; j < h; j++) {
//				std::swap(file.data[(w * j + i) * factor + k],
//					file.data[(w * (j + 1) - i - 1) * factor + k]);
//			}
//		}
//	}
//
//}
//
//void rotate_counter_clockwise(PNM_File &file) {
//	std::vector <unsigned char> ndata;
//	ndata.resize(file.size);
//	int h = file.header.height, w = file.header.width, factor = 1;
//	if (file.header.format == '6') {
//		factor = 3;
//	}
//	for (int k = 0; k < factor; k++) {
//		for (int i = 0; i < w; i++) {
//			for (int j = 0; j < h; j++) {
//				ndata[(h * i + j) * factor + k] = file.data[(w * (j + 1) - i - 1) * factor + k];
//			}
//		}
//	}
//	file.data = ndata;
//	std::swap(file.header.height, file.header.width);
//}
//
//void rotate_clockwise(PNM_File &file) {
//	rotate_counter_clockwise(file);
//	horisontal_reflection(file);
//	vertical_reflection(file);
//}

double get_vect_mult(Vect a, Vect b) {
	return a.x * b.y - a.y * b.x;
}

Vect get_norm(Point a, Point b) {
	return Vect(a, b);
}


Rect get_line(Point a, Point b, double thick) {
	Vect s = get_norm(a, b);
	double l = s.len();
	s.x = s.x * thick / (2 * l);
	s.y = s.y * thick / (2 * l);
	return Rect(Point(-s.y + a.x, s.x + a.y), Point(s.y + a.x, -s.x + a.y),
		Point(s.y + b.x, -s.x + b.y), Point(-s.y + b.x, s.x + b.y));
}

bool get_pos(Point a, Rect rec) {
	double mult = get_vect_mult(Vect(rec.A, rec.B), Vect(a, rec.A));
	double sign = mult / (mult == 0 ? 1 : abs(mult));
	mult = get_vect_mult(Vect(rec.B, rec.C), Vect(a, rec.B));
	if (sign == 0 && mult != 0) {
		sign = mult / abs(mult);
	}
	if (sign != mult / (mult == 0 ? 1 : abs(mult))) {
		return false;
	}

	mult = get_vect_mult(Vect(rec.C, rec.D), Vect(a, rec.C));
	if (sign == 0 && mult != 0) {
		sign = mult / abs(mult);
	} 
	if (sign != mult / (mult == 0 ? 1 : abs(mult))) {
		return false;
	}

	mult = get_vect_mult(Vect(rec.D, rec.A), Vect(a, rec.D));
	if (sign != mult / (mult == 0 ? 1 : abs(mult))) {
		return false;
	}
	return true;
}

double get_conj(int x, int y, Rect rec) {
	int cnt = 0;
	int size = 0;
	for (double i = x; i < x + 1; i += EPS) {
		for (double j = y; j < y + 1; j += EPS, size++) {
			if (get_pos(Point(i, j), rec)) {
				cnt++;
			}
		}
	}
	return double(cnt) / size;
}

double do_gamma(double color, double gamma) {
	return pow(color, gamma);
}

void draw_line(PNM_File &file, int bright, double thick, Point s, Point f, double gamma) {
	int w = file.header.width;
	int h = file.header.height;

	Rect rec = get_line(s, f, thick);

	for (int x = 0; x < h; x++) {
		if (x == 25) {
			int t = 8;
		}
		for (int y = 0; y < w; y++) {
			if (x == 8 && y == 28) {
				int t = 8;
			}
			if (file.data[x * w + y] != 255) {
				int t = 8;
			}
			double conj = get_conj(x, y, rec);
			if (conj != 0.0 && conj != 1.0) {
				int t = 8;
			}
			double tmp = do_gamma(file.data[x * w + y] / 255., gamma) * (1.0 - conj) + conj * do_gamma(double(bright) / 255., gamma);
			tmp = do_gamma(tmp, 1. / gamma);
			if (tmp > 0.9999) {
				tmp = 1.0;
			}
			file.data[x * w + y] = tmp * 255;
		}
	}

}