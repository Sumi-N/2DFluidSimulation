#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "TextureAttribute.h"

#define WIDTH 1920
#define HEIGHT 1080

GLFWwindow* window;

unsigned int vao = 0;
unsigned int vbo;

struct MousePosition
{
	float x = 0, y = 0;
	float oldx = 0, oldy = 0;
	bool isLeftButtonPressing;
};

MousePosition mousepoint;

void SetUpQuad()
{
	if (vao == 0)
	{
		constexpr float vertices[] = {
			// positions        
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
}

void RenderQuad()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("x position %f, y position %f \n", xpos, ypos);
	mousepoint.oldx = mousepoint.x;
	mousepoint.oldy = mousepoint.y;
	mousepoint.x = (float)xpos;
	mousepoint.y = HEIGHT - (float)ypos;
	return;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			//mousestate.isRightButtonPressing = true;
		}
		else
		{
			//mousestate.isRightButtonPressing = false;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			mousepoint.isLeftButtonPressing = true;
		}
		else if (action == GLFW_RELEASE)
		{
			mousepoint.isLeftButtonPressing = false;
		}
	}
}

int main()
{
	// Initialize GLFW
	if (glfwInit() == GL_FALSE)
	{
		// If glfw initialization failed
		std::cerr << "Can't initialize GLFW" << std::endl;
		return 0;
	}

	// Register a function which will execute when the program finish 
	atexit(glfwTerminate);

	//Select OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creating a window
	window = glfwCreateWindow(WIDTH, HEIGHT, "2D Fluid Simulation", NULL, NULL);
	if (window == NULL)
	{
		// If the window is not created
		std::cerr << "Can't create GLFW window." << std::endl;
		glfwTerminate();
		return 0;
	}
	// Attached the OpenGL to this window
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Can't initialize GLEW" << std::endl;
		return 0;
	}

	// The timing to wait for V-Sync
	glfwSwapInterval(1);

	// Set background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glfwSetCursorPosCallback(window, cursorPositionCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	///////////////////////////////////////////////////

	// CreateShader
	Shader* shader = new Shader();
	shader->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/shader.glsl.frag");
	shader->LoadShader();

	// CreateShader for addforce
	Shader* addforce = new Shader();
	addforce->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/splat.glsl.frag");
	addforce->LoadShader();

	// CreateShader for advect
	Shader* advect = new Shader();
	advect->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/advect.glsl.frag");
	advect->LoadShader();

	// CreateShader for diffuse
	Shader* diffuse = new Shader();
	diffuse->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/jacobi.glsl.frag");
	diffuse->LoadShader();

	// CreateShader for divergence
	Shader* divergence = new Shader();
	divergence->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/divergence.glsl.frag");
	divergence->LoadShader();

	// CreateShader for pressure
	Shader* pressure = new Shader();
	pressure->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/jacobi_pressure.glsl.frag");
	pressure->LoadShader();

	// CreateShader for gradient
	Shader* gradient = new Shader();
	gradient->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/gradient.glsl.frag");
	gradient->LoadShader();

	// CreateShader for boundary
	Shader* boundary = new Shader();
	boundary->SetShader("../../Assets/Shaders/shader.glsl.vert", "../../Assets/Shaders/boundary.glsl.frag");
	boundary->LoadShader();

	// Setup quad
	SetUpQuad();

	// Load graphic texture
	TextureAttribute* graphctexture = new TextureAttribute();
	graphctexture->Load("../../Assets/Textures/Untitled.png");

	// Create graphic texture
	GLuint graphctextureid;
	GLuint graphctextureunit = 0;
	glGenTextures(1, &graphctextureid);
	glActiveTexture(GL_TEXTURE0 + graphctextureunit);
	glBindTexture(GL_TEXTURE_2D, graphctextureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, graphctexture->width, graphctexture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, graphctexture->pixels.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//////////////////////////////////////////////////////////////////////////////////////////////

	// Create advect frame buffer
	GLuint advectframebufferid;
	glGenFramebuffers(1, &advectframebufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, advectframebufferid);

	// Create graphic texture
	GLuint advecttextureid;
	GLuint advecttextureunit = 3;
	glGenTextures(1, &advecttextureid);
	glActiveTexture(GL_TEXTURE0 + advecttextureunit);
	glBindTexture(GL_TEXTURE_2D, advecttextureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, graphctexture->width, graphctexture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, advecttextureid, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error! FrameBuffer is not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///////////////////////////////////////////////////////////////////////////////

	// Create frame buffer for velocity

	GLuint velocityframebufferid;
	glGenFramebuffers(1, &velocityframebufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, velocityframebufferid);

	GLuint velocitytextureid;
	GLuint velocitytextureunit = 1;
	glGenTextures(1, &velocitytextureid);
	glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
	glBindTexture(GL_TEXTURE_2D, velocitytextureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, graphctexture->width, graphctexture->height, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, velocitytextureid, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error! FrameBuffer is not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///////////////////////////////////////////////////////////////////////////////

	// Create frame buffer for pressure

	GLuint pressureframebufferid;
	glGenFramebuffers(1, &pressureframebufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, pressureframebufferid);

	GLuint pressuretextureid;
	GLuint pressuertextureunit = 2;
	glGenTextures(1, &pressuretextureid);
	glActiveTexture(GL_TEXTURE0 + pressuertextureunit);
	glBindTexture(GL_TEXTURE_2D, pressuretextureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, graphctexture->width, graphctexture->height, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pressuretextureid, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error! FrameBuffer is not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	///////////////////////////////////////////////////////////////////////////////

	// Create frame buffer for divergence

	GLuint divergenceframebufferid;
	glGenFramebuffers(1, &divergenceframebufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, divergenceframebufferid);

	GLuint divergencetextureid;
	GLuint divergencetextureunit = 4;
	glGenTextures(1, &divergencetextureid);
	glActiveTexture(GL_TEXTURE0 + divergencetextureunit);
	glBindTexture(GL_TEXTURE_2D, divergencetextureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, graphctexture->width, graphctexture->height, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, divergencetextureid, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error! FrameBuffer is not complete");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	///////////////////////////////////////////////////

	// write texture to advect frame buffer once
	glViewport(0, 0, WIDTH, HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, advectframebufferid);
	{
		advect->BindShader();

		glActiveTexture(GL_TEXTURE0 + graphctextureunit);
		glBindTexture(GL_TEXTURE_2D, graphctextureid);

		glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
		glBindTexture(GL_TEXTURE_2D, velocitytextureid);

		RenderQuad();
	}





	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		glfwPollEvents();

		if (mousepoint.isLeftButtonPressing)
		{
			// Add force
			glViewport(0, 0, WIDTH, HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, velocityframebufferid);
			{
				addforce->BindShader();

				GLint loc2 = glGetUniformLocation(addforce->programid, "power");
				if (loc2 != -1)
				{
					glUniform2f(loc2, mousepoint.x - mousepoint.oldx, mousepoint.y -mousepoint.oldy);
				}

				GLint loc = glGetUniformLocation(addforce->programid, "point");
				if (loc != -1)
				{
					glUniform2f(loc, mousepoint.x, mousepoint.y);
				}

				glActiveTexture(GL_TEXTURE0 + graphctextureunit);
				glBindTexture(GL_TEXTURE_2D, velocitytextureid);

				RenderQuad();
			}
		}

		// Calculate advect
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, advectframebufferid);
		{
			advect->BindShader();

			glActiveTexture(GL_TEXTURE0 + graphctextureunit);
			glBindTexture(GL_TEXTURE_2D, advecttextureid);

			glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
			glBindTexture(GL_TEXTURE_2D, velocitytextureid);

			RenderQuad();
		}

		////Calculate diffuse
		//glViewport(0, 0, WIDTH, HEIGHT);
		//glBindFramebuffer(GL_FRAMEBUFFER, advectframebufferid);
		//diffuse->BindShader();
		//for (int i = 0; i < 50; i++)
		//{

		//	{
		//		glActiveTexture(GL_TEXTURE0 + graphctextureunit);
		//		glBindTexture(GL_TEXTURE_2D, advecttextureid);

		//		glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
		//		glBindTexture(GL_TEXTURE_2D, advecttextureid);

		//		RenderQuad();
		//	}
		//}

		//Calculate divergence
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, divergenceframebufferid);
		{
			divergence->BindShader();

			glActiveTexture(GL_TEXTURE0 + graphctextureunit);
			glBindTexture(GL_TEXTURE_2D, velocitytextureid);

			RenderQuad();
		}

		// Calculate pressure
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, pressureframebufferid);
		pressure->BindShader();
		for (int i = 0; i < 50; i++)
		{

			{
				glActiveTexture(GL_TEXTURE0 + graphctextureunit);
				glBindTexture(GL_TEXTURE_2D, pressuretextureid);

				glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
				glBindTexture(GL_TEXTURE_2D, divergencetextureid);

				RenderQuad();
			}
		}

		//Calculate gradient
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, velocityframebufferid);
		{
			gradient->BindShader();

			glActiveTexture(GL_TEXTURE0 + graphctextureunit);
			glBindTexture(GL_TEXTURE_2D, pressuretextureid);

			glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
			glBindTexture(GL_TEXTURE_2D, velocitytextureid);

			RenderQuad();
		}

		// Show result
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		{
			shader->BindShader();

			glActiveTexture(GL_TEXTURE0 + graphctextureunit);
			glBindTexture(GL_TEXTURE_2D, advecttextureid);

			glActiveTexture(GL_TEXTURE0 + velocitytextureunit);
			glBindTexture(GL_TEXTURE_2D, velocitytextureid);

			RenderQuad();
		}

		glfwSwapBuffers(window);
	}

	return 0;
}