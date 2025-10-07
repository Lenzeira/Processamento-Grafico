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
const int segments = 30;

// Shaders (sem alteração)
// ...

int main()
{
    // ... (Setup inicial do GLFW/GLAD) ...
    GLuint shaderID = setupShader();
    GLuint VAO = setupGeometry();
    glUseProgram(shaderID);
    GLint colorLoc = glGetUniformLocation(shaderID, "ourColor");

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        
        // --- Animação da Boca ---
        float time = glfwGetTime();
        float mouthFactor = (sin(time * 8.0f) + 1.0f) / 2.0f; // Varia entre 0 (fechada) e 1 (aberta)
        
        // Atualiza apenas os vértices da boca
        float radius = 0.7f;
        GLfloat mouthTopVtx[] = { radius * cos(mouthFactor * 0.5f), radius * sin(mouthFactor * 0.5f), 0.0f };
        GLfloat mouthBottomVtx[] = { radius * cos(-mouthFactor * 0.5f), radius * sin(-mouthFactor * 0.5f), 0.0f };

        glBindBuffer(GL_ARRAY_BUFFER, glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&VAO); glBindBuffer(GL_ARRAY_BUFFER, 0); GLuint vbo; glGetVertexArrayiv(VAO, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&vbo); vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 1 * 3 * sizeof(GLfloat), sizeof(mouthTopVtx), mouthTopVtx);
        glBufferSubData(GL_ARRAY_BUFFER, (segments + 1) * 3 * sizeof(GLfloat), sizeof(mouthBottomVtx), mouthBottomVtx);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glUniform4f(colorLoc, 1.0f, 0.9f, 0.0f, 1.0f); // Cor Amarela
        
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

GLuint setupGeometry()
{
    float radius = 0.7f;
    std::vector<GLfloat> vertices;
    
    vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(0.0f);

    float angleStep = 2.0f * M_PI / segments;
    for (int i = 0; i <= segments; ++i)
    {
        float angle = i * angleStep;
        vertices.push_back(radius * cos(angle));
        vertices.push_back(radius * sin(angle));
        vertices.push_back(0.0f);
    }
    
    // (Código de criação do VAO/VBO, usando GL_DYNAMIC_DRAW para o VBO)
}

// (Inclua key_callback e setupShader aqui)
