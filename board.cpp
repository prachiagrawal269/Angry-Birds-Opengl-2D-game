#include "globals.h"

class rectangle
{
public:
    float l;
    //float defaultCx;
   // float defaultCy;
    float b;
    float cenx;
    float ceny;
    float r;
    float translateX;
    float translateY;
    float rColor[3];

    void initRect(float len, float brd, float x, float y, float rad)
    {
      l = len;
      b = brd;
      cenx = x;
      ceny = y;
    //  defaultCx = x;
     // defaultCy = y;
      r = rad;
    //  translateX = 0;
     // translateY = 0;
    }

    void resetTranslateRect()
    {
      translateX = 0;
      translateY = 0;
    }

    void initRectColor(float c1, float c2, float c3)
    {
      rColor[0] = c1;
      rColor[1] = c2;
      rColor[2] = c3;
    }

    void rectTranslateX(float x)
    {
      translateX += x;
    }

    void rectTranslateY(float y)
    {
      translateY += y;
    }

};
//rectangle rect;

class ball
{
public:

    float cenx;
    float ceny;
    float rad;
    float bColor[4];
    float velocity;
    float vox;
    float voy;
    float vx;
    float vy;
    float angle;
    float ax;
    float ay;
    float basePositionX;
    float basePositionY;
    float translateX;
    float translateY;
    bool frictionFlag;
    bool isMovable;
    bool moveFlag;    
    float initTime;

    void initBallCentre(float x, float y, float r, bool move)
    {
      cenx = x;
      ceny = y;
      rad = r;
      translateX = 0;
      translateY = 0;
      basePositionX = cenx;
      basePositionY = ceny;
      moveFlag = 0;
      isMovable = move;
      frictionFlag = 0;
      velocity = 0;
      angle =0;
    }

    void initBallColor(float c1, float c2, float c3)
    {
      bColor[0] = c1;
      bColor[1] = c2;
      bColor[2] = c3;
    }

    void initVelocity(float v, float a)
    {
      velocity = v;
      angle = a;
      vox = v*cos(DEG2RAD(a));
      voy = v*sin(DEG2RAD(a));
      vx = vox;
      vy = voy;
     // ax = aax;
      //ay = aay;
    }

    void setinitTime()
    {
      initTime = glfwGetTime();
    }

    float getinitTime()
    {
      return initTime;
    }
    void resetBall()
    {
      basePositionX = cenx;
      basePositionY = ceny;
      translateX = 0;
      translateY = 0;
    } 

    void setbasePositionX(float x)
    {
      basePositionX += x;
    }

    void setbasePositionY(float y)
    {
      basePositionY += y;
    }

    float getCurrCoordinatesX()
    {
      return basePositionX + translateX;
    }

    float getCurrCoordinatesY()
    {
      return basePositionY + translateY;
    }

    void resetPosition()
    {
      basePositionX = basePositionX + translateX;
      basePositionY = basePositionY + translateY;
      translateX = 0;
      translateY = 0;
    }

    void updateBallVelocity()
    {
       double res;
       double current_time, timeElapsed;
       current_time = glfwGetTime();
       timeElapsed = current_time - getinitTime();

       res = exp ((-1*gravity*timeElapsed)/vt);

       if(moveFlag)
       {
         vx = vox*res;
         vy = voy*res - vt*(1-res);
         cout<<"update:: vx: "<<vx<<" "<<"vy: "<<vy<<endl;
       }
    }

    /*update com[onents of velocity upon collision */
    void updateVelocityComponents(float newVelocity, float angl) 
    {
      velocity = newVelocity;
      vx = velocity*cos(DEG2RAD(angl));
      vy = velocity*sin(DEG2RAD(angl));
      angle = angl;
      vox = vx;
      voy = vy;
 // cout<<"angle: "<<angle<<" "<<"new velocity: "<<ball_velocity<<endl;
  //cout<<"x: "<<vox<<" "<<"y: "<<voy<<endl;
      setinitTime();

    }

    void setballVelocity()
    {
      velocity = sqrt(vx*vx + vy*vy);
    }

    float getballVelocity()
    {
      return sqrt(vx*vx + vy*vy);
    }

    float getTheta()
    {
      float theta;
      theta = atan2(vy, vx);
      theta = (theta*(180))/PI;
      return theta;
    }

    void setTheta()
    {
      float theta;
      theta = atan2(vy, vx);
      theta = (theta*(180))/PI;
      angle = theta;
    }


