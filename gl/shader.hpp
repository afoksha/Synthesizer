#ifndef _shader_included_245196303876410928396854735684527895739527664352362354
#define _shader_included_245196303876410928396854735684527895739527664352362354

#include <vector>
#include <string>

#include "GL/glew.h"

#include "uniform.hpp"
#include "dsa_uniform.hpp"

struct uniform_t;

//=======================================================================================================================================================================================================================
// GLSL shader structure
//=======================================================================================================================================================================================================================
struct glsl_shader_t
{
    //===================================================================================================================================================================================================================
    // shader identificator and type
    // type must be one of GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, or GL_COMPUTE_SHADER
    //===================================================================================================================================================================================================================
    GLint id;
    GLenum type;

    glsl_shader_t();
    glsl_shader_t(GLenum shader_type, const char* filename);
    glsl_shader_t(const char* shader_source, GLenum shader_type);

    void init(GLenum shader_type, const char* file_name);

    operator int () const
        { return id; }

    ~glsl_shader_t();

    static GLint compile_from_string(GLenum shader_type, const char* source_code);
};

//=======================================================================================================================================================================================================================
// GLSL program structure
//=======================================================================================================================================================================================================================
struct glsl_program_t
{
    //===================================================================================================================================================================================================================
    // program identificator
    //===================================================================================================================================================================================================================
    GLuint id;

    //===================================================================================================================================================================================================================
    // constructors : an OpenGL program can be either :
    // * a single GL_COMPUTE_SHADER
    // * a combination of GL_VERTEX_SHADER + GL_FRAGMENT_SHADER stages
    // * a combination of GL_VERTEX_SHADER + GL_GEOMETRY_SHADER + GL_FRAGMENT_SHADER stages
    // * a combination of GL_VERTEX_SHADER + GL_TESS_CONTROL_SHADER + GL_TESS_EVALUATION_SHADER + GL_FRAGMENT_SHADER stages
    // * a combination of GL_VERTEX_SHADER + GL_TESS_CONTROL_SHADER + GL_TESS_EVALUATION_SHADER + GL_GEOMETRY_SHADER + GL_FRAGMENT_SHADER stages
    //===================================================================================================================================================================================================================
    glsl_program_t();
    glsl_program_t(const glsl_shader_t& cs);
    glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& fs);
    glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& gs,  const glsl_shader_t& fs);
    glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& fs);
    glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& gs, const glsl_shader_t& fs);

    glsl_program_t(glsl_program_t&& other);
    glsl_program_t& operator = (glsl_program_t&& other);

    void generate_id();

    void attach(GLint shader_id);
    void attach(const glsl_shader_t& shader);

    void link();
    void link(const glsl_shader_t& cs);
    void link(const glsl_shader_t& vs, const glsl_shader_t& fs);
    void link(const glsl_shader_t& vs, const glsl_shader_t& gs,  const glsl_shader_t& fs);
    void link(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& fs);
    void link(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& gs, const glsl_shader_t& fs);

    ~glsl_program_t();

    uniform_t operator[] (const char* name) const;
    uniform_t operator[] (const std::string& name) const;

    operator int () const
        { return id; }

    GLint uniform_id(const char* name) const;

    GLuint subroutine_index(GLenum shader_type, const char* name) const;
    GLuint subroutine_location(GLenum shader_type, const char* name) const;

    /* buffer_mode should be must be GL_INTERLEAVED_ATTRIBS or GL_SEPARATE_ATTRIBS */
    template<size_t N> void transform_fb_varyings(const std::array<const char*, N> names, GLenum buffer_mode)
    {
        glTransformFeedbackVaryings(id, N, names.data(), buffer_mode);
        link();
    }

    void bind_ubo(const char* block_name, GLuint target) const;
    void bind_ssbo(const char* block_name, GLuint target) const;

    void enable() const;
    void disable() const;

    GLint get_param(GLint param_name);
    void dump_param(GLint param_name, const char* description);
    void dump_info();
};

#endif /* _shader_included_245196303876410928396854735684527895739527664352362354 */
