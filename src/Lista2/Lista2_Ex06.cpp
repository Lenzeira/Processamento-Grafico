#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// Estrutura para um vértice com posição e cor
struct Vertex {
    float x, y, z;
    float r, g, b;
};

// Variáveis globais para gerenciar os vértices
vector<Vertex> finishedTriangles; // Armazena todos os triângulos finalizados
vector<Vertex> currentPoints;     // Armazena os pontos (1 ou 2) do triângulo atual
GLuint VAO_triangles, VBO_triangles;
GLuint VAO_points, VBO_points;

// Protótipos
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
GLuint setupShader();
void setupGeometry();
vec3 generateRandomColor();

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 vColor;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * vec4(position, 1.0);
        vColor = color;
    }
)";
const GLchar* fragmentShaderSource = R"(
    #version 400
    in vec3 vColor;
    out vec4 color;
    void main() {
        color = vec4(vColor, 1.0);
    }
)";

int main()
{
    glfwInit();
    srand(time(nullptr)); // Inicializa o gerador de números aleatórios
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 6 - Criando Triangulos com Mouse", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Registra os callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint shaderID = setupShader();
    setupGeometry();
    glUseProgram(shaderID);

    // Configura a projeção ortográfica para corresponder à tela
    mat4 projection = ortho(0.0f, (float)WIDTH, (float)HEIGHT, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));

    cout << "Clique na tela para criar vertices. A cada 3 cliques, um triangulo sera formado." << endl;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glPointSize(10.0f);

        // 1. Desenha todos os triângulos que já foram finalizados
        if (!finishedTriangles.empty())
        {
            glBindVertexArray(VAO_triangles);
            glDrawArrays(GL_TRIANGLES, 0, finishedTriangles.size());
        }

        // 2. Desenha os pontos do triângulo que está sendo criado
        if (!currentPoints.empty())
        {
            glBindVertexArray(VAO_points);
            // Atualiza o VBO dos pontos a cada frame (caso necessário)
            glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
            glBufferData(GL_ARRAY_BUFFER, currentPoints.size() * sizeof(Vertex), currentPoints.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, currentPoints.size());
        }

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO_triangles);
    glDeleteBuffers(1, &VBO_triangles);
    glDeleteVertexArrays(1, &VAO_points);
    glDeleteBuffers(1, &VBO_points);
    glfwTerminate();
    return 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Cor para o próximo triângulo. É gerada no primeiro clique.
        static vec3 currentColor = generateRandomColor();

        // Adiciona o novo ponto à lista de pontos atuais
        currentPoints.push_back({ (float)xpos, (float)ypos, 0.0f, currentColor.r, currentColor.g, currentColor.b });

        // Se acabamos de adicionar o 3º ponto, um triângulo está completo
        if (currentPoints.size() == 3)
        {
            // Move os 3 pontos para a lista de triângulos finalizados
            finishedTriangles.insert(finishedTriangles.end(), currentPoints.begin(), currentPoints.end());
            currentPoints.clear(); // Limpa a lista de pontos para o próximo triângulo

            // Atualiza o VBO dos triângulos com os novos dados
            glBindVertexArray(VAO_triangles);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
            glBufferData(GL_ARRAY_BUFFER, finishedTriangles.size() * sizeof(Vertex), finishedTriangles.data(), GL_DYNAMIC_DRAW);
            
            // Gera uma nova cor para o próximo triângulo
            currentColor = generateRandomColor();
        }
    }
}

void setupGeometry()
{
    // VAO/VBO para os triângulos finalizados
    glGenVertexArrays(1, &VAO_triangles);
    glGenBuffers(1, &VBO_triangles);
    glBindVertexArray(VAO_triangles);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_triangles);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
    glEnableVertexAttribArray(1);

    // VAO/VBO para os pontos do triângulo em formação
    glGenVertexArrays(1, &VAO_points);
    glGenBuffers(1, &VBO_points);
    glBindVertexArray(VAO_points);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, r));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

vec3 generateRandomColor()
{
    // Gera cores mais vivas, evitando tons muito escuros
    return vec3(
        (rand() % 128 + 128) / 255.0f,
        (rand() % 128 + 128) / 255.0f,
        (rand() % 128 + 128) / 255.0f
    );
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
