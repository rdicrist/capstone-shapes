#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "common/Angel.h"
#include "shader.h"

#include <stdlib.h>
#include <stdio.h>
#include <vector>

using namespace Angel;


//Globals there is a way to avoid this, but for now let's do the bad thing.
enum{_SQUARE, _TRIANGLE, _RECT, _DI, _PAR , _NUM_OBJ };
int current_object;

//GLFW error callback
static void error_callback(int error, const char* description)
{
  fprintf(stderr, "Error: %s\n", description);
}

//GLFW keypress callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    current_object = (current_object+1)%_NUM_OBJ;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        current_object = (current_object-1 + _NUM_OBJ)%_NUM_OBJ;
}

//Initialize everything
void init(GLuint &glShaderProgram, GLuint &triVao, GLuint &sqVao, GLuint &rectVao, GLuint &diVao, GLuint &parVao){

  GLuint vertex_shader, fragment_shader;
  GLint vpos_location, vcolor_location;
  GLuint tri_buffer;
  GLuint sq_buffer;

    //ricki did here
    GLuint rect_buffer;
    GLuint di_buffer;
    GLuint par_buffer;


    //look into making 3d
  vec2 square[12]            = {
      vec2(-.25, -.25),
      vec2(-.25,  .75),
      vec2( .75, -.25),
      vec2( .75,  .75),
    
      vec2(-.5, -.5),
      vec2(-.25, -.25),
      vec2(.5, -.5),
      vec2(.75, -.25),
      
      vec2(-.5, -.5),
      vec2(-.5, .5),
      vec2(-.25, -.25),
      vec2(-.25, .75)};
    
  vec3 square_colors[12]     = {
      vec3( 1.0, 0.0, 1.0),
      vec3( 1.0, 0.0, 1.0),
      vec3( 1.0, 0.0, 1.0),
      vec3( 1.0, 0.0, 1.0),
      
      vec3( .9, 0.0, 1.0),
      vec3( .9, 0.0, 1.0),
      vec3( .9, 0.0, 1.0),
      vec3( .9, 0.0, 1.0),
    
    vec3( .8, 0.0, 1.0),
    vec3( .8, 0.0, 1.0),
    vec3( .8, 0.0, 1.0),
    vec3( .8, 0.0, 1.0)};
    


    

  vec2 triangle[11]          = {
      vec2(-.25, -.25),
      vec2(.25, .75),
      vec2(.75,-.25),
      
      vec2(-.5, -.5),
      vec2(-.25, -.25),
      vec2(.5,-.5),
      vec2(.75,-.25),
      
      vec2(-.5, -.5),
      vec2(0, .5),
      vec2(-.25,-.25),
      vec2(.25,.75)
  };
    
  vec3 triangle_colors[11]   = {
    vec3( 1.0, 1.0, 0.0),
    vec3( 1.0, 1.0, 0.0),
    vec3( 1.0, 1.0, 0.0),
      
      vec3( 1.0, 0.9, 0.0),
      vec3( 1.0, 0.9, 0.0),
      vec3( 1.0, 0.9, 0.0),
      vec3( 1.0, 0.9, 0.0),
      
      vec3( 0.9, 0.9, 0.0),
      vec3( 0.9, 0.9, 0.0),
      vec3( 0.9, 0.9, 0.0),
      vec3( 0.9, 0.9, 0.0)
  };

    vec2 rectangle[12] = {
        
        vec2(-.25, -.5),
        vec2(-.25, 1),
        vec2(.75, -.5),
        vec2(.75, 1),
        
        vec2(-.5, -.75),
        vec2(-.25, -.5),
        vec2(.5, -.75),
        vec2(.75, -.5),
        
        vec2(-.5, -.75),
        vec2(-.5, .75),
        vec2(-.25, -.5),
        vec2(-.25, 1)
    };

    vec3 rectangle_colors[12] ={vec3(0,1,0), vec3(0,1,0), vec3(0,1,0), vec3(0,1,0),
        
        vec3(0,.9,0), vec3(0,.9,0), vec3(0,.9,0), vec3(0,.9,0),
        
        vec3(0,.8,0), vec3(0,.8,0), vec3(0,.8,0), vec3(0,.8,0)
    };

    vec2 diamond[12] = {
        
        vec2(.1875,-.5625),
        vec2(-.1875, .1875),
        vec2(.5625, .1875),
        vec2(.1875,.9375),
        
        vec2(0,-.75),
        vec2(-.375, 0),
        vec2(.1875, -.5625),
        vec2(-.1875,.1875),
        
        vec2(-.375,0),
        vec2(0, .75),
        vec2(-.1875,.1875),
        vec2(.1875,.9375)
        
    };

    vec3 diamond_colors[12] = {vec3(0,0,0), vec3(0,0,0), vec3(0,0,0), vec3(0,0,0),
        vec3(.1,.1,.1), vec3(.1,.1,.1), vec3(.1,.1,.1), vec3(.1,.1,.1),
        vec3(.2,.2,.2), vec3(.2,.2,.2), vec3(.2,.2,.2), vec3(.2,.2,.2)
    };
    
    vec2 parallelogram[12] = {
        
        vec2(-.5,-.25),
        vec2(-.25, .75),
        vec2(.75, -.25),
        vec2(1,.75),
        
        vec2(-.75,-.5),
        vec2(-.5, .5),
        vec2(-.5, -.25),
        vec2(-.25, .75),

        vec2(-.75,-.5),
        vec2(-.5,-.25),
        vec2(.5, -.5),
        vec2(.75, -.25)
        
    };
    
    vec3 parallelogram_colors[12] = {vec3(0,0,1), vec3(0,0,1), vec3(0,0,1), vec3(0,0,1),
        vec3(0,0,.9), vec3(0,0,.9), vec3(0,0,.9), vec3(0,0,.9),
        vec3(0,0,.8), vec3(0,0,.8), vec3(0,0,.8), vec3(0,0,.8)
    };


  //String with absolute paths to our shaders, shader_path set by CMAKE hack
  std::string vshader = shader_path + "vshader.glsl";
  std::string fshader = shader_path + "fshader.glsl";

  //Read in shader code to a character array
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  //Create and compile vertex shader
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  check_shader_compilation(vshader, vertex_shader);

  //Create and compile fragment shader
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  check_shader_compilation(fshader, fragment_shader);

  //Create shader program from the 2 shaders
  glShaderProgram = glCreateProgram();
  glAttachShader(glShaderProgram, vertex_shader);
  glAttachShader(glShaderProgram, fragment_shader);
  glLinkProgram(glShaderProgram);
  check_program_link(glShaderProgram);

  //bind or find the numerical location for shader variables
  glBindFragDataLocation(glShaderProgram, 0, "fragColor");
  vpos_location   = glGetAttribLocation(glShaderProgram, "vPos");
  vcolor_location = glGetAttribLocation(glShaderProgram, "vColor" );

  // Create a vertex array object
  glGenVertexArrays( 1, &triVao );
  //Set GL state to use vertex array object
  glBindVertexArray( triVao );

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
  glGenVertexArrays( 1, &sqVao );
  //Set GL state to use vertex array object
  glBindVertexArray( sqVao );

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



    //ricki did rectangle here
    // Create a vertex array object
    glGenVertexArrays( 1, &rectVao );
    //Set GL state to use vertex array object
    glBindVertexArray( rectVao );

    //Generate buffer to hold our vertex data
    glGenBuffers( 1, &rect_buffer );
    //Set GL state to use this buffer
    glBindBuffer( GL_ARRAY_BUFFER, rect_buffer );

    //Create GPU buffer to hold vertices and color
    glBufferData( GL_ARRAY_BUFFER, sizeof(rectangle)+sizeof(rectangle_colors), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(rectangle), rectangle );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(rectangle), sizeof(rectangle_colors), rectangle_colors );

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray( vcolor_location );

    glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(rectangle)) );

    glBindVertexArray(0);

    //ricki did diamond here
    // Create a vertex array object
    glGenVertexArrays( 1, &diVao );
    //Set GL state to use vertex array object
    glBindVertexArray( diVao );

    //Generate buffer to hold our vertex data
    glGenBuffers( 1, &di_buffer );
    //Set GL state to use this buffer
    glBindBuffer( GL_ARRAY_BUFFER, di_buffer );

    //Create GPU buffer to hold vertices and color
    glBufferData( GL_ARRAY_BUFFER, sizeof(diamond)+sizeof(diamond_colors), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(diamond), diamond );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(diamond), sizeof(diamond_colors), diamond_colors );

    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray( vcolor_location );

    glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(diamond)) );

    glBindVertexArray(0);
    
    //ricki did parallelogram here
    // Create a vertex array object
    glGenVertexArrays( 1, &parVao );
    //Set GL state to use vertex array object
    glBindVertexArray( parVao );
    
    //Generate buffer to hold our vertex data
    glGenBuffers( 1, &par_buffer );
    //Set GL state to use this buffer
    glBindBuffer( GL_ARRAY_BUFFER, par_buffer );
    
    //Create GPU buffer to hold vertices and color
    glBufferData( GL_ARRAY_BUFFER, sizeof(parallelogram)+sizeof(parallelogram_colors), NULL, GL_STATIC_DRAW );
    //First part of array holds vertices
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(parallelogram), parallelogram );
    //Second part of array hold colors (offset by sizeof(triangle))
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(parallelogram), sizeof(parallelogram_colors), parallelogram_colors );
    
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray( vcolor_location );
    
    glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    glVertexAttribPointer( vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(parallelogram)) );
    
    glBindVertexArray(0);
    

}

