#include <glad/glad.h>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <iostream>
#include <ostream>

namespace myPrimitive {

class LineGrid {
    GLuint VAO;

    float padding;
    float step;

    float SCR_WIDTH;
    float SCR_HEIGHT;

    float max_x;
    float max_y;
    GLuint n;
public:
    LineGrid(float a_SCR_WIDTH, float a_SCR_HEIGHT, float a_padding, float a_step) {
        SCR_WIDTH = a_SCR_WIDTH;
        SCR_HEIGHT = a_SCR_HEIGHT;
        padding = a_padding;
        step = a_step;
    };
    ~LineGrid() {};

    /* ONCE */
    void compile_shader();
    void initialize();
    void draw();
    GLuint shaderProgram;
};

void LineGrid::compile_shader()
{
    const char *vertexShaderSource = "#version 420 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource = "#version 420 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(0.16f, 0.16f, 0.16f, 1.0f);\n"
        "}\n\0";

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
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
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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
    shaderProgram = glCreateProgram();
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
}



void LineGrid::initialize()
{
    max_x = SCR_WIDTH - padding;
    max_y = SCR_HEIGHT - padding;
    this->compile_shader();

    std::vector<float> lines_vertices;

    float min_x = padding;
    for (float y = padding; y <= max_y; y += step) {
        lines_vertices.push_back(min_x);
        lines_vertices.push_back(y);
        lines_vertices.push_back(0.0f);
        n++;
        lines_vertices.push_back(max_x);
        lines_vertices.push_back(y);
        lines_vertices.push_back(0.0f);
        n++;
    }

    for (float x = padding; x <= max_x; x += step) {
        lines_vertices.push_back(x);
        lines_vertices.push_back(padding);
        lines_vertices.push_back(0.0f);
        n++;
        lines_vertices.push_back(x);
        lines_vertices.push_back(max_y);
        lines_vertices.push_back(0.0f);
        n++;
    }

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines_vertices.size() * sizeof(float), &lines_vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    glm::mat4 projection = glm::ortho(0.0f, 512.0f, 512.0f, 0.0f, -1.0f, 1.0f);
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
}

void LineGrid::draw()
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, n);
    glBindVertexArray(0);
}


}

