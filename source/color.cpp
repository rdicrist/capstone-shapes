#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/Angel.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>

using namespace Angel;

GLuint program;
GLuint tri_vao;
GLuint sq_vao;

enum{_RENDER_TRIANGLE, _RENDER_SQUARE, _NUM_RENDER_OBJ};
int current_draw = _RENDER_TRIANGLE;


static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    current_draw = (current_draw+1)%_NUM_RENDER_OBJ;
}

void init(){
  
  GLuint vertex_shader, fragment_shader;
  GLint vpos_location, vcolor_location;
  GLuint tri_buffer;
  GLuint sq_buffer;
  
  vec2 square[4]            = { vec2(-0.5, -0.5),
                                vec2(-0.5,  0.5),
                                vec2( 0.5, -0.5),
                                vec2( 0.5,  0.5)};
  vec3 square_colors[4]     = { vec3( 1.0, 0.0, 0.0),
                                vec3( 1.0, 0.0, 0.0),
                                vec3( 1.0, 0.0, 0.0),
                                vec3( 1.0, 0.0, 0.0),};
  
  vec2 triangle[3]          = { vec2(-1.0, -1.0),
                                vec2( 0.0,  1.0),
                                vec2( 1.0, -1.0)};
  vec3 triangle_colors[3]   = { vec3( 0.0, 0.0, 1.0),
                                vec3( 1.0, 0.0, 0.0),
                                vec3( 0.0, 1.0, 0.0)};

  
  vec2 concave_poly[9]      = { vec2(0.44,  0.65),
                                vec2(-0.42, 0.65),
                                vec2(-0.86, -0.28),
                                vec2(-0.04, -0.70),
                                vec2(0.29,  -0.46),
                                vec2(-0.18, -0.14),
                                vec2(0.19, 0.28),
                                vec2(0.57, -0.18),
                                vec2(0.90, 0.12)};
  

  std::string vshader = shader_path + "vshader.glsl";
  std::string fshader = shader_path + "fshader.glsl";
  
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());
  
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_shader_compilation(vshader, vertex_shader);
  
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_shader_compilation(fshader, fragment_shader);
  
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  
  glLinkProgram(program);
  check_program_link(program);
  
  glBindFragDataLocation(program, 0, "fragColor");
  
  vpos_location   = glGetAttribLocation(program, "vPos");
  vcolor_location = glGetAttribLocation( program, "vColor" );
  
  
  // Create a vertex array object
  glGenVertexArrays( 1, &tri_vao );
  //Set GL state to use vertex array object
  glBindVertexArray( tri_vao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &tri_buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, tri_buffer );
  
  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, sizeof(triangle)+sizeof(triangle_colors), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(triangle), triangle );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(triangle), sizeof(triangle_colors), triangle_colors );
  
  glEnableVertexAttribArray(vpos_location);
  glEnableVertexAttribArray( vcolor_location );
  
  glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(triangle)) );

  glBindVertexArray(0);
  
  
  // Create a vertex array object
  glGenVertexArrays( 1, &sq_vao );
  //Set GL state to use vertex array object
  glBindVertexArray( sq_vao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &sq_buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, sq_buffer );
  
  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, sizeof(square)+sizeof(square_colors), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(square), square );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(square), sizeof(square_colors), square_colors );
  
  glEnableVertexAttribArray(vpos_location);
  glEnableVertexAttribArray(vcolor_location);
  
  glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(square)) );

  glBindVertexArray(0);
}

void animate(){
  //Do something every second
  if(glfwGetTime() > 1.0){
    glfwSetTime(0.0);
    //Do something Here
  
  }
}

int main(void)
{
  GLFWwindow* window;
  
  glfwSetErrorCallback(error_callback);
  
  if (!glfwInit())
    exit(EXIT_FAILURE);
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  glfwWindowHint(GLFW_SAMPLES, 4);

  
  window = glfwCreateWindow(512, 512, "Rainbow triangle", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  
  glfwSetKeyCallback(window, key_callback);
  
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);
  
  init();
  
  glClearColor(1.0, 1.0, 1.0, 1.0);
  
  while (!glfwWindowShouldClose(window)){
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    animate();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(program);
    
    switch(current_draw){
      case _RENDER_TRIANGLE:
        glBindVertexArray( tri_vao );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        break;
      case _RENDER_SQUARE:
        glBindVertexArray( sq_vao );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        break;
     }
    
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  
  }
  
  glfwDestroyWindow(window);
  
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
