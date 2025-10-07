#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int setupShader();
int setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders e funções setupGeometry, key_callback, setupShader são idênticos ao código do Exercício 4
// ... (pode copiar as mesmas funções do código acima) ...

const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 vColor;
    void main()
    {
        gl_Position = vec4(position, 1.0);
        vColor = color;
    }
)";
const GLchar* fragmentShaderSource = R"(
    #version 400
    in vec3 vColor;
    out vec4 color;
    void main()
    {
        color = vec4(vColor, 1.0);
    }
)";

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5 - Quatro Quadrantes", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        
        glBindVertexArray(VAO);

        // Canto inferior esquerdo
        glViewport(0, 0, width / 2, height / 2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Canto inferior direito
        glViewport(width / 2, 0, width / 2, height / 2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Canto superior esquerdo
        glViewport(0, height / 2, width / 2, height / 2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        // Canto superior direito
        glViewport(width / 2, height / 2, width / 2, height / 2);
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
        // Posição (x,y,z)    Cor (r,g,b)
         0.0f,  0.5f, 0.0f,    1.0f, 0.2f, 0.8f,
        -0.5f, -0.5f, 0.0f,    1.0f, 0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,    1.0f, 0.9f, 0.2f
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

int setupShader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}
