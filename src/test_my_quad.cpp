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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HELLO!", NULL, NULL);
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float q_a = 48.0f;
    float padding = 64.0f;

    myPrimitive::LineGrid grid((float)SCR_WIDTH, (float)SCR_HEIGHT, padding, q_a);
    grid.initialize();

    myPrimitive::Quad quad;
    quad.initialize();

    float min_x_grid = padding + q_a/2.0f;
    float min_y_grid = padding + q_a/2.0f;
    float max_x_grid = (float)SCR_WIDTH - padding - q_a/2.0f;
    float max_y_grid = (float)SCR_HEIGHT - padding - q_a/2.0f;
    float step_quads = 2.0f * q_a;


    // time management
    GLfloat currentFrame = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
    GLfloat time = 0.0f;
    GLuint  fps = 0;
    GLuint  last_fps = 0;

    /* -----------
     *  Main loop
     * ----------- */
    while (!glfwWindowShouldClose(window))
    {
        // - calculate time spent on last frame
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // - periodcally display the FPS the game is running in
        time += deltaTime;
        ++fps;
        if (time >= 1.0f) {
            time = 1.0f - time;
            if (fps != last_fps) {
                glfwSetWindowTitle(window, std::string("FPS: " + std::to_string(fps)).c_str());
                last_fps = fps;
                std::cout << fps << "\n";
            }
            fps = 0;
        }

        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float timeValue = glfwGetTime();
        float angle = timeValue * 64.0f;

        grid.draw();
        for (float y = min_y_grid; y < max_y_grid; y += step_quads) {
            for (float x = min_x_grid; x < max_x_grid; x += step_quads) {
                quad.draw(glm::vec3(x      , y      , 0.0f),  angle, q_a);
                quad.draw(glm::vec3(x + q_a, y + q_a, 0.0f), -angle, q_a);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);

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
