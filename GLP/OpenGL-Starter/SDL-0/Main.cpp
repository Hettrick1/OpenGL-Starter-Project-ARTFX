#include <iostream>
#include <SDL.h>
#include <glew.h>

#include <string>
#include <fstream>
#include <deque>

using namespace std;
//#define GLEW_STATIC

float vertices[] = {
	//position				//color
	 0.05f,  0.05f, 0.0f,		1.0f, 0.0f, 0.0f,//top right
	 0.05f, -0.05f, 0.0f,		1.0f, 0.0f, 0.0f,//bottom right
	-0.05f, -0.05f, 0.0f,		1.0f, 0.0f, 0.0f,//bottom left
	-0.05f,  0.05f, 0.0f,		1.0f, 0.0f, 0.0f,// top right
};
unsigned int indices[] = {
	0, 1, 3, //first triangle
	1, 2, 3  //second triangle
};

std::deque<float> snakeTailX;
std::deque<float> snakeTailY;
float speedX = 0.5f, speedY = 0.0f, offsetX = 1.0f, offsetY = 0.0f;

GLuint VAO, VBO, EBO;
unsigned int vertexShader, fragmentShader;
unsigned int shaderProgram;

string LoadShader(string fileName);
void InitializeGame();

float positionX, positionY;

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

	InitializeGame();

	//load shader files
	string vertexFile1 = LoadShader("SimpleVertexShader2.vert").c_str();
	const char* vertexShaderSource1 = vertexFile1.c_str();

	string fragmentFile1 = LoadShader("SimpleFragmentShader2.frag");
	const char* fragmentShaderSource1 = fragmentFile1.c_str();

	//gen buffers
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VAO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//vertex shader1 
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource1, NULL);
	glCompileShader(vertexShader);


	//fragment shader1
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader);


	//shaderpProgram1
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);


	//unload buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	double deltaTime = 0;
	float oldTick = SDL_GetTicks(), currentTick = 0;

	float x = 0, y = 0;

	bool isRunning = true;
	while (isRunning) {
		currentTick = SDL_GetTicks();
		deltaTime = (currentTick - oldTick)/1000;
		x += speedX * deltaTime;
		y += speedY * deltaTime;

		if (x > 0.09f || x < -0.09f) {
			positionX += 0.1 *offsetX;

			snakeTailY.pop_back();
			snakeTailX.pop_back();
			snakeTailX.push_front(positionX);
			snakeTailY.push_front(positionY);
			x = 0;
		}
		else if (y > 0.09f || y < -0.09f) {
			positionY += 0.1 * offsetY;

			snakeTailX.pop_back();
			snakeTailY.pop_back();
			snakeTailX.push_front(positionX);
			snakeTailY.push_front(positionY);
			y = 0;
		}


		if (snakeTailX[0] + 0.05f > 1 || snakeTailX[0] - 0.05f < -1) {
			InitializeGame();
		}
		if (snakeTailY[0] + 0.05f > 1 || snakeTailY[0] - 0.05f < -1) {
			InitializeGame();
		}

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
				if (event.key.keysym.sym == SDLK_RIGHT) {
					if (speedX == 0) {
						speedX = 0.5f;
						speedY = 0.0f;
						offsetX = 1.0f;
						offsetY = 0.0f;
					}
				}
				if (event.key.keysym.sym == SDLK_LEFT) {
					if (speedX == 0) {
						speedX = -0.5f;
						speedY = 0.0f;
						offsetX = -1.0f;
						offsetY = 0.0f;
					}
				}
				if (event.key.keysym.sym == SDLK_UP) {
					if (speedY == 0) {
						speedX = 0.0f;
						speedY = 0.5f;
						offsetX = 0.0f;
						offsetY = 1.0f;
					}
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					if (speedY == 0) {
						speedX = 0.0f;
						speedY = -0.5f;
						offsetX = 0.0f;
						offsetY = -1.0f;
					}
				}
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		// Draw function

		for (int i = 0; i < snakeTailX.size(); i++) {
			glUseProgram(shaderProgram);
			int vertexUniform = glGetUniformLocation(shaderProgram, "offset");
			glUniform3f(vertexUniform, floor(snakeTailX[i] * 10)/10, floor(snakeTailY[i] * 10)/10, 0.0f);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

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

void InitializeGame() {
	positionX = 0;
	positionY = 0;
	snakeTailX.clear();
	snakeTailY.clear();
	speedX = 0.5f;
	speedY = 0.0f;
	offsetX = 1.0f;
	offsetY = 0.0f;
	for (int i = 0; i < 16; i++) {
		float o = -i * 0.1;
		snakeTailX.push_back(o);
		snakeTailY.push_back(0);
	}
}

