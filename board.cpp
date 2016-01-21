#include "globals.h"

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

VAO* createRectangle (float l, float b, float c1, float c2, float c3)
{
  int i;
  GLfloat* vertex_buffer_data = new GLfloat [3*6];
  GLfloat* color_buffer_data = new GLfloat [3*6];
  // GL3 accepts only Triangles. Quads are not supported

  vertex_buffer_data [0] = l/2;
  vertex_buffer_data [1] = b/2;
  vertex_buffer_data [2] = 0;

  vertex_buffer_data [3] = l/2;
  vertex_buffer_data [4] = -1*(b/2);
  vertex_buffer_data [5] = 0; 

  vertex_buffer_data [6] = -1*(l/2);
  vertex_buffer_data [7] = -1*(b/2);
  vertex_buffer_data [8] = 0;

  vertex_buffer_data [9] = -1*(l/2);
  vertex_buffer_data [10] = -1*(b/2);
  vertex_buffer_data [11] = 0;

  vertex_buffer_data [12] = -1*(l/2);
  vertex_buffer_data [13] = b/2;
  vertex_buffer_data [14] = 0; 

  vertex_buffer_data [15] = l/2;
  vertex_buffer_data [16] = b/2;
  vertex_buffer_data [17] = 0;

  
  for(i=0; i<6; i++)
  {
    color_buffer_data [3*i] = c1;
    color_buffer_data [3*i + 1] = c2;
    color_buffer_data [3*i + 2] = c3;
  }

  // create3DObject creates and returns a handle to a VAO that can be used later
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* createCircle (float rad, float cx, float cy)
{

  GLfloat* vertex_buffer_data = new GLfloat [3*360];
  GLfloat* color_buffer_data = new GLfloat [3*360];
  
   for(int i=0 ; i<360 ; i++) {
      vertex_buffer_data [3*i] = (rad * cos(DEG2RAD(i))) ;
      vertex_buffer_data [3*i + 1] = (rad * sin(DEG2RAD(i)));
      vertex_buffer_data [3*i + 2] = 0;

      color_buffer_data [3*i] = 0;
      color_buffer_data [3*i + 1] = 0;
      color_buffer_data [3*i + 2] = 1;

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

VAO* createObstacle(float l, float b, float c1, float c2, float c3, float cenx, float ceny, float e)
{

  double tmp;
  tmp = sqrt(((l/2)*(l/2))+((b/2)*(b/2)));
  Obstacles.push_back(make_pair(make_pair(cenx, ceny), make_pair(e, tmp)));
  return  createRectangle(l, b, c1, c2, c3);

}

} ;