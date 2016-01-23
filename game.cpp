#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "globals.h"
#include "board.cpp"
//#include "statistics.cpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

//float last_position_x = rectangle_cx;
//float last_position_y = rectangle_cy+1+0.5;

struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

GLuint programID;
board myboard;
ball scoop, movobs[3];
rectangle stand, obst[4], speedBar[11];
int lives = 3;
float zoom = 1;
float pan = 0;

/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* get angle between object's velocity and horizontal 
double getTheta()
{
  theta = atan2(vy, vx);
  theta = (theta*(180))/PI;
  return theta;
}*/

/*get distance between two points (x1,y1) and (x2,y2)*/
float getDIst(float x1, float y1, float x2, float y2)
{
  float tmp;
  tmp = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
  return tmp; 
}

/*get velocity of the ball*/
/*void getballVelocity()
{
  ball_velocity = sqrt(vx*vx + vy*vy);
}*/


/*update com[onents of velocity upon collision
void updateVelocityComponents(ball Ball, float newVelocity, double angle)
{
  Ball.velocity = newVelocity;
  vx = ball_velocity*cos(DEG2RAD(angle));
  vy = ball_velocity*sin(DEG2RAD(angle));
  vox = vx;
  voy = vy;
 // cout<<"angle: "<<angle<<" "<<"new velocity: "<<ball_velocity<<endl;
  //cout<<"x: "<<vox<<" "<<"y: "<<voy<<endl;
  Ball.setinitTime();
 // ball_Inittime = glfwGetTime();
}
*/

/*set positions on change of ball's state*/
/*void resetLastPosition()
{
  last_position_x = last_position_x + ball_shift_dist_x;
  last_position_y = last_position_y + ball_shift_dist_y;
  ball_shift_dist_x = 0;
  ball_shift_dist_y = 0;
}
*/
/*Update ball's position as per velocity
void updateBallPosition()
{
  double res ;

  double current_time, timeElapsed;
  current_time = glfwGetTime();
  timeElapsed = current_time - ball_Inittime;

  if(ball_velocity==0)
  {
    resetLastPosition();
  }
  else
  {
    res = exp (-1*((gravity*timeElapsed)/vt)); 
    ball_shift_dist_x = ((vox*vt)/gravity)*(1 - res);
    ball_shift_dist_y = ((voy+vt)*(vt/gravity))*(1-res) - (vt*timeElapsed);
    if(ball_shift_dist_y + last_position_y <=-10)
    {
      resetLastPosition();
      friction_init_time =current_time;
      friction = 1;
      velocityFriction = vx;
    }
  }
} */




/*void checkMovableCollision(ball Tool, ball Target)
{
  float alpha, theta, v;
  alpha = getAngle(Tool.basePositionX + Tool.translateX, Tool.basePositionY + Tool.translateY, 
      Target.basePositionX + Target.translateX, Target.basePositionY + Target.translateY);

  theta = Tool.getTheta();
  theta = DEG2RAD(theta);

  if(theta < PI/2 + alpha && theta > -1*(PI/2) + alpha)
  {
    

  }

}*/

/*update velocity as per gravity and air friction
void updateBallVelocity()
{-
  double res;
  double current_time, timeElapsed;
  current_time = glfwGetTime();
  timeElapsed = current_time - ball_Inittime;

  res = exp ((-1*gravity*timeElapsed)/vt);
  if(ball_move_flag)
  {
    vx = vox*res;
    vy = voy*res - vt*(1-res);
  }
} 
*/

