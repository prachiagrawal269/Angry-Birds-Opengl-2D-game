/* file containing all the required header files */
#ifndef globals_h
#define globals_h

#include <bits/stdc++.h>
using namespace std;

#define GLM_FORCE_RADIANS
#define DEG2RAD(deg) (deg * PI / 180)
#define PI 3.141592653589

// globals variables
/*float friction = 0;
float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true; */
float canon_shift_dist_x = 0;
float canon_shift_dist_y = 0;
/*float ball_shift_dist_x = 0;
float ball_shift_dist_y = 0;
float friction_dist = 0;
float velocityFriction = 0; 
float friction_init_time = 0; */

float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
float arrow_rotation = 0;
float turn = 0;
//double theta = 45;

/*float ball_radius = 0.5;
float ball_cx = -9;
float ball_cy = -7.5;
float ball_cx_curr = -9;
float ball_cy_curr = -7.5;
float rectangle_cx = -9;
float rectangle_cy = -9;

float ball_velocity ; */
float threshold_velocity = 0.25;
/*double ball_Inittime = 0;
bool ball_move_flag = 0; */
float gravity = 2;
float airdragConstant = 0.09;
float frictionCoefficient = 0.5;

float vt = gravity/airdragConstant ;
/*float vox, voy, vx, vy;
*/
/*obstacle's center coordinates, restitution coefficient and its radius*/
vector<pair<pair<float, float>, pair<float, float> > > Obstacles; 

/* functions */
void createObstacle(float l, float b, float c1, float c2, float c3);

/* all bodies have same mass of 1 unit*/
#endif