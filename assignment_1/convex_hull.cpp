#include <vector>
#include <iostream>

#include "convex_hull.hpp"
#include "geometric_objects.hpp"

ConvexHull::ConvexHull(std::vector<Point>& points) : points(points), valid(false)
{
	if (points.size() < 3)
	{
		std::cout << "The point vector must have at least 3 elements." << std::endl;
		valid = false;
		return;
	}
	
	for (size_t i = 2; i < points.size(); i++)
	{
		if (calc_orientation(points[i - 2], points[i - 1], points[i]) != 0)
		{
			valid = true;
		}
	}

	if (!valid)
	{
		std::cout << "All elements in the point vector are colinear." << std::endl;
	}
}

Polygon ConvexHull::monotone_chain()
{
	if (!valid) { return Polygon(); }

	std::vector<Point> u, l;
	int size = (int)points.size();

	u.push_back(points[0]);
	u.push_back(points[1]);
	for (int i = 2; i < size; i++)
	{
		u.push_back(points[i]);
		while (u.size() > 2 && calc_orientation(u[u.size() - 3], u[u.size() - 2], u[u.size() - 1]) < 0)
		{
			u.erase(u.end() - 2);
		}
	}

	l.push_back(points[size - 1]);
	l.push_back(points[size - 2]);
	for (int i = size - 3; i > -1; i--)
	{
		l.push_back(points[i]);
		while (l.size() > 2 && calc_orientation(l[l.size() - 3], l[l.size() - 2], l[l.size() - 1]) < 0)
		{
			l.erase(l.end() - 2);
		}
	}

	u.pop_back();
	l.pop_back();
	u.insert(u.end(), l.begin(), l.end());

	return Polygon(u);
}

Polygon ConvexHull::jarvis_march()
{
	if (!valid) { return Polygon(); }

	int p1 = 0;
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i] < points[p1])
		{
			p1 = i;
		}
	}

	int p = p1;
	int q = 0;
	std::vector<Point> L;
	do
	{
		L.push_back(points[p]);

		q = (p + 1) % points.size();
		for (size_t i = 0; i < points.size(); i++)
		{
			if (calc_orientation(points[p], points[i], points[q]) > 0)
			{
				q = i;
			}
		}

		p = q;
	} while (p != p1);

	return Polygon(L);
}

double ConvexHull::calc_orientation(Point& a, Point& b, Point& c)
{
	return (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);
}

ConvexHull::~ConvexHull()
{
}