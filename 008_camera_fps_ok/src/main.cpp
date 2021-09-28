#include <GL/glew.h>
#include <SFML/Graphics.hpp>

#include "glm.hpp"

#include <iostream>
#include <cmath>

using namespace std;

#include "ShaderUtil.h"


const float vertices[] = {
	//front triangle 1
	-1,	1,	1,
	-1,	-1,	1,
	1,	1,	1,

	//front triangle 2
	-1,	-1,	1,
	1,	-1,	1,
	1,	1,	1,

	//back triangle 1
	-1,	1,	-1,
	1,	1,	-1,
	-1,	-1,	-1,

	//back triangle 2
	-1,	-1,	-1,
	1,	1,	-1,
	1,	-1,	-1,

	//top triangle 1
	-1,	1,	1,
	1,	1,	1,
	1,	1,	-1,

	//top triangle 2
	1,	1,	-1,
	-1,	1,	-1,
	-1,	1,	1,

	//bottom triangle 1
	-1,	-1,	1,
	1,	-1,	-1,
	1,	-1,	1,

	//bottom triangle 2
	1,	-1,	-1,
	-1,	-1,	1,
	-1,	-1,	-1,

	//right triangle 1
	1,	-1,	1,
	1,	-1,	-1,
	1,	1,	1,

	//right triangle 2
	1,	-1,	-1,
	1,	1,	-1,
	1,	1,	1,

	//left triangle 1
	-1,	-1,	1,
	-1,	1,	1,
	-1,	-1,	-1,

	//left triangle 2
	-1,	-1,	-1,
	-1,	1,	1,
	-1,	1,	-1
};

const float colors[] = {
	//front triangle 1
	0,	1,	1,
	0,	0,	1,
	1,	1,	1,

	//front triangle 2
	0,	0,	1,
	1,	0,	1,
	1,	1,	1,

	//back triangle 1
	0,	1,	0,
	1,	1,	0,
	0,	0,	0,

	//back triangle 2
	0,	0,	0,
	1,	1,	0,
	1,	0,	0,

	//top triangle 1
	0,	1,	1,
	1,	1,	1,
	1,	1,	0,

	//top triangle 2
	1,	1,	0,
	0,	1,	0,
	0,	1,	1,

	//bottom triangle 1
	0,	0,	1,
	1,	0,	0,
	1,	0,	1,

	//bottom triangle 2
	1,	0,	0,
	0,	0,	1,
	0,	0,	0,

	//right triangle 1
	1,	0,	1,
	1,	0,	0,
	1,	1,	1,

	//right triangle 2
	1,	0,	0,
	1,	1,	0,
	1,	1,	1,

	//left triangle 1
	0,	0,	1,
	0,	1,	1,
	0,	0,	0,

	//left triangle 2
	0,	0,	0,
	0,	1,	1,
	0,	1,	0
};


