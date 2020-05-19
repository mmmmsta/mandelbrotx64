#pragma comment(lib, "libs/glfw3.lib")

#include "glad/glad.h"
#include "KHR/khrplatform.h"
#include "GLFW/glfw3.h"
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"


struct Vertex {
	float x, y, z;
	float u, v;
};



class VertexArrayObject {
	

public:

	GLuint VAO;
	GLuint VBO;

	VertexArrayObject(void* vertices, unsigned int numVertices) {
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO); // vao saves buffer and layout of data

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);



		//position
		// 2: 3 coordinates; 3: type; 4: normalization; 5: stride; 6: offset in Vertex struct
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, x));
		glEnableVertexAttribArray(0);

		//color

		//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, r));
		//glEnableVertexAttribArray(1);


		//normals
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, nx));
		//glEnableVertexAttribArray(2);

		//texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(struct Vertex, u));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

	}

	virtual ~VertexArrayObject() {
		// i dont know if this is correct
		glDeleteVertexArrays(1, &VAO);

	}

	void bind() {
		glBindVertexArray(VAO);
	}

	void unbind() {
		glBindVertexArray(0);
	}


	
	
};

