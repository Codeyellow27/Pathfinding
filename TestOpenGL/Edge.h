#pragma once

#include <iostream>

class Edge
{
public:

	Edge(const std::string from_ = "-1", const std::string to_ = "-1", const float cost_ = 0.0f);

	float cost;
	std::string from;
	std::string to;


};