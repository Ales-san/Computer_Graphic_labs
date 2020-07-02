#pragma once

#include <algorithm>

#include "PNM_ex.h"

const int INF = 1e9;
const double EPS = 1e-9;

class component {
public:
	double a;
	double b;
	double c;
	component();
	component(double, double, double);
};

void check(std::vector<PNM_File> &files);

component get_component(std::vector<PNM_File> &files, int i, int j);

void write_component(std::vector<PNM_File> &files, int i, int j, component &new_color);

void to_HSV(std::vector<PNM_File> &files);

void from_HSV(std::vector<PNM_File> &files);

void to_HSL(std::vector<PNM_File> &files);

void from_HSL(std::vector<PNM_File> &files);

component to_YCbCr(double Kb, double Kr, component color);

component from_YCbCr(double Kb, double Kr, component color);

void from_YCbCr601(std::vector<PNM_File> &files);

void to_YCbCr601(std::vector<PNM_File> &files);

void from_YCbCr709(std::vector<PNM_File> &files);

void to_YCbCr709(std::vector<PNM_File> &files);

void to_YCoCg(std::vector<PNM_File> &files);

void from_YCoCg(std::vector<PNM_File> &files);


void to_CMY(std::vector<PNM_File> &files);

void from_CMY(std::vector<PNM_File> &files);

void foo(std::vector<PNM_File> &files);

void convert(std::vector<PNM_File> &files, int from, int to);