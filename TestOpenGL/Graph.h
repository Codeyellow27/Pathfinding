#pragma once

#include <map>
#include <vector>
#include <string>

#include "Edge.h"
#include "Node.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Graph
{
public:
	std::multimap<std::string, Edge> connection;

	std::string SearchDijkstra(std::string start, std::string end);
	std::string SearchAStar(std::string start, std::string end);
	void AddConnextion(const std::string from, const std::string to, const float cost);
	std::vector<Edge> GetConnexion(const std::string from);
	std::string GetConnexionFromEnd(const std::string end, std::vector<Node> nodeList);
	glm::vec2 GetPositionTarget(std::string target);

	int FindSmallestElement(std::vector<Node> openList);
	int FindSmallestElementWithEstimatedCostSoFar(std::vector<Node> openList);
	int ContainNode(std::vector<Node> openList, const Node search);
};