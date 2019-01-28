#include <SDL.h>

#include "window.hpp"

#define MARGIN 50
#define POINT_CIRCLE_RAD 3

Window::Window(int width, int height) : width(width), height(height), initialized(false)
{
	initialize();
}

void Window::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
		return;
	}

	window = SDL_CreateWindow("Convex Hull", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create Renderer: %s", SDL_GetError());
		return;
	}

	initialized = true;
}

void Window::handle_events()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			initialized = false;
		}
	}
}

void Window::render(std::vector<Point>& points, Polygon& convex_hull_polygon)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);

	draw_coordinate_system();
	draw_convex_hull(points, convex_hull_polygon);

	SDL_RenderPresent(renderer);
}

void Window::draw_convex_hull(std::vector<Point>& points, Polygon& convex_hull_polygon)
{
	draw_points(points);
	draw_polygon(convex_hull_polygon);
}

double Window::calc_scale(std::vector<Point>& points)
{
	int s = 0, b = 0;
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i].x < points[s].x)
		{
			s = i;
		}
		if (points[i].x > points[b].x)
		{
			b = i;
		}
	}

	double max_x = abs(points[s].x) > abs(points[b].x) ? abs(points[s].x) : abs(points[b].x);

	s = 0, b = 0;
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i].y < points[s].y)
		{
			s = i;
		}
		if (points[i].y > points[b].y)
		{
			b = i;
		}
	}

	double max_y = abs(points[s].y) > abs(points[b].y) ? abs(points[s].y) : abs(points[b].y);

	double max = max_x > max_y ? max_x : max_y;
	double max_points = (width - MARGIN - 1) / 2;
	return max_points / max;
}

void Window::convert_points_to_sdl_points(std::vector<Point>& points, std::vector<SDL_Point>& sdl_points)
{
	double scale = calc_scale(points);
	for (size_t i = 0; i < points.size(); i++)
	{
		SDL_Point sdl_point;
		sdl_point.x = (width / 2 + 1) + ((points[i].x * scale));
		sdl_point.y = (height / 2 + 1) - ((points[i].y * scale));
		sdl_points.push_back(sdl_point);
	}
}

void Window::draw_coordinate_system()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, 0, (height / 2) + 1, width, (height / 2) + 1);
	SDL_RenderDrawLine(renderer, (width / 2) + 1, 0, (width / 2) + 1, height);
}

void Window::draw_points(std::vector<Point>& points)
{
	std::vector<SDL_Point> sdl_points;
	convert_points_to_sdl_points(points, sdl_points);

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for (size_t i = 0; i < sdl_points.size(); i++)
	{
		SDL_Rect rect;
		rect.x = sdl_points[i].x - 1;
		rect.y = sdl_points[i].y - 1;
		rect.h = 3;
		rect.w = 3;
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Window::draw_polygon(Polygon& polygon)
{
	std::vector<SDL_Point> sdl_points;
	convert_points_to_sdl_points(polygon.points, sdl_points);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	for (size_t i = 0; i < sdl_points.size(); i++)
	{
		if (i < sdl_points.size() - 1)
		{
			SDL_RenderDrawLine(renderer, sdl_points[i].x, sdl_points[i].y,
				sdl_points[i + 1].x, sdl_points[i + 1].y);
		}
		else
		{
			SDL_RenderDrawLine(renderer, sdl_points[i].x, sdl_points[i].y,
				sdl_points[0].x, sdl_points[0].y);
		}		
	}
}

Window::~Window()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}