/* Render the VBOs handled by VAO */
void draw3DObject (board::VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_L:
              if(canon_shift_dist_x > -2)
              {
                canon_shift_dist_x -= 0.25; 
                scoop.setbasePositionX(-0.25);
                stand.rectTranslateX(-0.25);
             //   last_position_x = rectangle_cx + canon_shift_dist_x;
              }               
             break;

            case GLFW_KEY_R:
              if(canon_shift_dist_x < 20)
              {
                canon_shift_dist_x += 0.25;
                scoop.setbasePositionX(0.25);
                stand.rectTranslateX(0.25);

               // last_position_x = rectangle_cx + canon_shift_dist_x;
              }  
              break;

            case GLFW_KEY_RIGHT:
              pan += 1;

            break;

            case GLFW_KEY_LEFT:
              pan -= 1;

            break;

            case GLFW_KEY_UP:
              zoom -= 0.1;
            //  cout<<"up\n";
              break;

            case GLFW_KEY_DOWN:
              zoom += 0.1;
              break;

            case GLFW_KEY_F:
              if(scoop.velocity<10)
                scoop.velocity += 1;
              break;

            case GLFW_KEY_S:
              if(scoop.velocity>=1)
                scoop.velocity -= 1;
              break;

            case GLFW_KEY_SPACE:
              if(scoop.moveFlag==0)
              {
                //resetLastPosition();
                //  vox = ball_velocity*cos(DEG2RAD(theta));
                 // voy = ball_velocity*sin(DEG2RAD(theta));
                  scoop.resetPosition();
                  scoop.moveFlag = 1;
                  scoop.setinitTime();
                 // ball_Inittime = glfwGetTime();
              }
              break;

            /* theta is not in radians anywhere*/
            case GLFW_KEY_A:
                if(arrow_rotation < 45)
                  arrow_rotation += 5;
                scoop.angle = arrow_rotation + 45;
                break;

            case GLFW_KEY_B:
                if(arrow_rotation > -45)
                  arrow_rotation -= 5;
                scoop.angle = arrow_rotation + 45;
                break;
            default: 
                break;
        }
    }
    else if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                quit(window);
                break;
            default:
                break;
        }
    }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
	switch (key) {
		case 'Q':
		case 'q':
            quit(window);
            break;
		default:
			break;
	}
}

/* Executed when a mouse button is pressed/released */
void mouseButton (GLFWwindow* window, int button, int action, int mods)
{
    switch (button) {
/*        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_RELEASE)
                triangle_rot_dir *= -1;
            break; */
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS) {
                pan 
            }
            break;
        default:
            break; 
    }
}

void scrollCallback(GLFWwindow* window, double x, double y)
{
    zoom += float(y)/4;

    if(zoom < 0)
      zoom = 0;
}
 
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	  GLfloat fov = 90.0f;
	  glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);
    // Ortho projection for 2D views
}

board::VAO *triangle, *rect , *circle, *arrow, *ground, *obstacle[10], *obsMovable, *speed_bar[11];

void draw ()
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (programID);
  Matrices.projection = glm::ortho(zoom*(-12.0f + pan), zoom*(12.0f + pan), zoom*(-12.0f), zoom*(12.0f), 0.1f, 500.0f);
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  glm::vec3 target (0, 0, 0);
  glm::vec3 up (0, 1, 0);
}

void drawRectangle (rectangle Rect, board::VAO *rectan)
 {
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;	// MVP = Projection * View * Model

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(Rect.cenx + Rect.translateX, Rect.ceny + Rect.translateY , 0));        // glTranslatef
  Matrices.model *= (translateRectangle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(rectan);
}

/*void drawSquare (float cenx, float ceny, board::VAO *sqr)
 {
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;  // MVP = Projection * View * Model

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(cenx, ceny, 0));        // glTranslatef
  Matrices.model *= (translateRectangle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(sqr);
}
*/

/* function to draw static obstacles on the board */
/*void drawObstacle (rectangle Rect, board::VAO *obs)
 {
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP; 

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(Rect.cenx, Rect.ceny, 0));        
  Matrices.model *= (translateRectangle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(obs);
}*/

/* ball object */
void drawCircle (board::VAO *circl, ball Ball)
{
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); 
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP; 

  /* forming circle */
  Matrices.model = glm::mat4(1.0f);
//  glm::mat4 translateCircle = glm::translate (glm::vec3(last_position_x + ball_shift_dist_x, last_position_y + ball_shift_dist_y, 0));        // glTranslatef
  glm::mat4 translateCircle = glm::translate (glm::vec3(Ball.basePositionX + Ball.translateX, Ball.basePositionY + Ball.translateY, 0));        // glTranslatef
  Matrices.model *= (translateCircle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]); 
  draw3DObject(circl);

}

/*void drawMovableObs (board::VAO *circl, ball Ball)
{
  
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); 
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP; 

  // forming circle 
  Matrices.model = glm::mat4(1.0f);

//  ball_cx_curr = last_position_x + ball_shift_dist_x;
//  ball_cy_curr = last_position_y + ball_shift_dist_y;
  glm::mat4 translateCircle = glm::translate (glm::vec3(Ball.basePositionX + Ball.translateX, Ball.basePositionY + Ball.translateY, 0));        // glTranslatef
  Matrices.model *= (translateCircle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]); 
  draw3DObject(circl);
}
*/

