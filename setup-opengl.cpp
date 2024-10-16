#include <iostream>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

unsigned int program; // ask about in
unsigned int VBO;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.6f, 0.3f, 0.9f, 1.0f);\n"
"}\n\0";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void shaderProgram()
{
	// build and compile our shader program

	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // ask about int
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // ask
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPLICATION_FAILED\n" << infoLog << endl;
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLIATION_FAILED\n" << infoLog << endl;
	}

	// link shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	//check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// once shaders are linked to the program
	// we dont need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set up vertex data and buffers
	// ---------------------------------------------------------
	// Set up vertex data - in our RAM
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // Left
		0.5f, -0.5f, 0.0f, // right
		0.0f, 0.5f, 0.0f // top
	};

	// Buffer object?
	// A chunck of memory on the GPU that stores data
	//[GPU : (Buffer)]
	// It stores data such as: vertex, positions, normals, indices, and etc.
	// [GPU : (Buffers(vertex data))]
	// Buffer objects allow the CPU to send large amounts of data
	// to the GPU
	// [CPU : vertices[]] ---> [GPU : Buffers(vertices[])] --> Process and render

	// Types of Buffer Objects?
	// VBO - Vertex Buffer Object:
	// Stores vertex attributes like positions, colors, texture coordinates, normals, and etc

	// Generate buffer - create unique ID for the buffer object
	//[GPU : ID]
	// We ae merely initializing an ID, which will reference a buffer object(s)
	//[GPU : ID] --> [GPU : ID = Buffer Object]
	// No memory is allocated yet
	glGenBuffers(1, &VBO);

	// glBindBUffer - bind the ID we just created
	// We are telling OpenGL that we want to use this specific ID
	// And inform which type of buffer we want to use on this ID
	// GL_ARRAY_BUFFER - is a target used for vertex attribute data
	// VBO - a buffer that stores vertex data in the GPU memory.
	// This allows the GPU to access data directly during rendering without
	// needing to communicate with the CPU each time.
	// Using them together - bind VBO to GL_ARRAY_BUFFER, OpenGL knows
	// that the buffer will store vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData - allocates(makes space for VBO) memory on the GPU for the buffer
	// and uploads the vertex data to the GPU memory. Now, the vertex data 
	// is stored in the VBO (GPU memory), ready for rendering
	// takes a target(buffer type), size of the data(in bytes) - how to memory to allocate, 
	// data which will be copied into the buffer, usage
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void createWindow()
{
	if (GLFW_TRUE != glfwInit())
	{
		cout << "Failed to init glfw" << '\n';
	}
	else
	{
		cout << "glfw init success" << '\n';
	}

	glfwSetErrorCallback(error_callback);

	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Step 1", NULL, NULL);

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Buffer swaping but checking refresh rate first - ask
	glfwShowWindow(window);

	auto ginit = glewInit();
	if (ginit != GLEW_OK)
	{
		cout << "Failed init glew" << '\n';
		return;
	}
	else
	{
		auto glVersionBytes = (char*)glGetString(GL_VERSION);
		auto glVendorBytes = (char*)glGetString(GL_VENDOR);
		string glversion((char*)glVersionBytes);
		string glvendor((char*)glVendorBytes);



		cout << "glew init success" << '\n';
		cout << glversion << '\n';
		cout << glvendor << '\n';
		cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	}
}

void setupOpenGL()
{
	glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
}

int main()
{
	createWindow();
	setupOpenGL();
	shaderProgram();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT); // color created on line 187, is used here

		glUseProgram(program); // shader program is used
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// layout for vertex attribute data for rendering
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// used to render primitives from the vertex data 
		// currently stored in vertex buffer objects (VBOs).
		// start at 0 index of data set and use blocks of 3
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

}
