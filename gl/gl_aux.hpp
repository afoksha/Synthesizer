#ifndef _gl_info_included_51376526031756134171810846531849755265172359657814125
#define _gl_info_included_51376526031756134171810846531849755265172359657814125

//========================================================================================================================================================================================================================
// Module to easily dump OpenGL implementation-dependent information
//========================================================================================================================================================================================================================
#include <cstdio>

#include "GL/glew.h"
#include "glm/glm.hpp"

enum {
    OPENGL_BASIC_INFO = 0x00000001,
    OPENGL_EXTENSIONS_INFO = 0x00000002,
    OPENGL_COMPUTE_SHADER_INFO = 0x00000004,
};

namespace gl_aux {

inline void dump_info(unsigned int categories)
{
    if (categories & OPENGL_BASIC_INFO)
    {
        printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));
        printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
        printf("GL_SHADING_LANGUAGE_VERSION = %s\n\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    if (categories & OPENGL_EXTENSIONS_INFO)
    {
        GLint glNumExtensions;
        glGetIntegerv(GL_NUM_EXTENSIONS, &glNumExtensions);
        printf("GL_NUM_EXTENSIONS = %d\n", glNumExtensions);
        for (GLint i = 0; i < glNumExtensions; ++i)
            printf("\t#%d : %s\n", i, glGetStringi(GL_EXTENSIONS, i));
    }

    if (categories & OPENGL_COMPUTE_SHADER_INFO)
    {
        GLint glMaxComputeWorkGroupInvocations;
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &glMaxComputeWorkGroupInvocations);
        printf("GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS = %d\n", glMaxComputeWorkGroupInvocations);

        glm::ivec3 glMaxComputeWorkGroupCount;
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &glMaxComputeWorkGroupCount.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &glMaxComputeWorkGroupCount.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &glMaxComputeWorkGroupCount.z);
        printf("GL_MAX_COMPUTE_WORK_GROUP_COUNT = %d x %d x %d\n", glMaxComputeWorkGroupCount.x, glMaxComputeWorkGroupCount.y, glMaxComputeWorkGroupCount.z);

        glm::ivec3 glMaxComputeWorkGroupSize;
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &glMaxComputeWorkGroupSize.x);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &glMaxComputeWorkGroupSize.y);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &glMaxComputeWorkGroupSize.z);
        printf("GL_MAX_COMPUTE_WORK_GROUP_SIZE = %d x %d x %d\n", glMaxComputeWorkGroupSize.x, glMaxComputeWorkGroupSize.y, glMaxComputeWorkGroupSize.z);
   }
}

