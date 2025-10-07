#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry(); // Modificamos esta função

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5a - Triangulos (com EBO)", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // --- Setup dos Shaders e Geometria ---
    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
    
    glUseProgram(shaderID);

    // --- Loop de Renderização (simplificado e corrigido) ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Fundo azul escuro
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        
        // Define a cor dos triângulos
        glUniform4f(colorLoc, 0.2f, 0.8f, 0.3f, 1.0f); // Um tom de verde

        // Nova chamada de desenho usando o Index Buffer (EBO)
        // Dizemos para desenhar 6 vértices, seguindo a ordem do nosso buffer de índices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

// --- Geometria (com a grande mudança para EBO) ---
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

    // O "mapa" que conecta os vértices para formar os triângulos
    GLuint indices[] = {
        0, 1, 2,  // Primeiro triângulo: conecta os vértices 0, 1 e 2
        2, 3, 4   // Segundo triângulo: conecta os vértices 2, 3 e 4
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO); // Gera o buffer para o índice

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
