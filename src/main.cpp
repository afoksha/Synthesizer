#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "pinch_data.hpp"
#include "touch_points.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const char* vsTouchpoints = "#version 330 core\n"
    "layout (location = 0) in vec2 tp_in;\n"
    "uniform vec2 scale;\n"
    "void main()\n"
    "{\n"
    "   vec2 tp = 2.0 * tp_in * scale - 1.0;\n"
    "   gl_Position = vec4(tp.x, tp.y, 0.0, 1.0);\n"
    "}\0";

const char* fsTouchpoints = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// #define touchpoints continental_touchpoints
// #define touchpoints_count continental_touchpoints_count
// #define pinch_data continental_pinch_data
// #define pinch_data_count continental_pinch_data_count

#define touchpoints lg_touchpoints
#define touchpoints_count lg_touchpoints_count
#define pinch_data lg_pinch_data
#define pinch_data_count lg_pinch_data_count

unsigned int createShaderProgram(const char* vsSource, const char* fsSource)
{
    unsigned int vs_id = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fs_id = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int program_id = glCreateProgram();

    glShaderSource(vs_id, 1, &vsSource, NULL);
    glCompileShader(vs_id);
    glShaderSource(fs_id, 1, &fsSource, NULL);
    glCompileShader(fs_id);

    glAttachShader(program_id, vs_id);
    glAttachShader(program_id, fs_id);
    glLinkProgram(program_id);

    return program_id;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //===============================================================================================================================================================================================================
    // GLEW library initialization
    //===============================================================================================================================================================================================================
    glewExperimental = true;                                                                                                // needed in core profile
    GLenum result = glewInit();                                                                                             // initialise GLEW
    if (result != GLEW_OK)
    {
        glfwTerminate();
        printf("Failed to initialize GLEW : %s", glewGetErrorString(result));
        return -1;
    }
    printf("GLEW library initialization done ... ");

    unsigned int touchpoints_shader = createShaderProgram(vsTouchpoints, fsTouchpoints);
    glUseProgram(touchpoints_shader);

    glm::vec2 scale = glm::vec2(1.0f / 1888, 1.0f / 1728);
    int scaleLocation = glGetUniformLocation(touchpoints_shader, "scale");
    glUniform2fv(scaleLocation, 1, glm::value_ptr(scale));

    // ===================================================================================================================================================
    unsigned int vao_id, vbo_id;
    glGenVertexArrays(1, &vao_id); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(1, &vbo_id);

    glBindVertexArray(vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(touchpoints), glm::value_ptr(touchpoints[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_LINES, 0, touchpoints_count);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao_id);
    glDeleteBuffers(1, &vbo_id);
    glDeleteProgram(touchpoints_shader);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