void drawArrow (ball Ball)
{

  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateArrow = glm::translate (glm::vec3(Ball.cenx + canon_shift_dist_x, Ball.ceny + canon_shift_dist_y, 0));        // glTranslatef
  glm::mat4 rotateArrow = glm::rotate((float)(DEG2RAD(arrow_rotation)), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
  glm::mat4 arrowTransform = translateArrow * rotateArrow;
  Matrices.model *= ( arrowTransform ); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(arrow);

}

void drawGround ()
{

  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;

   /* forming arrow*/
  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateArrow = glm::translate (glm::vec3(0, -10, 0));        
  glm::mat4 arrowTransform = translateArrow ;

  Matrices.model *= ( arrowTransform ); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(ground);

}


float getAngle(float x1, float y1, float x2, float y2)
{
  return atan2(y2-y1, x2-x1);
}

void checkMovableCollision()
{
//  cout<<"entered movable collision\n";
  float alpha, theta, v;

  scoop.setTheta();
  alpha = getAngle(scoop.basePositionX + scoop.translateX, scoop.basePositionY + scoop.translateY, 
      movobs[0].basePositionX + movobs[0].translateX, movobs[0].basePositionY + movobs[0].translateY);

  theta = scoop.getTheta();
  theta = DEG2RAD(theta);



  /* condition for collision */
  if(theta < PI/2 + alpha && theta > -1*(PI/2) + alpha)
  {
   //  cout<<"condition check\n";
  //  cout<<"dist: "<<getDIst(scoop.getCurrCoordinatesX(), scoop.getCurrCoordinatesY(), movobs[0].getCurrCoordinatesX(), movobs[0].getCurrCoordinatesY())<<endl;
   // cout<<"r: "<<scoop.rad+movobs[0].rad<<endl;

    // detect collision
    if(getDIst(scoop.getCurrCoordinatesX(), scoop.getCurrCoordinatesY(), movobs[0].getCurrCoordinatesX(), movobs[0].getCurrCoordinatesY()) <= scoop.rad + movobs[0].rad)
    {

      cout<<"collision detected\n";
      //cout<<scoop.vx<<" "<<scoop.vy<<endl;
     
   //   cout<<"alpha: "<<(180*alpha)/PI<<" "<<"tehta: "<<(theta*180)/PI<<endl;
      cout<<(180*(theta-alpha))/PI<<endl;
      scoop.resetPosition();
      movobs[0].resetPosition();

      scoop.setTheta();
  //    cout<<"scoop angle: "<<scoop.angle<<endl;
    //  cout<<"scoop vel before: "<<scoop.vx<<" "<<scoop.vy<<endl;


      scoop.velocity = abs(scoop.velocity*(sin(DEG2RAD(theta-alpha))));
      movobs[0].velocity = abs(scoop.velocity*(cos(DEG2RAD(theta-alpha))));

//      cout<<"scoop velocity total: "<<scoop.velocity<<endl;
      scoop.updateVelocityComponents(scoop.velocity, (180*(PI/2 + alpha))/PI);
      movobs[0].updateVelocityComponents(movobs[0].velocity, (180*alpha)/PI);

  //    cout<<"scoop vel: "<<scoop.vx<<" "<<scoop.vy<<endl;
    // cout<<"mov vel: "<<movobs[0].vx<< " "<<movobs[0].vy<<endl;

    //  scoop.angle = (180*(PI/2 + alpha))/PI;
     // movobs[0].angle = (180*alpha)/PI;

      // check for friction for both scoop and movobs[0]
      if(!scoop.checkFriction())
      {
        scoop.updateBallPosition();
        if(scoop.vx<0)
          scoop.translateX -= 0.2;
        else
          scoop.translateX += 0.2;
        if(scoop.vy>0)
          scoop.translateY += 0.2;
        else 
          scoop.translateY -= 0.2;
        scoop.resetPosition();
      }
      else
        cout<<"scoop: friction\n";
        // frictional motion for movobs[0] obstacle
        cout<<"movobs[0]: "<<movobs[0].checkFriction()<<" "<<movobs[0].vx<< endl;
    }
  
  }

}

/*check collision with the obstacles*/
void checkCollision()
{
  float x, y, r, e, vel, angleDash, alpha, theta;
  double angle;
  int i;
  pair<float, float> p, q;
  for( i=0; i<Obstacles.size(); i++)
  {
    p = Obstacles[i].first; // center coordinates
    x = p.first;
    y = p.second;
    q = Obstacles[i].second; // 'e: restitution' and radius
    r = q.second;
    e = q.first;

   // cout<<"r: "<<r<<"dist: "<<getDIst(scoop.basePositionX + scoop.translateX, scoop.basePositionY + scoop.translateY, x, y)<<endl;
    if(getDIst(scoop.basePositionX +scoop.translateX, scoop.basePositionY + scoop.translateY, x, y) <= r)
    {

      cout<<"collided  with obstacle no. "<<i+1<< " ";
      scoop.resetPosition();
   //   cout<<"vx: "<<scoop.vx<< " "<<"vy: "<<scoop.vy<<endl;
      scoop.angle = scoop.getTheta();
//      cout<<"angle: "<<scoop.angle<<endl;
      //if(vx>0)
    /*  if(scoop.vx!=0)
        scoop.angle = 180 + scoop.angle;
//      else if(vx==0)
      else
        scoop.angle = -1*(scoop.angle); */

      theta = scoop.getTheta();
      theta = DEG2RAD(theta);
      alpha = getAngle(scoop.basePositionX + scoop.translateX, scoop.basePositionY + scoop.translateY, x, y);
      
      cout<<(180*theta)/PI<<" "<<(180*alpha)/PI<<endl;
     // cout<<"sv: "<<scoop.velocity<<endl;
      vel = getDIst(scoop.velocity*sin(theta-alpha), (e*scoop.velocity)*cos(theta-alpha), 0, 0);
   //   cout<<"comp1: "<<scoop.velocity*sin(theta-alpha)<<" "<<(scoop.velocity)*cos(theta-alpha)<<endl;

    //  cout<<"comp2: "<<scoop.velocity*sin(theta-alpha)<<" "<<(e*scoop.velocity)*cos(theta-alpha)<<endl;
     // cout<<"snv vel: "<<vel<<endl;
      angleDash = atan2(e, tan(theta-alpha));

      scoop.updateVelocityComponents(vel, angleDash);
      cout<<"updated velocity: "<<"vx: "<<scoop.vx<<" "<<"vy: "<<scoop.vy<<endl;
      scoop.updateBallPosition();
      if(scoop.vx<0)
        scoop.translateX -= 0.2;
      else
        scoop.translateX += 0.2;
      if(scoop.vy>0)
        scoop.translateY += 0.2;
      else 
        scoop.translateY -= 0.2;
      scoop.resetPosition();
   //   cout<<scoop.basePositionX<<" "<<scoop.basePositionY<<endl;
   //   drawCircle(circle, scoop);

       // cout<<"translate "<<scoop.translateX<<" "<<scoop.translateY<<endl;
      
    }
  }
}



/* create speed bar */
void CreateSpeedBar()
{

   for(int i=1; i<11; i++)
   {
      speedBar[i].initRect(1, 1.5, -10, 0.7*(i+1), 0.9);
      speedBar[i].initRectColor(float(i)/10, 1-(float(i)/10), 0);
      speed_bar[i-1] = myboard.createRectangle(speedBar[i]);
    }
}

/* displaying speed bar */
void DrawSpeedBar (float velocity) 
{
   for(int i=0; i< int(velocity); i++)
   {
      drawRectangle(speedBar[i+1], speed_bar[i]);
   }
} 


GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; 

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Flying Scoop", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);
    glfwSetWindowCloseCallback(window, quit);
    glfwSetKeyCallback(window, keyboard);      
    glfwSetCharCallback(window, keyboardChar); 
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButton);  

    return window;
}

