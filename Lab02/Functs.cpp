#include <algorithm>
#include <vector>

#include "PNM_ex.h"
#include "geom.h"

double get_vect_mult(Vect a, Vect b) {
	return a.x * b.y - a.y * b.x;
}

Vect get_norm(Point a, Point b) {
	return Vect(a, b);
}


Rect get_line(Point a, Point b, double thick) {
	Vect s = get_norm(a, b);
	double l = s.len();
	s.x = s.x * thick / (2. * l);
	s.y = s.y * thick / (2. * l);
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

void centring(Point &a) {
	if (a.x - int(a.x) <= EPS) {
		a.x += 0.5;
	}
	if (a.y - int(a.y) < EPS) {
		a.y += 0.5;
	}
}

void draw_line(PNM_File &file, int bright, double thick, Point s, Point f, double gamma) {
	int w = file.header.width;
	int h = file.header.height;

	centring(s);
	centring(f);

	Rect rec = get_line(s, f, thick);

	for (int x = 0; x < h; x++) {
		for (int y = 0; y < w; y++) {
			double conj = get_conj(x, y, rec);
			double tmp = do_gamma(file.data[x * w + y] / 255., gamma) * (1.0 - conj) + conj * do_gamma(double(bright) / 255., gamma);
			tmp = do_gamma(tmp, 1. / gamma);
			if (tmp > 0.9999) {
				tmp = 1.0;
			}
			file.data[x * w + y] = tmp * 255;
		}
	}

}