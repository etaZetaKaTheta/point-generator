#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "glad.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "shader.h"

#define PI 3.14159

struct Vertex
{
	float x, y;
};

int screenWidth = 1000, screenHeight = 1000;

float distanceThreshold = 0.5f;
int points = 5;
float pointScale = 10.0f;

bool greenCircleActive = true;
bool redCirclesActive = true;

bool lastRed = false;

std::vector<Vertex> redPoints;
std::vector<Vertex> bluePoints;
std::vector<Vertex> circlePoints;
std::vector<Vertex> sqrtcirclePoints;

std::random_device rd;
std::default_random_engine gen(rd());

void renderCircles()
{
	for (int i = 0; i < redPoints.size(); i++)
	{
		Vertex currentRedVertex = redPoints.at(i);

		for (float angle = 0.0f; angle <= (2.0f * PI); angle += 0.01f)
		{
			Vertex currentVertex = Vertex(currentRedVertex.x + distanceThreshold * sin(angle), currentRedVertex.y + distanceThreshold * cos(angle));
			circlePoints.push_back(currentVertex);
		}
	}
}

void rendersqrtCircle()
{
	for (float angle = 0.0f; angle <= (2.0f * PI); angle += 0.01f)
	{
		Vertex currentVertex = Vertex((distanceThreshold * (sqrt(2) / 2)) * sin(angle), (distanceThreshold * (sqrt(2) / 2)) * cos(angle));
		sqrtcirclePoints.push_back(currentVertex);
	}
}

float randFloat(float low, float high)
{
	std::uniform_real_distribution<float> dist(low, high);
	return dist(gen);
}

void checkBluePointThreshold()
{
	float currentBlueX = randFloat(-1.0f, 1.0f);
	float currentBlueY = randFloat(-1.0f, 1.0f);

	for (int i = 0; i < redPoints.size(); i++)
	{
		Vertex currentRedVertex = redPoints.at(i);
		if (sqrt(pow((currentRedVertex.x - currentBlueX), 2.0f) + pow((currentRedVertex.y - currentBlueY), 2.0f)) > distanceThreshold)
		{
			checkBluePointThreshold();
			return;
		}
	}

	bluePoints.push_back(Vertex(currentBlueX, currentBlueY));
}

void checkRedPointThreshold()
{
	float currentRedX = randFloat(-1.0f, 1.0f);
	float currentRedY = randFloat(-1.0f, 1.0f);

	for (int i = 0; i < bluePoints.size(); i++)
	{
		Vertex currentBlueVertex = bluePoints.at(i);
		if (sqrt(pow((currentBlueVertex.x - currentRedX), 2.0f) + pow((currentBlueVertex.y - currentRedY), 2.0f)) > distanceThreshold)
		{
			checkRedPointThreshold();
			return;
		}
	}

	redPoints.push_back(Vertex(currentRedX, currentRedY));
}

void calculatePoints()
{
	//redPoints.push_back(Vertex(randFloat(-1.0f, 1.0f), randFloat(-1.0f, 1.0f)));
	lastRed = true;

	for (int i = 0; i < points; i++)
	{
		if (lastRed)
		{
			checkBluePointThreshold();
			lastRed = false;
		}
		else
		{
			checkRedPointThreshold();
			lastRed = true;
		}
	}
}

