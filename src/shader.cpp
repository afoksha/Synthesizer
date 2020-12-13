//=======================================================================================================================================================================================================================
// shader + program structure methods implementation
//=======================================================================================================================================================================================================================
#include <memory>

#include "shader.hpp"
#include "utils.hpp"
#include "log.hpp"

//=======================================================================================================================================================================================================================
// shader methods implementation
//=======================================================================================================================================================================================================================

glsl_shader_t::glsl_shader_t() : id(0) {}

glsl_shader_t::glsl_shader_t(GLenum shader_type, const char* file_name)
    { init(shader_type, file_name); }

glsl_shader_t::glsl_shader_t(const char* shader_source, GLenum shader_type)
{
    type = shader_type;
    id = compile_from_string(shader_type, shader_source);
}

void glsl_shader_t::init(GLenum shader_type, const char* file_name)
{
    type = shader_type;
    debug_msg("Creating shader : source file = %s", file_name);
    immutable_array_t<uint8_t> source = utils::fileio::freadall(file_name, true);   /* null-terminate the string */
    id = compile_from_string(shader_type, (const char*) source.data);
}

glsl_shader_t::~glsl_shader_t()
    { glDeleteShader(id); }

GLint glsl_shader_t::compile_from_string(GLenum shader_type, const char* source_code)
{
    debug_msg("Compiling shader : type = %d. source code = \n", shader_type);
    put_msg(source_code);

    GLint shader_id = glCreateShader(shader_type);
    glShaderSource (shader_id, 1, &source_code, 0);
    glCompileShader (shader_id);
    GLint compile_status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);

    if (GL_TRUE == compile_status)
    {
        debug_msg("Shader id#%d of type %d successfully compiled.", shader_id, shader_type);
        GLint error_msg_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &error_msg_length);

        if (error_msg_length)
        {
            char* error_msg = static_cast<char*> (malloc((size_t) error_msg_length));
            glGetShaderInfoLog (shader_id, error_msg_length, 0, error_msg);
            debug_msg("Compiler message : ");
            put_msg(error_msg);
            free(error_msg);
        }

        return shader_id;
    }

    debug_msg("Error compiling shader id#%d, type = %d", shader_id, shader_type);
    GLint error_msg_length;
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &error_msg_length);

    if (error_msg_length)
    {
        char* error_msg = static_cast<char*> (malloc((size_t) error_msg_length));
        glGetShaderInfoLog (shader_id, error_msg_length, 0, error_msg);
        debug_msg("Compiler message : ");
        put_msg(error_msg);
        free(error_msg);
    }

    glDeleteShader(shader_id);
    return 0;
}

//=======================================================================================================================================================================================================================
// program methods implementation
//=======================================================================================================================================================================================================================

glsl_program_t::glsl_program_t() : id(0)
    { }

glsl_program_t::glsl_program_t(const glsl_shader_t& cs) : id(0)
    { link(cs); }

glsl_program_t::glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& fs) : id(0)
    { link(vs, fs); }

glsl_program_t::glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& gs, const glsl_shader_t& fs) : id(0)
    { link (vs, gs, fs); }

glsl_program_t::glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& fs) : id(0)
    { link(vs, tcs, tes, fs); }

glsl_program_t::glsl_program_t(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& gs, const glsl_shader_t& fs) : id(0)
    { link(vs, tcs, tes, gs, fs); }

glsl_program_t::glsl_program_t(glsl_program_t&& other)
    { std::swap (id, other.id); }

glsl_program_t& glsl_program_t::operator = (glsl_program_t&& other)
{
    std::swap (id, other.id);
    return *this;
}

void glsl_program_t::generate_id()
{
    if (id)
        glDeleteProgram(id);
    id = glCreateProgram();
}

void glsl_program_t::link(const glsl_shader_t& cs)
    { generate_id(); attach(cs); link(); };

void glsl_program_t::link(const glsl_shader_t& vs, const glsl_shader_t& fs)
    { generate_id(); attach(vs); attach(fs); link(); };

void glsl_program_t::link(const glsl_shader_t& vs, const glsl_shader_t& gs,  const glsl_shader_t& fs)
    { generate_id(); attach(vs); attach(gs); attach(fs); link(); };

void glsl_program_t::link(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& fs)
    { generate_id(); attach(vs); attach(tcs); attach(tes); attach(fs); link(); };

void glsl_program_t::link(const glsl_shader_t& vs, const glsl_shader_t& tcs, const glsl_shader_t& tes, const glsl_shader_t& gs, const glsl_shader_t& fs)
    { generate_id(); attach(vs); attach(tcs); attach(tes); attach(gs); attach(fs); link(); };


void glsl_program_t::attach(const glsl_shader_t& shader)
    { glAttachShader(id, shader.id); };

void glsl_program_t::attach(GLint shader_id)
    { glAttachShader(id, shader_id); }

