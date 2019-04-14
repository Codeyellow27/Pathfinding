#include <iostream>
#include <vector>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Shader.h"

#include "Triangle.h"
#include "Line.h"
#include "Text.h"
#include "Plane.h"
#include "Graph.h"

void processInput(GLFWwindow *window);
void mouseCall(GLFWwindow* window, double xPos, double yPos);
void RenderLetterNode(Shader textShader);
void SetPath(std::string l);
void UsePathfinding();

const GLuint WIDTH = 1280;
const GLuint HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;

Text text;

std::string startPath;
std::string endPath;
std::string methodPathfinding;

bool setStart = false;
std::vector<Line*> lineList;
std::map<std::string, int> lineMap;
Graph graph;
Plane* listPlane;
const int nbPlane = 9;

void DrawLine(Line& line, glm::mat4 transform, glm::vec3 color)
{
	line.ShaderUse();
	line.SetVec3("uColor", color);
	line.SetShaderMatrix("transform", transform);
	line.Draw();
}

void DrawPlane(Plane& plane, glm::mat4 transform,glm::vec3 color)
{
	plane.ShaderUse();
	plane.SetVec3("uColor", color);
	plane.SetShaderMatrix("transform", transform);
	plane.Draw();
}

void SetPlaneTransform(Plane& plane, glm::vec3 translate)
{
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, glm::vec3(0.05f, 0.05f, 0.05f));
	transform = glm::translate(transform, translate);
	plane.transform = transform;
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::string line;
	std::ifstream myfile("Connexions.txt");

	if (myfile.is_open())
	{
		int count = 0;
		while (std::getline(myfile, line))
		{
			std::string start = line.substr(0, 1);
			std::string end = line.substr(2, 1);
			std::string cost = line.substr(4, 4);
			float fcost = std::stof(cost);
			//std::cout << start << " " << end << " " << fcost << "\n";
			graph.AddConnextion(start, end, fcost);
			lineMap.insert(std::pair<std::string, int>(start + end, count++));
		}
	}

	myfile.close();

	startPath = "h";
	endPath = "d";

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Create windows

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pathfinding", nullptr, nullptr);

	int screenWidth;
	int screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseCall);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW " << std::endl;
		return EXIT_FAILURE;
	}

	text.LoadFont("Fonts/arial.ttf");
	glViewport(0, 0, screenWidth, screenHeight);
	text.SetPixelSize(0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	Shader textShader("Shaders/vShaderText.vs", "Shaders/fShaderText.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	textShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(textShader.id, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	text.LoadCharacter();
	text.Done();
	text.InitBuffer();


	for (int i = 0; i < 10; i++)
	{
		lineList.push_back(new Line());
	}

	SimpleShader simpleShader("Shaders/vSimpleShader.vs", "Shaders/fSimpleShader.fs");
	lineList[0]->SetLine(glm::vec3(-0.70f, 0.5f, 0.f), glm::vec3(-0.20f, 0.35f, 0.0f));
	lineList[0]->SetShader(simpleShader);
	lineList[1]->SetLine(glm::vec3(-0.70f, 0.5f, 0.f), glm::vec3(-0.70f, 0.2f, 0.0f));
	lineList[1]->SetShader(simpleShader);
	lineList[2]->SetLine(glm::vec3(-0.70f, 0.5f, 0.f), glm::vec3(-0.20f, 0.5f, 0.0f));
	lineList[2]->SetShader(simpleShader);
	lineList[3]->SetLine(glm::vec3(-0.15f, 0.5f, 0.f), glm::vec3(0.35f, 0.5f, 0.0f));
	lineList[3]->SetShader(simpleShader);
	lineList[4]->SetLine(glm::vec3(-0.15f, 0.5f, 0.f), glm::vec3(0.35f, 0.35f, 0.0f));
	lineList[4]->SetShader(simpleShader);

	lineList[5]->SetLine(glm::vec3(-0.20f, 0.35f, 0.0f), glm::vec3(0.1f, 0.15f, 0.0f));
	lineList[5]->SetShader(simpleShader);
	lineList[6]->SetLine(glm::vec3(0.35f, 0.35f, 0.0f), glm::vec3(0.1f, 0.15f, 0.0f));
	lineList[6]->SetShader(simpleShader);
	lineList[7]->SetLine(glm::vec3(0.35f, 0.5f, 0.0f), glm::vec3(0.55f, 0.7f, 0.0f));
	lineList[7]->SetShader(simpleShader);
	lineList[8]->SetLine(glm::vec3(-0.70f, 0.2f, 0.f), glm::vec3(-0.4f, -0.2f, 0.0f));
	lineList[8]->SetShader(simpleShader);
	lineList[9]->SetLine(glm::vec3(-0.20f, 0.35f, 0.f), glm::vec3(-0.4f, -0.2f, 0.0f));
	lineList[9]->SetShader(simpleShader);



	listPlane = new Plane[nbPlane];

	SetPlaneTransform(listPlane[0], glm::vec3(-14.0f, 10.0f, 0.0f));
	SetPlaneTransform(listPlane[1], glm::vec3(-3.5f, 10.0f, 0.0f));
	SetPlaneTransform(listPlane[2], glm::vec3(-3.5f, 7.0f, 0.0f));
	SetPlaneTransform(listPlane[3], glm::vec3(-14.0f, 4.0f, 0.0f));
	SetPlaneTransform(listPlane[4], glm::vec3(6.5f, 10.0f, 0.0f));
	SetPlaneTransform(listPlane[5], glm::vec3(6.5f, 7.0f, 0.0f));
	SetPlaneTransform(listPlane[6], glm::vec3(-8.0f, -4.0f, 0.0f));
	SetPlaneTransform(listPlane[7], glm::vec3(2.0f, 3.0f, 0.0f));
	SetPlaneTransform(listPlane[8], glm::vec3(11.0f, 14.0f, 0.0f));

	for (int i = 0; i < nbPlane; i++)
	{
		listPlane[i].SetShader(simpleShader);
	}

	startPath = "c";
	endPath = "e";
	methodPathfinding = "A*";

	UsePathfinding();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		glm::mat4 transform = glm::mat4(1.0f);

		simpleShader.Use();

		for (int i = 0; i < lineList.size(); i++)
		{
			DrawLine(*lineList[i], transform, lineList[i]->shapeColor);
		}

		for (int i = 0; i < nbPlane; i++)
		{
			DrawPlane(listPlane[i], listPlane[i].transform,glm::vec3(1.0f,1.0f,1.0f));
		}

		textShader.Use();

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::string pathfinding = "Method " + methodPathfinding;
		std::string start = "start " + startPath;
		std::string end = " end " + endPath;

		text.RenderText(textShader, pathfinding, 25.0f, 100.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
		text.RenderText(textShader, start, 25.0f, 75.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
		text.RenderText(textShader, end, 20.0f, 50.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));

		RenderLetterNode(textShader);
			
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

void UsePathfinding()
{
	for (int i = 0; i < lineList.size(); i++)
	{
		lineList[i]->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	std::string path = "";
	if (methodPathfinding.compare("Dijkstra") == 0)
	{
		path = graph.SearchDijkstra(startPath, endPath);
	}
	else if (methodPathfinding.compare("A*") == 0)
	{
		path = graph.SearchDijkstra(startPath, endPath);
	}

	if (path.length() > 0)
	{
		for (int i = 0; i < path.length() - 1; i++)
		{
			std::string test = path.substr(i, 2);
			lineList[lineMap[test]]->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		}
	}

	for (int i = 0; i < nbPlane; i++)
	{
		listPlane[i].SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

void RenderLetterNode(Shader textShader)
{
	text.RenderText(textShader, "a", 190.0f, 560.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "b", 530.0f, 560.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "c", 530.0f, 500.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "d", 190.0f, 380.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "e", 840.0f, 560.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "f", 840.0f, 450.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "g", 700.0f, 450.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "h", 980.0f, 630.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
	text.RenderText(textShader, "i", 380.0f, 250.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		methodPathfinding = "Dijkstra";
		UsePathfinding();
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		methodPathfinding = "A*";
		UsePathfinding();
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		SetPath("a");
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		SetPath("b");
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		SetPath("c");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		SetPath("d");
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		SetPath("e");
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		SetPath("f");
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		SetPath("g");
	}

	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		SetPath("h");
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		SetPath("i");
	}

}

void SetPath(std::string l)
{
	if (!setStart)
	{
		startPath = l;
		setStart = true;
	}
	else
	{
		if (startPath.compare(l) != 0)
		{
			endPath = l;
			setStart = false;
			UsePathfinding();
		}
	}

}

void mouseCall(GLFWwindow* window, double xPos, double yPos)
{

}