int main()
{
	calculatePoints();
	renderCircles();
	rendersqrtCircle();

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "point-generator", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Fenster konnte nicht geöffnet werden!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	gladLoadGL();

	glViewport(0, 0, screenWidth, screenHeight);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader mainShader("standard.vert", "standard.frag");

	GLuint redVAO, redVBO, blueVAO, blueVBO, circVAO, circVBO, sqrtcircVAO, sqrtcircVBO;

	glGenVertexArrays(1, &redVAO);
	glGenBuffers(1, &redVBO);

	glBindVertexArray(redVAO);

	glBindBuffer(GL_ARRAY_BUFFER, redVBO);
	glBufferData(GL_ARRAY_BUFFER, redPoints.size() * sizeof(Vertex), redPoints.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//

	glGenVertexArrays(1, &blueVAO);
	glGenBuffers(1, &blueVBO);

	glBindVertexArray(blueVAO);

	glBindBuffer(GL_ARRAY_BUFFER, blueVBO);
	glBufferData(GL_ARRAY_BUFFER, bluePoints.size() * sizeof(Vertex), bluePoints.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//

	glGenVertexArrays(1, &circVAO);
	glGenBuffers(1, &circVBO);

	glBindVertexArray(circVAO);

	glBindBuffer(GL_ARRAY_BUFFER, circVBO);
	glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(Vertex), circlePoints.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//

	glGenVertexArrays(1, &sqrtcircVAO);
	glGenBuffers(1, &sqrtcircVBO);

	glBindVertexArray(sqrtcircVAO);

	glBindBuffer(GL_ARRAY_BUFFER, sqrtcircVBO);
	glBufferData(GL_ARRAY_BUFFER, sqrtcirclePoints.size() * sizeof(Vertex), sqrtcirclePoints.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_PROGRAM_POINT_SIZE);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
		glViewport(0, 0, screenWidth, screenHeight);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainShader.activate();

		glUniform1f(glGetUniformLocation(mainShader.id, "pointSize"), pointScale);

		glUniform4f(glGetUniformLocation(mainShader.id, "currentColor"), 1.0f, 0.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(mainShader.id, "pointSize"), pointScale / 2);

		glBindVertexArray(redVAO);
		glDrawArrays(GL_POINTS, 0, redPoints.size());

		glUniform4f(glGetUniformLocation(mainShader.id, "currentColor"), 0.0f, 0.0f, 1.0f, 1.0f);

		glBindVertexArray(blueVAO);
		glDrawArrays(GL_POINTS, 0, bluePoints.size());

		if (greenCircleActive)
		{
			glUniform4f(glGetUniformLocation(mainShader.id, "currentColor"), 0.0f, 1.0f, 0.0f, 1.0f);

			glBindVertexArray(sqrtcircVAO);
			glDrawArrays(GL_POINTS, 0, sqrtcirclePoints.size());
		}
		if (redCirclesActive)
		{
			glUniform4f(glGetUniformLocation(mainShader.id, "currentColor"), 1.0f, 0.0f, 0.0f, 1.0f);

			glBindVertexArray(circVAO);
			glDrawArrays(GL_POINTS, 0, circlePoints.size());
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Lionel hat einen fetten Schwanz");
		ImGui::InputInt("Punkte", &points);
		ImGui::InputFloat("Größe", &pointScale);
		ImGui::InputFloat("Maximale Distanz", &distanceThreshold);
		if (ImGui::RadioButton("Rote Kreise", redCirclesActive))
		{
			if (redCirclesActive)
			{
				redCirclesActive = false;
			}
			else
			{
				redCirclesActive = true;
			}
		}
		if (ImGui::RadioButton("Wurzel 2 Kreis", greenCircleActive))
		{
			if (greenCircleActive)
			{
				greenCircleActive = false;
			}
			else
			{
				greenCircleActive = true;
			}
		}
		if (ImGui::Button("Generieren"))
		{
			redPoints.clear();
			bluePoints.clear();
			circlePoints.clear();
			calculatePoints();
			renderCircles();
			rendersqrtCircle();
			glBindBuffer(GL_ARRAY_BUFFER, redVBO);
			glBufferData(GL_ARRAY_BUFFER, redPoints.size() * sizeof(Vertex), redPoints.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, blueVBO);
			glBufferData(GL_ARRAY_BUFFER, bluePoints.size() * sizeof(Vertex), bluePoints.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, circVBO);
			glBufferData(GL_ARRAY_BUFFER, circlePoints.size() * sizeof(Vertex), circlePoints.data(), GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, sqrtcircVBO);
			glBufferData(GL_ARRAY_BUFFER, sqrtcirclePoints.size() * sizeof(Vertex), sqrtcirclePoints.data(), GL_DYNAMIC_DRAW);
		}
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	mainShader.dispose();
	glDeleteVertexArrays(1, &redVAO);
	glDeleteBuffers(1, &redVBO);

	glDeleteVertexArrays(1, &blueVAO);
	glDeleteBuffers(1, &blueVBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
