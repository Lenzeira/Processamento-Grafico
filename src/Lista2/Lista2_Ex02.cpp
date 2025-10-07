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
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos); // Novo callback para o mouse
GLuint setupShader();
GLuint setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

// Variável global para armazenar a posição do mouse
vec2 mousePos;

// Shaders MODIFICADOS para aceitar uma matriz de modelo
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    
    uniform mat4 model;
    uniform mat4 projection;
    
    void main()
    {
        gl_Position = projection * model * vec4(position, 1.0);
    }
)";
const GLchar* fragmentShaderSource = R"(
    #version 400
    out vec4 color;
    uniform vec4 inputColor;
    void main()
    {
        color = inputColor;
    }
)";

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ex 2 - Objeto seguindo o mouse", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    // --- NOVO: Registrando o callback do mouse ---
    glfwSetCursorPosCallback(window, cursor_position_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);

    // Obtém a localização dos uniforms no shader
    GLint modelLoc = glGetUniformLocation(shaderID, "model");
    GLint projLoc = glGetUniformLocation(shaderID, "projection");
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

    // Configura a matriz de projeção UMA VEZ (ela não muda)
    mat4 projection = ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // --- NOVO: Atualiza a matriz de modelo a cada frame ---
        mat4 model = mat4(1.0f); // Matriz identidade
        // Translada o quadrado para a posição atual do mouse
        model = translate(model, vec3(mousePos.x, mousePos.y, 0.0f));
        
        // Envia a nova matriz de modelo para o shader
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

        // Define a cor do quadrado
        glUniform4f(colorLoc, 1.0f, 0.5f, 0.2f, 1.0f); // Laranja

        // Desenha o quadrado
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

// --- NOVA FUNÇÃO: Callback do Cursor ---
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Atualiza a variável global com a posição do mouse
    mousePos.x = xpos;
    mousePos.y = ypos;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Geometria de um quadrado centrado na origem
int setupGeometry()
{
    GLfloat vertices[] = {
        // Posições
        -25.0f, -25.0f, 0.0f,
         25.0f, -25.0f, 0.0f,
         25.0f,  25.0f, 0.0f,

        -25.0f, -25.0f, 0.0f,
         25.0f,  25.0f, 0.0f,
        -25.0f,  25.0f, 0.0f
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
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
