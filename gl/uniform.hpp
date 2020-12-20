#ifndef _uniform_included_12854961357283548273562875462587340867534896739846347
#define _uniform_included_12854961357283548273562875462587340867534896739846347

#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.hpp"
#include "shader_program.hpp"
#include "immutable_array.hpp"

struct dsa_uniform_t;
struct glsl_program_t;
struct glsl_shader_program_t;

//=======================================================================================================================================================================================================================
// structure representing OpenGL uniform variable
//=======================================================================================================================================================================================================================

struct uniform_t
{
    GLuint program_id;
    GLint location;

    uniform_t() {}
    ~uniform_t() {}

    uniform_t(const glsl_program_t& program, const char* name);
    uniform_t(const glsl_shader_program_t& shader_program, const char* name);

    void init(const glsl_program_t& program, const char* name);
    void init(const glsl_shader_program_t& shader_program, const char* name);

    uniform_t& operator = (const uniform_t& rhs);
    uniform_t& operator = (const dsa_uniform_t& rhs);

    operator bool() const
        { return location != -1; }

    operator GLint() const
        { return location; }

    static void subroutine(GLenum shadertype, const GLuint* indices)
        { glUniformSubroutinesuiv(shadertype, 1, indices); }
    static void subroutines(GLenum shadertype, GLsizei count, const GLuint* indices)
        { glUniformSubroutinesuiv(shadertype, count, indices); }

    template<std::size_t N> static void subroutines(GLenum shadertype, const std::array<GLuint, N>& attachments)
    {
        glUniformSubroutinesuiv(shadertype, N, attachments.data());
    }

