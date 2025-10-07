#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLuint setupShader();
GLuint setupGeometry();

const GLuint WIDTH = 800, HEIGHT = 600;

// Shaders MODIFICADOS para aceitar cor por vértice
const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor; // Atributo de cor do vértice
    
    out vec3 ourColor; // Variável para passar a cor para o fragment shader

    void main()
    {
	    gl_Position = vec4(aPos, 1.0);
        ourColor = aColor; // Repassa a cor
    }
)";
const GLchar* fragmentShaderSource = R"(
    #version 400
    out vec4 FragColor;

    in vec3 ourColor; // Recebe a cor interpolada do vertex shader

    void main()
    {
	    FragColor = vec4(ourColor, 1.0); // Usa a cor recebida
    }
)";

int main()
{
    // --- Inicialização (sem alteração) ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5c - Pontos Coloridos", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, WIDTH, HEIGHT);
    
    // --- Setup (sem alteração) ---
    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    
    glUseProgram(shaderID);

    // --- Loop de Renderização (modificado) ---
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glPointSize(20); // Aumenta o tamanho dos pontos

        glBindVertexArray(VAO);
        
        // A chamada glUniform4f não é mais necessária!
        // A cor agora vem dos dados dos vértices.

        glDrawArrays(GL_POINTS, 0, 6);

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

// Função de Shader (sem alteração na lógica, mas usa os novos sources)
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

// --- Geometria (com a grande mudança para cor por vértice) ---
int setupGeometry()
{
    // Agora cada vértice tem 6 floats: 3 para posição (xyz) e 3 para cor (rgb)
    GLfloat vertices[] = {
        // Posição (x, y, z)    Cor (r, g, b)
		-0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // v0 - Vermelho
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // v1 - Verde
		 0.0f,  0.0f, 0.0f,     0.0f, 0.0f, 1.0f, // v2 - Azul
		 0.0f,  0.0f, 0.0f,     1.0f, 1.0f, 0.0f, // v3 - Amarelo (mesma posição, cor diferente)
		 0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f, // v4 - Ciano
		 0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 1.0f  // v5 - Magenta
	};

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // --- Configuração dos Atributos ---
    // Atributo de Posição (layout location = 0)
    // Lê 3 floats, a cada 6 floats (stride), começando do início (offset 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo de Cor (layout location = 1)
    // Lê 3 floats, a cada 6 floats (stride), começando após os 3 primeiros de posição (offset de 3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

    return VAO;
}
