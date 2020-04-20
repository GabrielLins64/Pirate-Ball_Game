#include <math.h>

# define PI 3.14159265358979323846

float * cameraSpecifications(float camera_distance, float theta, float psi, float hero_x, float hero_y, float hero_z) {
	theta *= PI/180.0;
	psi *= PI/180.0;
	float eye_y = -camera_distance*sin(theta);
	float b = -camera_distance*cos(theta); // Projeção de camera_distance (olho->olhar) no plano XZ
	float eye_x = b*sin(psi) + hero_x;
	float eye_z = b*cos(psi) + hero_z;
	float *cs = new float[3];
	cs[0] = eye_x;
	cs[1] = eye_y;
	cs[2] = eye_z;
	return cs;
}

float moveHeroX(float psi, float hero_speed) {return sin(psi*PI/180.0)*hero_speed;}
float moveHeroZ(float psi, float hero_speed) {return cos(psi*PI/180.0)*hero_speed;}