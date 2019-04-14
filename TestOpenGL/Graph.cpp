
#include "Graph.h"


void Graph::AddConnextion(const std::string from, const std::string to, const float cost)
{
	Edge e(from, to, cost);
	connection.insert(std::pair<std::string, Edge>(from, e));
}

std::string Graph::GetConnexionFromEnd(const std::string end, std::vector<Node> nodeList)
{
	std::vector<Node>::iterator it = nodeList.begin();
	for (it; it != nodeList.end(); it++)
	{
		//std::cout << it->connexion.from << " : " << it->connexion.to << std::endl;

		if (it->connexion.to == end)
		{
			return it->connexion.from;
		}
	}

	return "";
}


std::vector<Edge> Graph::GetConnexion(std::string from)
{
	std::vector<Edge> result;

	std::multimap<std::string, Edge>::const_iterator it = connection.find(from);

	for (it; it != connection.end(); it++)
	{
		if (it->first == from)
		{
			//std::cout << it->first << " : " << it->second.to << std::endl;
			result.push_back(it->second);
		}

	}

	return result;
}



int Graph::FindSmallestElement(std::vector<Node> openList)
{
	float costSoFarMin = 100.0f;
	int indice = -1;

	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i].costSoFar < costSoFarMin)
		{
			costSoFarMin = openList[i].costSoFar;
			indice = i;
		}
	}

	return indice;
}

int Graph::FindSmallestElementWithEstimatedCostSoFar(std::vector<Node> openList)
{
	float estimatedCostSoFarMin = 100.0f;
	int indice = -1;

	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i].estimatedCostSoFar < estimatedCostSoFarMin)
		{
			estimatedCostSoFarMin = openList[i].estimatedCostSoFar;
			indice = i;
		}
	}

	return indice;
}

int Graph::ContainNode(std::vector<Node> openList, const Node search)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (search.id == openList[i].id)
		{
			return i;

		}
	}

	return -1;
}

glm::vec2 Graph::GetPositionTarget(std::string target)
{
	char end = target[0];

	switch (end)
	{
	case 'a':
		return glm::vec2(-14.0f, 10.0f);
		break;

	case 'b':
		return glm::vec2(-3.5f, 10.0f);
		break;

	case 'c':
		return glm::vec2(-3.5f, 7.0f);
		break;

	case 'd':
		return glm::vec2(-14.0f, 4.0f);
		break;

	case 'e':
		return glm::vec2(6.5f, 10.0f);
		break;

	case 'f':
		return glm::vec2(6.5f, 7.0f);
		break;

	case 'g':
		return glm::vec2(2.0f, 3.0f);
		break;

	case 'h':
		return glm::vec2(11.0f, 14.0f);
		break;

	case 'i':
		return glm::vec2(-8.0f, -4.0f);
		break;
	}

	return glm::vec2(0.0f, 0.0f);
}

