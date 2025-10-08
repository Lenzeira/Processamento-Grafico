#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();


const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar *vertexShaderSource = R"(
    #version 400
    layout (location = 0) in vec3 position;
    void main()
    {
	    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
)";

const GLchar *fragmentShaderSource = R"(
    #version 400
    uniform vec4 inputColor;
    out vec4 color;
    void main()
    {
	    color = inputColor;
    }
)";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio Retangulo!", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Falha ao criar a janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros de funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}

	// Obtendo as informações de versão
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	// Gerando a geometria de um retângulo
	GLuint VAO = setupGeometry();

	// Buscando o endereço da variável uniform "inputColor" no shader
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	glUseProgram(shaderID);

	double prev_s = glfwGetTime();
	double title_countdown_s = 0.1;

	// Loop da aplicação
	while (!glfwWindowShouldClose(window))
	{
		// Lógica para exibir o FPS no título da janela
		{
			double curr_s = glfwGetTime();
			double elapsed_s = curr_s - prev_s;
			prev_s = curr_s;

			title_countdown_s -= elapsed_s;
			if (title_countdown_s <= 0.0 && elapsed_s > 0.0)
			{
				double fps = 1.0 / elapsed_s;
				char tmp[256];
				sprintf(tmp, "Ola Retangulo! \tFPS %.2lf", fps);
				glfwSetWindowTitle(window, tmp);
				title_countdown_s = 0.1;
			}
		}

		// Checa eventos de input
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fundo cinza escuro
		glClear(GL_COLOR_BUFFER_BIT);

		// Define a espessura das linhas e o tamanho dos pontos
		glLineWidth(10);
		glPointSize(20);

		// Conecta ao buffer de geometria (VAO)
		glBindVertexArray(VAO);

		// 1. Desenha o retângulo preenchido em amarelo
		glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f); // Cor amarela
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// 2. Desenha o contorno dos dois triângulos em magenta
		glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f); // Cor magenta
		glDrawArrays(GL_LINE_LOOP, 0, 3); // Contorno do primeiro triângulo
		glDrawArrays(GL_LINE_LOOP, 3, 3); // Contorno do segundo triângulo

		// 3. Desenha os vértices em branco
		glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // Cor branca
		glDrawArrays(GL_POINTS, 0, 6);

		// Desconecta o VAO (boa prática)
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}

	// Desaloca os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a GLFW
	glfwTerminate();
	return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função para configurar os shaders (idêntica à original)
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}
	// Linkando os shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Função para configurar a geometria
int setupGeometry()
{
	// AQUI ESTÁ A MUDANÇA: Coordenadas para formar um retângulo com dois triângulos.
	// Os 6 vértices definem os dois triângulos que, juntos, formam o retângulo.
	GLfloat vertices[] = {
		// Triângulo 1
		 0.5f,  0.5f, 0.0f,  // Canto Superior Direito
		 0.5f, -0.5f, 0.0f,  // Canto Inferior Direito
		-0.5f,  0.5f, 0.0f,  // Canto Superior Esquerdo
		// Triângulo 2
		 0.5f, -0.5f, 0.0f,  // Canto Inferior Direito
		-0.5f, -0.5f, 0.0f,  // Canto Inferior Esquerdo
		-0.5f,  0.5f, 0.0f   // Canto Superior Esquerdo
	};

	GLuint VBO, VAO;
	
	// Geração do VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Geração do VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	// Atributo de posição do vértice
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Desvincula o VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Desvincula o VAO
	glBindVertexArray(0);

	return VAO;
}
