#include <iostream>
#include <SDL.h>
#include <glew.h>

#include <string>
#include <fstream>

using namespace std;
//#define GLEW_STATIC

float vertices[] = {
	// positions             // colors
	 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f

};

GLuint VAO, VBO;
unsigned int vertexShader, fragmentShader;
unsigned int shaderProgram;

string LoadShader(string fileName);

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
	int width = 400;
	int height = 350;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);
	//SDL_WINDOW_OPENGL is a u32 flag !


	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	/////////SETTING UP OPENGL WITH GLEW///
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	string vertexFile = LoadShader("SimpleVertexShader.vert").c_str();
	const char* vertexShaderSource = vertexFile.c_str();

	string fragmentFile = LoadShader("SimpleFragmentShader.frag");
	const char* fragmentShaderSource = fragmentFile.c_str();

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VAO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//vertex shader 
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	//fragment shader 
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//shaderpProgram
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set the viewing frame through which we will see the objects
	glViewport(0, 0, width, height);
	//Put the color you want here for the background
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	bool isRunning = true;
	while (isRunning) {
		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		// Draw function
		glUseProgram(shaderProgram);

		float timeValue = (float)SDL_GetTicks() / 1000;
		float newPosX = (sin(timeValue) / 2.0f);
		int vertexUniform = glGetUniformLocation(shaderProgram, "offset");
		glUseProgram(shaderProgram);
		glUniform3f(vertexUniform, newPosX, 0.0f, 0.0f);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);


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

