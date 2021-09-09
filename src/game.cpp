/*
UNIVERSIDADE ESTADUAL DO CEARÁ - UECE
Curso de Graduação em Ciência da Computação
Disciplina: Computação Gráfica
Docente: Matheus Magalhaes de Carvalho
Discentes - Matrículas / Atribuição:
	Francisco Bruno Duarte Castro - 1440480 / Apresentação
	Gabriel Furtado Lins Melo - 1394225 / Desenvolvimento
	Hianuy Esperidião de Sousa Pinto - 1394198 / Apresentação
*/

#include <GL/glut.h>
#include <iostream> // Cout for debugging
#include "drawings.h"
#include "geometry.h"
#include "hero.h"
#include "math.h"

using namespace std;

// Variáveis principais
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x
// Janela e Execução:
int _window_width = 1100; // Originais, para usar após o FullScreen
int _window_height = 700;
int window_width = 1100;
int window_height = 700;
int FPS = 80;
int FPms = 1000 / FPS;
bool paused = 0;
bool lost = 0;
// Câmera:
float camera_distance = 42.0;
float rotation_speed = 0.3;
float theta = -9.6; // Ângulo do vetor olhar->olho com a projeção do mesmo no plano XZ (será chamada de b, a partir de agora).
float psi = 337.8;  // Ângulo do vetor x da câmera (saindo da origem), com b (proj_XZ (olhar->olho)).
// Personagem:
float color[] = {0.0, 0.0, 0.0};
float initial_pos[] = {180.0, 15.0, -180.0};
float radius = 1.0;
Ball hero = Ball(1.0, color, initial_pos); // Parameters: Radius, color (r,g,b), initial position (x,y,z)
// Vilão:
Wave onda = Wave();
// Objetivo:
Tesouro tesouro = Tesouro();
int pontos = 0;
char spontos[13];
// Controle de ações:
int mouse_x = window_width / 2, mouse_y = window_height / 2;
bool _up = 0, _down = 0, _left = 0, _right = 0;
bool _jump = 0, _increase = 0, _decrease = 0;
bool full_screen = 0;
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x

// Declaração das funções
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x
void inicializar(void);
void animar(void);
void gerenciarEventos();
void reiniciar();
void pausar();
void despausar();
void tela_cheia();
void colisoes();
void caldo();
void perder();
void pontuar();
void pegar_tesouro();
void soltar_tesouro();
void gerenciarMovimentos(int v);
void manageKeyboardCharDOWN(unsigned char key, int x, int y);
void manageKeyboardCharUP(unsigned char key, int x, int y);
void manageMouseRotation(int x, int y);
void mouseFunc(int button, int state, int x, int y);
void ParametrosIluminacao(void);
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x

// Main:
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x
int main(int argc, char **argv)
{
    points_to_char(pontos, spontos);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Game");
    glutDisplayFunc(animar);
    glutTimerFunc(FPms, gerenciarMovimentos, 0);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutMouseFunc(mouseFunc);
    glutKeyboardFunc(manageKeyboardCharDOWN);
    glutKeyboardUpFunc(manageKeyboardCharUP);
    glutPassiveMotionFunc(manageMouseRotation);
    inicializar();
    glutMainLoop();
}
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x

// Definição das Funções:
// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x
void inicializar(void)
{                                            /* Inicializa parâmetros de rendering */
    glClearColor(0.54f, 0.83f, 0.81f, 1.0f); /* Define a cor de fundo da janela de visualização como branca */
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHT0);
    gluPerspective(40.0f, ((GLfloat)window_width / (GLfloat)window_height), 1, 1600.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SMOOTH);
    // glEnable(GL_FLAT);
}

void gerenciarEventos()
{
    colisoes();
    onda.run();
    caldo();
    if (hero.is_carrying_treasure())
        tesouro.set_position(hero.get_x(), hero.get_y() + hero.get_radius(), hero.get_z());
    tesouro.update();
    hero.update_position();
}

