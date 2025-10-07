#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint loadShader(const string& vertexPath, const string& fragmentPath); // Função modificada
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 1 (Shaders Externos)", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Agora a função loadShader recebe os caminhos dos arquivos
	GLuint shaderID = loadShader("shader.vert", "shader.frag");
	GLuint VAO = setupGeometry();
	glUseProgram(shaderID);

	// Criação da matriz de projeção
	mat4 projection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0, 0, WIDTH, HEIGHT);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	GLfloat vertices[] = {
		// Posição (x,y,z)   Cor (r,g,b)
		 0.0f,  5.0f, 0.0f,   1.0f, 0.0f, 0.0f,
		-5.0f, -5.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		 5.0f, -5.0f, 0.0f,   0.0f, 0.0f, 1.0f,
	};
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}

// Nova função para carregar, compilar e linkar shaders de arquivos externos
GLuint loadShader(const string& vertexPath, const string& fragmentPath)
{
    string vertexCode, fragmentCode;
    ifstream vShaderFile, fShaderFile;
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure& e) {
        cerr << "ERRO::SHADER::ARQUIVO_NAO_LIDO_COM_SUCESSO" << endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    
    GLuint vertex, fragment;
    
	vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
	GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    
	glDeleteShader(vertex);
    glDeleteShader(fragment);
    return shaderProgram;
}