    void updateBallPosition()
    {
      double res, t ;

      double current_time, timeElapsed;
      current_time = glfwGetTime();
      t = getinitTime();
      timeElapsed = current_time - t;

    //  cout<<"time: "<<timeElapsed<<endl;
   //   cout<<"velocity: "<<velocity<<endl;
  /*  if(velocity==0)
      {
        resetPosition();
      }
      else
      { */
        res = exp (-1*((gravity*timeElapsed)/vt)); 
        translateX = ((vox*vt)/gravity)*(1 - res);
        translateY = ((voy+vt)*(vt/gravity))*(1-res) - (vt*timeElapsed);
      //  cout<<"translate "<<translateX<<" "<<translateY<<endl;
       /* if(translateY + basePositionY <=-10)
        {
          resetPosition();
          friction_init_time =current_time;
          friction = 1;
          velocityFriction = vx;
        } */
  //    }
    }

    bool checkFriction()
    {
      if(!frictionFlag)
      {
        if(abs(vy)<=threshold_velocity && translateY + basePositionY <=-10+rad)
        {
        //  cout<<"friction identified\n";
          frictionFlag = 1;
          resetPosition();
          setinitTime();
        //  friction_init_time = glfwGetTime();
        //  friction = 1;
          vox = vx;
          return 1;
        } 
      else
        return 0;
      }
      else
        return 1;
    }

    void initFriction()
    {
      double timeElapsed, tmp;
      timeElapsed = glfwGetTime() - getinitTime() + 0.01;

      tmp = abs(vox) - (frictionCoefficient*(gravity*timeElapsed));
      if(vx<0)
      {
        if(vx*(-1*tmp) <0)
        {
          vx = 0;
          vy = 0;
          frictionFlag = 0;
          moveFlag = 0;
          turn = 1;
          resetPosition();
          setballVelocity();
        }
        else
          vx = -1*tmp;
      }
      else if(vx!=0)
      {
        if(vx*tmp <0 )
        {
          vx = 0;
          vy = 0;
          frictionFlag = 0;
          moveFlag = 0;
          turn = 1;
          resetPosition();
          setballVelocity();
        }
        else
          vx = tmp;
      }

    //  cout<<"vtx: "<<vx<<" "<<threshold_velocity<<endl;;
      if(abs(vx)>threshold_velocity && vx!=0)
      {
   //     cout<<"time elap: "<<timeElapsed<<endl;
        tmp = abs(abs(vox*timeElapsed) - ((timeElapsed*timeElapsed)*(gravity*frictionCoefficient))/2); 
      //  cout<<"tmp: "<<tmp<<endl;
        
     //   cout<<"tx: "<<(timeElapsed*timeElapsed)*(gravity*frictionCoefficient)/2<<" "; 
      //  cout<<vox*timeElapsed<<endl;
        if(vx<0)
          translateX = -1*(tmp);
        else
          translateX = tmp;
      }
      else
      {
        resetPosition();
        setballVelocity();
      }

  //    cout<<"transx: "<<translateX<<endl;
    }

    /* returns 1 in case no life exists*/
    bool checkLife()
    {
      if(vx==0 && vy==0)
        return 1;

      else if(abs(basePositionX + translateX)>=12)
        return 1;

      else
        return 0;
    }
  

};



/*class containing functions to create objects*/
class board
{

public:

  struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
  };
  typedef struct VAO VAO; 


  VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
  {
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO 
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors 
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
  }

  VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
  {
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
  }


  VAO* createTriangle ()
  {
  /* ONLY vertices between the bounds specified in glm::ortho will be visible on screen */

  /* Define vertex array as used in glBegin (GL_TRIANGLES) */
  static const GLfloat vertex_buffer_data [] = {
    0, 1,0, // vertex 0
    -1,-1,0, // vertex 1
    1,-1,0, // vertex 2
  };

  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 0
    1,0,0, // color 1
    1,0,0, // color 2
  };

  // create3DObject creates and returns a handle to a VAO that can be used later
  return create3DObject(GL_TRIANGLES, 3, vertex_buffer_data, color_buffer_data, GL_LINE);
  }

