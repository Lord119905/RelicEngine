//Using SDL and standard IO 
#include <glad\glad.h>
#include <SDL2\SDL.h>
#include <iostream>

const char *vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char *fragmentShaderSource = 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main(int argc, char *args[])
{
	/////////////////////////////////////////////////////////////////////////////////////
	// Initialize SDL and Window with OpenGL Context
	/////////////////////////////////////////////////////////////////////////////////////
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "ERROR: SDL: Failed to initialize!\n"
				  << SDL_GetError() << std::endl;
		system("PAUSE");
		return -1;
	}

	// Set OpenGL Context Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Create OpenGL Window
	SDL_Window *window = SDL_CreateWindow
						 ("Relic-Engine", 
						 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
						 800, 800, 
						 SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		std::cout << "ERROR: SDL: Failed to create a new window!\n"
				  << SDL_GetError() << std::endl;
		return -1;
	}

	// Create OpenGL Context
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/////////////////////////////////////////////////////////////////////////////////////
	// Inizialize GLAD
	/////////////////////////////////////////////////////////////////////////////////////
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "ERROR: GLAD: Failed to initialize! \n";
		return -1;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Compile Shaders
	/////////////////////////////////////////////////////////////////////////////////////
	// Vertex Shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR: OpenGL: Shader: Compilation of Vertex Shader failed!\n"
				  << infoLog << std::endl;
	}

	// Fragment Shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR: OpenGL: Shader: Compilation of Fragment Shader failed!\n"
				  << infoLog << std::endl;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// Link Shaders
	/////////////////////////////////////////////////////////////////////////////////////
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR: OpenGL: Shader: Linking of Shader Program failed!\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/////////////////////////////////////////////////////////////////////////////////////
	// Set Up Vertex Data (And Buffer(s)), Configure Vertex Attributes
	/////////////////////////////////////////////////////////////////////////////////////
	float vertices[] = {
		-0.5f,  -0.5f, 0.0f,
		 0.0f,  -0.5f, 0.0f,
		-0.25f,  0.0f, 0.0f,
		 0.5f,  -0.5f, 0.0f,
		 0.25f,  0.0f, 0.0f,
		 0.0f,   0.5f, 0.0f
	};
	unsigned int indices[] = {
		0, 2, 1,
		1, 4, 3,
		2, 5, 4
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object 
	glBindVertexArray(VAO);

	// Then bind and set vertex buffer(s)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Then configure vertex attributes(s)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind the Vertex Array Buffer, it's not needed anymore
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the Vertex Array Object, so it's not incidently modified
	glBindVertexArray(0);

	/////////////////////////////////////////////////////////////////////////////////////
	// Game Loop
	/////////////////////////////////////////////////////////////////////////////////////
	bool bClose = false;
	while (!bClose)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EventType::SDL_QUIT:
				bClose = true;
				break;
			case SDL_EventType::SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					bClose = true;
				}
				break;
			default:
				break;
			}
		}

		// Set the Background Color to a cyan-ish color
		glClearColor(.4f, .8f, .8f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Triforce!
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}

	// Quit SDL 
	SDL_Quit();
	return 0;
}