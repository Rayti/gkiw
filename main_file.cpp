/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include "wtypes.h"
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"
#include "objloader.hpp"
#include "Camera.h"
#include "Mouse.h"
#include "mapGenerator.h"
#include "BallStats.hpp"


float aspectRatio=1;
float deltaTime = 0.0f;

const int mapSize = 60;
float map_translate_y = -3.0f;
float map_flatness = 0.05;


//MK
int window_x = 1000;
int window_y = 1000;
//.

ShaderProgram *sp;
Camera camera = Camera();
Mouse mouse = Mouse();
mapGenerator map = mapGenerator(map_flatness, mapSize);

GLuint tex0; //grass
GLuint tex1; //grassSpecularMap
GLuint tex2; //metal
GLuint tex3; //lightSource

const int balls_amount = 4;
std::vector <glm::vec3> initial_positions;
std::vector <BallStats> balls;

//Map
float* mapVertices = map.getMapVertices();
float* mapNormals = map.getMapNormals();
float* mapColors = map.getMapColors();
float* mapTexCoords = map.getMapTexCoords();
int mapVertexCount = map.getMapVertexCount();
float** mapHeight = map.getMapHeights();

//Lamp
float* lampVertices;
float* lampNormals;
float* lampColors;
float* lampTexCoords;
int lampVertexCount;

//Light
float* lightVertices;
float* lightNormals;
float* lightColors;
float* lightTexCoords;
int lightVertexCount;

float* vertices;
float* normals;
float* texCoords;
float* colors;
int vertexCount = loadOBJ("models\\sphere12.obj",
	vertices, texCoords, normals, colors);

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
		//MK
		if (key == GLFW_KEY_ESCAPE) exit(EXIT_FAILURE);
		//.

		//SZJ
		if (key == GLFW_KEY_A) camera.keyboardInput(LEFT, deltaTime);
		if (key == GLFW_KEY_D) camera.keyboardInput(RIGHT, deltaTime);
		if (key == GLFW_KEY_W) camera.keyboardInput(FORWARD, deltaTime);
		if (key == GLFW_KEY_S) camera.keyboardInput(BACKWARD, deltaTime);
    }
    if (action==GLFW_RELEASE) {
		if (key == GLFW_KEY_A) camera.setMovingLeft();
		if (key == GLFW_KEY_D) camera.setMovingRight();
		if (key == GLFW_KEY_W) camera.setMovingForward();
		if (key == GLFW_KEY_S) camera.setMovingBackward();
    }
}

//SZJ
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.mouseInput(mouse.getXdiff(xpos), mouse.getYdiff(ypos));
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

//SZJ
GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image; //Alokuj wektor do wczytania obrazka
	unsigned width, height; //zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//import do pamieci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci karty graficznej skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return tex;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	//MK
	//initial_positions.push_back(glm::vec3(4.35f, 5.5f, 4.2f));
	//initial_positions.push_back(glm::vec3(3.35f, 5.5f, -4.6f));
	//initial_positions.push_back(glm::vec3(-3.35f, 5.5f, 3.6f));
	//initial_positions.push_back(glm::vec3(-4.35f, 5.5f, -3.6f));
	//
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);
	//SZJ
	glfwSetCursorPosCallback(window, mouse_callback);
	lampVertexCount = loadOBJ("models\\lamp.obj",
		lampVertices, lampTexCoords, lampNormals, lampColors);
	lightVertexCount = loadOBJ("models\\light.obj",
		lightVertices, lightTexCoords, lightNormals, lightColors);

	tex0 = readTexture("grass.png");
	tex1 = readTexture("SpecularMap.png");
	tex2 = readTexture("metal.png");
	tex3 = readTexture("lightSource.png");

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
	for (int i = 0; i < balls_amount; i++) {
		balls.push_back(BallStats(initial_positions[i]));
	}
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}


