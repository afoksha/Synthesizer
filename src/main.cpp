#include <cstdio>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_window.hpp"
#include "shader.hpp"
#include "gl_aux.hpp"
#include "log.hpp"

#include "pinch_data.hpp"
#include "touch_data.hpp"

struct demo_window_t : public imgui_window_t
{
    demo_window_t(const char* title, int glfw_samples, int version_major, int version_minor, int res_x, int res_y, bool fullscreen = true)
        : imgui_window_t(title, glfw_samples, version_major, version_minor, res_x, res_y, fullscreen, true /*, true */)
    {
        gl_aux::dump_info(OPENGL_BASIC_INFO | OPENGL_EXTENSIONS_INFO);
    }

    //===================================================================================================================================================================================================================
    // event handlers
    //===================================================================================================================================================================================================================
    void on_key(int key, int scancode, int action, int mods) override
    {
    }

    void on_mouse_move() override
    {
    }

    void update_ui() override
    {
        /* shaders settings window */
        ImGui::SetNextWindowSize(ImVec2(512, 256), ImGuiCond_FirstUseEver);
        ImGui::Begin("Data deviation", 0);
        ImGui::End();
    }
};

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

    glUnmapBuffer(GL_ARRAY_BUFFER);
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

    glUnmapBuffer(GL_ARRAY_BUFFER);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    return touch_lines;
}

int main()
{
    //===================================================================================================================================================================================================================
    // initialize GLFW library
    // create GLFW window and initialize GLEW library
    // 8AA samples, OpenGL 3.3 context, screen resolution : 1920 x 1080, fullscreen
    //===================================================================================================================================================================================================================
    if (!glfw::init())
        exit_msg("Failed to initialize GLFW library. Exiting ...");

    const int res_x = 1920;
    const int res_y = 1080;
    demo_window_t window("PBR Lighting + Bloom Effect Shader", 4, 3, 3, res_x, res_y, true);

    //===================================================================================================================================================================================================================
    // program begin
    //===================================================================================================================================================================================================================
    glsl_program_t touchpoints_shader(glsl_shader_t(GL_VERTEX_SHADER,   "glsl/touch.vs"),
                                      glsl_shader_t(GL_FRAGMENT_SHADER, "glsl/touch.fs"));
    touchpoints_shader.enable();
    uniform_t uni_ts_scale = touchpoints_shader["scale"];
    uniform_t uni_ts_color = touchpoints_shader["color"];

    glm::vec2 lg_scale = glm::vec2(1.0f / 1888, 1.0f / 1728);
    glm::vec2 continental_scale = glm::vec2(1.0f / 1624, 1.0f / 1728);

    lines_t lg_touch_lines = generate_touch_lines(lg_touchpoints, lg_touchpoints_pair_count, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));
    lines_t lg_nctch_lines = generate_touch_lines(lg_touchpoints_nocomp, lg_touchpoints_nocomp_pair_count, glm::vec4(1.0f, 1.0f, 0.0f, 0.5f));

    lines_t lg_pinch_lines = generate_pinch_lines(lg_pinch_data, lg_pinch_data_count, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));
    lines_t lg_ncpch_lines = generate_pinch_lines(lg_pinch_data_nocomp, lg_pinch_data_nocomp_count, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f));

    lines_t continental_touch_lines = generate_touch_lines(continental_touchpoints, continental_touchpoints_pair_count, glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
    lines_t continental_pinch_lines = generate_pinch_lines(continental_pinch_data, continental_pinch_data_count, glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //===================================================================================================================================================================================================================
    // main loop begin
    //===================================================================================================================================================================================================================
    while (!window.should_close())
    {
        window.new_frame();

        glClearColor(0.09f, 0.01f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        touchpoints_shader.enable();

//        /* render LG data */
//        uni_ts_scale = lg_scale;
//
//        uni_ts_color = lg_touch_lines.color;
//        lg_touch_lines.render();
//
//        uni_ts_color = lg_pinch_lines.color;
//        lg_pinch_lines.render();

        /* render LG no compression data */
        uni_ts_scale = lg_scale;

        uni_ts_color = lg_nctch_lines.color;
        lg_nctch_lines.render();

        uni_ts_color = lg_ncpch_lines.color;
        lg_ncpch_lines.render();

//        /* render continental data */
//        uni_ts_scale = continental_scale;
//
//        uni_ts_color = continental_touch_lines.color;
//        continental_touch_lines.render();
//
//        uni_ts_color = continental_pinch_lines.color;
//        continental_pinch_lines.render();

        window.end_frame();
        //===============================================================================================================================================================================================================
        // After end_frame call ::
        //  - GL_DEPTH_TEST is disabled
        //  - GL_CULL_FACE is disabled
        //  - GL_SCISSOR_TEST is enabled
        //  - GL_BLEND is enabled -- blending mode GL_SRC_ALPHA/GL_ONE_MINUS_SRC_ALPHA with blending function GL_FUNC_ADD
        //  - VAO binding is destroyed
        //===============================================================================================================================================================================================================
        glDisable(GL_SCISSOR_TEST);
        glDisable(GL_BLEND);
    }

    //===================================================================================================================================================================================================================
    // terminate the program and exit
    //===================================================================================================================================================================================================================
    glfw::terminate();
    return 0;
}

