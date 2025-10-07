#define _USE_MATH_DEFINES
#include <iostream>
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

const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    uniform mat4 transform; // Nova matriz de transformação
    void main()
    {
	    gl_Position = transform * vec4(position, 1.0);
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

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 6e - Estrela Animada", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    GLint transformLoc = glGetUniformLocation(shaderID, "transform");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        float scale = (sin(time * 2.0f) / 4.0f) + 0.75f; 
        float angle = time;
        
        float transform[16] = {
            scale * cos(angle), scale * sin(angle), 0.0f, 0.0f,
           -scale * sin(angle), scale * cos(angle), 0.0f, 0.0f,
            0.0f,               0.0f,              1.0f, 0.0f,
            0.0f,               0.0f,              0.0f, 1.0f
        };
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

        glBindVertexArray(VAO);
        glUniform4f(colorLoc, 1.0f, 0.9f, 0.2f, 1.0f);
        
        int points = 5;
        glDrawArrays(GL_TRIANGLE_FAN, 0, 2 * points + 2);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

GLuint setupGeometry()
{
    int points = 5;
    float outerRadius = 0.8f;
    float innerRadius = 0.4f;
    std::vector<GLfloat> vertices;
    
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);

    float angleStep = M_PI / points;
    for (int i = 0; i <= 2 * points; ++i)
    {
        float radius = (i % 2 == 0) ? outerRadius : innerRadius;
        float angle = i * angleStep;
        vertices.push_back(radius * cos(angle - M_PI / 2.0f));
        vertices.push_back(radius * sin(angle - M_PI / 2.0f));
        vertices.push_back(0.0f);
    }
    
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
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
