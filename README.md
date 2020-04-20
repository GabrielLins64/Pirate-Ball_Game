Requisitos: c++ e opengl (glut)
Instalação destes no Linux: sudo apt install freeglut3-dev freeglut3

Comando para rodar:
g++ game.cpp -o game -lglut -lGL -lGLU && ./game

Teclas:
W, S, A, D - Frente, Trás, Esquerda, Direita;
Barra de Espaço - Pular;
i - aumentar de tamanho;
k - diminuir de tamanho;
p - pausar;
esc - sair
Botão esquerdo do mouse - Pegar ouro;
Botão direito do mouse - Soltar ouro;
Scroll do mouse - Modificar a distância da câmera;
Mover o mouse - Rodar visualização.
