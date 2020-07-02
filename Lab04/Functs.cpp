#include <algorithm>
#include <vector>
#include <exception>

#include "PNM_ex.h"
#include "Functs.h"

component::component(): a(0), b(0), c(0) {}

component::component(double a, double b, double c): a(a), b(b), c(c) {}

void check(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	if (files.size() == 3 &&
		(files[1].header.height != h || files[2].header.height != h ||
			files[1].header.width != w || files[2].header.width != w)) {
		throw std::exception("Wrong parametres of width or\and height!\n");
	}
}

component get_component(std::vector<PNM_File> &files, int i, int j) {
	int w = files[0].header.width;

	component color;
	if (files.size() == 1) {
		if (files[0].data[i * w * 3 + j * 3 + 1] == 46) {
			int t = 5;
		}
		color.a = double(files[0].data[i * w * 3 + j * 3 + 0]) / 255;
		color.b = double(files[0].data[i * w * 3 + j * 3 + 1]) / 255;
		color.c = double(files[0].data[i * w * 3 + j * 3 + 2]) / 255;
	}
	else {
		color.a = double(files[0].data[i * w + j]) / 255;
		color.b = double(files[1].data[i * w + j]) / 255;
		color.c = double(files[2].data[i * w + j]) / 255;
	}

	return color;
}

void write_component(std::vector<PNM_File> &files, int i, int j, component &new_color) {
	int w = files[0].header.width;

	component color;
	if (files.size() == 1) {
		files[0].data[i * w * 3 + j * 3 + 0] = new_color.a;
		files[0].data[i * w * 3 + j * 3 + 1] = new_color.b;
		files[0].data[i * w * 3 + j * 3 + 2] = new_color.c;
	}
	else {
		files[0].data[i * w + j] = new_color.a;
		files[1].data[i * w + j] = new_color.b;
		files[2].data[i * w + j] = new_color.c;
	}

}

void to_HSV(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color;
			new_color.c = std::max(color.a, std::max(color.b, color.c));
			double sub = new_color.c - std::min(color.a, std::min(color.b, color.c));
			if (new_color.c == 0) {
				new_color.b = 0;
			} else {
				new_color.b = sub / new_color.c;
			}
			if (new_color.b == 0) {
				new_color.a = 0;
			} else {
				double Cr = (new_color.c - color.a) / sub;
				double Cg = (new_color.c - color.b) / sub;
				double Cb = (new_color.c - color.c) / sub;
				if (abs(color.a - new_color.c) < EPS) {
					new_color.a = Cb - Cg;
				}
				if (abs(color.b - new_color.c) < EPS) {
					new_color.a = 2 + Cr - Cb;
				}
				if (abs(color.c - new_color.c) < EPS) {
					new_color.a = 4 + Cg - Cr;
				}
				new_color.a *= 60;
				if (new_color.a < 0 && new_color.a != -INF) {
					new_color.a += 360;
				}
			}
			new_color.a = round(new_color.a / 360 * 255);
			new_color.b = round(new_color.b * 100);
			new_color.c = round(new_color.c * 100);
			write_component(files, j, k, new_color);
		}
	}
}

void from_HSV(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			color.a *= 360;
			color.b *= 2.55;
			color.c *= 2.55;

			component new_color;
			double mul = color.b * color.c;
			double f = color.a / 60;
			double x = (1 - abs((f - int(f)) + int(f) % 2 - 1)) * mul;

			if (color.a < 60) {
				new_color.a = mul;
				new_color.b = x;
			} else if(color.a < 120) {
				new_color.a = x;
				new_color.b = mul;
			} else if(color.a < 180) {
				new_color.b = mul;
				new_color.c = x;
			} else if(color.a < 240) {
				new_color.b = x;
				new_color.c = mul;
			} else if(color.a < 300) {
				new_color.a = x;
				new_color.c = mul;
			}else if(color.a < 360) {
				new_color.a = mul;
				new_color.c = x;
			}

			new_color.a += color.c - mul;
			new_color.b += color.c - mul;
			new_color.c += color.c - mul;

			new_color.a = round(new_color.a * 255.);
			new_color.b = round(new_color.b * 255.);
			new_color.c = round(new_color.c * 255.);

			write_component(files, j, k, new_color);
		}
	}
}