int main() {
	//Open SFML Window == Valid OpenGL Context
	sf::RenderWindow window(sf::VideoMode(800, 600), "Wawsom!", sf::Style::Default, sf::ContextSettings(24, 0, 0, 3, 3));
	window.setVerticalSyncEnabled(true);

	//Get some basic setting info through OpenGL
	sf::ContextSettings settings = window.getSettings();
	cout << settings.majorVersion << "." << settings.minorVersion << endl;

	sf::Vector2i mousePos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
	float sensitivity = 0.01f;

	//initialize glew to load all available opengl functions
	cout << "Initializing GlEW..." << endl;
	bool glewInitResult = (glewInit() == GLEW_OK);
	cout << "GlEW Initialized:" << glewInitResult << endl;

	//create shader program (see shaderutil)
	GLuint programID = ShaderUtil::createProgam("vertexshader.vert", "fragmentshader.frag");
	glUseProgram(programID);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	GLuint vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint colorBufferId;
	glGenBuffers(1, &colorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLint vertexIndex = glGetAttribLocation(programID, "vertex");
	GLint colorIndex = glGetAttribLocation(programID, "color");

	glm::mat4 cameraMatrix(glm::translate(glm::vec3(6.0f, 2.0f, 6.0f)));
	glm::vec3 cameraPosition = glm::vec3(cameraMatrix[3]);
	cameraMatrix = glm::translate(cameraPosition) * glm::rotate(glm::radians(45.0f), glm::vec3(0, 1, 0)) * glm::translate(-cameraPosition) * cameraMatrix;

	float fov = 60.0f;
	float near = 0.1f;
	float far = 1000.0f;
	float h = tanf(glm::radians(fov / 2.0f)) * near;
	float w = h * (4.0f / 3.0f);

	float diffLength;
	sf::Vector2i prevMousePos;
	sf::Vector2i diff;
	sf::Vector2f diffNormalized;

	while (window.isOpen()) {

		mousePos = sf::Mouse::getPosition(window);

		diff = sf::Vector2i(mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y);
		diffLength = sqrt(diff.x * diff.x + diff.y * diff.y);
		if (diffLength == 0) {
			diffLength = 1.0f;
		}

		diffNormalized = sf::Vector2f(diff.x / diffLength, diff.y / diffLength);

		//translate camera
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			//notice the -!
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(0, 0, -0.1f));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(0, 0, 0.1f));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(-0.1f, 0, 0));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(0.1f, 0, 0));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(0, 0.1f, 0));
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
			cameraMatrix = cameraMatrix * glm::translate(glm::vec3(0, -0.1f, 0));
		}

		//rotate camera
		cameraPosition = glm::vec3(cameraMatrix[3]);


		/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (diffNormalized.x != 0.0f) {
				cameraMatrix = glm::rotate(-diffNormalized.x * sensitivity, glm::vec3(0, 1, 0)) * cameraMatrix;
			}
			if (diffNormalized.y != 0.0f) {
				cameraMatrix = glm::rotate(-diffNormalized.y * sensitivity, glm::vec3(1, 0, 0)) * cameraMatrix;
			}
		}*/
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			//one way to rotate around the camera world y axis
			cameraMatrix = glm::translate(cameraPosition) * glm::rotate(0.01f, glm::vec3(0, 1, 0)) * glm::translate(-cameraPosition) * cameraMatrix;
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			//another way to rotate around the center world y axis
			cameraMatrix[3] = glm::vec4(0, 0, 0, 1);
			cameraMatrix = glm::rotate(-0.01f, glm::vec3(0, 1, 0)) * cameraMatrix;
			cameraMatrix[3] = glm::vec4(cameraPosition, 1);
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			//go up
			cameraMatrix = cameraMatrix * glm::rotate(0.01f, glm::vec3(1, 0, 0));
			//cameraMatrix = glm::translate(cameraPosition) * glm::rotate(0.01f, glm::vec3(1, 0, 0)) * glm::translate(-cameraPosition) * cameraMatrix;
		} if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			//go down using the old way, same as post multiplying this rotation matrix
			cameraMatrix = glm::rotate(cameraMatrix, -0.01f, glm::vec3(1, 0, 0));
			//cameraMatrix = glm::translate(cameraPosition) * glm::rotate(-0.01f, glm::vec3(1, 0, 0)) * glm::translate(-cameraPosition) * cameraMatrix;
		}

		//(re)constructing an oriented positioned camera matrix out of a position and a quaternion is also possible
		//but we'll get there in another lecture

		glm::mat4 viewMatrix = glm::inverse(cameraMatrix);

		sf::Vector2u windowSize = window.getSize();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov), windowSize.x / (float)windowSize.y, 0.1f, 1000.0f);
		glm::mat4 projMatrix = glm::frustum(-w, w, -h, h, near, far);
		glm::mat4 rotMatrix = glm::rotate(glm::radians(30.0f), glm::vec3(0, 1, 0));
		glm::mat4 minusRotMatrix = glm::inverse(rotMatrix);
		//glm::mat4 twoPointProjMatrix = glm::twoPointPerspectiveRH(-w, w, -h, h, near, far);
		glm::mat4 twoPointProjMatrix = minusRotMatrix * projMatrix * rotMatrix;
		glm::mat4 threePointProjMatrix = glm::threePointPerspectiveRH(-w, w, -h, h, near, far);

		glm::mat4 projMatrixPartOne = minusRotMatrix * projMatrix;

		glm::mat4 vp = twoPointProjMatrix * viewMatrix;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnableVertexAttribArray(vertexIndex);
		glEnableVertexAttribArray(colorIndex);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferId);
		glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glm::mat4 modelMatrix1(glm::scale(glm::vec3(3, 0.01f, 3)));
		//glUniformMatrix4fv(glGetUniformLocation(programID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(vp * modelMatrix1));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projMatrixPartOne"), 1, GL_FALSE, glm::value_ptr(projMatrixPartOne));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projMatrixPartTwo"), 1, GL_FALSE, glm::value_ptr(rotMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(twoPointProjMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix1));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelMatrix2(glm::translate(glm::vec3(0, 1, 0)));
		//glUniformMatrix4fv(glGetUniformLocation(programID, "mvpMatrix"), 1, GL_FALSE, glm::value_ptr(vp * modelMatrix2));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projMatrixPartOne"), 1, GL_FALSE, glm::value_ptr(projMatrixPartOne));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projMatrixPartTwo"), 1, GL_FALSE, glm::value_ptr(rotMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(twoPointProjMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(modelMatrix2));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableVertexAttribArray(vertexIndex);
		glDisableVertexAttribArray(colorIndex);

		//display it
		window.display();

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized) {
				glViewport(0, 0, event.size.width, event.size.height);
				mousePos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
			}
			if (event.type == sf::Event::MouseWheelMoved) {
				if (event.mouseWheel.delta < 0) {
					fov++;
				}
				else if (event.mouseWheel.delta > 0) {
					fov--;
				}
			}
			if (event.type == sf::Event::MouseMoved) {
			}
		}

		prevMousePos = mousePos;
	}

	return 0;
}