void animar(void)
{
    if (lost)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        Lost_Menu(window_width, window_height, spontos);
        return;
    }
    if (paused)
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        Pause_Menu(window_width, window_height, spontos, full_screen);
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* Limpa a janela de visualização com a cor de fundo especificada */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    ParametrosIluminacao();
    glutTimerFunc(FPms, gerenciarMovimentos, 0);
    float *cs = cameraSpecifications(camera_distance, theta, psi, hero.get_x(), hero.get_y(), hero.get_z());
    gluLookAt(cs[0], cs[1] + hero.get_radius() + hero.get_y(), cs[2], /* eye */
              hero.get_x(), hero.get_y(), hero.get_z(),               /* look */
              0.0, 1.0, 0.0);                                         /* _up */
    Desenha_Chao();
    Desenha_Paredes();
    Desenha_Bau();
    gerenciarEventos();
    // DEBUGGING:
    // ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X
    // cout << theta << ", " << psi << ", " << camera_distance << endl;
    // cout << "(" << hero.get_x() << ", " << hero.get_y() << ", " << hero.get_z() << ")" << endl;
    // ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X ~~~~~~~~~~~~~~~~~ X
    glutSwapBuffers();
}

void reiniciar()
{
    pontos = 0;
    points_to_char(pontos, spontos);
    hero.drop_treasure();
    hero.set_position(initial_pos[0], initial_pos[1], initial_pos[2]);
    hero.stop();
    tesouro.respawn();
    onda.restart();
    theta = -9.6;
    psi = 337.8;
    camera_distance = 42;
    lost = 0;
    despausar();
}

void pausar()
{
    paused = 1;
    glutPostRedisplay();
}

void despausar()
{
    paused = 0;
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPostRedisplay();
}

void tela_cheia()
{
    if (full_screen)
    {
        despausar();
        full_screen = 0;
        glutReshapeWindow(_window_width, _window_height);
    }
    else
    {
        despausar();
        full_screen = 1;
        glutFullScreen();
    }
}

void colisoes()
{
    float spz = hero.get_speed_z();
    float spx = hero.get_speed_x();
    float r = hero.get_radius();
    if ((hero.get_z() + r >= 200.0 && spz > 0) || (hero.get_z() - r <= -200.0 && spz < 0))
    {
        float dir[] = {0.0, 0.0, 1.0};
        hero.force(dir, spz * 0.8);
    }
    if ((hero.get_x() + r >= 200.0 && spx > 0) || (hero.get_x() - r <= -200.0 && spx < 0))
    {
        float dir[] = {1.0, 0.0, 0.0};
        hero.force(dir, spx * 0.8);
    }
}

void caldo()
{
    if (hero.check_lost(onda.get_x(), onda.get_z()))
        perder();
}

void perder()
{
    lost = 1;
}

void pontuar()
{
    pontos += 1;
    points_to_char(pontos, spontos);
    tesouro.respawn();
    onda.get_faster();
}

void pegar_tesouro()
{
    float hx = hero.get_x(), hz = hero.get_z();
    float tx = tesouro.get_x(), tz = tesouro.get_z();
    if (hx <= tx + 5 && hx >= tx - 5)
        if (hz <= tz + 5 && hz >= tz - 5)
        {
            hero.grab_treasure();
        }
}

void soltar_tesouro()
{
    if (hero.is_carrying_treasure())
    {
        hero.drop_treasure();
        float tx = tesouro.get_x();
        float tz = tesouro.get_z();
        if (tx >= 186 && tx <= 193)
            if (tz >= -193 && tz <= -186)
                pontuar();
    }
}

void gerenciarMovimentos(int v)
{
    if (_up)
        hero.move_forward(psi);
    if (_down)
        hero.move_back(psi);
    if (_left)
        hero.move_left(psi);
    if (_right)
        hero.move_right(psi);
    if (_jump)
        hero.jump();
    if (_increase)
        hero.increase();
    if (_decrease)
        hero.decrease();
    glutWarpPointer(window_width / 2, window_height / 2);
    glutPostRedisplay();
}

