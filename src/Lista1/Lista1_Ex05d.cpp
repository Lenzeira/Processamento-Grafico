#include <iostream>
#include <cmath> // Para a função sin()
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders (sem alteração)
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    void main()
    {
	    gl_Position = vec4(position.x, position.y, position.z, 1.0);
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
    // --- Inicialização ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5d - Animado (com EBO)", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // --- Setup ---
    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    
    glUseProgram(shaderID);

    // --- Loop de Renderização Animado ---
    while (!glfwWindowShould
