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

// Variável global para a posição do objeto
vec2 objectPos(WIDTH / 2.0f, HEIGHT / 2.0f);

// Shaders MODIFICADOS para aceitar a matriz de modelo
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 vColor;
    
    uniform mat4 model;      // Matriz de modelo do objeto
    uniform mat4 projection; // Matriz de projeção da câmera
    
    void main()
    {
        gl_Position = projection * model * vec4(position, 1.0);
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ex 2 - Triangulo Interativo", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);

    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    GLint projLoc = glGetUniformLocation(shaderID, "projection");

    mat4 projection = ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Cria a matriz de modelo a cada frame
        mat4 model = mat4(1.0f);
        model = translate(model, vec3(objectPos.x, objectPos.y, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

        // Desenha o triângulo
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

// Callback do teclado MODIFICADO para mover o objeto
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    float speed = 10.0f;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_W) objectPos.y -= speed; // Y é invertido, então subtrai para subir
        if (key == GLFW_KEY_S) objectPos.y += speed;
        if (key == GLFW_KEY_A) objectPos.x -= speed;
        if (key == GLFW_KEY_D) objectPos.x += speed;
    }
}

// Geometria de um triângulo menor, centrado na origem
int setupGeometry()
{
    GLfloat vertices[] = {
        // Posição (x,y,z)    Cor (r,g,b)
         0.0f, -50.0f, 0.0f,   1.0f, 0.0f, 0.0f,
       -50.0f,  50.0f, 0.0f,   0.0f, 1.0f, 0.0f,
        50.0f,  50.0f, 0.0f,   0.0f, 0.0f, 1.0f,
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
