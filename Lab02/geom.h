#pragma once

const double EPS = 0.1;

class Point {
public:
	double x, y;
	Point();
	Point(double, double);
};

class Vect {
public:
	double x, y;
	Vect(double, double);
	Vect(Point, Point);
	double len();
};

class Rect {
public:
	Point A, B, C, D;
	Rect(Point, Point, Point, Point);
};