/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */
 // ball_velocity = 5;
  stand.initRect(2, 2, -9, -9, 1.44);
  stand.initRectColor(1, 0, 0);
  stand.resetTranslateRect();
	rect = myboard.createRectangle (stand);
  Obstacles.push_back(make_pair(make_pair(-9, -9), make_pair(0.5, 1.44)));

 // ball_radius = 0.5;
 // ball_cx = -9;
  //ball_cy = -7.5;
  scoop.initBallCentre(-9, -7.5, 0.5, 1);
  scoop.initBallColor(0, 0, 1);
  scoop.initVelocity(5, 45);
  
// creating movable obstacles
  movobs[0].initBallCentre( 0, -9, 1, 1);
  movobs[0].initBallColor(1, 0.5, 0);
  movobs[0].initVelocity(0, 0);

  //circle = myboard.createCircle (scoop.rad, scoop.bColor[0], scoop.bColor[1], scoop.bColor[2]);
  circle = myboard.createCircle (scoop);
  obsMovable = myboard.createCircle(movobs[0]);

  arrow = myboard.createArrow ();
  ground = myboard.createGround ();
 // obsMovable = myboard.createCircle (1, 1, 0.75, 0);


  CreateSpeedBar();
  /*speed bar
  for(int i=1; i<11; i++)
  {
    speed_bar[i-1] = myboard.createRectangle(1, 1.5, float(i)/10, 1-(float(i)/10), 0);
  } */

  /*creating obstacles on the board*/
  //for(int i=0; i<3; i++)
  //{


    // obstacles creation
    obst[1].initRect(1, 4, -6.5, -8, 0.5);
    obst[1].resetTranslateRect();
    obst[1].initRectColor(1, 0.5, 0.5);

    obst[2].initRect(3, 6, 3.5, -7, 0.5);
    obst[2].resetTranslateRect();
    obst[2].initRectColor(0.5, 1, 0.5);

    obstacle[1] = myboard.createObstacle( obst[1], 0.5);
    obstacle[2] = myboard.createObstacle( obst[2], 0.5);
	

    programID = LoadShaders( "game.vert", "game.frag" );
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

  scoop.updateVelocityComponents(scoop.velocity, 45);
	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

