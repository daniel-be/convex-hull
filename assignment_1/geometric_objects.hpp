#pragma once
#include <vector>

struct Point
{
	double x, y;

	Point(double x, double y);

	bool operator<(const Point& p) const;
};

struct Polygon
{
	std::vector<Point> points;

	Polygon();
	Polygon(std::vector<Point> points);
};