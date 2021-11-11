#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "KHR/khrplatform.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include "abstract.h"
#include "window.h"
#include "shader.h"


// declaration
void processInput(GLFWwindow* window);
void saveImage(const char* filepath, GLFWwindow* w);

// settings
int width = 1000;
int height= 600;
int startwidth = width;
int startheight = height;
bool fullScreen = false;

// variables
glm::vec2 dr = glm::vec2(0);
float zoom = 1.0f;
float dt = 0;
int maxit = 20;
float last_change = 0.0f;
float t;
glm::vec2 centerPos = glm::vec2(0, 0);

int colorPermute = 0;
int invertColor = false;

bool julia = false;
bool z2 = true;
bool z3 = false;
bool z4 = false;
bool z5 = false;
bool maxDistColor = true;
GLFWmonitor* monitor;
const GLFWvidmode* mode;

bool justSaved = false;

int main() {
	// console text

	const char* text = "  ________________________________________________ \n \n"
		"  --- Mandelbrot Viewer ---\n"
		"  ________________________________________________\n \n"
		"  WASD: move \n"
		"  Q / E: decrease / increase maximum iteration number \n"
		"  Arrow UP / DOWN: zoom in / out \n"
		"  R: reset to start position and zoom \n \n"
		"  Enter: show position and zoom \n"
		"  G: got to console input position and zoom \n"
		"  F: save image in .exe folder \n"
		"  Space: switch to fullscreen \n"
		"  J: begin with julia set iteration at c = -0.6 + i0.6 \n"
		"  2,3,4,5: enable / disable z^n iteration \n"
		"  P: switch colors \n"
		"  I: invert colors  \n"
		"  ________________________________________________ \n \n";

	std::cout << text;

	// prep
	float positions[4 * 3] = { -1.0f, -1.0f, 0.0f,
							1.0f, -1.0f, 0.0f,
							-1.0f, 1.0f, 0.0f,
							1.0f, 1.0f, 0.0f };

	float textureCrd[4 * 2] = { 0.0f, 0.0f,
							1.0f, 0.0f,
							0.0f, 1.0f,
							1.0f, 1.0f };

	// initializing Vertex array
	Vertex vertices[4];
	for (unsigned int i = 0; i < 4; i++) {
		vertices[i] = Vertex{ positions[3 * i + 0], positions[3 * i + 1], positions[3 * i + 2], textureCrd[2 * i + 0], textureCrd[2 * i + 1] };
	}

	unsigned int indices[6] = { 0,1,2,
								1,2,3 };


	// initializing window
	int windowFlag;
	GLFWwindow* window = initializeWindow(&windowFlag, width, height);
	if (windowFlag != 0) {
		return -1;
	}

	// create shader
	
	Shader shader("C:\\Users\\mista\\Documents\\GitHub\\GridFluidSim3D\\mandelbrotx64\\shaders\\basic.vert", "C:\\Users\\mista\\Documents\\GitHub\\GridFluidSim3D\\mandelbrotx64\\shaders\\mandelbrot.frag");
	shader.use();

	// create vao
	VertexArrayObject vbuffer(vertices, 4);
	vbuffer.bind();

	// create element array buffer
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);



	int monitorCount = 2;
	GLFWmonitor** monitorHandles = glfwGetMonitors(&monitorCount);


	monitor = monitorHandles[0];
	mode = glfwGetVideoMode(monitor);


	// main loop
	while (!glfwWindowShouldClose(window))
	{	
		

		t = (float)glfwGetTime();
		processInput(window);

		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		glfwGetWindowSize(window, &width, &height);
		shader.setFloat("width", (float)width);
		shader.setFloat("height", (float)height);

		centerPos = centerPos + dr * 1.0f * dt;
		shader.setVec2("centerPos", centerPos);

		shader.setFloat("zoom", zoom);

		shader.setInt("maxit", maxit);

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		shader.setMat4("proj", proj);

		shader.setInt("colorPermute", colorPermute);
		shader.setBool("invertColor", invertColor);
		
		shader.setBool("julia", julia);
		shader.setBool("z2", z2);
		shader.setBool("z3", z3);
		shader.setBool("z4", z4);
		shader.setBool("z5", z5);
		shader.setBool("maxDistColor", maxDistColor);

		if (!justSaved)
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		else if (glfwGetTime() - last_change > 0.5) {
			justSaved = false;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		dt = (float)glfwGetTime() - t;
	}

	glfwTerminate();
	return 0;
}


void processInput(GLFWwindow* window)
{
	
	// close when pressing escape
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	// xy movement using WASD
	dr = dr * 0.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dr = dr + glm::vec2(0, 1);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dr = dr + glm::vec2(0, -1);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		dr = dr + glm::vec2(-1, 0);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		dr = dr + glm::vec2(1, 0);

	if (abs(dr.x) + abs(dr.y) > 0) {
		dr = normalize(dr) * zoom;
	}
	

	// zoom width up and down arrow
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		zoom = zoom * (1.0f - dt * 0.7f);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		zoom = zoom / (1.0f - dt * 0.7f);
	}


	// change maximum number of iterations using q and e
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.05f) {
			maxit++;
			last_change = (float)glfwGetTime();
		}
		
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.05f) {
			maxit--;
			last_change = (float)glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {

		zoom = 1.0f;
		centerPos = glm::vec2(0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 3.0f) {
			std::cout << "zoom: " << zoom << std::endl;
			std::cout << "centered at: " << centerPos.x << " + i" << centerPos.y << std::endl;
			last_change = (float)glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f && !fullScreen) {
			float newcenterx;
			float newcentery;
			float newzoom;
			bool inputError;
			try {
				std::cout << "\n Enter Re(c): " << std::endl;
				std::cin >> newcenterx;
				std::cout << "Enter Im(c): " << std::endl;
				std::cin >> newcentery;
				std::cout << "Enter zoom: " << std::endl;
				std::cin >> newzoom;

			}
			catch (int e) {
				std::cout << "Input error " <<e<< std::endl;
				inputError = true;
			}
			
			if (!inputError) {
				centerPos.x = newcenterx;
				centerPos.y = newcentery;
				zoom = newzoom;
			}
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			colorPermute++;
			colorPermute = colorPermute % 6;
			last_change = glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			invertColor = !invertColor;
			last_change = glfwGetTime();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			julia = !julia;
			last_change = glfwGetTime();
			std::cout << julia << z2 << z3 << z4 << z5 << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			z2 = !z2;
			last_change = glfwGetTime();
			std::cout << julia << z2 << z3 << z4 << z5 << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			z3 = !z3;
			last_change = glfwGetTime();
			std::cout << julia << z2 << z3 << z4 << z5 << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			z4 = !z4;
			last_change = glfwGetTime();
			std::cout << julia << z2 << z3 << z4 << z5 << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 0.2f) {
			z5 = !z5;
			last_change = glfwGetTime();
			std::cout << julia << z2 << z3 << z4 << z5 << std::endl;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 1.0f) {
			const char filename[]= "fractal.png";

			saveImage(filename, window);
			justSaved = true;
			last_change = glfwGetTime();
			std::cout << "Saved Image" << std::endl;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (glfwGetTime() - last_change > 5.0f) {
			if (fullScreen) {
				glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			else {
				glfwSetWindowMonitor(window, NULL, 100, 100, startwidth, startheight, mode->refreshRate);
			}
			fullScreen = !fullScreen;
		}
	}
}



void saveImage(const char* filepath, GLFWwindow* w) {
	// from : https://lencerf.github.io/post/2019-09-21-save-the-opengl-rendering-to-image-file/
	int width, height;
	glfwGetFramebufferSize(w, &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}