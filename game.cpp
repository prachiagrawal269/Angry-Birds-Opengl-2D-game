#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "globals.h"
#include "board.cpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

struct GLMatrices {
	glm::mat4 projection;
	glm::mat4 model;
	glm::mat4 view;
	GLuint MatrixID;
} Matrices;

GLuint programID;
board myboard;

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

/* Executed when a regular key is pressed/released/held-down */
/* Prefered for Keyboard events */
void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
     // Function is called first on GLFW_PRESS.

    if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_LEFT:
                if(canon_shift_dist_x > -2)
                {
                  canon_shift_dist_x -= 0.25;
                //  box_shift_dist_x -= 0.25;
                }
                break;

            case GLFW_KEY_RIGHT:
              if(canon_shift_dist_x < 20)
              {
                canon_shift_dist_x += 0.25;
              //  ball_shift_dist_x += 0.25;
              }  
              break;

            case GLFW_KEY_F:
              if(ball_velocity<5)
                ball_velocity += 1;
              break;

            case GLFW_KEY_S:
              if(ball_velocity)
                ball_velocity -= 1;
              break;

            case GLFW_KEY_SPACE:
              if(ball_move_flag==0)
              {
                  vox = ball_velocity*cos(DEG2RAD(arrow_rotation));
                  voy = ball_velocity*sin(DEG2RAD(arrow_rotation));
                  ball_move_flag = 1;
                  ball_Inittime = glfwGetTime();
              }
              break;

            case GLFW_KEY_A:
                if(arrow_rotation < 85)
                  arrow_rotation += 5;
                break;

            case GLFW_KEY_B:
                if(arrow_rotation > 5)
                  arrow_rotation -= 5;
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
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_RELEASE)
                triangle_rot_dir *= -1;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_RELEASE) {
                rectangle_rot_dir *= -1;
            }
            break;
        default:
            break;
    }
}


/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=width, fbheight=height;

    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	  GLfloat fov = 90.0f;

	// sets the viewport of openGL renderer
	  glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);
    // Ortho projection for 2D views
    Matrices.projection = glm::ortho(-12.0f, 12.0f, -12.0f, 12.0f, 0.1f, 500.0f);
}

board::VAO *triangle, *rectangle, *circle, *arrow, *ground, *obstacle;

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw ()
{
  // clear the color and depth in the frame buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // use the loaded shader program
  // Don't change unless you know what you are doing
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (0, 0, 0);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);

}
  void drawRectangle ()
 {
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;	// MVP = Projection * View * Model

  // Load identity to model matrix

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(rectangle_cx + canon_shift_dist_x, rectangle_cy + canon_shift_dist_y , 0));        // glTranslatef
 /* glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
//  translateRectangle = glm::translate (glm::vec3(-9 + ball_shift_dist_x, -9 + ball_shift_dist_y, 0));
  Matrices.model *= (translateRectangle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix */
  draw3DObject(rectangle);
}

void drawObstacle (float cenx, float ceny)
 {
  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;  // MVP = Projection * View * Model

  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateRectangle = glm::translate (glm::vec3(cenx, ceny, 0));        // glTranslatef
 /* glm::mat4 rotateRectangle = glm::rotate((float)(rectangle_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
//  translateRectangle = glm::translate (glm::vec3(-9 + ball_shift_dist_x, -9 + ball_shift_dist_y, 0));
  Matrices.model *= (translateRectangle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix */
  draw3DObject(obstacle);
}

void drawCircle ()
{
 Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;  // MVP = Projection * View * Model

  /* forming circle */
  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateCircle = glm::translate (glm::vec3(rectangle_cx +  canon_shift_dist_x + ball_shift_dist_x + friction_dist, ball_cy + ball_shift_dist_y, 0));        // glTranslatef
 // glm::mat4 rotateRectangle = glm::rotate((float)(arrow_rotation*M_PI/180.0f), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
  Matrices.model *= (translateCircle); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]); 

  // draw3DObject draws the VAO given to it using current MVP matrix */
  draw3DObject(circle);
}

void drawArrow ()
{

  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;

   /* forming arrow*/
  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateArrow = glm::translate (glm::vec3(ball_cx + canon_shift_dist_x, ball_cy + canon_shift_dist_y, 0));        // glTranslatef
 // glm::mat4 translateArrow = glm::translate (glm::vec3(ball_cx, ball_cy, 0)); 
  glm::mat4 rotateArrow = glm::rotate((float)(DEG2RAD(arrow_rotation -45)), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
//  translateArrow = glm::translate (glm::vec3(-1*(ball_cx + ball_shift_dist_x), -1*(ball_cy + ball_shift_dist_y), 0));

 // glm::mat4 translateArrow = glm::translate (glm::vec3(ball_shift_dist_x, ball_shift_dist_y, 0)); 
  glm::mat4 arrowTransform = translateArrow * rotateArrow;

  Matrices.model *= ( arrowTransform ); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix */
  draw3DObject(arrow);

}

void drawGround ()
{

  Matrices.view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0)); // Fixed camera for 2D (ortho) in XY plane
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;

   /* forming arrow*/
  Matrices.model = glm::mat4(1.0f);

  glm::mat4 translateArrow = glm::translate (glm::vec3(0, -10, 0));        // glTranslatef
 // glm::mat4 translateArrow = glm::translate (glm::vec3(ball_cx, ball_cy, 0)); 
 // glm::mat4 rotateArrow = glm::rotate((float)(DEG2RAD(arrow_rotation -45)), glm::vec3(0,0,1)); // rotate about vector (-1,1,1) */
