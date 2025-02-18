#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "stb_image/stb_image.h"
#include <learnopengl/filesystem.h>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>



// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window,double *mouse, bool * shouldDraw, bool * press);
void DisplayFramebufferTexture(unsigned int textureID, Shader *program,unsigned int VAO,glm::vec2 R);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool inflow=true;
double fps = 0.0; // this will store the final fps for the last second

char title_string[50];
double lt;
int nbFrames = 0;
float vertices[] = {
    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

float TVvertices[] = {
    1.0f,  1.0f,   1.0f, 1.0f, // top right
    1.0f, -1.0f,   1.0f, 0.0f, // bottom right
    -1.0f, -1.0f,   0.0f, 0.0f, // bottom left
    -1.0f,  1.0f,   0.0f, 1.0f  // top left
};

unsigned int indices[] = {
    0,1,3,
    1,2,3
};

int main()
{
    
    if (__cplusplus == 201703L)
        std::cout << "C++17" << std::endl;
    else if (__cplusplus == 201402L)
        std::cout << "C++14" << std::endl;
    else if (__cplusplus == 201103L)
        std::cout << "C++11" << std::endl;
    else if (__cplusplus == 199711L)
        std::cout << "C++98" << std::endl;
    else
        std::cout << "pre-standard C++" << std::endl;


    std::filesystem::path path = std::filesystem::current_path();
    const std::string pathString = path.string();

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Projection", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::string parentPath = (std::string)pathString;

    std::cout << parentPath << std::endl;

    const std::string vertexShader = "/Shaders/VertexShader.vs";
    const std::string vertexPath = parentPath + vertexShader;
    const char * vertexShaderPath = vertexPath.c_str();

    const std::string TVvertexShader = "/Shaders/TextureViewer.vs.vert";
    const std::string TVvertexPath = parentPath + TVvertexShader;
    const char * TVvertexShaderPath = TVvertexPath.c_str();


    const std::string fragmentShader = "/Shaders/Image.fs";
    const std::string fragmentPath = parentPath + fragmentShader;
    const char * fragmentShaderPath = fragmentPath.c_str();
   


    const std::string fragmentShaderA = "/Shaders/BufferA.fs";
    const std::string fragmentPathA = parentPath + fragmentShaderA;
    const char * fragmentShaderAPath = fragmentPathA.c_str();


    const std::string fragmentShaderB = "/Shaders/BufferB.fs";
    const std::string fragmentPathB = parentPath + fragmentShaderB;
    const char * fragmentShaderBPath = fragmentPathB.c_str();

    const std::string fragmentShaderC = "/Shaders/BufferC.fs";
    const std::string fragmentPathC = parentPath + fragmentShaderC;
    const char * fragmentShaderCPath = fragmentPathC.c_str();

    const std::string TVShaderString = "/Shaders/TextureViewer.fs.frag";
    const std::string TVShaderPathST = parentPath + TVShaderString;
    const char * TVShaderPath = TVShaderPathST.c_str();



     const std::string cString = parentPath+ "/Shaders/common.incl";
    const char * commonPath = cString.c_str();
    Shader Imageprogram(commonPath,vertexShaderPath,fragmentShaderPath);
   // Shader BufferAprogram(commonPath,vertexShaderPath,fragmentShaderAPath);
    //Shader BufferCprogram(commonPath,vertexShaderPath,fragmentShaderCPath);
    //Shader BufferBprogram(commonPath,vertexShaderPath,fragmentShaderBPath);
    const char * commonPath1 = cString.c_str();
    Shader TVShaderprogram(commonPath1,TVvertexShaderPath,TVShaderPath);

    ////////////////////
    // OPENGL BÁSICOS //
    ////////////////////
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *)0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (3*sizeof(float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void *) (6*sizeof(float)) );
    glEnableVertexAttribArray(2);



    ////////////////////
    // OPENGL BÁSICOS //
    ////////////////////
    unsigned int TV_VBO, TV_VAO, TV_EBO;
    glGenVertexArrays(1, &TV_VAO);
    glGenBuffers(1, &TV_VBO);
    glGenBuffers(1, &TV_EBO);
    glBindVertexArray(TV_VAO);

    glBindBuffer(GL_ARRAY_BUFFER,TV_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TVvertices), TVvertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TV_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),(void *)0 );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float),(void *) (2*sizeof(float)) );
    glEnableVertexAttribArray(1);


    ////////////////////
    //    TEXTURES    //
    ////////////////////

    //Abrindo textura presente nas pastas do projeto]
   // const std::string container = "/../black.jpg";
    //const std::string texturePath = pathString + container;

    //std::cout << texturePath << std::endl;
    //const char * finalPath = texturePath.c_str();
    //std::cout << "finalPath:" << finalPath << std::endl;
    //int width, height, nrChannels;
    //unsigned char *data = stbi_load(finalPath, &width, &height, &nrChannels, 0);



    //printf("hello");

    ////////////////////
    //  FRAMEBUFFER   //
    ////////////////////

     

    //Criando o framebuffer
    unsigned int FBO_0;
    glGenFramebuffers(1, &FBO_0);

    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO_0);

    //Para utilizar o framebuffer, é necessário ligar a ele uma textura
    unsigned int iChannel_0;

    glGenTextures(1, &iChannel_0);
    glBindTexture(GL_TEXTURE_2D, iChannel_0);
     
    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    //LIGANDO A TEXTURA AO FRAMBUFFER

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iChannel_0, 0 );

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo_0;
        glGenRenderbuffers(1, &rbo_0);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_0);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_0); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
     //glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Criando o framebuffer
    unsigned int FBO_1;
    glGenFramebuffers(1, &FBO_1);
    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO_1);


    unsigned int iChannel_1;

    glGenTextures(1, &iChannel_1);
    glBindTexture(GL_TEXTURE_2D, iChannel_1);

    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    //LIGANDO A TEXTURA AO FRAMBUFFER

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iChannel_1, 0);

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo_1;
        glGenRenderbuffers(1, &rbo_1);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_1);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_1); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Criando o framebuffer
    unsigned int FBO_2;
    glGenFramebuffers(1, &FBO_2);
    //Ligando o framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,FBO_2);

    unsigned int iChannel_2;

    glGenTextures(1, &iChannel_2);
    glBindTexture(GL_TEXTURE_2D, iChannel_2);

    //Gerando uma textura vazia para ligar ao framebuffer
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    //LIGANDO A TEXTURA AO FRAMBUFFER

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, iChannel_2, 0 );

    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        unsigned int rbo_2;
        glGenRenderbuffers(1, &rbo_2);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo_2);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_2); // now actually attach it
        // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now


    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
         std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

   // int MaxTextureImageUnits;
    //glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureImageUnits);

//glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int imageTexture = loadTexture(FileSystem::getPath("Shaders/Lapis.png").c_str());
double * mouse = (double *) malloc(sizeof(double)*4);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textura);
    //Shaderprogram2.use();
    //Shaderprogram2.setSampler("Texture");
    glm::vec2 resolution(SCR_WIDTH,SCR_HEIGHT);
    //Shaderprogram2.setVec2("iResolution",resolution);
    Imageprogram.use();
    Imageprogram.setVec2("iResolution",resolution);

    bool shouldDraw = false;
        bool press = false;
        float currentTime;
        int frame=0;


    ////////////////////
    //  RENDER LOOP   //
    ////////////////////
    while(!glfwWindowShouldClose(window)){
        currentTime = static_cast<float>(glfwGetTime());
        //Renderizando para o framebuffer criado

          processInput(window,mouse, &shouldDraw, & press);


           //glEnable(GL_DEPTH_TEST);

        if(shouldDraw)
        {
         /* glBindFramebuffer(GL_FRAMEBUFFER, FBO_0);

          // make sure we clear the framebuffer's content
          glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Utilização de um segundo shader para calcular vizinhança
         BufferAprogram.use();
        BufferAprogram.setInt("iChannel0",0);
        BufferAprogram.setInt("iChannel1",1);
        BufferAprogram.setVec2("iResolution",resolution) ;
        BufferAprogram.setVec4("iMouse", mouse);
        BufferAprogram.setFloat("iTime",currentTime);
        BufferAprogram.setInt("iFrame",frame);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iChannel_1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, imageTexture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        //glBindVertexArray(0);
          //  glUseProgram(0);

         glBindFramebuffer(GL_FRAMEBUFFER, FBO_1);

         // make sure we clear the framebuffer's content
         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        BufferBprogram.use();
        BufferBprogram.setInt("iChannel0",0);
        BufferBprogram.setVec2("iResolution",resolution) ;
        BufferBprogram.setVec4("iMouse", mouse);
        BufferBprogram.setFloat("iTime",currentTime);
        BufferBprogram.setInt("iFrame",frame);
        BufferBprogram.setBool("inflow",inflow);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, iChannel_0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        //glBindVertexArray(0);
          //  glUseProgram(0);



        glBindFramebuffer(GL_FRAMEBUFFER, FBO_2);
        // make sure we clear the framebuffer's content
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        BufferCprogram.use();

        BufferCprogram.setSampler("iChannel0",0);
        BufferCprogram.setVec2("iResolution",resolution) ;
        BufferCprogram.setVec4("iMouse", mouse);
        BufferCprogram.setFloat("iTime",currentTime);
        BufferCprogram.setInt("iFrame",frame);
        glBindVertexArray(VAO);
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, iChannel_0);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        //glBindVertexArray(0);
        //glUseProgram(0);*/



            //Renderizando para a tela
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //glDisable(GL_DEPTH_TEST);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            /*TVShaderprogram.use();
            TVShaderprogram.setFloat("iTime",currentTime);
            DisplayFramebufferTexture(iChannel_1,&TVShaderprogram,TV_VAO,resolution);*/


        Imageprogram.use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, iChannel_0);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, iChannel_1);
        //glActiveTexture(GL_TEXTURE1);
        //glBindTexture(GL_TEXTURE_2D, iChannel_2);
        glBindVertexArray(VAO);
        Imageprogram.setVec2("iResolution",resolution) ;
        //Imageprogram.setSampler("iChannel0",0);
        //Imageprogram.setSampler("iChannel1",1);
        Imageprogram.setFloat("iTime",currentTime);
        Imageprogram.setVec4("iMouse", mouse);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        double CurrentTime = glfwGetTime();
        double past = CurrentTime - lt;
        if(past>0.2) {
        //timer.stop();
        fps = (float)nbFrames/past;
        sprintf(title_string, "Flow - FPS = %.2f ", fps);
        glfwSetWindowTitle(window, title_string);
        lt+=0.2;
        nbFrames = 0;
        }
        nbFrames++;
         frame++;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();


    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteFramebuffers(1, &FBO_0);
    glDeleteFramebuffers(1, &FBO_1);
    glDeleteFramebuffers(1, &FBO_2);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window,double *mouse, bool * shouldDraw, bool * press)
{
    //Caso aperte 'esc', indica que a janela deve ser fechada
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    //Alocando o vetor que contém o mouse
    //double * mouse = (double *) malloc(sizeof(double)*4);

    //Encontrando a posição do mouse
    glfwGetCursorPos(window,&mouse[0], &mouse[1]);
    mouse[2] = 0.0;
    mouse[3] = 0.0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) mouse[2] = 1.0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) mouse[3] = 1.0;

    //Normalizando as coordenadas do cursor
    int h,w;
    glfwGetFramebufferSize(window, &w, &h);
    //mouse[1] = h-mouse[1];
    //mouse[0] = ((mouse[0]*2.0)/w) -1.0;
    mouse[1] = h-mouse[1];//((mouse[1]*2.0)/h) -1.0;
    //return mouse;

    if(glfwGetKey(window, 32) == GLFW_PRESS && !*press) {
        *shouldDraw = !*shouldDraw;
        lt = glfwGetTime();
        * press = true;
    }

if(glfwGetKey(window,GLFW_KEY_I) == GLFW_PRESS && !*press) {
        inflow = !inflow;
        * press = true;
    }
    if(glfwGetKey(window, 32) == GLFW_RELEASE) * press = false;
    //if(glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) * press = false;
}

void DisplayFramebufferTexture(unsigned int textureID,Shader *program, unsigned int VAO,glm::vec2 R)
{

       program->use();


           program->setSampler("fboAttachment",0);
           program->setVec2("iResolution",R);
           glBindVertexArray(VAO);

           glActiveTexture(GL_TEXTURE0);
           glBindTexture(GL_TEXTURE_2D, textureID);

           glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
           glBindVertexArray(0);
       glUseProgram(0);
}

//call: chamada quando um determinado evento acontece
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    printf("%i:%i\n", width,height);
}