//Animation callback.  For now this does nothing
void animate(){
  //Do something every second
  if(glfwGetTime() > 1.0){
    glfwSetTime(0.0);
    //Do something Here

  }
}

//Main
int main(void)
{
  GLFWwindow* window;
  GLuint glShaderProgram;
  GLuint triVao;
  GLuint sqVao;

    //ricki did
    GLuint rectVao;
    GLuint diVao;
    GLuint parVao;

  //Set the error callback defined above
  glfwSetErrorCallback(error_callback);

  //Init glfw
  if (!glfwInit())
    exit(EXIT_FAILURE);

  //Enforce OpenGL 3.2
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //Multisampling
  glfwWindowHint(GLFW_SAMPLES, 4);

  //Create window
  window = glfwCreateWindow(512, 512, "Capstone Shapes", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  //Set key callback
  glfwSetKeyCallback(window, key_callback);

  //GLFW setup calls
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
  glfwSwapInterval(1);

  //Init the geomerty and shaders
  init(glShaderProgram, triVao, sqVao, rectVao, diVao, parVao);

  //set the background/clear color
  glClearColor(1.0, 1.0, 1.0, 1.0);

  //main loop, loop forever until user closes window
  while (!glfwWindowShouldClose(window)){

    //Grab window size in pixels
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    //Set viewport
    glViewport(0, 0, width, height);

    //Call animation update
    animate();

    //Clear color buffer bit
    glClear(GL_COLOR_BUFFER_BIT);

    //Enable our shader program
    glUseProgram(glShaderProgram);

    //Draw
    switch(current_object){
      case _TRIANGLE:
        glBindVertexArray( triVao );
            glDrawArrays(GL_TRIANGLES, 0, 3);
            
            glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 7, 4);
        break;
      case _SQUARE:
        glBindVertexArray( sqVao );
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            //make bottom paralellogram
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            //make side parrallellogram
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            
        break;

            //ricki did, make two new cases
        case _RECT:
            glBindVertexArray( rectVao );
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            break;

        case _DI:
            glBindVertexArray( diVao );
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            break;
            
        case _PAR:
            glBindVertexArray( parVao );
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
            break;
     }
    glBindVertexArray(0);

    //Swap our double buffers
    glfwSwapBuffers(window);

    //GLFW event handler
    glfwPollEvents();

  }

  //Clean up
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

