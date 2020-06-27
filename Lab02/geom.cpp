#include "geom.h"
#include <cmath>
Point::Point() : x(0), y(0) {}

Point::Point(double x, double y) : x(x), y(y) {}

double Vect::len() {
	return std::sqrt(x * x + y * y);
}

Vect::Vect(double x, double y) {
	x = x;
	y = y;
}

Vect::Vect(Point a, Point b) {
	x = b.x - a.x;
	y = b.y - a.y;
}

Rect::Rect(Point a, Point b, Point c, Point d) : A(a), B(b), C(c), D(d) {};