#pragma once

#include "Edge.h"

class Node
{
public:

	Node(const std::string id_ = "", const float costSoFar_ = 0.0f, const float estimatedCostSoFar = 0.0f);

	std::string id;
	float costSoFar;
	float estimatedCostSoFar;
	Edge connexion;

};