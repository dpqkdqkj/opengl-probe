#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Quad.hpp"
#include "LineGrid.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 512;
const unsigned int SCR_HEIGHT = 512;

const char *vertexShaderSource = "#version 420 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * model * vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    //"   gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 420 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n\0";

int main()
{
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
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
    GLuint shaderProgram = glCreateProgram();
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

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    glUseProgram(shaderProgram);
    glm::mat4 projection = glm::ortho(0.0f, 512.0f, 512.0f, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    float q_a = 48.0f;
    float center_x = (float)SCR_WIDTH/2.0f;
    float center_y = (float)SCR_HEIGHT/2.0f;

    myPrimitive::Quad quad(shaderProgram);
    quad.initialize();

    myPrimitive::LineGrid grid;
    grid.initialize();
    glUseProgram(grid.shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(grid.shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float angle = timeValue * 64.0f;
        //float angle = timeValue * 48.0f;

        glUseProgram(grid.shaderProgram);
        grid.draw();

        /*
        for (float y = q_a / 2.0f + q_a; y < (float)SCR_HEIGHT - q_a; y += q_a)
            for (float x = q_a / 2.0f + q_a; x < (float)SCR_WIDTH - q_a; x += q_a)
                quad.draw(glm::vec3(x, y, 0.0f), 0.0f, q_a);
                */
        glUseProgram(shaderProgram);

        for (float y = 64.0f + q_a/2; y < (float)SCR_HEIGHT - 64.0f; y += 2 * q_a) {
            for (float x = 64.0f + q_a/2; x < (float)SCR_WIDTH - 64.0f; x += 2 * q_a) {
                quad.draw(glm::vec3(x, y, 0.0f), angle, q_a);
                quad.draw(glm::vec3(x + q_a, y + q_a, 0.0f), -angle, q_a);
            }
        }
        /*
        for (float y = 64.0f + q_a/2 + q_a; y < (float)SCR_HEIGHT - 64.0f; y += 2 * q_a) {
            for (float x = 64.0f + q_a/2 + q_a; x < (float)SCR_WIDTH - 64.0f; x += 2 * q_a) {
                //quad.draw(glm::vec3(x, y, 0.0f), -angle, q_a/2.0f);
                quad.draw(glm::vec3(x, y, 0.0f), -angle, q_a);
            }
        }
        */
              
        //quad.draw(glm::vec3(q_a/2, q_a/2, 0.0f), angle, q_a);
        /*
        quad.draw(glm::vec3(center_x + q_a, center_y - q_a, 0.0f), angle, q_a);
        quad.draw(glm::vec3(center_x      , center_y      , 0.0f), -angle, q_a);
        quad.draw(glm::vec3(center_x - q_a, center_y + q_a, 0.0f), angle, q_a);
        quad.draw(glm::vec3(center_x - q_a, center_y - q_a, 0.0f), angle, q_a);
        */

        /*
        quad.draw(glm::vec3(center_x - q_a/2, center_y - q_a/2, 0.0f), angle/2, q_a);
        quad.draw(glm::vec3(center_x - q_a/2, center_y - q_a/2, 0.0f), angle*2, q_a);

        quad.draw(glm::vec3(0.0f, 0.0f, 0.0f), angle*2, q_a);
        quad.draw(glm::vec3((float)SCR_WIDTH - 29.0f, (float)SCR_HEIGHT - 29.0f, 0.0f), angle*12, q_a);
        */
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
