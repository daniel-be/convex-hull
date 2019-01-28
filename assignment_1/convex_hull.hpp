#pragma once
#include "geometric_objects.hpp"

class ConvexHull
{
public:
	bool valid;

	ConvexHull(std::vector<Point>& points);
	Polygon monotone_chain();
	Polygon jarvis_march();
	~ConvexHull();

private:
	std::vector<Point>& points;

	double calc_orientation(Point& a, Point& b, Point& c);
};