void loadModel(glm::mat4 M, float* verticesLocal, float* colorsLocal, float* normalsLocal, float* texCoordsLocal,
	int vertexCountLocal, GLuint texNR, bool isMap) {
	//glm::mat4 M = glm::mat4(1.0f);
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verticesLocal); //Wskaż tablicę z danymi dla atrybutu vertex

	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsLocal); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normalsLocal); //Wskaż tablicę z danymi dla atrybutu normal

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoordsLocal);

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texNR);

	if (isMap) {
		glUniform1i(sp->u("textureMap1"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
	}

	glDrawArrays(GL_TRIANGLES, 0, vertexCountLocal); //Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu normal
	glDisableVertexAttribArray(sp->a("texCoord0"));
}

float speed_y = 2.0f;
float time_meter = 0.0f;
float min_temp[4] = { 50.0f, 50.0f, 50.0f, 50.0f };
float temp[4];
bool time_flag = false;
glm::vec3 more_temp_vertex;

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float deltaTime) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
         glm::vec3(0, 0, -2.5),
         glm::vec3(0,0,0),
         glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku



    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania

    //glm::mat4 M=glm::mat4(1.0f);
	//M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	//M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu
	//M = glm::translate(M, glm::vec3(0.0f, 1.0f, -2.5f));

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(camera.getViewMatrix(deltaTime)));
	
	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 M_map = glm::translate(M, glm::vec3(0.0f, map_translate_y, 0.0f));
	loadModel(M_map, mapVertices, mapColors, mapNormals, mapTexCoords, mapVertexCount, tex0, true);

	float lampScale = 3.0f;

	glm::mat4 lampM = glm::scale(glm::translate(M, glm::vec3(mapSize / 2 - 5, 7, mapSize / 2 - 6)), glm::vec3(lampScale, lampScale, lampScale));
	loadModel(lampM, lampVertices, lampColors, lampNormals, lampTexCoords, lampVertexCount, tex2, false);
	loadModel(lampM, lightVertices, lightColors, lightNormals, lightTexCoords, lightVertexCount, tex3, false);


	//glm::mat4 M = glm::mat4(1.0f);
	//loadModel(M, vertices, colors, normals, texCoords, vertexCount);

	time_meter += deltaTime;
	if (time_meter >= 0.5f) {
		time_flag = true;
		time_meter = 0.0f;
	}
	for (int i = 0; i < balls.size(); i++) {
		glm::mat4 Mloop = glm::mat4(1.0f);
		//for (int j = 0; j < balls.size(); j++) {
		//	if (glm::distance(balls[i].position, balls[j].position) <= balls[i].radius + balls[j].radius
		//		&& i != j ) {
		//		balls[i].update_positon_ball_collision(deltaTime, balls[j].position, balls[j].direction);
		//		balls[j].update_positon_ball_collision(deltaTime, balls[i].position, balls[i].direction);
		//		break;
		//	}
		//}
		bool touch_map_flag = false;
		for (int j = 0; j < mapVertexCount*4; j+=4) {
			glm::vec3 tempVertex = glm::vec3(mapVertices[j], mapVertices[j + 1] + map_translate_y, mapVertices[j + 2]);
			//printf("%f -- %f / %f / %f\n", glm::distance(balls[i].position, tempVertex), tempVertex.x, tempVertex.y, tempVertex.z);
			//printf("%f / %f / %f\n", mapNormals[j], mapNormals[j + 1], mapNormals[j + 2], mapNormals[j + 3]);
			if (glm::distance(balls[i].position, tempVertex)
				<= balls[i].radius) {
				balls[i].update_positon_map_touch(deltaTime, tempVertex, map_flatness, map_translate_y);
				touch_map_flag = true;
				//printf("Should've bounce!\n");
			}
			temp[i] = glm::distance(balls[i].position, tempVertex);
			if (temp[i] < min_temp[i]) {
				min_temp[i] = temp[i];
				more_temp_vertex = tempVertex;
			}
			
		}
		if(!touch_map_flag) balls[i].update_position(deltaTime);
		Mloop = glm::translate(Mloop, balls[i].position);
		loadModel(Mloop, vertices, colors, normals, texCoords, vertexCount, tex2, false);
		if (time_flag == true) {
			//printf("ball nr: %d; dist: %f; radius: %f\n",i, min_temp[i], balls[i].radius);
			//printf("v: %f / %f / %f\n", more_temp_vertex.x, more_temp_vertex.y, more_temp_vertex.z);
			//printf("b: %f / %f / %f\n", balls[i].position.x, balls[i].position.y, balls[i].position.z);
		}
	}
	time_flag = false;

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	float temp_input_x, temp_input_y, temp_input_z;
	for (int i = 0; i < 4; i++) {
		printf("Podaj wspolrzedne do upuszczenia kulki\nx: ");
		std::cin >> temp_input_x;
		printf("\ny: ");
		std::cin >> temp_input_y;
		printf("\nz: ");
		std::cin >> temp_input_z;
		printf("\n");
		initial_positions.push_back(glm::vec3(temp_input_x, temp_input_y, temp_input_z));
		initial_positions[i].x += 0.34f;
		initial_positions[i].y += 0.34f;
		initial_positions[i].z += 0.34f;
	}
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(window_x, window_y, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla

	glfwSetTime(0); //Zeruj timer


	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		deltaTime = glfwGetTime();
		glfwSetTime(0); //Zeruj timer
		drawScene(window, deltaTime); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