std::string Graph::SearchDijkstra(std::string start, std::string end)
{
	Node startNode(start);

	std::string path = "";

	std::vector<Node> openList;
	std::vector<Node> closeList;
	openList.push_back(startNode);

	Node current;

	while (openList.size() > 0)
	{
		int currentIndice = FindSmallestElement(openList);

		if (currentIndice == -1)
		{
			std::cout << "Eroor - 1 " << std::endl;
			return "";
		}

		current = openList[currentIndice];
		if (current.id == end)
		{
			break;
		}
		std::vector<Edge> connexions = GetConnexion(current.id);
		for (int i = 0; i < connexions.size(); i++)
		{
			Node endNode = connexions[i].to;
			float endNodeCost = current.costSoFar + connexions[i].cost;
			if (ContainNode(closeList, endNode) != -1)
			{
				continue;
			}
			else if (ContainNode(openList, endNode) != -1)
			{
				Node* endNodeRecord = nullptr;
				*endNodeRecord = openList[ContainNode(openList, *endNodeRecord)];

				if (endNodeRecord)
				{
					if (endNodeRecord->costSoFar <= endNodeCost)
					{
						continue;
					}
				}
			}
			else
			{
				Node endNodeRecord(endNode.id);
				endNodeRecord.costSoFar = endNodeCost;
				endNodeRecord.connexion = connexions[i];
				if (ContainNode(openList, endNodeRecord) == -1)
				{
					openList.push_back(endNodeRecord);
				}
			}
		}
		openList.erase(openList.begin() + currentIndice);
		closeList.push_back(current);
	}

	if (current.id == end)
	{
		//std::cout << "Chemin trouve vers " << end << std::endl;
		std::vector<std::string> reversePath;
		reversePath.push_back(current.id);
		while (current.id != start)
		{
			std::string result = GetConnexionFromEnd(current.id, closeList);
			if (result.size() == 0)
			{
				result = GetConnexionFromEnd(current.id, openList);
			}
			reversePath.push_back(result);
			current.id = result; // recuperer le node par le caractere...
		}
		/*std::cout << "Result " << std::endl;
		for (int i = 0; i < path.size(); i++)
		{
			std::cout << path[i] << std::endl;
		}*/
		for (int i = reversePath.size() - 1; i >= 0; i--)
		{
			path.append(reversePath[i]);
		}
		return path;
	}

	return "";
}

std::string Graph::SearchAStar(std::string start, std::string end)
{
	Node startNode(start);

	std::string path = "";

	std::vector<Node> openList;
	std::vector<Node> closeList;
	openList.push_back(startNode);

	Node current;

	while (openList.size() > 0)
	{
		int currentIndice = FindSmallestElementWithEstimatedCostSoFar(openList);

		if (currentIndice == -1)
		{
			std::cout << "Eroor - 1 " << std::endl;
			return "";
		}

		current = openList[currentIndice];
		if (current.id == end)
		{
			break;
		}
		std::vector<Edge> connexions = GetConnexion(current.id);
		for (int i = 0; i < connexions.size(); i++)
		{
			Node endNode = connexions[i].to;
			float distance = glm::distance(GetPositionTarget(endNode.id), GetPositionTarget(end));
			float endNodeCost = current.costSoFar + connexions[i].cost;
			float endNodeEstimatedCostSoFar = current.costSoFar + connexions[i].cost + distance;
			if (ContainNode(closeList, endNode) != -1)
			{
				continue;
			}
			else if (ContainNode(openList, endNode) != -1)
			{
				Node* endNodeRecord = nullptr;
				*endNodeRecord = openList[ContainNode(openList, *endNodeRecord)];

				if (endNodeRecord)
				{
					if (endNodeRecord->costSoFar <= endNodeCost)
					{
						continue;
					}
				}
			}
			else
			{
				Node endNodeRecord(endNode.id);
				endNodeRecord.costSoFar = endNodeCost;
				endNodeRecord.estimatedCostSoFar = endNodeEstimatedCostSoFar;
				endNodeRecord.connexion = connexions[i];
				if (ContainNode(openList, endNodeRecord) == -1)
				{
					openList.push_back(endNodeRecord);
				}
			}
		}
		openList.erase(openList.begin() + currentIndice);
		closeList.push_back(current);
	}

	if (current.id == end)
	{
		//std::cout << "Chemin trouve vers " << end << std::endl;
		std::vector<std::string> reversePath;
		reversePath.push_back(current.id);
		while (current.id != start)
		{
			std::string result = GetConnexionFromEnd(current.id, closeList);
			if (result.size() == 0)
			{
				result = GetConnexionFromEnd(current.id, openList);
			}
			reversePath.push_back(result);
			current.id = result; // recuperer le node par le caractere...
		}
		/*std::cout << "Result " << std::endl;
		for (int i = 0; i < path.size(); i++)
		{
			std::cout << path[i] << std::endl;
		}*/
		for (int i = reversePath.size() - 1; i >= 0; i--)
		{
			path.append(reversePath[i]);
		}
		return path;
	}

	return "";
}

