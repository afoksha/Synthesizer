#include <cstdio>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/imgui_window.hpp"
#include "gl/shader.hpp"
#include "gl/gl_aux.hpp"
#include "gl/log.hpp"

#include <AL/al.h>
#include <AL/alc.h>

#ifdef LIBAUDIO
#include <audio/wave.h>
#define BACKEND "libaudio"
#else
#include <AL/alut.h>
#define BACKEND "alut"
#endif

static void list_audio_devices(const ALCchar *devices)
{
    const ALCchar* device = devices;
    const ALCchar* next = devices + 1;
    size_t len = 0;

    fprintf(stdout, "Devices list:\n");
    fprintf(stdout, "----------\n");
    while (device && *device != '\0' && next && *next != '\0')
    {
        fprintf(stdout, "%s\n", device);
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    fprintf(stdout, "----------\n");
}

void alValidate(const char* msg)
{
    ALenum error = alGetError();
    if (error != AL_NO_ERROR)
    {
        fprintf(stderr, "%s\n", msg);
        std::exit(-1);
    }
}

static inline ALenum to_al_format(short channels, short samples)
{
    return (samples == 16) ? (channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16) :
           (samples == 8 ) ? (channels > 1 ? AL_FORMAT_STEREO8  : AL_FORMAT_MONO8 ) :
           -1;
}

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

int play_music(int argc, char **argv)
{
    const ALCchar *devices;
    const ALCchar *defaultDeviceName = argv[1];
    int ret;
#ifdef LIBAUDIO
    WaveInfo *wave;
#endif
    char *bufferData;
    ALCdevice *device;
    ALvoid *data;
    ALCcontext *context;
    ALsizei size, freq;
    ALenum format;
    ALuint buffer, source;
    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    ALboolean loop = AL_FALSE;
    ALCenum error;

    fprintf(stdout, "Using " BACKEND " as audio backend\n");

    ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
    if (enumeration == AL_FALSE)
        fprintf(stderr, "enumeration extension not available\n");

    list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

    if (!defaultDeviceName)
        defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

    device = alcOpenDevice(defaultDeviceName);
    if (!device) {
        fprintf(stderr, "unable to open default device\n");
        return -1;
    }

    fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

    alGetError();

    context = alcCreateContext(device, NULL);
    if (!alcMakeContextCurrent(context)) {
        fprintf(stderr, "failed to make default context\n");
        return -1;
    }
    alValidate("make default context");

    /* set orientation */
    alListener3f(AL_POSITION, 0, 0, 1.0f);
    alValidate("listener position");
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alValidate("listener velocity");
    alListenerfv(AL_ORIENTATION, listenerOri);
    alValidate("listener orientation");

    alGenSources((ALuint)1, &source);
    alValidate("source generation");

    alSourcef(source, AL_PITCH, 1);
    alValidate("source pitch");
    alSourcef(source, AL_GAIN, 1);
    alValidate("source gain");
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alValidate("source position");
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alValidate("source velocity");
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alValidate("source looping");

    alGenBuffers(1, &buffer);
    alValidate("buffer generation");

#ifdef LIBAUDIO
    /* load data */
    wave = WaveOpenFileForReading("test.wav");
    if (!wave)
    {
        fprintf(stderr, "failed to read wave file\n");
        return -1;
    }

    ret = WaveSeekFile(0, wave);
    if (ret)
    {
        fprintf(stderr, "failed to seek wave file\n");
        return -1;
    }

    bufferData = malloc(wave->dataSize);
    if (!bufferData)
    {
        perror("malloc");
        return -1;
    }

    ret = WaveReadFile(bufferData, wave->dataSize, wave);
    if (ret != wave->dataSize)
    {
        fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
        return -1;
    }

    alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
            bufferData, wave->dataSize, wave->sampleRate);
    alValidate("failed to load buffer data");
#else
    alutLoadWAVFile((ALbyte*) "wav/test.wav", &format, &data, &size, &freq, &loop);
    alValidate("loading wav file");

    alBufferData(buffer, format, data, size, freq);
    alValidate("buffer copy");
#endif

    alSourcei(source, AL_BUFFER, buffer);
    alValidate("buffer binding");

    alSourcePlay(source);
    alValidate("source playing");

    ALint source_state = AL_PLAYING;

    while (source_state == AL_PLAYING)
    {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        alValidate("source state get");
    }

    /* exit context */
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);

    return 0;
}

int main(int argc, char **argv)
{
    play_music(argc, argv);

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

