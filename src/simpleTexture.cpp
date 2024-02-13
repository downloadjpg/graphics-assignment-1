// Based on templates from learnopengl.com
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_img/stb_image.h"


#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_img/stb_image_write.h"

#include "scene.h"
#include "camera.h"

#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// BRODY HERE ===============================================================================================
void animateObjects();
void set_texture();
Scene* current_scene;
Camera* camera;

bool animating = false; 
int manualCameraLookAtIndex = 0;
const int FPS = 30; 
const float movieDuration = 5.0f; // seconds
const int maxFrames = FPS * movieDuration;
const float frameDuration = 1.0f / (float) FPS;
int frame = 0;

// ============================================================================================================

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"ourColor = aColor;\n"
	"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(texture1, TexCoord);\n"
    "}\n\0";
    

int main(int argc, char* argv[])
{
    // BRODY HERE ===============================================================================================
        for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-a") {
            animating = true;
            break;
        }
    }
    // ============================================================================================================
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Display RGB Array", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // // GLEW: load all OpenGL function pointers
    glewInit();

    // build and compile the shaders
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // BRODY HERE: Changed the vertex positions to cover the whole window
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  -1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // BRODY HERE ===============================================================================================
    current_scene = new Scene();
    camera = current_scene->camera;

    // ============================================================================================================

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        if (!animating)
            processInput(window);

        // render
        // ------
    
        animateObjects();
        set_texture();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind Texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // BRODY HERE ===============================================================================================
    // Camera movement
    float timeDelta = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->move(vec3(0,0,-1), timeDelta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->move(vec3(0,0,1), timeDelta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->move(vec3(-1,0,0), timeDelta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->move(vec3(1,0,0), timeDelta);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->move(vec3(0,1,0), timeDelta);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->move(vec3(0,-1,0), timeDelta);

    // TODO: Camera lookAt
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        manualCameraLookAtIndex -= 1;
        manualCameraLookAtIndex = std::max(0, manualCameraLookAtIndex);
        manualCameraLookAtIndex = std::min(manualCameraLookAtIndex, (int) current_scene->surfaces.size() - 1);
        camera->lookAt(current_scene->surfaces.at(manualCameraLookAtIndex)->origin);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        manualCameraLookAtIndex += 1;
        manualCameraLookAtIndex = std::max(0, manualCameraLookAtIndex);
        manualCameraLookAtIndex = std::min(manualCameraLookAtIndex, (int) current_scene->surfaces.size() - 1);
        camera->lookAt(current_scene->surfaces.at(manualCameraLookAtIndex)->origin);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera->lookAt(vec3(0,0,0));
    }

    // Swapping projection type
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        camera->setProjectionType(Camera::ProjectionType::perspective);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        camera->setProjectionType(Camera::ProjectionType::orthographic);
        // ============================================================================================================
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// BRODY HERE ===============================================================================================
void set_texture(){
        // Create the image (RGB Array) to be displayed
        const int width  = 512; // keep it in powers of 2!
        const int height = 512; // TODO: update camera width/height to match

        unsigned char* image = current_scene->renderer->renderImage();

        // Save image to file
        if (animating) {
            stbi_write_jpg(("output/frame" + std::to_string(frame) + ".jpg").c_str(), width, height, 3, image, 100);
        }
        // Render to screen
        unsigned char *data = &image[0];
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
}


void animateObjects() {
    if (animating) {
        frame += 1;
        if (frame > maxFrames) {
            animating = false;
        }
        float t = (float) frame / (float) maxFrames;
        // animations!!

        // rotate the camera around the origin
        float A = 12 - 4 * cos(4 * 3.14159 * t);
        float x = A * cos(2 * 3.14159 * t);
        float z = A * sin(2 * 3.14159 * t) - 10;
        float y = 10 - 8* t;
        camera->origin = vec3(x, y, z);
        
        camera->lookAt(vec3(0, 0, -10));

        current_scene->lights[2]->color = vec3(1, 1 - t, t);
    }
}
// ============================================================================================================