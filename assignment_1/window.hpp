#pragma once
#include <SDL.h>

#include "geometric_objects.hpp"

class Window
{
public:
	bool initialized;

	Window(int width, int height);
	void initialize();
	void handle_events();
	void render(std::vector<Point>& points, Polygon& convex_hull_polygon);
	~Window();

private:
	int width, height;
	SDL_Window* window;
	SDL_Renderer* renderer;

	double calc_scale(std::vector<Point>& points);
	void convert_points_to_sdl_points(std::vector<Point>& points, std::vector<SDL_Point>& sdl_points);
	void draw_coordinate_system();
	void draw_points(std::vector<Point>& points);
	void draw_polygon(Polygon& polygon);
	void draw_convex_hull(std::vector<Point>& points, Polygon& convex_hull_polygon);
};
