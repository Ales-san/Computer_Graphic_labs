#pragma once

#include <algorithm>


#include "PNM_ex.h"
#include "geom.h"

void inverse(PNM_File &file);

void vertical_reflection(PNM_File &file);

void horisontal_reflection(PNM_File &file);

void rotate_counter_clockwise(PNM_File &file);

void rotate_clockwise(PNM_File &file);



double get_vect_mult(Point a, Point b);

Vect get_norm(Point a, Point b);


Rect get_line(Point a, Point b, double thick);


bool get_pos(Point a, Rect rec);

double get_conj(int x, int y, Rect rec);

double do_gamma(double color, double gamma);

void draw_line(PNM_File &file, int bright, double thick, Point s, Point f, double gamma);
