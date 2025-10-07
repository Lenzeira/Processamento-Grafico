# Processamento Gráfico - Lista de Exercícios 1
### UNISINOS - Disciplina de Processamento Gráfico

[cite_start]Este repositório contém as soluções para a primeira lista de exercícios da disciplina de Processamento Gráfico, focada na introdução à OpenGL Moderna, Shaders e Buffers[cite: 5].

Cada exercício foi resolvido aplicando diferentes técnicas e conceitos fundamentais da API, como forma de explorar a matéria e criar soluções únicas.

---

## 🚀 Exercícios Implementados

A lista de exercícios práticos aborda desde o desenho de primitivas simples até a composição de cenas e animações.

* [cite_start]**Exercício 5: Desenho de 2 Triângulos** [cite: 18]
    * [cite_start]`5a`: Desenho com polígono preenchido (`GL_TRIANGLES`)[cite: 19].
    * [cite_start]`5b`: Desenho apenas com contorno (`GL_LINE_LOOP`)[cite: 20].
    * [cite_start]`5c`: Desenho apenas dos vértices (`GL_POINTS`)[cite: 21].
    * [cite_start]`5d`: Combinação das três formas de desenho em uma única cena[cite: 22].

* **Exercício 6: Geometria Paramétrica**
    * [cite_start]Utilização da equação paramétrica do círculo para gerar vértices de diversas formas[cite: 23].
    * [cite_start]`6a`: Desenho de um **Octágono**[cite: 25].
    * [cite_start]`6b`: Desenho de um **Pentágono**[cite: 26].
    * [cite_start]`6c`: Desenho de um **Pac-Man**, controlando o arco do círculo[cite: 27].
    * [cite_start]`6d`: Desenho de uma **Fatia de Pizza**[cite: 28].
    * [cite_start]`6e`: (Desafio) Desenho de uma **Estrela** de 5 pontas, com lógica de raios alternados[cite: 29].

* **Exercício 7: A Espiral**
    * [cite_start]Implementação do desenho de uma espiral, utilizando uma variação da equação paramétrica onde o raio aumenta com o ângulo[cite: 30].

* **Exercício 8: Triângulo com Cores por Vértice**
    * [cite_start]Desenho de um triângulo com cores diferentes em cada vértice (vermelho, verde e azul), que são interpoladas suavemente pela superfície[cite: 33]. A técnica utilizada foi a de **Atributos de Vértice** para a cor.

* **Exercício 9: Desenho Livre (A Casa)**
    * [cite_start]Reprodução de um desenho utilizando múltiplas primitivas[cite: 40]. A solução implementada utiliza um **único VBO** para todos os componentes da casa, e múltiplas chamadas `glDrawArrays` com offsets para renderizar cada parte, otimizando o número de `binds`.

---

## 🛠️ Tecnologias Utilizadas

* C++
* OpenGL 4.0
* GLAD (para carregamento das funções do OpenGL)
* GLFW (para criação da janela e gerenciamento de input)

---

## 👤 Autor

* Guilherme Lenzi
