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

        ImGui::Begin("Synthesizer settings", 0);

        ImGui::Text("Select accords: ");

        static int x = 0;

        ImGui::Separator();
        ImGui::Columns(3, "dgeflkssgs", false);

        ImGui::RadioButton("Dur + Dur", &x, 0);
        ImGui::RadioButton("Mol\' + Mol\'", &x, 1);
        ImGui::NextColumn();
        ImGui::RadioButton("Dur + Mol", &x, 2);
        ImGui::RadioButton("Maj + Maj", &x, 3);
        ImGui::NextColumn();
        ImGui::RadioButton("Min + Min", &x, 4);
        ImGui::RadioButton("Ostal\'noje", &x, 5);
        ImGui::NextColumn();

        ImGui::Columns(1);

        ImGui::Separator();

        static int takt = 4;
        ImGui::SliderInt("Number of tactes", &takt, 1, 20);

        ImGui::Separator();

        static int octave = 5;
        ImGui::SliderInt("Octave", &octave, 1, 12);

        ImGui::Separator();

        static bool enable_syncopes = false;
        ImGui::Checkbox("Syncopes enabled", &enable_syncopes);

        ImGui::Separator();

        static int instrument = 1;
        ImGui::Columns(3, "dgeflkssgs", false);
        ImGui::RadioButton("Pivanina", &instrument, 1);
        ImGui::RadioButton("Getarka", &instrument, 2);
        ImGui::NextColumn();
        ImGui::RadioButton("Bzdudka", &instrument, 3);
        ImGui::RadioButton("Trombon", &instrument, 4);
        ImGui::NextColumn();
        ImGui::RadioButton("Kontrabzdas", &instrument, 5);
        ImGui::RadioButton("Bzdarabzdan", &instrument, 6);
        ImGui::NextColumn();

        ImGui::Columns(1);

        ImGui::End();

        ImGui::ShowDemoWindow();
    }
};

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
    demo_window_t window("Synthesizer", 4, 3, 3, res_x, res_y, true);

    //===================================================================================================================================================================================================================
    // program begin
    //===================================================================================================================================================================================================================

    //===================================================================================================================================================================================================================
    // main loop begin
    //===================================================================================================================================================================================================================
    while (!window.should_close())
    {
        window.new_frame();

        glClearColor(0.09f, 0.01f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


















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

