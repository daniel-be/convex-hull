#pragma once
#include <vector>

#include "geometric_objects.hpp"

void convert_point_list(char* point_list, std::vector<Point>& points_vec);
void split_string(std::string str, std::vector<std::string>& splitted, char delimiter);