void to_HSL(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color;
			double max_c = std::max(color.a, std::max(color.b, color.c));
			double min_c = std::min(color.a, std::min(color.b, color.c));
			new_color.c = (max_c + min_c) / 2;
			
			if (new_color.c == 0 || max_c == min_c) {
				new_color.b = 0;
			} else if(new_color.c <= 0.5) {
				new_color.b = (max_c - min_c) / (2 * new_color.c);
			} else {
				new_color.b = (max_c - min_c) / (2 - 2 * new_color.c);
			}

			if (max_c == min_c) {
				new_color.a = 0;
			} else if(abs(max_c - color.a) < EPS && color.b >= color.c) {
				new_color.a = (color.b - color.c) / (max_c - min_c) * 60;
			} else if (abs(max_c - color.a) < EPS && color.b < color.c) {
				new_color.a = (color.b - color.c) / (max_c - min_c) * 60 + 360;
			} else if (abs(max_c - color.b) < EPS) {
				new_color.a = (color.c - color.a) / (max_c - min_c) * 60 + 120;
			} else if (abs(max_c - color.c) < EPS) {
				new_color.a = (color.a - color.b) / (max_c - min_c) * 60 + 240;
			}
				
			new_color.a = round(new_color.a / 360 * 255);
			new_color.b = round(new_color.b * 255);
			new_color.c = round(new_color.c * 255);
			write_component(files, j, k, new_color);
		}
	}
}

void from_HSL(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color;

			double q;
			if (color.c < 0.5) {
				q = color.c * (color.b + 1.);
			} else {
				q = color.c + color.b - (color.c * color.b);
			}
			double p = 2. * color.c - q;
			double t[3], col[3];
			t[0] = color.a + 1. / 3.;
			t[1] = color.a;
			t[2] = color.a - 1. / 3.;
			for (int f = 0; f < 3; f++) {
				if (t[f] < 0) {
					t[f] = t[f] + 1;
				} else if (t[f] > 1) {
					t[f] = t[f] - 1;
				}
				double c;
				if (t[f] < 1. / 6.) {
					col[f] = p + ((q - p) * 6 * t[f]);
				} else if(t[f] < 0.5) {
					col[f] = q;
				} else if (t[f] < 2. / 3.) {
					col[f] = p + ((q - p) * (2. / 3. - t[f]) * 6);
				} else {
					col[f] = p;
				}
			}

			new_color.a = round(col[0] * 255);
			new_color.b = round(col[1] * 255);
			new_color.c = round(col[2] * 255);
			write_component(files, j, k, new_color);
		}
	}
}


component to_YCbCr(double Kb, double Kr, component color) {
	component new_color;
	new_color.a = Kr * color.a + (1 - Kr - Kb) * color.b + Kb * color.c;
	new_color.b = 0.5 * (color.c - new_color.a) / (1 - Kb);
	new_color.c = 0.5 * (color.a - new_color.a) / (1 - Kr);

	new_color.a = round(new_color.a * 255);
	new_color.b = round((new_color.b + 0.5) * 255);
	new_color.c = round((new_color.c + 0.5) * 255);

	return new_color;
}

void check_borders(double &x) {
	if (x > 1) {
		x = 1;
	}
	if (x < 0) {
		x = 0;
	}
}