namespace texture {

inline const char* format_to_string(GLenum format)
{
    switch (format)
    {
        case GL_RED:              return "GL_RED";
        case GL_RG:               return "GL_RG";
        case GL_RGB:              return "GL_RGB";
        case GL_BGR:              return "GL_BGR";
        case GL_RGBA:             return "GL_RGBA";
        case GL_BGRA:             return "GL_BGRA";
        case GL_RED_INTEGER:      return "GL_RED_INTEGER";
        case GL_RG_INTEGER:       return "GL_RG_INTEGER";
        case GL_RGB_INTEGER:      return "GL_RGB_INTEGER";
        case GL_BGR_INTEGER:      return "GL_BGR_INTEGER";
        case GL_RGBA_INTEGER:     return "GL_RGBA_INTEGER";
        case GL_BGRA_INTEGER:     return "GL_BGRA_INTEGER";
        case GL_STENCIL_INDEX:    return "GL_STENCIL_INDEX";
        case GL_DEPTH_COMPONENT:  return "GL_DEPTH_COMPONENT";
        case GL_DEPTH_STENCIL:    return "GL_DEPTH_STENCIL";
    }
    return "";
}

inline const char* internal_format_to_string(GLenum format)
{
    switch (format)
    {
        /* normalized integral formats */
        case GL_R8:               return "GL_R8";
        case GL_R8_SNORM:         return "GL_R8_SNORM";
        case GL_R16:              return "GL_R16";
        case GL_R16_SNORM:        return "GL_R16_SNORM";
        case GL_RG8:              return "GL_RG8";
        case GL_RG8_SNORM:        return "GL_RG8_SNORM";
        case GL_RG16:             return "GL_RG16";
        case GL_RG16_SNORM:       return "GL_RG16_SNORM";
        case GL_RGB8:             return "GL_RGB8";
        case GL_RGB8_SNORM:       return "GL_RGB8_SNORM";
        case GL_RGB16:            return "GL_RGB16";
        case GL_RGB16_SNORM:      return "GL_RGB16_SNORM";
        case GL_RGBA8:            return "GL_RGBA8";
        case GL_RGBA8_SNORM:      return "GL_RGBA8_SNORM";
        case GL_RGBA16:           return "GL_RGBA16";
        case GL_RGBA16_SNORM:     return "GL_RGBA16_SNORM";

        /* normalized unsigned packed integral formats */
        case GL_R3_G3_B2:         return "GL_R3_G3_B2";
        case GL_RGB4:             return "GL_RGB4";
        case GL_RGB5:             return "GL_RGB5";
        case GL_RGB10:            return "GL_RGB10";
        case GL_RGB12:            return "GL_RGB12";
        case GL_RGBA2:            return "GL_RGBA2";
        case GL_RGBA4:            return "GL_RGBA4";
        case GL_RGB565:           return "GL_RGB565";
        case GL_RGB5_A1:          return "GL_RGB5_A1";
        case GL_RGB10_A2:         return "GL_RGB10_A2";
        case GL_RGBA12:           return "GL_RGBA12";

        /* normalized unsigned SRGB integral formats */
        case GL_SRGB8:            return "GL_SRGB8";
        case GL_SRGB8_ALPHA8:     return "GL_SRGB8_ALPHA8";

        /* floating point formats */
        case GL_R16F:             return "GL_R16F";
        case GL_RG16F:            return "GL_RG16F";
        case GL_RGB16F:           return "GL_RGB16F";
        case GL_RGBA16F:          return "GL_RGBA16F";
        case GL_R32F:             return "GL_R32F";
        case GL_RG32F:            return "GL_RG32F";
        case GL_RGB32F:           return "GL_RGB32F";
        case GL_RGBA32F:          return "GL_RGBA32F";
        case GL_R11F_G11F_B10F:   return "GL_R11F_G11F_B10F";
        case GL_RGB9_E5:          return "GL_RGB9_E5";

        /* integral formats */
        case GL_RGB10_A2UI:       return "GL_RGB10_A2UI";
        case GL_R8I:              return "GL_R8I";
        case GL_R8UI:             return "GL_R8UI";
        case GL_R16I:             return "GL_R16I";
        case GL_R16UI:            return "GL_R16UI";
        case GL_R32I:             return "GL_R32I";
        case GL_R32UI:            return "GL_R32UI";
        case GL_RG8I:             return "GL_RG8I";
        case GL_RG8UI:            return "GL_RG8UI";
        case GL_RG16I:            return "GL_RG16I";
        case GL_RG16UI:           return "GL_RG16UI";
        case GL_RG32I:            return "GL_RG32I";
        case GL_RG32UI:           return "GL_RG32UI";
        case GL_RGB8I:            return "GL_RGB8I";
        case GL_RGB8UI:           return "GL_RGB8UI";
        case GL_RGB16I:           return "GL_RGB16I";
        case GL_RGB16UI:          return "GL_RGB16UI";
        case GL_RGB32I:           return "GL_RGB32I";
        case GL_RGB32UI:          return "GL_RGB32UI";
        case GL_RGBA8I:           return "GL_RGBA8I";
        case GL_RGBA8UI:          return "GL_RGBA8UI";
        case GL_RGBA16I:          return "GL_RGBA16I";
        case GL_RGBA16UI:         return "GL_RGBA16UI";
        case GL_RGBA32I:          return "GL_RGBA32I";
        case GL_RGBA32UI:         return "GL_RGBA32UI";

        /* Compressed Internal Texture Formats */
        /* DXT-BPTC formats */
        case GL_COMPRESSED_RED_RGTC1:                      return "GL_COMPRESSED_RED_RGTC1";
        case GL_COMPRESSED_SIGNED_RED_RGTC1:               return "GL_COMPRESSED_SIGNED_RED_RGTC1";
        case GL_COMPRESSED_RG_RGTC2:                       return "GL_COMPRESSED_RG_RGTC2";
        case GL_COMPRESSED_SIGNED_RG_RGTC2:                return "GL_COMPRESSED_SIGNED_RG_RGTC2";
        case GL_COMPRESSED_RGBA_BPTC_UNORM:                return "GL_COMPRESSED_RGBA_BPTC_UNORM";
        case GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM:          return "GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM";
        case GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT:          return "GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT";
        case GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT:        return "GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT";

        /* GL_ETC1_RGB8_OES is the only ETC1 format: it is superceded by GL_COMPRESSED_RGB8_ETC2,
        but still may be used as an identifier in texture container files */
        case 0x8D64:                                       return "GL_ETC1_RGB8_OES";

        /* ETC2 formats */
        case GL_COMPRESSED_R11_EAC:                        return "GL_COMPRESSED_R11_EAC";
        case GL_COMPRESSED_SIGNED_R11_EAC:                 return "GL_COMPRESSED_SIGNED_R11_EAC";
        case GL_COMPRESSED_RG11_EAC:                       return "GL_COMPRESSED_RG11_EAC";
        case GL_COMPRESSED_SIGNED_RG11_EAC:                return "GL_COMPRESSED_SIGNED_RG11_EAC";
        case GL_COMPRESSED_RGB8_ETC2:                      return "GL_COMPRESSED_RGB8_ETC2";
        case GL_COMPRESSED_SRGB8_ETC2:                     return "GL_COMPRESSED_SRGB8_ETC2";
        case GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2:  return "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2";
        case GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2: return "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2";
        case GL_COMPRESSED_RGBA8_ETC2_EAC:                 return "GL_COMPRESSED_RGBA8_ETC2_EAC";
        case GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC:          return "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC";

      #if 0
        /* ASTC formats: ignoring them for now */

        case GL_COMPRESSED_RGBA_ASTC_4x4_KHR: return "GL_COMPRESSED_RGBA_ASTC_4x4_KHR";                     /* 0x93B0 */
        case GL_COMPRESSED_RGBA_ASTC_5x4_KHR: return "GL_COMPRESSED_RGBA_ASTC_5x4_KHR";                     /* 0x93B1 */
        case GL_COMPRESSED_RGBA_ASTC_5x5_KHR: return "GL_COMPRESSED_RGBA_ASTC_5x5_KHR";                     /* 0x93B2 */
        case GL_COMPRESSED_RGBA_ASTC_6x5_KHR: return "GL_COMPRESSED_RGBA_ASTC_6x5_KHR";                     /* 0x93B3 */
        case GL_COMPRESSED_RGBA_ASTC_6x6_KHR: return "GL_COMPRESSED_RGBA_ASTC_6x6_KHR";                     /* 0x93B4 */
        case GL_COMPRESSED_RGBA_ASTC_8x5_KHR: return "GL_COMPRESSED_RGBA_ASTC_8x5_KHR";                     /* 0x93B5 */
        case GL_COMPRESSED_RGBA_ASTC_8x6_KHR: return "GL_COMPRESSED_RGBA_ASTC_8x6_KHR";                     /* 0x93B6 */
        case GL_COMPRESSED_RGBA_ASTC_8x8_KHR: return "GL_COMPRESSED_RGBA_ASTC_8x8_KHR";                     /* 0x93B7 */
        case GL_COMPRESSED_RGBA_ASTC_10x5_KHR: return "GL_COMPRESSED_RGBA_ASTC_10x5_KHR";                   /* 0x93B8 */
        case GL_COMPRESSED_RGBA_ASTC_10x6_KHR: return "GL_COMPRESSED_RGBA_ASTC_10x6_KHR";                   /* 0x93B9 */
        case GL_COMPRESSED_RGBA_ASTC_10x8_KHR: return "GL_COMPRESSED_RGBA_ASTC_10x8_KHR";                   /* 0x93BA */
        case GL_COMPRESSED_RGBA_ASTC_10x10_KHR: return "GL_COMPRESSED_RGBA_ASTC_10x10_KHR";                 /* 0x93BB */
        case GL_COMPRESSED_RGBA_ASTC_12x10_KHR: return "GL_COMPRESSED_RGBA_ASTC_12x10_KHR";                 /* 0x93BC */
        case GL_COMPRESSED_RGBA_ASTC_12x12_KHR: return "GL_COMPRESSED_RGBA_ASTC_12x12_KHR";                 /* 0x93BD */

        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR";     /* 0x93D0 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR";     /* 0x93D1 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR";     /* 0x93D2 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR";     /* 0x93D3 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR";     /* 0x93D4 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR";     /* 0x93D5 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR";     /* 0x93D6 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR";     /* 0x93D7 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR";   /* 0x93D8 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR";   /* 0x93D9 */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR";   /* 0x93DA */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR"; /* 0x93DB */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR"; /* 0x93DC */
        case GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR: return "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR"; /* 0x93DD */
      #endif
    }
    return "";
}

/* Pixel data type parameter values and the corresponding GL data types */
inline const char* type_to_string(GLenum type)
{
    switch (type)
    {
        case GL_UNSIGNED_BYTE:                  return "GL_UNSIGNED_BYTE";                      /* ubyte */
        case GL_BYTE:                           return "GL_BYTE";                               /* byte */
        case GL_UNSIGNED_SHORT:                 return "GL_UNSIGNED_SHORT";                     /* ushort */
        case GL_SHORT:                          return "GL_SHORT";                              /* short */
        case GL_UNSIGNED_INT:                   return "GL_UNSIGNED_INT";                       /* uint */
        case GL_INT:                            return "GL_INT";                                /* int */
        case GL_HALF_FLOAT:                     return "GL_HALF_FLOAT";                         /* half */
        case GL_FLOAT:                          return "GL_FLOAT";                              /* float */
        case GL_UNSIGNED_BYTE_3_3_2:            return "GL_UNSIGNED_BYTE_3_3_2";                /* ubyte */
        case GL_UNSIGNED_BYTE_2_3_3_REV:        return "GL_UNSIGNED_BYTE_2_3_3_REV";            /* ubyte */
        case GL_UNSIGNED_SHORT_5_6_5:           return "GL_UNSIGNED_SHORT_5_6_5";               /* ushort */
        case GL_UNSIGNED_SHORT_5_6_5_REV:       return "GL_UNSIGNED_SHORT_5_6_5_REV";           /* ushort */
        case GL_UNSIGNED_SHORT_4_4_4_4:         return "GL_UNSIGNED_SHORT_4_4_4_4";             /* ushort */
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:     return "GL_UNSIGNED_SHORT_4_4_4_4_REV";         /* ushort */
        case GL_UNSIGNED_SHORT_5_5_5_1:         return "GL_UNSIGNED_SHORT_5_5_5_1";             /* ushort */
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:     return "GL_UNSIGNED_SHORT_1_5_5_5_REV";         /* ushort */
        case GL_UNSIGNED_INT_8_8_8_8:           return "GL_UNSIGNED_INT_8_8_8_8";               /* uint */
        case GL_UNSIGNED_INT_8_8_8_8_REV:       return "GL_UNSIGNED_INT_8_8_8_8_REV";           /* uint */
        case GL_UNSIGNED_INT_10_10_10_2:        return "GL_UNSIGNED_INT_10_10_10_2";            /* uint */
        case GL_UNSIGNED_INT_2_10_10_10_REV:    return "GL_UNSIGNED_INT_2_10_10_10_REV";        /* uint */
        case GL_UNSIGNED_INT_24_8:              return "GL_UNSIGNED_INT_24_8";                  /* DEPTH_STENCIL */
        case GL_UNSIGNED_INT_10F_11F_11F_REV:   return "GL_UNSIGNED_INT_10F_11F_11F_REV";       /* uint */
        case GL_UNSIGNED_INT_5_9_9_9_REV:       return "GL_UNSIGNED_INT_5_9_9_9_REV";           /* uint */
        case GL_FLOAT_32_UNSIGNED_INT_24_8_REV: return "GL_FLOAT_32_UNSIGNED_INT_24_8_REV";     /* DEPTH_STENCIL */
    }
    return "";
}

inline const char* target_to_string(GLenum target)
{
    switch (target)
    {
        case GL_TEXTURE_1D:                     return "GL_TEXTURE_1D";
        case GL_TEXTURE_2D:                     return "GL_TEXTURE_2D";
        case GL_TEXTURE_3D:                     return "GL_TEXTURE_3D";
        case GL_TEXTURE_1D_ARRAY:               return "GL_TEXTURE_1D_ARRAY";
        case GL_TEXTURE_2D_ARRAY:               return "GL_TEXTURE_2D_ARRAY";
        case GL_TEXTURE_RECTANGLE:              return "GL_TEXTURE_RECTANGLE";
        case GL_TEXTURE_CUBE_MAP:               return "GL_TEXTURE_CUBE_MAP";
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:    return "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:    return "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:    return "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:    return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:    return "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:    return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";
        case GL_TEXTURE_CUBE_MAP_ARRAY:         return "GL_TEXTURE_CUBE_MAP_ARRAY";
        case GL_TEXTURE_BUFFER:                 return "GL_TEXTURE_BUFFER";
        case GL_TEXTURE_2D_MULTISAMPLE:         return "GL_TEXTURE_2D_MULTISAMPLE";
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:   return "GL_TEXTURE_2D_MULTISAMPLE_ARRAY";
    }
    return "";
}

inline const char* component_type_to_string(GLenum type)
{
    switch (type)
    {
        case GL_SIGNED_NORMALIZED:   return "SNORM";
        case GL_UNSIGNED_NORMALIZED: return "UNORM";
        case GL_FLOAT:               return "FLOAT";
        case GL_INT:                 return "INT";
        case GL_UNSIGNED_INT:        return "UINT";
    }
    return "NONE";
}

inline void dump_info(GLenum target, GLint levels)
{
    for (int l = 0; l < levels; ++l)
    {
        printf("\nTexture level #%d info:\n", l);

        GLint x, y, z, w;
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_WIDTH,  &x);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_HEIGHT, &y);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_DEPTH,  &z);
        printf("\tWIDTH x HEIGHT x DEPTH : {%d x %d x %d}\n", x, y, z);

        GLint internal_format;
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);
        printf("\tINTERNAL_FORMAT : %s\n", internal_format_to_string(internal_format));

        glGetTexLevelParameteriv(target, l, GL_TEXTURE_RED_SIZE,   &x);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_GREEN_SIZE, &y);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_BLUE_SIZE,  &z);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_ALPHA_SIZE, &w);
        printf("\tRED_SIZE x GREEN_SIZE x BLUE_SIZE x ALPHA_SIZE : {%d x %d x %d x %d}\n", x, y, z, w);

        glGetTexLevelParameteriv(target, l, GL_TEXTURE_RED_TYPE,   &x);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_GREEN_TYPE, &y);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_BLUE_TYPE,  &z);
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_ALPHA_TYPE, &w);
        printf("\tRED_TYPE x GREEN_TYPE x BLUE_TYPE x ALPHA_TYPE : {%s x %s x %s x %s}\n",
            component_type_to_string(x), component_type_to_string(y), component_type_to_string(z), component_type_to_string(w));

        GLint compressed;
        glGetTexLevelParameteriv(target, l, GL_TEXTURE_COMPRESSED, &compressed);
        printf("\tTEXTURE_COMPRESSED : %d\n", compressed);

        if (compressed)
        {
            GLint compressed_image_size;
            glGetTexLevelParameteriv(target, l, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_image_size);
            printf("\tCOMPRESSED_IMAGE_SIZE : %d\n", compressed_image_size);
        }

        //================================================================================================================================================================================================================
        // parameters valid for depth and buffer textures are not queried for now ::
        //   GL_TEXTURE_DEPTH_TYPE, GL_TEXTURE_DEPTH_SIZE, GL_TEXTURE_BUFFER_OFFSET
        //================================================================================================================================================================================================================
    }
}

} /* namespace texture */

} /* namespace gl_aux */

#endif /* _gl_info_included_51376526031756134171810846531849755265172359657814125 */
