# Projeto de Fundamentos de Computação Gráfica

Este repositório contém os exercícios e trabalhos desenvolvidos para a disciplina de Fundamentos de Computação Gráfica.

**Autor:**
* Guilherme Lenzi de Oliveira

**Tecnologias Utilizadas:**
* C++
* OpenGL
* GLFW
* GLAD
# Respostas - OpenGL e Shaders

## Questão 1
O **GLSL (OpenGL Shading Language)** é a linguagem utilizada para programar shaders no OpenGL.  
Os dois tipos principais são:  
- **Vertex Shader:** responsável por processar e transformar as posições dos vértices.  
- **Fragment Shader:** calcula a cor final de cada fragmento que formará os pixels na tela.

---

## Questão 2
As **primitivas gráficas** representam as formas básicas que o OpenGL desenha, como **pontos**, **linhas** e **triângulos**.  
O armazenamento dos vértices dessas primitivas é feito nos **VBOs (Vertex Buffer Objects)**, que guardam os dados diretamente na memória da GPU para maior desempenho.

---

## Questão 3
- **VBO:** armazena as informações dos vértices.  
- **VAO:** organiza e referencia os atributos que cada vértice possui.  
- **EBO:** guarda os índices que indicam a ordem de uso dos vértices.  

A ligação entre eles permite desenhar objetos complexos de forma eficiente e reutilizando dados.

---

## Questão 8
Uma configuração simples é usar apenas um **VBO** e um **VAO**.  
O VBO contém os vértices do triângulo, enquanto o VAO guarda a configuração dos atributos.  
Sem utilizar o EBO, o desenho é feito diretamente na sequência dos vértices armazenados — ideal para formas pequenas e simples.
