#include <iostream>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
using std::string;


GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;

unsigned int program;
unsigned int VBO;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void shaderProgram()
{

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX:COMPLIATION_FAILED\n" << infoLog << endl;
	}

	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPPILATION_FAILED\n" << infoLog << endl;
	}

	// link shaders
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
	};
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		0.5f, -0.5f, 0.0f, // right
		0.5f, 0.5f, 0.0f, // top
		-0.5f, -0.5f, 0.0f, // left
		0.5f, 0.5f, 0.0f, // top
		-0.5f, 0.5f, 0.0 // top left
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribute(s).

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void createWindow()
{
	if (GLFW_TRUE != glfwInit())
	{
		cout << "Failed to init glfw" << '\n';
	}
	else
		cout << "glfw init success" << '\n';



	glfwSetErrorCallback(error_callback);



	window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Step1", NULL, NULL);



	glfwSetKeyCallback(window, key_callback);



	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwShowWindow(window);



	auto ginit = glewInit();
	if (ginit != GLEW_OK)
	{
		cout << "Failed to init glew" << '\n';
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

void setupOpengl()
{
	glClearColor(0.7f, 0.2f, 0.3f, 1.0f);
}

int main()
{
	createWindow();

	setupOpengl();

	shaderProgram();


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // 4 times?

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 3, 3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	// Wireframe polygons
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

}