    //===================================================================================================================================================================================================================
    // integral vectors
    //===================================================================================================================================================================================================================
    void operator = (const GLint arg)
        { glUniform1i(location, arg); }
    void operator = (const glm::ivec2& arg)
        { glUniform2iv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::ivec3& arg)
        { glUniform3iv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::ivec4& arg)
        { glUniform4iv(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const GLint (&arg) [S])
        { glUniform1iv(location, S, &arg); }
    template <size_t S> void operator = (const glm::ivec2 (&arg) [S])
        { glUniform2iv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::ivec3 (&arg) [S])
        { glUniform3iv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::ivec4 (&arg) [S])
        { glUniform4iv(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<GLint, S>& array)
        { glUniform1iv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::ivec2, S>& array)
        { glUniform2iv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::ivec3, S>& array)
        { glUniform3iv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::ivec4, S>& array)
        { glUniform4iv(location, S, array.data()); }



    //===================================================================================================================================================================================================================
    // signed integral 64-bit vectors
    //===================================================================================================================================================================================================================
    void operator = (const int64_t arg)
        { glUniform1i64NV(location, arg); }
    void operator = (const glm::i64vec2& arg)
        { glUniform2i64vNV(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::i64vec3& arg)
        { glUniform3i64vNV(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::i64vec4& arg)
        { glUniform4i64vNV(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of signed 64-bit integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const int64_t (&arg) [S])
        { glUniform1i64vNV(location, S, &arg); }
    template <size_t S> void operator = (const glm::i64vec2 (&arg) [S])
        { glUniform2i64vNV(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::i64vec3 (&arg) [S])
        { glUniform3i64vNV(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::i64vec4 (&arg) [S])
        { glUniform4i64vNV(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of signed 64-bit integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<int64_t, S>& array)
        { glUniform1i64vNV(location, S, &array[0]); }
    template <size_t S> void operator = (const std::array<glm::i64vec2, S>& array)
        { glUniform2i64vNV(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::i64vec3, S>& array)
        { glUniform3i64vNV(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::i64vec4, S>& array)
        { glUniform4i64vNV(location, S, glm::value_ptr(array[0])); }



    //===================================================================================================================================================================================================================
    // unsigned integral vectors
    //===================================================================================================================================================================================================================
    void operator = (const unsigned int arg)
        { glUniform1ui(location, arg); }
    void operator = (const glm::uvec2& arg)
        { glUniform2uiv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::uvec3& arg)
        { glUniform3uiv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::uvec4& arg)
        { glUniform4uiv(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of unsigned integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const GLuint (&arg) [S])
        { glUniform1uiv(location, S, &arg); }
    template <size_t S> void operator = (const glm::uvec2 (&arg) [S])
        { glUniform2uiv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::uvec3 (&arg) [S])
        { glUniform3uiv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::uvec4 (&arg) [S])
        { glUniform4uiv(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of unsigned integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<GLuint, S>& array)
        { glUniform1uiv(location, S, &array[0]); }
    template <size_t S> void operator = (const std::array<glm::uvec2, S>& array)
        { glUniform2uiv(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::uvec3, S>& array)
        { glUniform3uiv(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::uvec4, S>& array)
        { glUniform4uiv(location, S, glm::value_ptr(array[0])); }


    //===================================================================================================================================================================================================================
    // unsigned integral 64-bit vectors
    //===================================================================================================================================================================================================================
    void operator = (const uint64_t arg)
        { glUniform1ui64NV(location, arg); }
    void operator = (const glm::u64vec2& arg)
        { glUniform2ui64vNV(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::u64vec3& arg)
        { glUniform3ui64vNV(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::u64vec4& arg)
        { glUniform4ui64vNV(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of unsigned 64-bit integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const uint64_t (&arg) [S])
        { glUniform1ui64vNV(location, S, &arg); }
    template <size_t S> void operator = (const glm::u64vec2 (&arg) [S])
        { glUniform2ui64vNV(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::u64vec3 (&arg) [S])
        { glUniform3ui64vNV(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::u64vec4 (&arg) [S])
        { glUniform4ui64vNV(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of unsigned 64-bit integral vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<uint64_t, S>& array)
        { glUniform1ui64vNV(location, S, &array[0]); }
    template <size_t S> void operator = (const std::array<glm::u64vec2, S>& array)
        { glUniform2ui64vNV(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::u64vec3, S>& array)
        { glUniform3ui64vNV(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::u64vec4, S>& array)
        { glUniform4ui64vNV(location, S, glm::value_ptr(array[0])); }



    //===================================================================================================================================================================================================================
    // floating point single-precision vectors
    //===================================================================================================================================================================================================================
    void operator = (const GLfloat arg)
        { glUniform1f(location, arg); }
    void operator = (const glm::vec2& arg)
        { glUniform2fv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::vec3& arg)
        { glUniform3fv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::vec4& arg)
        { glUniform4fv(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of floating point single-precision vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const GLfloat (&arg) [S])
        { glUniform1fv(location, S, (const GLfloat*) &arg); }
    template <size_t S> void operator = (const glm::vec2 (&arg) [S])
        { glUniform2fv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::vec3 (&arg) [S])
        { glUniform3fv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::vec4 (&arg) [S])
        { glUniform4fv(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of floating point single-precision vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<GLfloat, S>& array)
        { glUniform1fv(location, S, &array[0]); }
    template <size_t S> void operator = (const std::array<glm::vec2, S>& array)
        { glUniform2fv(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::vec3, S>& array)
        { glUniform3fv(location, S, glm::value_ptr(array[0])); }
    template <size_t S> void operator = (const std::array<glm::vec4, S>& array)
        { glUniform4fv(location, S, glm::value_ptr(array[0])); }

    //===================================================================================================================================================================================================================
    // immutable array of float/vec2/vec3/vec4
    //===================================================================================================================================================================================================================
    void operator = (const immutable_array_t<GLfloat>& array)
        { glUniform1fv(location, array.size, array.data); }
    void operator = (const immutable_array_t<glm::vec2>& array)
        { glUniform2fv(location, array.size, glm::value_ptr(array[0])); }
    void operator = (const immutable_array_t<glm::vec3>& array)
        { glUniform3fv(location, array.size, glm::value_ptr(array[0])); }
    void operator = (const immutable_array_t<glm::vec4>& array)
        { glUniform4fv(location, array.size, glm::value_ptr(array[0])); }

    //===================================================================================================================================================================================================================
    // floating point double-precision vectors
    //===================================================================================================================================================================================================================
    void operator = (const double arg)
        { glUniform1d(location, arg); }
    void operator = (const glm::dvec2& arg)
        { glUniform2dv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::dvec3& arg)
        { glUniform3dv(location, 1, glm::value_ptr(arg)); }
    void operator = (const glm::dvec4& arg)
        { glUniform4dv(location, 1, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // c-arrays of floating point double-precision vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const GLdouble (&arg) [S])
        { glUniform1dv(location, S, &arg); }
    template <size_t S> void operator = (const glm::dvec2 (&arg) [S])
        { glUniform2dv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dvec3 (&arg) [S])
        { glUniform3dv(location, S, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dvec4 (&arg) [S])
        { glUniform4dv(location, S, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // c++ std::arrays of floating point double-precision vectors
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const std::array<GLdouble, S>& array)
        { glUniform1fv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::dvec2, S>& array)
        { glUniform2fv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::dvec3, S>& array)
        { glUniform3fv(location, S, array.data()); }
    template <size_t S> void operator = (const std::array<glm::dvec4, S>& array)
        { glUniform4fv(location, S, array.data()); }



    //===================================================================================================================================================================================================================
    // floating point single-precision matrices
    //===================================================================================================================================================================================================================
    void operator = (const glm::mat2& arg)
        { glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat2x3& arg)
        { glUniformMatrix2x3fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat2x4& arg)
        { glUniformMatrix2x4fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    void operator = (const glm::mat3x2& arg)
        { glUniformMatrix3x2fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat3& arg)
        { glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat3x4& arg)
        { glUniformMatrix3x4fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    void operator = (const glm::mat4x2& arg)
        { glUniformMatrix4x2fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat4x3& arg)
        { glUniformMatrix4x3fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::mat4& arg)
        { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // arrays of floating point single-precision matrices
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const glm::mat2 (&arg) [S])
        { glUniformMatrix2fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat2x3 (&arg) [S])
        { glUniformMatrix2x3fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat2x4 (&arg) [S])
        { glUniformMatrix2x4fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

    template <size_t S> void operator = (const glm::mat3x2 (&arg) [S])
        { glUniformMatrix3x2fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat3 (&arg) [S])
        { glUniformMatrix3fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat3x4 (&arg) [S])
        { glUniformMatrix3x4fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

    template <size_t S> void operator = (const glm::mat4x2 (&arg) [S])
        { glUniformMatrix4x2fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat4x3 (&arg) [S])
        { glUniformMatrix4x3fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::mat4 (&arg) [S])
        { glUniformMatrix4fv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

    //===================================================================================================================================================================================================================
    // floating point double-precision matrices
    //===================================================================================================================================================================================================================
    void operator = (const glm::dmat2& arg)
        { glUniformMatrix2dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat2x3& arg)
        { glUniformMatrix2x3dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat2x4& arg)
        { glUniformMatrix2x4dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    void operator = (const glm::dmat3x2& arg)
        { glUniformMatrix3x2dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat3& arg)
        { glUniformMatrix3dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat3x4& arg)
        { glUniformMatrix3x4dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    void operator = (const glm::dmat4x2& arg)
        { glUniformMatrix4x2dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat4x3& arg)
        { glUniformMatrix4x3dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }
    void operator = (const glm::dmat4& arg)
        { glUniformMatrix4dv(location, 1, GL_FALSE, glm::value_ptr(arg)); }

    //===================================================================================================================================================================================================================
    // arrays of floating point double-precision matrices
    //===================================================================================================================================================================================================================
    template <size_t S> void operator = (const glm::dmat2 (&arg) [S])
        { glUniformMatrix2dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat2x3 (&arg) [S])
        { glUniformMatrix2x3dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat2x4 (&arg) [S])
        { glUniformMatrix2x4dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

    template <size_t S> void operator = (const glm::dmat3x2 (&arg) [S])
        { glUniformMatrix3x2dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat3 (&arg) [S])
        { glUniformMatrix3dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat3x4 (&arg) [S])
        { glUniformMatrix3x4dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

    template <size_t S> void operator = (const glm::dmat4x2 (&arg) [S])
        { glUniformMatrix4x2dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat4x3 (&arg) [S])
        { glUniformMatrix4x3dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }
    template <size_t S> void operator = (const glm::dmat4 (&arg) [S])
        { glUniformMatrix4dv(location, S, GL_FALSE, glm::value_ptr(arg[0])); }

};

#endif  // _uniform_included_12854961357283548273562875462587340867534896739846347