component from_YCbCr(double Kb, double Kr, component color) {

	color.b -= 0.5;
	color.c -= 0.5;

	component new_color;

	new_color.a = color.a + (1 - Kr) * color.c / 0.5;
	new_color.b = color.a - 2 * Kb * (1 - Kb) * color.b / (1 - Kb - Kr) - 2 * Kr * (1 - Kr) * color.c / (1 - Kb - Kr);
	new_color.c = color.a + (1 - Kb) * color.b / 0.5;

	check_borders(new_color.a);
	check_borders(new_color.b);
	check_borders(new_color.c);

	new_color.a = round(new_color.a * 255);
	new_color.b = round(new_color.b * 255);
	new_color.c = round(new_color.c * 255);
	return new_color;
}

void from_YCbCr601(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color = from_YCbCr(0.114, 0.299, color);			
			write_component(files, j, k, new_color);
		}
	}
}

void to_YCbCr601(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color = to_YCbCr(0.114, 0.299, color);
			write_component(files, j, k, new_color);
		}
	}
}

void from_YCbCr709(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color = from_YCbCr(0.0722, 0.2126, color);
			write_component(files, j, k, new_color);
		}
	}
}

void to_YCbCr709(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color = to_YCbCr(0.0722, 0.2126, color);
			write_component(files, j, k, new_color);
		}
	}
}

void to_YCoCg(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);
			component new_color;
			
			new_color.a = 0.25 * color.a + 0.5 * color.b + 0.25 * color.c;
			new_color.b = 0.5 * color.a - 0.5 * color.c;
			new_color.a = -0.25 * color.a + 0.5 * color.b - 0.25 * color.c;

			if (new_color.a < 0) {
				new_color.a = 0;
			}

			if (new_color.a > 1) {
				new_color.a = 1;
			}

			new_color.a = round(new_color.a * 255);
			new_color.b = round((new_color.b + 0.5) * 255);
			new_color.c = round((new_color.c + 0.5) * 255);

			write_component(files, j, k, new_color);
		}
	}
}

void from_YCoCg(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			color = get_component(files, j, k);

			color.b -= 0.5;
			color.c -= 0.5;

			component new_color;

			new_color.a = color.a + color.b - color.c;
			new_color.b = color.a + color.c;
			new_color.a = color.a - color.b - color.c;

			new_color.a = round(new_color.a * 255);
			new_color.b = round(new_color.b * 255);
			new_color.c = round(new_color.c * 255);

			write_component(files, j, k, new_color);
		}
	}
}


void to_CMY(std::vector<PNM_File> &files) {
	int h = files[0].header.height;
	int w = files[0].header.width;
	check(files);
	for (int j = 0; j < h; j++) {
		for (int k = 0; k < w; k++) {
			component color;
			if (files.size() == 1) {
				if (files[0].data[j * w * 3 + k * 3 + 1] == 133) {
					int t = 5;
				}
				color.a = (files[0].data[j * w * 3 + k * 3 + 0]);
				color.b = (files[0].data[j * w * 3 + k * 3 + 1]);
				color.c = (files[0].data[j * w * 3 + k * 3 + 2]);
			}
			else {
				color.a = (files[0].data[j * w + k]);
				color.b = (files[1].data[j * w + k]);
				color.c = (files[2].data[j * w + k]);
			}
			component new_color;

			new_color.a = 255 - color.a;
			new_color.b = 255 - color.b;
			new_color.c = 255 - color.c;

			write_component(files, j, k, new_color);
		}
	}
}

void from_CMY(std::vector<PNM_File> &files) {
	to_CMY(files);
}

void foo(std::vector<PNM_File> &files) {
	//doing nothing
}

void convert(std::vector<PNM_File> &files, int from, int to) {
	void(*convfrom[7])(std::vector<PNM_File>&) = { foo, from_HSL, from_HSV, from_YCbCr601, from_YCbCr709, from_YCoCg, from_CMY };
	void(*convto[7])(std::vector<PNM_File>&) = { foo, to_HSL, to_HSV, to_YCbCr601, to_YCbCr709, to_YCoCg, to_CMY };

	convfrom[from](files);
	convto[to](files);
}