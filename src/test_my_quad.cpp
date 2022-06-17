#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Quad.hpp"
#include "LineGrid.hpp"
#include "Player.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, myGame::Player& player);

myGame::Player player;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

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

//glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    float q_a = 48.0f;
    //float q_a = 128.0f;
    float padding = 64.0f;

    myPrimitive::LineGrid grid((float)SCR_WIDTH, (float)SCR_HEIGHT, padding, q_a);
    grid.initialize();

    myPrimitive::Quad quad;
    quad.initialize();

    //myGame::Player player;
    player.size = q_a;
    player.p_pos = glm::vec3(padding + q_a * 1 + q_a/2.0f, padding + q_a * 1 + q_a/2.0f, 0.0f);
    player.max_x_pos = (float)SCR_WIDTH - padding - q_a/2.0f;
    player.max_y_pos = (float)SCR_HEIGHT - padding - q_a/2.0f;
    player.min_x_pos = padding + q_a/2.0f;
    player.min_y_pos = padding + q_a/2.0f;
    player.initialize();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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
    unsigned int n_loop = 0;
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /*
        n_loop = (n_loop + 1) % 60;
        if (n_loop == 3) player.p_pos.x -= player.size;
        if (player.p_pos.x < min_x_grid) player.p_pos.x = max_x_grid;
        */

        grid.draw();

        /*
        float angle = currentFrame * 64.0f;
        for (float y = min_y_grid; y < max_y_grid; y += step_quads) {
            for (float x = min_x_grid; x < max_x_grid; x += step_quads) {
                quad.draw(glm::vec3(x      , y      , 0.0f),  angle, q_a);
                quad.draw(glm::vec3(x + q_a, y + q_a, 0.0f), -angle, q_a);
            }
        }
        */

        player.draw();

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

GLfloat currentFrame = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;
    float q_a = 48.0f;
    float padding = 64.0f;
    float max_x_grid = (float)SCR_WIDTH - padding - q_a/2.0f;
    float max_y_grid = (float)SCR_HEIGHT - padding - q_a/2.0f;
    float min_x_grid = padding + q_a/2.0f;
    float min_y_grid = padding + q_a/2.0f;

enum MOVE_DIR {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT,
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    /*
    if (action == GLFW_RELEASE) {
        return;
    }
    switch (key) {
            case GLFW_KEY_J: player.p_pos.y += player.size; break;
            case GLFW_KEY_K: player.p_pos.y -= player.size; break;
            case GLFW_KEY_L: player.p_pos.x += player.size; break;
            case GLFW_KEY_H: player.p_pos.x -= player.size; break;
        }
    */
    //GLfloat currentFrame = 0.0f, deltaTime = 0.0f, lastFrame = 0.0f;

    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    if ((action != GLFW_RELEASE) && deltaTime >= 0.02f) {
        unsigned int dir = 5;
        switch (key) {
        case GLFW_KEY_J: dir = DOWN; break;
        case GLFW_KEY_K: dir = UP; break;
        case GLFW_KEY_H: dir = LEFT; break;
        case GLFW_KEY_L: dir = RIGHT; break;
        }
        player.move(dir);
        lastFrame = currentFrame;
        //std::cout << deltaTime << "\n";
    }


    /*
    if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        player.p_pos.y += player.size;
    }
    */

    /*
    if (key == GLFW_KEY_J && action != GLFW_RELEASE) {
        player.p_pos.y += player.size;
    }

    if (key == GLFW_KEY_K && action == GLFW_RELEASE) {
        player.p_pos.y -= player.size;
    }

    if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
        player.p_pos.x += player.size;
    }

    if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
        player.p_pos.x -= player.size;
    }
    */

    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << xpos << " " << ypos << "\n";
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
