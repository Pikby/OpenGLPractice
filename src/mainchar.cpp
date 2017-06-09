enum Type {STATIC,DYNAMIC,STREAM};

#include <string>
#include "headers/SOIL.h"
#include <iostream>
#include <math.h>
#include <vector>

// GLEW
// #define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Add the shader configs
#include "headers/shaders.h"
#include "headers/block.h"

#include "headers/mainchar.h"


float gravity = 0.01;

MainChar::MainChar(float x, float y, const char* newTexture)
{
  mainCharShader = new Shader("../src/shaders/shader.vs","../src/shaders/shader.fs");
  xpos = x/10;
  ypos = y/10;
  texture = newTexture;
  type = STREAM;
  refresh();
}

MainChar::MainChar(const char* newTexture, int newType)
{
  mainCharShader = new Shader("../src/shaders/shader.vs","../src/shaders/shader.fs");
  texture = newTexture;
  type = newType;
  xpos = 0.0f;
  ypos = 0.0f;
  refresh();
}

void MainChar::refresh()
{

  GLfloat vertices[] =
  {
    0.0f, 0.0f, 0.0f,0.0f,0.0f,
    0.1f, 0.0f, 0.0f,0.0f,0.0f,
    0.0f, 0.0f, 0.1f,0.0f,0.0f,
    0.1f, 0.0f, 0.1f,0.0f,0.0f,
    0.05f,0.05f,0.05f,0.1f,0.5f,1.0f,

  };

  GLuint indices[] =
  {
    //front
    0,1,4,

    //rside
    0,2,4,

    //back
    2,3,4,
    //lside
    1,3,4,

    //bottom
    0,1,2,
    1,2,3
  };

  //Generate and bind the buffers
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &EBO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  //Draws the object for which ever type it is
  switch(type)
  {
    case STATIC:
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STATIC_DRAW);
    break;
    case DYNAMIC:
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_DYNAMIC_DRAW);
    break;
    case STREAM:
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices, GL_STREAM_DRAW);
    break;
    default :
    std::cout << "INVALID DRAW TYPE  \n STOPPING REFRESH\n";
    return;

  }

  //Position
  glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

  //Texture
  glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);


  glGenTextures(1, &glTexture);
  glBindTexture(GL_TEXTURE_2D, glTexture);

  //Set the texture properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //Load and bind the texture from the class
  int texWidth, texHeight;
  unsigned char* image = SOIL_load_image(texture, &texWidth, &texHeight, 0 , SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight,0,GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);

}

void MainChar::draw(glm::mat4 camera)
{
  glBindTexture(GL_TEXTURE_2D, glTexture);
  mainCharShader->Use();

  glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800/ (float)600, 0.1f, 100.0f);
  mainCharShader->setMat4("projection", projection);

  glm::mat4 view = camera;
  mainCharShader->setMat4("view", view);


  glm::mat4 model;
  glm::vec3 newPos;
  newPos.x = xpos;
  newPos.y = ypos;
  newPos.z = 0.0f;
  model = glm::translate(model, newPos);
  mainCharShader->setMat4("model", model);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT,0);
  glBindVertexArray(0);


}

void MainChar::update(std::vector <Block> blkArray)
{
  bool isBlocked = false;
  for(int i = 0 ; i<blkArray.size();i++)
  {
    if(xpos >= blkArray.at(i).xpos && xpos <= blkArray.at(i).xpos + 0.1f)
    {
      if(ypos - gravity >= blkArray.at(i).ypos && ypos - gravity <= blkArray.at(i).ypos + 0.1f)
      {
        isBlocked = true;
      }
    }
  }
  if(!isBlocked) ypos = ypos - gravity;
}