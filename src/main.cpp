#include <cstdio>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

#include "pinch_data.hpp"
#include "touch_data.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// #define touchpoints continental_touchpoints
// #define touchpoints_count continental_touchpoints_count
// #define pinch_data continental_pinch_data
// #define pinch_data_count continental_pinch_data_count

#define touchpoints lg_touchpoints
#define touchpoints_count lg_touchpoints_count
#define pinch_data lg_pinch_data
#define pinch_data_count lg_pinch_data_count

struct lines_t
{
    int N;
    GLuint vao_id;
    GLuint vbo_id;
    glm::vec4 color;

    lines_t() {}

    lines_t(GLuint vao_id, GLuint vbo_id, glm::vec4 color)
        : vao_id(vao_id), vbo_id(vbo_id), color(color) {}

    lines_t& operator = (const lines_t& other)
    {
        N = other.N;
        vao_id = other.vao_id;
        vbo_id = other.vbo_id;
        color = other.color;
        return *this;
    }

    void render()
    {
        glBindVertexArray(vao_id);
        glDrawArrays(GL_LINES, 0, 2 * N);
    }

};


lines_t generate_pinch_lines(const pinch_data_t* pinch_data, int N, const glm::vec4& color)
{
    lines_t pinch_lines;

    pinch_lines.N = N;
    pinch_lines.color = color;

    glGenVertexArrays(1, &pinch_lines.vao_id);
    glGenBuffers(1, &pinch_lines.vbo_id);

    unsigned int buffer_size = 2 * N * sizeof(glm::vec2);

    glBindVertexArray(pinch_lines.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, pinch_lines.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, 0,  GL_STATIC_DRAW);

    glm::vec2* buffer_data = (glm::vec2*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < N; ++i)
    {
        *(buffer_data++) = pinch_data[i].point0;
        *(buffer_data++) = pinch_data[i].point1;
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return pinch_lines;
}

lines_t generate_touch_lines(const glm::vec2* touch_data, int N, const glm::vec4& color)
{
    lines_t touch_lines;

    touch_lines.N = N;
    touch_lines.color = color;

    glGenVertexArrays(1, &touch_lines.vao_id);
    glGenBuffers(1, &touch_lines.vbo_id);

    unsigned int buffer_size = 2 * N * sizeof(glm::vec2);

    glBindVertexArray(touch_lines.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, touch_lines.vbo_id);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, 0,  GL_STATIC_DRAW);

    glm::vec2* buffer_data = (glm::vec2*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < N; ++i)
    {
        *(buffer_data++) = touch_data[2 * i + 0];
        *(buffer_data++) = touch_data[2 * i + 1];
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return touch_lines;
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
        printf("Failed to create GLFW window.\n");
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

    glsl_program_t touchpoints_shader(glsl_shader_t(GL_VERTEX_SHADER,   "glsl/touch.vs"),
                                      glsl_shader_t(GL_FRAGMENT_SHADER, "glsl/touch.fs"));
    touchpoints_shader.enable();
    uniform_t uni_ts_scale = touchpoints_shader["scale"];
    uniform_t uni_ts_color = touchpoints_shader["color"];

    glm::vec2 lg_scale = glm::vec2(1.0f / 1888, 1.0f / 1728);
    glm::vec2 continental_scale = glm::vec2(1.0f / 1624, 1.0f / 1728);

    lines_t lg_touch_lines = generate_touch_lines(lg_touchpoints, lg_touchpoints_pair_count, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));
    lines_t lg_pinch_lines = generate_pinch_lines(lg_pinch_data, lg_pinch_data_count, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));

    lines_t continental_touch_lines = generate_touch_lines(continental_touchpoints, continental_touchpoints_pair_count, glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
    lines_t continental_pinch_lines = generate_pinch_lines(continental_pinch_data, continental_pinch_data_count, glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* render LG data */
        uni_ts_scale = lg_scale;

        uni_ts_color = lg_touch_lines.color;
        lg_touch_lines.render();

        uni_ts_color = lg_pinch_lines.color;
        lg_pinch_lines.render();

//        /* render continental data */
//        uni_ts_scale = continental_scale;
//
//        uni_ts_color = continental_touch_lines.color;
//        continental_touch_lines.render();
//
//        uni_ts_color = continental_pinch_lines.color;
//        continental_pinch_lines.render();
//
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(touchpoints_shader);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
