#include "geometric_objects.hpp"

Point::Point(double x, double y) : x(x), y(y) { }

bool Point::operator<(const Point& p) const
{
	return x < p.x || (x == p.x && y < p.y);
}

Polygon::Polygon() : points(std::vector<Point>()) { }

Polygon::Polygon(std::vector<Point> points) : points(points) { }