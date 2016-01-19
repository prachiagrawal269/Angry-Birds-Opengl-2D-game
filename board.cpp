#define DEG2RAD(deg) (deg * PI / 180)
#define PI 3.141592653589

//#include "globals.cpp"
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

VAO* createRectangle ()
{
  // GL3 accepts only Triangles. Quads are not supported
  static const GLfloat vertex_buffer_data [] = {
    1,1,0, // vertex 1
    1,-1,0, // vertex 2
    -1,-1,0, // vertex 3

     -1,-1,0, // vertex 3
     -1,1,0, // vertex 4
     1,1,0  // vertex 1
  };

  static const GLfloat color_buffer_data [] = {
    1,0,0, // color 1
    1,0,0, // color 2
    1,0,0, // color 3

    1,0,0, // color 3
    1,0,0, // color 4
    1,0,0  // color 1
  };

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

};