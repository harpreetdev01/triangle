#include <iostream>
#include <fmt/core.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>



using namespace std;



GLFWwindow* window;
auto closeWindow = false;
const int WIN_WIDTH = 1920;
const int WIN_HEIGHT = 1080;



unsigned int program; // int for program?
unsigned int VBO; // why VBO an int?



struct Quad
{



};



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // location 0 and 1?
"layout (location = 1) in vec3 aColor;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos, 1.0);\n"
" vertexColor = aColor;\n"
"}\0";



const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 vertexColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
//" FragColor = ourColor;\n"
" FragColor = vec4(vertexColor, 1.0);\n"
"}\n\0"; // 0?



static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
};



static void shaderProgram()
{
	// build and compile our shader program



	// vertex shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);



	// check for vertex shader compile errrors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SUCCESS::VERTEX::COMPLICATION_FAILED\n" << infoLog << endl;
	}



	// fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);



	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPLICATION_FAILED\n" << infoLog << endl;
	}



	// link shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);



	// check for linking errors
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}



	// delete uneeded shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Set up vertex data and buffers
	/*float vertices[] =
	{
	-0.5f, -0.5f, 0.0f, // Bottom left
	0.5f, -0.5f, 0.0f, // Bottom right
	0.0f, 0.5f, 0.0f // Top
	};
	*/



	float vertices[] = {
		// Positions // Colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Red Line
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,



		-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Green Line
		0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,



		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // Blue Line
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};



	// Generate ID for buffer object
	glGenBuffers(1, &VBO);



	// Bind buffer with ID and inform which type of buffer we want to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO the ID?



	// Allocate and upload data to our buffer ID
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
	glfwSwapInterval(1); // difference between this and swap buffers?
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
	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}



/*
* Update the positions of the vertices and upload them to GPU
*/
void updatePosition()
{
	// Declare array for vertices
	float vertices[] = {
		// Positions // Colors
		-0.5f, -0.5f , 0.0f, 0.7f, 0.0f, 0.0f, // left
		0.5f, -0.5f , 0.0f, 0.7f, 0.0f, 0.0f, // right
		0.5f, 0.5f, 0.0f, 0.7f, 0.0f, 0.0f, // top



		// triangle 1
		// /x
		// / |
		// / |
		// / |
		// x----x



		-0.5f, -0.5f , 0.0f, 0.7f, 0.0f, 0.0f, // left
		0.5f, 0.5f, 0.0f, 0.7f, 0.0f, 0.0f, // top
		-0.5f, 0.5f , 0.0f, 0.7f, 0.0f, 0.0f, // top left



		// triangle 2
		// x----x
		// | /
		// | /
		// | /
		// |/
		// x
	};



	glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO the ID?
	// Allocate and upload data to our buffer ID
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Upload array to GPU
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



int main()
{
	createWindow();
	setupOpenGL();
	shaderProgram();



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);



		// Update the uniform color
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		glUseProgram(program);



		//Update vertices
		updatePosition();



		// Bind buffer with the vertices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);



		glEnableVertexAttribArray(0); // Enable slot 0 in vertex shader
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 sizeof(float), (void)0);



		glEnableVertexAttribArray(1); // Enable slot 1 in vertex shader
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 sizeof(float), (void)(3 * sizeof(float))); // ask



		// render primitives
		glDrawArrays(GL_TRIANGLES, 0, 6);



		glDisableVertexAttribArray(0); // Disable slot 0 in vertex shader
		glDisableVertexAttribArray(1); // Disable slot 1 in vertex shader



		// Unbind buffer with the vertices
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}