#include <GL/glut.h>
#include <math.h>
#include <stdlib.h> // rand(), srand()
#include <time.h>   // time()

#define PI 3.14159265358979323846
#define C 57.29577951308232087685

int random_integer(int min, int max)
{
    return (rand() % (max + 1 - min) + min);
}

class Tesouro
{
private:
    float x, y, z, r;

public:
    Tesouro();
    float get_x() { return x; }
    float get_z() { return z; }
    void set_position(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    void respawn();
    void draw();
    void update();
};
Tesouro::Tesouro()
{
    r = 0;
    x = 1;
    y = 2.5;
    z = -1;
    respawn();
}
void Tesouro::update()
{
    r += 1;
    draw();
}
void Tesouro::respawn()
{
    srand(time(NULL));
    x = random_integer(-195, 195);
    y = 2.5;
    z = random_integer(-195, 195);
    while (x > 0 && z < 0)
    {
        x = random_integer(-195, 195);
        z = random_integer(-195, 195);
    }
}
void Tesouro::draw()
{
    float color[] = {1.0, 1.0, 0.0};
    float Ka[] = {0.3, 0.3, 0.0};
    float Ks[] = {1.0, 1.0, 1.0};
    float Ke[] = {0.3, 0.3, 0.0};
    float Ke2[] = {0.0, 0.0, 0.0};
    GLfloat Se[] = {18};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
    glTranslatef(x, y, z);
    glRotatef(r, 0.0, 1.0, 0.0);
    glutSolidCube(2);
    glRotatef(-r, 0.0, 1.0, 0.0);
    glTranslatef(-x, -y, -z);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke2);
}

