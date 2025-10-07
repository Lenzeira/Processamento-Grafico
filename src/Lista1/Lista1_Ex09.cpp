// Lista1_Ex09.cpp - Casa com VBO Unificado e Janela
#include <iostream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Protótipos
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders de cor uniforme
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    void main() { gl_Position = vec4(position, 1.0); }
)";

const GLchar* fragmentShaderSource = R"(
    #version 400
    uniform vec4 inputColor;
    out vec4 color;
    void main() { color = inputColor; }
)";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9 - Casa com Janela", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);
    GLint uColor = glGetUniformLocation(shaderID, "inputColor");
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.9f, 0.9f, 1.0f, 1.0f); // Fundo azul claro (céu)
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        // Chão Laranja (6 vértices, começando do índice 0)
        glUniform4f(uColor, 0.8f, 0.6f, 0.2f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Paredes da casa (bege) (6 vértices, começando do índice 6)
        glUniform4f(uColor, 0.9f, 0.85f, 0.7f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 6, 6);

        // Telhado (vermelho) (3 vértices, começando do índice 12)
        glUniform4f(uColor, 0.8f, 0.2f, 0.2f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 12, 3);

        // Porta (marrom escuro) (6 vértices, começando do índice 15)
        glUniform4f(uColor, 0.4f, 0.2f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 15, 6);
    
        // Janela (amarela) (6 vértices, começando do índice 21)
        glUniform4f(uColor, 1.0f, 0.9f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 21, 6);


        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

GLuint setupGeometry()
{
    std::vector<GLfloat> allVertices;

    // Vértices do chão (6 vértices) - Offset 0
    GLfloat ground[] = { -1.0f, -0.6f, 0.0f, 1.0f, -0.6f, 0.0f, 1.0f, -0.5f, 0.0f,
                         -1.0f, -0.6f, 0.0f, 1.0f, -0.5f, 0.0f, -1.0f, -0.5f, 0.0f };
    allVertices.insert(allVertices.end(), std::begin(ground), std::end(ground));

    // Vértices das paredes (6 vértices) - Offset 6
    GLfloat walls[] = { -0.4f, -0.5f, 0.0f, 0.4f, -0.5f, 0.0f, 0.4f,  0.2f, 0.0f,
                        -0.4f, -0.5f, 0.0f, 0.4f,  0.2f, 0.0f, -0.4f,  0.2f, 0.0f };
    allVertices.insert(allVertices.end(), std::begin(walls), std::end(walls));

    // Vértices do telhado (3 vértices) - Offset 12
    GLfloat roof[] = { -0.5f, 0.2f, 0.0f, 0.5f, 0.2f, 0.0f, 0.0f, 0.7f, 0.0f };
    allVertices.insert(allVertices.end(), std::begin(roof), std::end(roof));

    // Vértices da porta (6 vértices) - Offset 15
    GLfloat door[] = { -0.1f, -0.5f, 0.0f, 0.1f, -0.5f, 0.0f, 0.1f, 0.0f, 0.0f,
                       -0.1f, -0.5f, 0.0f, 0.1f,  0.0f, 0.0f, -0.1f, 0.0f, 0.0f };
    allVertices.insert(allVertices.end(), std::begin(door), std::end(door));
    
    // Vértices da janela (6 vértices) - Offset 21
    GLfloat window_quad[] = { 0.15f, -0.1f, 0.0f, 0.35f, -0.1f, 0.0f, 0.35f, 0.1f, 0.0f,
                              0.15f, -0.1f, 0.0f, 0.35f,  0.1f, 0.0f, 0.15f, 0.1f, 0.0f };
    allVertices.insert(allVertices.end(), std::begin(window_quad), std::end(window_quad));

    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(GLfloat), allVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
