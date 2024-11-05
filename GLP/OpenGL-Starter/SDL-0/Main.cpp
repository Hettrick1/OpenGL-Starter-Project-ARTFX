#include <iostream>
#include <SDL.h>
#include <glew.h>

#include <string>
#include <fstream>

using namespace std;
//#define GLEW_STATIC

float vertices[] = {
	// positions             // colors
	 -0.05f,  0.2f, 0.0f,	1.0f, 0.0f, 0.0f, //A
	  0.05f,  0.2f, 0.0f,	0.0f, 1.0f, 0.0f, //E
	  0.0f,   0.1f, 0.0f,	0.0f, 0.0f, 1.0f, //B
	  0.15f,  0.0f, 0.0f,	1.0f, 0.0f, 0.0f, //I
	  0.05f,  0.0f, 0.0f,	0.0f, 1.0f, 0.0f, //D
	  0.05f, -0.2f, 0.0f,   0.0f, 0.0f, 1.0f, //H
	  0.0f,  -0.1f, 0.0f,   1.0f, 0.0f, 0.0f, //F
	 -0.05f, -0.2f, 0.0f,   0.0f, 1.0f, 0.0f, //G
	 -0.05f,  0.0f, 0.0f,	0.0f, 0.0f, 1.0f, //C
	 -0.15f,  0.0f, 0.0f,	1.0f, 0.0f, 0.0f, //K
	  0.0f,   0.1f, 0.0f,	0.0f, 1.0f, 0.0f, //B
	 -0.05f,  0.2f, 0.0f,	0.0f, 0.0f, 1.0f, //A
};

float maxX = 0.15;
float maxY = 0.2;

GLuint VAO[2], VBO[2];
unsigned int vertexShader[2], fragmentShader[2];
unsigned int shaderProgram[2];

string LoadShader(string fileName);

float speedX = 0.5f;
float speedY = 0.5f;
float newPosY, newPosX;

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}

	///////////SETTING UP SDL/////////////
	//Create a simple window

	int width = 800;
	int height = 700;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);

	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew

	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	//load shader files
	string vertexFile1 = LoadShader("SimpleVertexShader.vert").c_str();
	const char* vertexShaderSource1 = vertexFile1.c_str();

	string fragmentFile1 = LoadShader("SimpleFragmentShader.frag");
	const char* fragmentShaderSource1 = fragmentFile1.c_str();

	string vertexFile2 = LoadShader("SimpleVertexShader2.vert").c_str();
	const char* vertexShaderSource2 = vertexFile2.c_str();

	string fragmentFile2 = LoadShader("SimpleFragmentShader2.frag");
	const char* fragmentShaderSource2 = fragmentFile2.c_str();

	//gen buffers
	glGenBuffers(2, VBO);
	glGenBuffers(2, VAO);

	glBindVertexArray(VAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(VAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
	glEnableVertexAttribArray(1); 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//vertex shader1 
	vertexShader[0] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[0], 1, &vertexShaderSource1, NULL);
	glCompileShader(vertexShader[0]);


	//fragment shader1
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[0], 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader[0]);

	//vertex shader2
	vertexShader[1] = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader[1], 1, &vertexShaderSource2, NULL);
	glCompileShader(vertexShader[1]);


	//fragment shader2 
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader[1], 1, &fragmentShaderSource2, NULL);
	glCompileShader(fragmentShader[1]);

	//shaderpProgram1
	shaderProgram[0] = glCreateProgram();
	glAttachShader(shaderProgram[0], vertexShader[0]);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	glUseProgram(shaderProgram[0]);

	//shaderpProgram2
	shaderProgram[1] = glCreateProgram();
	glAttachShader(shaderProgram[1], vertexShader[1]);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);

	glUseProgram(shaderProgram[1]);

	//unload buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double deltaTime = 0;
	float oldTick = SDL_GetTicks(), currentTick = 0;

	bool isRunning = true;
	while (isRunning) {
		currentTick = SDL_GetTicks();
		deltaTime = (currentTick - oldTick)/1000;

		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		// Draw function

		newPosX += (speedX * deltaTime);
		newPosY += (speedY * deltaTime);
		if (newPosX - maxX <= -1 || newPosX + maxX >= 1) {
			speedX *= -1;
		}
		if (newPosY - maxY <= -1 || newPosY + maxY >= 1) {
			speedY *= -1;
		}
		glUseProgram(shaderProgram[0]);
		int vertexUniform = glGetUniformLocation(shaderProgram[0], "offset");
		glUniform3f(vertexUniform, newPosX, newPosY, 0.0f);

		glUseProgram(shaderProgram[1]);
		int vertexUniform2 = glGetUniformLocation(shaderProgram[1], "offset");
		glUniform3f(vertexUniform2, -newPosX, -newPosY, 0.0f);

		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

		oldTick = currentTick;
		SDL_GL_SwapWindow(Window); // Swapbuffer
	}
	// Quit
	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);

	return 0;
}

string LoadShader(string fileName) {
	ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
		cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}