class Ball
{
private:
    float radius;
    GLfloat color[3];
    float speed_rate, _speed_rate;
    float jump_force;
    float x, y, z;
    float speed_x, speed_y, speed_z;
    float rotation_x;
    float rotation_z;
    float drag_norm;
    float gravity_norm;
    bool carrying_treasure;

public:
    Ball(float r, float c[], float pos[]);
    // Métodos de gerenciamento:
    void set_position(float x_, float y_, float z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }
    void update_position();                      // Continuar movimento da bolinha
    bool check_lost(float wave_x, float wave_z); // Checa se encosta na onda
    void drag_force();                           // Força de arraste
    void gravity();                              // Gravidade
    // void wall_collision(float obj_pos[], float psi); // Colisão com objetos
    void draw();
    float get_x() { return x; }
    float get_y() { return y; }
    float get_z() { return z; }
    float get_speed_x() { return speed_x; }
    float get_speed_y() { return speed_y; }
    float get_speed_z() { return speed_z; }
    float get_radius() { return radius; }
    // Métodos de ação:
    void force(float dir[], float norm); // Força contra a bolinha. Parâmetros (direção[3] e float norma)
    void move_forward(float psi)
    {
        speed_x += move_x(psi);
        speed_z += move_z(psi);
    }
    void move_back(float psi)
    {
        speed_x -= move_x(psi);
        speed_z -= move_z(psi);
    }
    void move_left(float psi)
    {
        speed_x += move_z(psi);
        speed_z -= move_x(psi);
    }
    void move_right(float psi)
    {
        speed_x -= move_z(psi);
        speed_z += move_x(psi);
    }
    void jump(); /*{speed_y+=speed_rate;}*/
    void increase()
    {
        if (radius <= 7.0)
            radius += 0.1;
    }
    void decrease()
    {
        if (radius >= 0.4)
            radius -= 0.1;
    }
    float move_x(float psi) { return sin(psi * PI / 180.0) * speed_rate; }
    float move_z(float psi) { return cos(psi * PI / 180.0) * speed_rate; }
    void grab_treasure();
    void drop_treasure();
    bool is_carrying_treasure() { return carrying_treasure; }
    void get_heavier();
    void get_lighter();
    void stop()
    {
        speed_x = 0.0;
        speed_y = 0.0;
        speed_z = 0.0;
    }
};
Ball::Ball(float r, GLfloat c[], float pos[])
{
    jump_force = 0.75;
    carrying_treasure = 0;
    radius = r;
    speed_rate = 0.02;
    _speed_rate = 0.02;
    color[0] = c[0];
    color[1] = c[1];
    color[2] = c[2];
    set_position(pos[0], pos[1], pos[2]);
    speed_x = 0.0;
    speed_y = 0.0;
    speed_z = 0.0;
    drag_norm = 0.985;
    gravity_norm = 0.02;
    rotation_x = 0.0;
    rotation_z = 0.0;
}
void Ball::grab_treasure()
{
    carrying_treasure = 1;
    get_heavier();
}
void Ball::drop_treasure()
{
    carrying_treasure = 0;
    get_lighter();
}
void Ball::get_heavier()
{
    speed_rate = 0.01;
    jump_force = 0.56;
}
void Ball::get_lighter()
{
    speed_rate = _speed_rate;
    jump_force = 0.75;
}
bool Ball::check_lost(float wave_x, float wave_z)
{
    float z_wave_func = x + wave_z - wave_x; // y_wave = a*x_wave + b ; a = 1, b = y_wave - x_wave
    if (z >= z_wave_func - 5 && z <= z_wave_func + 10 && y <= 6.0)
        return 1;
    else
        return 0;
}
void Ball::update_position()
{
    x += speed_x;
    y += speed_y;
    z += speed_z;
    rotation_x += 18 * PI * speed_x;
    rotation_z += 18 * PI * speed_z;
    drag_force();
    gravity();
    draw();
}
void Ball::drag_force()
{
    speed_x *= drag_norm;
    speed_z *= drag_norm;
    speed_y *= drag_norm;
}
void Ball::gravity()
{
    if (y > radius)
        speed_y -= gravity_norm;
    if (y < radius)
    {
        float direction[] = {0.0, 1.0, 0.0};
        if (speed_y <= -0.1)
            force(direction, speed_y * 0.9);
        y = radius;
    }
}
void Ball::force(float dir[], float norm)
{
    if (dir[0])
        speed_x = -1 * dir[0] * norm;
    if (dir[1])
        speed_y = -1 * dir[1] * norm;
    if (dir[2])
        speed_z = -1 * dir[2] * norm;
}
void Ball::jump()
{
    float dir[] = {0.0, 1.0, 0.0};
    if (y == radius)
        force(dir, -jump_force);
}
void Ball::draw()
{
    glTranslatef(x, y, z);
    float Ka[] = {0.3, 0.3, 0.0};
    float Ks[] = {1.0, 1.0, 1.0};
    float Ke[] = {0.0, 0.0, 0.0};
    GLfloat Se[] = {18};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
    glRotatef(-rotation_x, 0.0, 0.0, 1.0);
    glRotatef(rotation_z, 1.0, 0.0, 0.0);
    GLUquadric *quad;
    quad = gluNewQuadric();
    gluSphere(quad, radius, 30, 30);
    glRotatef(-rotation_z, 1.0, 0.0, 0.0);
    glRotatef(rotation_x, 0.0, 0.0, 1.0);
    glTranslatef(-x, -y, -z);
}

class Wave
{
private:
    float wave_speed;
    float wave_x, wave_y, wave_z;
    GLUquadric *quad;

public:
    Wave();
    void draw();
    void run();
    float get_x() { return wave_x; }
    float get_z() { return wave_z; }
    void get_faster() { wave_speed += 0.4; }
    void restart();
};
Wave::Wave()
{
    quad = gluNewQuadric();
    wave_x = -230.0;
    wave_y = 1.0;
    wave_z = 230.0;
    wave_speed = 0.5;
}
void Wave::restart()
{
    wave_x = -230.0;
    wave_y = 1.0;
    wave_z = 230.0;
    wave_speed = 0.5;
}
void Wave::run()
{
    draw();
    if (wave_x <= 230)
    {
        wave_x += wave_speed;
        wave_z -= wave_speed;
    }
    else
    {
        wave_x = -230.0;
        wave_z = 230.0;
    }
}
void Wave::draw()
{
    float Ka[] = {0.0, 0.0, 0.5};
    float Kd[] = {0.0, 0.0, 0.55};
    float Ks[] = {1.0, 1.0, 1.0};
    float Ke[] = {0.0, 0.0, 0.2};
    GLfloat Se[] = {10};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, Ke);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Se);
    glTranslatef(wave_x - 800, wave_y, wave_z - 800);
    glRotatef(45.0, 0.0, 1.0, 0.0);
    gluCylinder(quad, 5.0, 5.0, 1600, 20, 20);
    glRotatef(-45.0, 0.0, 1.0, 0.0);
    glTranslatef(-wave_x + 800, -wave_y, -wave_z + 800);
}