//  translateArrow = glm::translate (glm::vec3(-1*(ball_cx + ball_shift_dist_x), -1*(ball_cy + ball_shift_dist_y), 0));

 // glm::mat4 translateArrow = glm::translate (glm::vec3(ball_shift_dist_x, ball_shift_dist_y, 0)); 
  glm::mat4 arrowTransform = translateArrow ;

  Matrices.model *= ( arrowTransform ); 
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

  // draw3DObject draws the VAO given to it using current MVP matrix */
  draw3DObject(ground);

}

/* Initialise glfw window, I/O callbacks and the renderer to use */
/* Nothing to Edit here */
GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Angry Birds", NULL, NULL);

    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
    /* Objects should be created before any other gl function and shaders */

	rectangle = myboard.createRectangle (2, 2, 1, 0, 0);
//  cout<<"created rectangle\n";

  ball_radius = 0.5;
  ball_cx = -9;
  ball_cy = -7.5;
  circle = myboard.createCircle (ball_radius, ball_cx, ball_cy);
  arrow = myboard.createArrow ();
  ground = myboard.createGround ();
 	
  obstacle = myboard.createObstacle(1, 4, 1, 0, 1);
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders( "game.vert", "game.frag" );
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

	reshapeWindow (window, width, height);

    // Background color of the scene
	glClearColor (0.0f, 0.0f, 0.0f, 0.0f); // R, G, B, A
//	glClearDepth (1.0f);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

// ensures that ball and canon are together always
/*void ballAndCanon()
{
  if(ball_move_flag==0)
  {
  //  ball_shift_dist_x = canon_shift_dist_x;
    //ball_shift_dist_y = canon_shift_dist_y;
  }

}*/

/* updating ball's position under gravity and air friction*/
void updateBallPosition()
{
  double res ;

  double current_time, timeElapsed;
  current_time = glfwGetTime();
  timeElapsed = current_time - ball_Inittime;

  if(ball_velocity==0)
  {
    ball_shift_dist_x = 0;
    ball_shift_dist_y = 0;
  }
  else
  {
    res = exp (-1*((gravity*timeElapsed)/vt)); 
    
  //ball_shift_dist_x = ball_velocity*(timeElapsed)*(cos(arrow_rotation));  

  ball_shift_dist_x = ((vox*vt)/gravity)*(1 - res);

 // ball_shift_dist_y = ball_velocity*(timeElapsed)*(sin(arrow_rotation));

//  ball_shift_dist_y -= (0.5)*gravity*(timeElapsed*(timeElapsed));
  ball_shift_dist_y = ((voy+vt)*(vt/gravity))*(1-res) - (vt*timeElapsed);

  /* ball touches the ground*/
  if(ball_shift_dist_y + ball_cy <=-10)
  {
    friction_init_time =current_time;
    friction = 1;
    velocityFriction = vx;
  }
  }

}

/* updating ball velocity under gravity and air friction*/
void updateBallVelocity()
{
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

/* initializing friction */
void initFriction()
{
  double timeElapsed;
  timeElapsed = glfwGetTime() - friction_init_time;

  vx = velocityFriction - (frictionCoefficient*(gravity*timeElapsed));

  if(vx>threshold_velocity)
    friction_dist = velocityFriction*timeElapsed - ((timeElapsed*timeElapsed)*(gravity*frictionCoefficient))/2; 
}



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
       // ballAndCanon();
        draw();
        drawGround();
        drawRectangle();
        updateBallVelocity();
        p = Obstacles[0].first;
       // cout<<"p"<<endl;
        //cout<<p.first<<" "<<p.second<<endl;
        drawObstacle(p.first, p.second);

      //  if(ball_velocity>threshold_velocity || ball_move_flag==0)
          drawCircle();
        if(ball_move_flag==0)
          drawArrow();

        // Swap Frame Buffer in double buffering
        glfwSwapBuffers(window);

        // Poll for Keyboard and mouse events
        glfwPollEvents();

        // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
        current_time = glfwGetTime(); // Time in seconds
        if ((current_time - last_update_time) >= 0.000001) {

          /* move the ball */
           if(ball_move_flag==1 && friction==0 )
                updateBallPosition();

          /* applying ground friction on the ball*/
           if(friction && vx>0)
              initFriction();
           
            last_update_time = current_time;
        }
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}