//VAO* createRectangle (float l, float b, float c1, float c2, float c3)
VAO* createRectangle (rectangle rect)
{
  int i;
  GLfloat* vertex_buffer_data = new GLfloat [3*6];
  GLfloat* color_buffer_data = new GLfloat [3*6];
  // GL3 accepts only Triangles. Quads are not supported

  vertex_buffer_data [0] = (rect.l)/2;
  vertex_buffer_data [1] = (rect.b)/2;
  vertex_buffer_data [2] = 0;

  vertex_buffer_data [3] = (rect.l)/2;
  vertex_buffer_data [4] = -1*(rect.b/2);
  vertex_buffer_data [5] = 0; 

  vertex_buffer_data [6] = -1*(rect.l/2);
  vertex_buffer_data [7] = -1*(rect.b/2);
  vertex_buffer_data [8] = 0;

  vertex_buffer_data [9] = -1*(rect.l/2);
  vertex_buffer_data [10] = -1*(rect.b/2);
  vertex_buffer_data [11] = 0;

  vertex_buffer_data [12] = -1*(rect.l/2);
  vertex_buffer_data [13] = (rect.b)/2;
  vertex_buffer_data [14] = 0; 

  vertex_buffer_data [15] = (rect.l)/2;
  vertex_buffer_data [16] = (rect.b)/2;
  vertex_buffer_data [17] = 0;

  
  for(i=0; i<6; i++)
  {
    color_buffer_data [3*i] = rect.rColor[0];
    color_buffer_data [3*i + 1] = rect.rColor[1];
    color_buffer_data [3*i + 2] = rect.rColor[2];
  }

  // create3DObject creates and returns a handle to a VAO that can be used later
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

//VAO* createCircle (float rad, float c1, float c2, float c3)
VAO* createCircle (ball Ball)
{

  GLfloat* vertex_buffer_data = new GLfloat [3*360];
  GLfloat* color_buffer_data = new GLfloat [3*360];
  
   for(int i=0 ; i<360 ; i++) {
      vertex_buffer_data [3*i] = (Ball.rad * cos(DEG2RAD(i))) ;
      vertex_buffer_data [3*i + 1] = (Ball.rad * sin(DEG2RAD(i)));
      vertex_buffer_data [3*i + 2] = 0;

      color_buffer_data [3*i] = Ball.bColor[0];
      color_buffer_data [3*i + 1] = Ball.bColor[1];
      color_buffer_data [3*i + 2] = Ball.bColor[2];

   }

   return create3DObject(GL_TRIANGLE_FAN, 360, vertex_buffer_data, color_buffer_data, GL_FILL);
   
}

VAO* createOval (float a, float b, ball Ball)
{
  GLfloat* vertex_buffer_data = new GLfloat [3*360];
  GLfloat* color_buffer_data = new GLfloat [3*360];
  
   for(int i=0 ; i<360 ; i++) {
      vertex_buffer_data [3*i] = (a * cos(DEG2RAD(i))) ;
      vertex_buffer_data [3*i + 1] = (b * sin(DEG2RAD(i)));
      vertex_buffer_data [3*i + 2] = 0;

      color_buffer_data [3*i] = Ball.bColor[0];
      color_buffer_data [3*i + 1] = Ball.bColor[1];
      color_buffer_data [3*i + 2] = Ball.bColor[2];

   }

   return create3DObject(GL_TRIANGLE_FAN, 360, vertex_buffer_data, color_buffer_data, GL_FILL);

}

VAO* createArrow ()
{
  GLfloat* vertex_buffer_data = new GLfloat [3*4];
  GLfloat* color_buffer_data = new GLfloat [3*4];
 
      vertex_buffer_data [0] = 0 ;
      vertex_buffer_data [1] = 0;
      vertex_buffer_data [2] = 0;

      vertex_buffer_data [3] = (cos(DEG2RAD(45))) ;
      vertex_buffer_data [4] = (sin(DEG2RAD(45)));
      vertex_buffer_data [5] = 0;

      color_buffer_data [0] = 1;
      color_buffer_data [1] = 1;
      color_buffer_data [2] = 1;
      color_buffer_data [3] = 1;
      color_buffer_data [4] = 1;
      color_buffer_data [5] = 1;

   return create3DObject(GL_LINES, 2, vertex_buffer_data, color_buffer_data, GL_LINE);

}

VAO* createGround ()
{
  GLfloat* vertex_buffer_data = new GLfloat [3*4];
  GLfloat* color_buffer_data = new GLfloat [3*4];
 
      vertex_buffer_data [0] = -12 ;
      vertex_buffer_data [1] = 0;
      vertex_buffer_data [2] = 0;

      vertex_buffer_data [3] = 12 ;
      vertex_buffer_data [4] = 0;
      vertex_buffer_data [5] = 0;

      color_buffer_data [0] = 0;
      color_buffer_data [1] = 1;
      color_buffer_data [2] = 0;
      color_buffer_data [3] = 0;
      color_buffer_data [4] = 1;
      color_buffer_data [5] = 0;

   return create3DObject(GL_LINES, 2, vertex_buffer_data, color_buffer_data, GL_LINE);

}

VAO* createObstacle(rectangle rect, float e)
{

  double tmp;
  tmp = sqrt(((rect.l*rect.l)/4)+((rect.b*rect.b)/4)) ;
  Obstacles.push_back(make_pair(make_pair(rect.cenx, rect.ceny), make_pair(e, tmp)));
  return  createRectangle(rect);

}

} ;