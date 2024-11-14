#include <iostream>
#include <fmt\core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

// Global Window variables
GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

// Global variables
unsigned int program;
unsigned int VBO;


// Vertex Shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos, 1.0);\n"
"}\0";

// Fragment Shader
const char* fragmentShaderSource = "version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.6f, 0.43, 0.2f, 1.0f);\n"
"}\0";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
};

// Build and compile shader program
static void shaderProgram()
{
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for vertex shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for Fragment Shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Link Shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	// Check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// Delete shaders - not needed as we have linked them into a program
	// where they now sit.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertices for the shape(s)
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // Bottom-left
		0.05f, -0.5f, 0.0f, // Bottom-right
		0.5f, 0.5f, 0.0f, // Top-right

		-0.5f, -0.5f, 0.0f, // Bottom-left
		0.5f, 0.5f, 0.0f, // Top-right
		-0.5f, 0.5f, 0.0f // Top-left
	};

	// Generate ID for buffer object
	glGenBuffers(1, &VBO);

	// Bind buffer obj to a target - Vertex data uses array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Allocate and upload data to our buffer obj
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
};

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

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwShowWindow(window);

	auto gint = glewInit();
	if (gint != GLEW_OK)
	{
		cout << "Failed init glew" << '\n';
		return;
	}
	else
	{
		auto glVersionBytes = (char*)glGetString(GL_VERSION);
		auto glVendorBytes = (char*)glGetString(GL_VENDOR);
		string glVersion((char*)glVersionBytes);
		string glVendor((char*)glVendorBytes);

		cout << "glew init success" << '\n';
		cout << glVersion << '\n';
		cout << glVendor << '\n';
		cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
	}
}

void setupOpenGL()
{
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}

int main()
{
	createWindow();
	setupOpenGL();
	shaderProgram();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}