#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry(); // Função que será modificada

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
    // --- Inicialização (sem alteração) ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5b - Contorno (com EBO)", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // --- Setup dos Shaders e Geometria ---
    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    
    glUseProgram(shaderID);

    // --- Loop de Renderização (modificado) ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(10); // Aumenta a espessura da linha

        glBindVertexArray(VAO);
        
        // Define a cor do contorno
        glUniform4f(colorLoc, 0.2f, 0.5f, 1.0f, 1.0f); // Cor azul

        // Novas chamadas de desenho usando o Index Buffer (EBO)
        // Desenhamos dois contornos separados a partir do mesmo buffer de índices
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
    }
    
    // --- Finalização (sem alteração) ---
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função de Shader (sem alteração)
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

// --- Geometria (modificada para usar EBO) ---
int setupGeometry()
{
    // Apenas os 5 vértices únicos
    GLfloat vertices[] = {
        // Posição (x, y, z)
        -0.5f,  0.5f, 0.0f, // Vértice 0
        -0.5f, -0.5f, 0.0f, // Vértice 1
         0.0f,  0.0f, 0.0f, // Vértice 2 (o ponto central)
         0.5f, -0.5f, 0.0f, // Vértice 3
         0.5f,  0.5f, 0.0f  // Vértice 4
    };

    // O "mapa" que conecta os vértices
    GLuint indices[] = {
        0, 1, 2,  // Contorno do primeiro triângulo
        2, 3, 4   // Contorno do segundo triângulo
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Vincula e envia os dados do índice para o EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}
