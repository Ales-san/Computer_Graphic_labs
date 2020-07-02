#pragma once

#include <algorithm>

#include "PNM_ex.h"

const int INF = 1e9;
const double EPS = 1e-3;

class component {
public:
	double a;
	double b;
	double c;
	component();
	component(double, double, double);
};

component get_component(PNM_File &file, int i, int j);

void write_component(PNM_File &file, int i, int j, component &new_color);

void cas(PNM_File &file, double sharpen);