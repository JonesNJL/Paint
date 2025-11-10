#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "shaderClass.hpp";
#include "VBO.hpp";
#include "EBO.hpp";
#include "VAO.hpp";
#include "SSBO.hpp"
#include "Region.hpp"
#include "RegionManager.hpp"
#include "ImageRegistry.hpp"
#include "Painter.hpp"
#include "Controls.hpp"
#include "GuiEventManager.hpp"

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);





	unsigned int vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, float(sqrt(3)) / 3, 0.0f,
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f

	};

	GLuint indices[] =
	{
		0, 3, 5,
		3, 2, 4,
		5, 4, 1,
		5, 4, 3,
		1, 3, 4,
		2, 3, 1
	};





	GLFWwindow* window = glfwCreateWindow(640, 640, "Epic Paint Program", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	//glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);
	glfwMaximizeWindow(window);

	gladLoadGL(); //Must go before glViewport

	int screenWidth;
	int screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	Shader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	unsigned int* ssboData = new unsigned int[3] { 1, 2, 3 };
	SSBO SSBO1(ssboData, 3, 4);
	SSBO1.Unbind();

	VAO1.LinkVBO(VBO1);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	



	GLuint ID;
	glGenBuffers(1, &ID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ID);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//glClearColor(0.0f, 0.65f, 0.85f, 1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);



	ImageRegistry imageRegistry;
	Painter painter = Painter(&imageRegistry);
	GuiEventManager guiEventManager(&painter);

	RegionManager regionManager;
	regionManager.Init(&imageRegistry, &guiEventManager, &painter, Int2(screenWidth, screenHeight));

	Controls controls(window, screenHeight);
	controls.CreateKeybinds();

	bool yes = true;

	int clickState = false;
	bool mouseReleased = true;


	while (!glfwWindowShouldClose(window))
	{
		//glClearColor(0.0f, 0.65f, 0.85f, 1.0f);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	
		shaderProgram.Activate();

		VAO1.Bind();



		std::vector<unsigned int> v;
		std::vector<GLuint> i;

		//regionManager.GetRenderDataX(v, i);
		regionManager.GetRenderData(v, i);

		unsigned int* bertices = v.data();
		GLuint* bndices = i.data();


		// Logging inital verticies, can be removed at some point :p
		if (yes)
		{
			std::cout << std::to_string(v.size()) << std::endl;

			//for (int j = 0; j < i.size(); j++)
			//{
				//std::cout << std::to_string(sizeof(GLuint) * i.size()) << std::endl;
				//std::cout << i[j] << std::endl;
			//}

			for (int j = 0; j < v.size(); j++)
			{
				std::cout << bertices[j] << std::endl;
			}
		}
		yes = false;



		controls.Update();
		regionManager.UpdateRegions(controls);



		

		//vertices[0] = -0.5f + float(rand() % 10) / 100;

		VBO1.UpdateVertices(bertices, sizeof(unsigned int) * v.size());
		EBO1.UpdateIndices(bndices, sizeof(GLuint) * i.size());

		//ssboData[0] = (ssboData[0] + 1) % 120;

		int length = 0; 
		unsigned int* ssboData = imageRegistry.GetData(length);

		SSBO1.UpdateData(ssboData, length);

		GLuint shaderScreenSizeLoc = glGetUniformLocation(shaderProgram.ID, "screenSize");
		glUniform2f(shaderScreenSizeLoc, screenWidth, screenHeight);

		SSBO1.Unbind();
		delete[] ssboData;
		ssboData = nullptr;

		

		glDrawElements(GL_TRIANGLES, i.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}






	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow;
	glfwTerminate();

	return 0;
}