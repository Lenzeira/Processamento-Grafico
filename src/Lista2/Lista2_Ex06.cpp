#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
int setupShader();
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    uniform mat4 projection;
    void main()
    {
	    gl_Position = projection * vec4(position, 1.0);
    }
)";

const GLchar* fragmentShaderSource = R"(
    #version 400
    uniform vec4 inputColor;
    out vec4 color;
    void main()
    {
	    color = inputColor;
    }
)";

static vector<vec3> g_pending;
static vector<vec3> g_vertices;
static vector<vec4> g_colors;
static GLuint gVBO = 0;
static GLuint gVAO = 0;

int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "L2_Ex06 - Clique para criar triangulos", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	GLuint shaderID = setupShader();
	glUseProgram(shaderID);

	gVAO = setupGeometry();

	GLint projLoc = glGetUniformLocation(shaderID, "projection");
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		int fbw, fbh;
		glfwGetFramebufferSize(window, &fbw, &fbh);
		mat4 projection = ortho(0.0, static_cast<double>(fbw), static_cast<double>(fbh), 0.0, -1.0, 1.0);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		if (!g_vertices.empty())
		{
			glBufferData(GL_ARRAY_BUFFER, g_vertices.size() * sizeof(vec3), g_vertices.data(), GL_DYNAMIC_DRAW);
		}
		else
		{
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
		}

		glBindVertexArray(gVAO);
		size_t triCount = g_vertices.size() / 3;
		for (size_t i = 0; i < triCount; ++i)
		{
			const vec4& c = g_colors[i];
			glUniform4f(colorLoc, c.r, c.g, c.b, c.a);
			glDrawArrays(GL_TRIANGLES, static_cast<GLint>(i * 3), 3);
		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	if (gVAO) glDeleteVertexArrays(1, &gVAO);
	if (gVBO) glDeleteBuffers(1, &gVBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		g_pending.emplace_back(static_cast<float>(xpos), static_cast<float>(ypos), 0.0f);

		if (g_pending.size() == 3)
		{
			g_vertices.push_back(g_pending[0]);
			g_vertices.push_back(g_pending[1]);
			g_vertices.push_back(g_pending[2]);

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> r_dist(0.3f, 0.6f);
			std::uniform_real_distribution<float> g_dist(0.1f, 0.4f);
			std::uniform_real_distribution<float> b_dist(0.7f, 1.0f);

			vec4 color(r_dist(gen), g_dist(gen), b_dist(gen), 1.0f);
			g_colors.push_back(color);

			g_pending.clear();
		}
	}
}

int setupShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry()
{
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return static_cast<int>(gVAO);
}
