#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#include <SDL.h>

#include "main.hpp"
#include "geometric_objects.hpp"
#include "convex_hull.hpp"
#include "window.hpp"

std::ostream& operator<<(std::ostream& stream, const Point& p)
{
	stream << p.x << "," << p.y;
	return stream;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cout << "Invalid command line parameters." << std::endl;
		return -1;
	}

	std::vector<Point> points;
	convert_point_list(argv[2], points);
	std::sort(points.begin(), points.end());
	ConvexHull convex_hull(points);

	if (convex_hull.valid)
	{
		Polygon convex_hull_polygon;
		if (strcmp(argv[1], "mc") == 0)
		{
			convex_hull_polygon = convex_hull.monotone_chain();
		}
		else if (strcmp(argv[1], "jm") == 0)
		{
			convex_hull_polygon = convex_hull.jarvis_march();
		}

		for (size_t i = 0; i < convex_hull_polygon.points.size(); i++)
		{
			std::cout << convex_hull_polygon.points[i] << std::endl;
		}

		Window window(1001, 1001);

		while (window.initialized)
		{
			window.handle_events();
			window.render(points, convex_hull_polygon);
		}
	}
	else
	{
		std::cout << "Error computing convex hull of given points." << std::endl;
	}

	return 0;
}

void convert_point_list(char* point_list, std::vector<Point>& points_vec)
{
	std::string point_list_str(point_list);
	std::vector<std::string> point_str_vec;
	split_string(point_list_str, point_str_vec, ';');

	for (size_t i = 0; i < point_str_vec.size(); i++)
	{
		std::vector<std::string> coords;
		split_string(point_str_vec[i], coords, ',');
		points_vec.push_back(Point(std::stod(coords[0]), std::stod(coords[1])));
	}
}

void split_string(std::string str, std::vector<std::string>& splitted, char delimiter)
{
	std::string item;
	std::istringstream iss(str);
	while (std::getline(iss, item, delimiter))
	{
		splitted.push_back(item);
	}
}
