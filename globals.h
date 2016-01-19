#define GLM_FORCE_RADIANS
#define DEG2RAD(deg) (deg * PI / 180)
#define PI 3.141592653589

// globals variables
float triangle_rot_dir = 1;
float rectangle_rot_dir = 1;
bool triangle_rot_status = true;
bool rectangle_rot_status = true;
float canon_shift_dist_x = 0;
float canon_shift_dist_y = 0;
float ball_shift_dist_x = 0;
float ball_shift_dist_y = 0;

float camera_rotation_angle = 90;
float rectangle_rotation = 0;
float triangle_rotation = 0;
float arrow_rotation = 45;

float ball_radius = 0.5;
float ball_cx = -9;
float ball_cy = -7.5;
float rectangle_cx = -9;
float rectangle_cy = -9;

float ball_velocity = 1;
float threshold_velocity = 0.25;
double ball_Inittime = 0;
bool ball_move_flag = 0;
float gravity = 4;
float airdragConstant = 0.3;

/* all bodies have same mass of 1 unit*/