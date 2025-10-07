#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Protótipos
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 800;

// Shaders MODIFICADOS para aceitar cor por vértice
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    void main()
    {
       gl_Position = vec4(aPos, 1.0);
       ourColor = aColor;
    }
)";
const GLchar* fragmentShaderSource = R"(
    #version 400
    out vec4 FragColor;
    in vec3 ourColor;
    void main()
    {
       FragColor = vec4(ourColor, 1.0);
    }
)";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 6 - Circulo (com Triangle Fan)", nullptr, nullptr);
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

        glBindVertexArray(VAO);
        
        // A cor agora vem dos atributos do vértice, não de um uniform
        // O último parâmetro é o número total de vértices a serem desenhados
        glDrawArrays(GL_TRIANGLE_FAN, 0, 36 + 2); // +2 para o centro e o vértice final que fecha o círculo

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

// --- Geometria (totalmente reescrita para ser mais eficiente) ---
int setupGeometry()
{
    int segments = 36; // Um círculo com 36 segmentos é bem suave
    float radius = 0.7f;
    
    // Usamos um std::vector para facilitar a adição de vértices
    std::vector<GLfloat> vertices;

    // Vértice central (branco)
    vertices.push_back(0.0f); // x
    vertices.push_back(0.0f); // y
    vertices.push_back(0.0f); // z
    vertices.push_back(1.0f); // r
    vertices.push_back(1.0f); // g
    vertices.push_back(1.0f); // b

    // Vértices da circunferência (azul ciano)
    for (int i = 0; i <= segments; i++)
    {
        float angle = i * (2.0f * M_PI / segments);
        // Posição
        vertices.push_back(cos(angle) * radius);
        vertices.push_back(sin(angle) * radius);
        vertices.push_back(0.0f);
        // Cor
        vertices.push_back(0.0f); // r
        vertices.push_back(1.0f); // g
        vertices.push_back(1.0f); // b
    }

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    // Atributo de Posição (layout location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de Cor (layout location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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