/* initializing friction */
/*void initFriction()
{
  double timeElapsed;
  timeElapsed = glfwGetTime() - friction_init_time;

  vx = velocityFriction - (frictionCoefficient*(gravity*timeElapsed));

  if(vx>threshold_velocity)
    ball_shift_dist_x = velocityFriction*timeElapsed - ((timeElapsed*timeElapsed)*(gravity*frictionCoefficient))/2; 
} */
 
int main (int argc, char** argv)
{
	int width = 960;
	int height = 690;
  pair<float, float> p;

  GLFWwindow* window = initGLFW(width, height);

	initGL (window, width, height);

    double last_update_time = glfwGetTime(), current_time;

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // OpenGL Draw commands

   //    if(!scoop.moveFlag)
          glfwPollEvents();

        draw();
        drawGround();
        drawRectangle(stand, rect);


        /*ensure that there is still life*/
        if(lives>0 && scoop.checkLife()==0)
        {
          if(scoop.moveFlag)
          scoop.updateBallVelocity();

        if(scoop.moveFlag==0)
          scoop.updateVelocityComponents(scoop.velocity, scoop.angle);
       
        scoop.setballVelocity();

        /* draw obstacles on the board */
        for(int i=1 ; i<Obstacles.size(); i++)
        {
         // p = Obstacles[i].first;         
          drawRectangle(obst[i], obstacle[i]);
        }


        /*displaying speed bar
        for(int i=0; i< int(ball_velocity); i++)
        {
          drawSquare(-10, 0.7*(i+1), speed_bar[i]);
        } */

        DrawSpeedBar(scoop.velocity);

        drawCircle(circle, scoop);
        drawCircle(obsMovable, movobs[0]);
      //  drawMovableObs (obsMovable, -1, -9);
        if(scoop.moveFlag==0 && turn==0)
          drawArrow(scoop);

        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
      

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.01 && scoop.basePositionX + scoop.translateX < 12) {

        if(scoop.moveFlag)
          checkCollision();

        if(scoop.moveFlag)
          checkMovableCollision();

      //  if(last_position_x + ball_shift_dist_x<12)
       //   drawCircle(circle);  

         scoop.checkFriction();


          /* move the ball */
           if(scoop.moveFlag==1 && scoop.frictionFlag ==0 )
            {
//            cout<<"bal movement\n";
              scoop.updateBallPosition();
            }
          /* applying ground friction on the ball*/ 
//           cout<<"ball: "<<scoop.frictionFlag<<" "<<scoop.vx<<endl;

           if(scoop.frictionFlag==1 && abs(scoop.vx)>0.05)
           {
          //    cout<<"ball friction\n";
              scoop.initFriction();
            //  cout<<scoop.vx<<endl;
           }

 //          cout<<"movobs: "<<movobs[0].frictionFlag<<" "<<movobs[0].vx<<endl;
           if(movobs[0].frictionFlag==1 && abs(movobs[0].vx)>0.05)
           {
             // cout<<"movable obstacle friction\n";
              movobs[0].initFriction();
             // cout<<movobs[0].vx<<endl;
           }

            last_update_time = current_time;
        }
      }
      else if(lives>0)
      {
        lives -= 1;
        stand.resetTranslateRect();
        scoop.resetBall();
        scoop.initVelocity(5, 45);
        movobs[0].resetBall();
        movobs[0].initBallCentre( 0, -9, 1, 1);
        movobs[0].initBallColor(1, 0.5, 0);
        movobs[0].initVelocity(0, 0);
        turn = 0;
        canon_shift_dist_x = 0;
        canon_shift_dist_y = 0;
        arrow_rotation = 0;
      }
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}