void glsl_program_t::link()
{
    debug_msg("Linking shader :: id = [%d].", id);
    glLinkProgram(id);
    GLint linkage_status;
    glGetProgramiv(id, GL_LINK_STATUS, &linkage_status);
    if (GL_TRUE == linkage_status)
    {
        debug_msg("Program [%d] successfully linked.", id);
        return;
    }
    GLint error_msg_length;
    glGetProgramiv (id, GL_INFO_LOG_LENGTH, &error_msg_length);

    debug_msg("Program [%d] link not successful. Log message length = %d", id, error_msg_length);
    if (error_msg_length)
    {
        char* error_msg = static_cast<char*>(malloc ((size_t) error_msg_length));
        glGetProgramInfoLog (id, error_msg_length, 0, error_msg);
        debug_msg("Program linkage error : ");
        put_msg(error_msg);
        free(error_msg);
    }
    glDeleteProgram(id);
    id = 0;
    exit_msg("Aborting program ...");
}

glsl_program_t::~glsl_program_t()
{
    glDeleteProgram(id);
}

uniform_t glsl_program_t::operator[] (const char* name) const
    { return uniform_t(*this, name); }

uniform_t glsl_program_t::operator[] (const std::string& name) const
    { return uniform_t(*this, name.c_str()); }


GLint glsl_program_t::uniform_id(const char * name) const
    { return glGetUniformLocation(id, name); }

GLuint glsl_program_t::subroutine_index(GLenum shader_type, const char* name) const
{
    GLuint index = glGetSubroutineIndex(id, shader_type, name);
    debug_msg("Program [%i] subroutine [%s] in shader of type = [%d] has index = [%d]", id, name, shader_type, index);
    return index;
}

GLuint glsl_program_t::subroutine_location(GLenum shader_type, const char* name) const
{
    GLuint location = glGetSubroutineUniformLocation(id, shader_type, name);
    debug_msg("Program [%i] subroutine [%s] uniform in shader of type = [%d] has location = [%d]", id, name, shader_type, location);
    return location;
}

void glsl_program_t::bind_ubo(const char* block_name, GLuint target) const
{
    GLuint ubi = glGetUniformBlockIndex(id, block_name);
    debug_msg("Program [%i] uniform block [%s] has index = [%d]", id, block_name, ubi);
    glUniformBlockBinding(id, ubi, target);
}

void glsl_program_t::bind_ssbo(const char* block_name, GLuint target) const
{
    GLuint ssbi = glGetProgramResourceIndex(id, GL_SHADER_STORAGE_BLOCK, block_name);
    debug_msg("Program [%i] shader storage block [%s] has index = [%d]", id, block_name, ssbi);
    glShaderStorageBlockBinding(id, ssbi, target);
}

void glsl_program_t::enable() const
    { glUseProgram(id); }

void glsl_program_t::disable() const
    { glUseProgram(0); }

GLint glsl_program_t::get_param(GLint param_name)
{
    GLint value;
    glGetProgramiv(id, param_name, &value);
    return value;
}

void glsl_program_t::dump_param(GLint param_name, const char* description)
{
    debug_msg("%s : %d", description, get_param(param_name));
}

void glsl_program_t::dump_info()
{
    dump_param(GL_DELETE_STATUS,                         "Program flagged for deletion");
    dump_param(GL_LINK_STATUS,                           "Last link operation");
    dump_param(GL_VALIDATE_STATUS,                       "Last validation operation");
    dump_param(GL_INFO_LOG_LENGTH,                       "Length of the log information");
    dump_param(GL_ATTACHED_SHADERS,                      "The number of shader objects attached to program");
    dump_param(GL_ACTIVE_ATOMIC_COUNTER_BUFFERS,         "The number of active attribute atomic counter buffers used by program");
    dump_param(GL_ACTIVE_ATTRIBUTES,                     "The number of active attribute variables for program");
    dump_param(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,           "The longest active attribute name for program");
    dump_param(GL_ACTIVE_UNIFORMS,                       "The number of active uniform variables for program");
    dump_param(GL_ACTIVE_UNIFORM_MAX_LENGTH,             "The length of the longest active uniform variable name for program");
    dump_param(GL_PROGRAM_BINARY_LENGTH,                 "The length of the program binary, in bytes");
    dump_param(GL_TRANSFORM_FEEDBACK_BUFFER_MODE,        "The buffer mode used when transform feedback is active (GL_SEPARATE_ATTRIBS or GL_INTERLEAVED_ATTRIBS)");
    dump_param(GL_TRANSFORM_FEEDBACK_VARYINGS,           "The number of varying variables to capture in transform feedback mode for the program");
    dump_param(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, "The longest variable name to be used for transform feedback");
    dump_param(GL_GEOMETRY_VERTICES_OUT,                 "The maximum number of vertices that the geometry shader in program will output");
    dump_param(GL_GEOMETRY_INPUT_TYPE,                   "Geometry shader input primitive type");
    dump_param(GL_GEOMETRY_OUTPUT_TYPE,                  "Geometry shader output primitive type");
    // GL_COMPUTE_WORK_GROUP_SIZE "The local work group size (x, y, z) of the compute program"
}
