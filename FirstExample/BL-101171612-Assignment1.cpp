//***************************************************************************
// BL-101171612-Assignment1.cpp by Liam Blake (C) 2019 All Rights Reserved.
//
// Assignment 1 submission.
//
// Description:
// A demonstration of how squares and scaling works in Open GL 
//
//*****************************************************************************
using namespace std;
#include <iostream>
#include "stdlib.h"
#include "time.h"
#include "vgl.h"
#include "LoadShaders.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <ctime>
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
float rotation = 45;
int numSquares;
GLuint Buffers[NumBuffers];
#define X_AXIS glm::vec3(1, 0, 0)
#define Y_AXIS glm::vec3(0, 1, 0)
#define Z_AXIS glm::vec3(0, 0, 1)
#define M_PI 3.1415926535897932384626433832795028841971

const GLuint NumVertices = 5;
const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2];

GLuint gVAO;
GLuint MatrixID;
glm::mat4 MVP;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 Projection;
GLuint colours_vbo;
float colors[] =
{
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	/*0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f*/
};
float points[] = {
	-0.5f, 0.5f, 0.0f, // 0
	0.5f, 0.5f, 0.0f,  // 1
	0.5f, -0.5f, 0.0f, // 2
	-0.5f, -0.5f, 0.0f // 3
};
/*
-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
*/
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation)
{
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));

	MVP = Projection * View * Model;
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	// GLuint stands for unsigned integer
}
GLuint points_vbo = 0;
int index = 0;
GLuint colors_vbo = 0;
#define MAX 100
float colors2[MAX][12];
void init(void)
{
	srand((unsigned)time(NULL));
	//Specifying the name of vertex and fragment shaders.
	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
		{ GL_NONE, NULL }
	};


	//Loading and compiling shaders
	GLuint program = LoadShaders(shaders);
	glUseProgram(program);	//My Pipeline is set up
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);
	MatrixID = glGetUniformLocation(program, "MVP");

	Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f); // generates the 3d box in slide 40 week 2
	// camera

	View = glm::lookAt(
		glm::vec3(0, 0, 1.0f), // camera is at (4, 3, 3) in world space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1.0f, 0) // head is up (set to 0, -1, 0 to look upside down)
	);



	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &colors_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	// colors2[20][12];
	for (int i = 0; i < MAX; i++)
	{
		/*float randomVertexColor[3] = {
			(float)(rand() % 255) / (float)(255),
			(float)(rand() % 255) / (float)(255),
			(float)(rand() % 255) / (float)(255)
		};
		*/
		colors2[i][0] = 1.0f;
		colors2[i][1] = 1.0f;
		colors2[i][2] = 1.0f;
		colors2[i][3] = 1.0f;
		colors2[i][4] = 1.0f;
		colors2[i][5] = 1.0f;
		colors2[i][6] = 1.0f;
		colors2[i][7] = 1.0f;
		colors2[i][8] = 1.0f;
		colors2[i][9] = 1.0f;
		colors2[i][10] = 1.0f;
		colors2[i][11] = 1.0f;

	}
	for (int i = 0; i < numSquares; i++)
	{
		float randomVertexColor[3] = {
			(float)(rand() % 255) / (float)(255),
			(float)(rand() % 255) / (float)(255),
			(float)(rand() % 255) / (float)(255)
		};
		for (int j = 0; j < 12; j += 3)
		{
			colors2[i][j] = randomVertexColor[0];
			colors2[i][j + 1] = randomVertexColor[1];
			colors2[i][j + 2] = randomVertexColor[2];
		}
	}

}


//---------------------------------------------------------------------
//
// display
//

void
display(void)
{
	/*for (int i = 0; i < 9; i++)
	{
		colors[i] = (float)(rand() % 200 + 1) / 100.0f - 1;
	}*/
	static int flag = 1;
	float scale = 19;
	// 70%
	glClear(GL_COLOR_BUFFER_BIT);
	// 0.25, 0, 0, 0.1
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//Selecting the buffer
	//glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	/*scale = 19;
	transformObject(scale, Z_AXIS, 0, glm::vec3(0, 0, 0));
	glDrawArrays(GL_LINE_LOOP, 0, 4);*/
	// Scale
	index = 0;
	float var;
	float opposite, adjacent;
	float rotationAxis = 0;
	int counter = 0;
	rotationAxis += rotation;
	var = tan(rotation * M_PI / 180.0f) + 1;
	opposite = (var - 1.0) / var;
	adjacent = 1.0 / var;
	scale *= sqrtf(pow(opposite, 2) + pow(adjacent, 2));
	while (counter <= numSquares)
	{
		counter++;
		float colorScale[] = {
		colors2[index][0], colors2[index][1], colors2[index][2],
		colors2[index][3],colors2[index][4], colors2[index][5],
		colors2[index][6],colors2[index][7], colors2[index][8],
		colors2[index][9],colors2[index][10], colors2[index][11]

		};
		index++;
		if (index == 19)
			index = 0;

		glBufferData(GL_ARRAY_BUFFER, sizeof(colorScale), colorScale, GL_STATIC_DRAW);
		rotationAxis += rotation;
		transformObject(scale, Z_AXIS, rotationAxis, glm::vec3(0, 0, 0));
		glDrawArrays(GL_LINE_LOOP, 0, 4);
		/*===========================| Rotated Vertice |===========================*/


		var = tan(rotation * M_PI / 180.0f) + 1;
		opposite = (var - 1.0) / var;
		adjacent = 1.0 / var;
		scale *= sqrtf(pow(opposite, 2) + pow(adjacent, 2));

		//float colorScale1[] = {
		//colors2[index][0], colors2[index][1], colors2[index][2],
		//colors2[index][3],colors2[index][4], colors2[index][5],
		//colors2[index][6],colors2[index][7], colors2[index][8],
		//colors2[index][9],colors2[index][10], colors2[index][11]

		//};
		//index++;
		//if (index == 19)
		//	index = 0;
		////scale = (scale / 2) / sin(rotation * M_PI / 180.0f);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(colorScale1), colorScale1, GL_STATIC_DRAW);
		//rotationAxis += rotation;

		//transformObject(scale, Z_AXIS, rotationAxis, glm::vec3(0, 0, 0));
		//glDrawArrays(GL_LINE_LOOP, 0, 4);

		//var = tan(rotation * M_PI / 180.0f) + 1;
		//opposite = (var - 1.0) / var;
		//adjacent = 1.0 / var;
		//scale *= sqrtf(pow(opposite, 2) + pow(adjacent, 2));

	}

	glFlush();
}

void idle()
{
	glutPostRedisplay();
}

//---------------------------------------------------------------------
//
// main
//

int
main(int argc, char** argv)
{
	system("cls");
	while (1)
	{
		cout << "Enter the number of squares (Betweeen 1 and 30): ";
		cin >> numSquares;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
		if (numSquares > 0 && numSquares < 30)
			break;
	}

	while (1)
	{
		cout << "\nEnter the rotation of the squares (Between 1 and 90): ";
		cin >> rotation;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore();
		}
		if (rotation > 0 && rotation < 90)
			break;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
	glutCreateWindow("Blake, Liam, 101171612");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init();

	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutMainLoop();



}