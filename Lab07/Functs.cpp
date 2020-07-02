#include <algorithm>
#include <vector>
#include <exception>

#include "PNM_ex.h"
#include "Functs.h"

component::component(): a(0), b(0), c(0) {}

component::component(double a, double b, double c): a(a), b(b), c(c) {}

component get_component(PNM_File &file, int i, int j) {
	int w = file.header.width;

	component color;
	if (file.header.format == '6') {
		color.a = double(file.data[i * w * 3 + j * 3 + 0]) / 255;
		color.b = double(file.data[i * w * 3 + j * 3 + 1]) / 255;
		color.c = double(file.data[i * w * 3 + j * 3 + 2]) / 255;
	}
	else {
		color.a = double(file.data[i * w + j]) / 255;
		color.b = -INF;
		color.c = -INF;
	}

	return color;
}

void write_component(PNM_File &file, int i, int j, component &new_color) {
	int w = file.header.width;

	component color;
	if (file.header.format == '6') {
		file.data[i * w * 3 + j * 3 + 0] = new_color.a;
		file.data[i * w * 3 + j * 3 + 1] = new_color.b;
		file.data[i * w * 3 + j * 3 + 2] = new_color.c;
	}
	else {
		file.data[i * w + j] = new_color.a;
	}

}

double get_ker(double bright, std::vector<double> ring, std::vector<double> circle, double sharpen) {
	if (bright == -INF) {
		return -INF;
	}
	double ring_min = bright;
	double ring_max = bright;
	for (auto p : ring) {
		ring_min = std::min(p, ring_min);
		ring_max = std::max(p, ring_max);
	}
	double area_min = ring_min;
	double area_max = ring_max;
	for (auto p : circle) {
		area_min = std::min(p, area_min);
		area_max = std::max(p, area_max);
	}
	area_min += ring_min;
	area_max += ring_max;

	double base = sqrt(std::min(area_min, 2 - area_max) / area_max);
	double weight = base * (-1.) / (8 * (1 - sharpen) + 5 * sharpen);
	double res = bright;
	for (auto p : ring) {
		res += p * weight;
	}
	return res / (1 + 4 * weight);
}


void cas(PNM_File &file, double sharpen) {
	int h = file.header.height;
	int w = file.header.width;
	std::pair<int, int> shift_circle[] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
	std::pair<int, int> shift_ring[] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			component color = get_component(file, i, j);
			std::vector<double> ring[3];
			std::vector<double> circle[3];
			for (auto shift : shift_ring) {
				if (i + shift.first >= 0 && i + shift.first < h &&
					j + shift.second >= 0 && j + shift.second < w) {
					component shift_color = get_component(file, i + shift.first, j + shift.second);
					ring[0].push_back(shift_color.a);
					ring[1].push_back(shift_color.b);
					ring[2].push_back(shift_color.c);
				}
			}
			for (auto shift : shift_circle) {
				if (i + shift.first >= 0 && i + shift.first < h &&
					j + shift.second >= 0 && j + shift.second < w) {
					component shift_color = get_component(file, i + shift.first, j + shift.second);
					circle[0].push_back(shift_color.a);
					circle[1].push_back(shift_color.b);
					circle[2].push_back(shift_color.c);
				}
			}
			component new_color;
			new_color.a = get_ker(color.a, ring[0], circle[0], sharpen);
			new_color.b = get_ker(color.b, ring[1], circle[1], sharpen);
			new_color.c = get_ker(color.c, ring[2], circle[2], sharpen);

			if (new_color.a < 0) {
				new_color.a = 0;
			}

			if (new_color.b < 0 && new_color.b != -INF) {
				new_color.b = 0;
			}

			if (new_color.c < 0 && new_color.c != -INF) {
				new_color.c = 0;
			}

			if (new_color.a > 1) {
				new_color.a = 1;
			}
			if (new_color.b > 1) {
				new_color.b = 1;
			}
			if (new_color.c > 1) {
				new_color.c = 1;
			}

			new_color.a = round(new_color.a * 255);
			new_color.b = round(new_color.b * 255);
			new_color.c = round(new_color.c * 255);

			write_component(file, i, j, new_color);
		}
	}
}