void manageKeyboardCharDOWN(unsigned char key, int x, int y)
{
    if (paused || lost)
        return;
    if (key == 'w')
        _up = 1;
    if (key == 's')
        _down = 1;
    if (key == 'a')
        _left = 1;
    if (key == 'd')
        _right = 1;
    if (key == '+')
        _increase = 1;
    if (key == '-')
        _decrease = 1;
    if (key == ' ')
        _jump = 1;
    glutPostRedisplay();
}

void manageKeyboardCharUP(unsigned char key, int x, int y)
{
    if (key == 'p')
    {
        if (paused)
            despausar();
        else
            pausar();
    }
    if (paused || lost)
        return;
    if (key == 'w')
        _up = 0;
    if (key == 's')
        _down = 0;
    if (key == 'a')
        _left = 0;
    if (key == 'd')
        _right = 0;
    if (key == '+')
        _increase = 0;
    if (key == '-')
        _decrease = 0;
    if (key == ' ')
        _jump = 0;
    if (key == 27)
        exit(1);
}

void mouseFunc(int button, int state, int x, int y)
{
    int fs_x = 0, fs_y = 0, fs_ex = 0;
    if (full_screen)
    { // Ajuste das coordenadas dos botões na tela cheia.
        x -= 110;
        y -= 35;
        fs_x = 50;
        fs_y = -5;
        fs_ex = 10;
    }
    if (lost)
    {
        if (x >= 450 && x <= 650 + fs_x)
        {
            if (y >= 350 + fs_y && y <= 400 + fs_y)
                reiniciar();
            else if (y >= 410 + fs_ex && y <= 460 + fs_ex)
                exit(0);
        }
    }
    if (!paused)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            pegar_tesouro();
        }
        if (button == GLUT_RIGHT_BUTTON)
        {
            soltar_tesouro();
        }
        if (button == 3 && camera_distance >= 10.0 + hero.get_radius())
            camera_distance -= 2.0;
        else if (button == 4 && camera_distance <= 60.0)
            camera_distance += 2.0;
    }
    else
    {
        if (x >= 450 && x <= 650 + fs_x)
        {
            if (y >= 230 + fs_y && y <= 280 + fs_y)
                despausar(); // CONTINUAR
            else if (y >= 290 + fs_y && y <= 340 + fs_y)
                reiniciar();
            else if (y >= 350 + fs_y && y <= 400 + fs_y)
                tela_cheia();
            else if (y >= 410 + fs_ex && y <= 460 + fs_ex)
                exit(0);
        }
    }
}

void manageMouseRotation(int x, int y)
{
    if (paused)
        return;
    psi += (mouse_x - x) * rotation_speed;
    theta += (mouse_y - y) * rotation_speed;
    if (theta >= 1.0)
        theta = 1.0;
    if (theta <= -89.0)
        theta = -89.0;
    if (psi >= 360.0)
        psi -= 360.0;
    if (psi <= 0.0)
        psi += 360.0;
}

void ParametrosIluminacao(void)
{
    /* Parâmetros para a Luz GL_LIGHT0 sendo uma fonte de Luz Pontual */
    GLfloat luzAmbiente[4] = {0.0, 0.0, 0.0, 1.0};  /* cor azul */
    GLfloat luzDifusa[4] = {1.0, 1.0, 1.0, 1.0};    /* cor branca */
    GLfloat luzEspecular[4] = {1.0, 1.0, 1.0, 1.0}; /* cor branca - brilho */
    GLfloat posicaoLuz[4] = {0.0, 50.0, 0.0, 1.0};  /* Fonte de Luz Pontual */
    /* Define os parâmetros da luz de número 0 (Luz Pontual) */
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
}

// x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ x