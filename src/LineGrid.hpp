#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

namespace myPrimitive {

class LineGrid {
    GLuint VAO;

    float padding = 64.0f;
    float step = 48.0f;

    float max_x = 512.0f - padding;
    float max_y = 512.0f - padding;

    GLuint n;

public:
    LineGrid() {};
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
    this->compile_shader();

    std::vector<float> lines_vertices;

    float min_x = padding;
    for (float y = padding; y <= max_y; y += step) {
        lines_vertices.push_back(min_x);
        lines_vertices.push_back(y);
        lines_vertices.push_back(0.0f);

        lines_vertices.push_back(max_x);
        lines_vertices.push_back(y);
        lines_vertices.push_back(0.0f);
        n++;
    }

    for (float x = padding; x <= max_x; x += step) {
        lines_vertices.push_back(x);
        lines_vertices.push_back(padding);
        lines_vertices.push_back(0.0f);

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
}

void LineGrid::draw()
{
    /*
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, size, 0.0f));

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    */

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2*n);
    glBindVertexArray(0);
}

}

