#include <overkill/gl_util.hpp>

GLsizei GLTypeSize(const GLenum type) //TODO replace with a "magical" template specifier i.e. template<>GLTypeSize<GL_FLOAT>(){return sizeof(GLfloat);}
{
    switch (type)
    {
		case GL_FLOAT										: return sizeof(GLfloat);
		case GL_FLOAT_VEC2									: return sizeof(GLfloat)*2;
		case GL_FLOAT_VEC3									: return sizeof(GLfloat)*3;
		case GL_FLOAT_VEC4									: return sizeof(GLfloat)*4;
		case GL_DOUBLE										: return sizeof(GLdouble);
		case GL_DOUBLE_VEC2									: return sizeof(GLdouble)*2;
		case GL_DOUBLE_VEC3									: return sizeof(GLdouble)*3;
		case GL_DOUBLE_VEC4									: return sizeof(GLdouble)*4;
		case GL_INT											: return sizeof(GLint);
		case GL_INT_VEC2									: return sizeof(GLint)*2;
		case GL_INT_VEC3									: return sizeof(GLint)*3;
		case GL_INT_VEC4									: return sizeof(GLint)*4;
		case GL_UNSIGNED_INT								: return sizeof(GLuint);
		case GL_UNSIGNED_INT_VEC2							: return sizeof(GLuint)*2;
		case GL_UNSIGNED_INT_VEC3							: return sizeof(GLuint)*3;
		case GL_UNSIGNED_INT_VEC4							: return sizeof(GLuint)*4;
		case GL_BOOL										: return sizeof(GLboolean);
		case GL_BOOL_VEC2									: return sizeof(GLboolean)*2;
		case GL_BOOL_VEC3									: return sizeof(GLboolean)*3;
		case GL_BOOL_VEC4									: return sizeof(GLboolean)*4;
		case GL_FLOAT_MAT2									: return sizeof(GLfloat)*2*2;
		case GL_FLOAT_MAT3									: return sizeof(GLfloat)*3*3;
		case GL_FLOAT_MAT4									: return sizeof(GLfloat)*4*4;
		case GL_FLOAT_MAT2x3								: return sizeof(GLfloat)*2*3;
		case GL_FLOAT_MAT2x4								: return sizeof(GLfloat)*2*4;
		case GL_FLOAT_MAT3x2								: return sizeof(GLfloat)*3*2;
		case GL_FLOAT_MAT3x4								: return sizeof(GLfloat)*3*4;
		case GL_FLOAT_MAT4x2								: return sizeof(GLfloat)*4*2;
		case GL_FLOAT_MAT4x3								: return sizeof(GLfloat)*4*3;
		case GL_DOUBLE_MAT2									: return sizeof(GLdouble)*2*2;
		case GL_DOUBLE_MAT3									: return sizeof(GLdouble)*3*3;
		case GL_DOUBLE_MAT4									: return sizeof(GLdouble)*4*4;
		case GL_DOUBLE_MAT2x3								: return sizeof(GLdouble)*2*3;
		case GL_DOUBLE_MAT2x4								: return sizeof(GLdouble)*2*4;
		case GL_DOUBLE_MAT3x2								: return sizeof(GLdouble)*3*2;
		case GL_DOUBLE_MAT3x4								: return sizeof(GLdouble)*3*4;
		case GL_DOUBLE_MAT4x2								: return sizeof(GLdouble)*4*2;
		case GL_DOUBLE_MAT4x3								: return sizeof(GLdouble)*4*3;
		case GL_SAMPLER_1D									: 
		case GL_SAMPLER_2D									: 
		case GL_SAMPLER_3D									: 
		case GL_SAMPLER_CUBE								: 
		case GL_SAMPLER_1D_SHADOW							: 
		case GL_SAMPLER_2D_SHADOW							: 
		case GL_SAMPLER_1D_ARRAY							: 
		case GL_SAMPLER_2D_ARRAY							: 
		case GL_SAMPLER_1D_ARRAY_SHADOW						: 
		case GL_SAMPLER_2D_ARRAY_SHADOW						: 
		case GL_SAMPLER_2D_MULTISAMPLE						: 
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY				: 
		case GL_SAMPLER_CUBE_SHADOW							: 
		case GL_SAMPLER_BUFFER								: 
		case GL_SAMPLER_2D_RECT								: 
		case GL_SAMPLER_2D_RECT_SHADOW						: 
		case GL_INT_SAMPLER_1D								: 
		case GL_INT_SAMPLER_2D								: 
		case GL_INT_SAMPLER_3D								: 
		case GL_INT_SAMPLER_CUBE							: 
		case GL_INT_SAMPLER_1D_ARRAY						: 
		case GL_INT_SAMPLER_2D_ARRAY						: 
		case GL_INT_SAMPLER_2D_MULTISAMPLE					: 
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY			: 
		case GL_INT_SAMPLER_BUFFER							: 
		case GL_INT_SAMPLER_2D_RECT							: 
		case GL_UNSIGNED_INT_SAMPLER_1D						: 
		case GL_UNSIGNED_INT_SAMPLER_2D						: 
		case GL_UNSIGNED_INT_SAMPLER_3D						: 
		case GL_UNSIGNED_INT_SAMPLER_CUBE					: 
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY				: 
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY				: 
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE			: 
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY	: 
		case GL_UNSIGNED_INT_SAMPLER_BUFFER					: 
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT				: 
		case GL_IMAGE_1D									: 
		case GL_IMAGE_2D									: 
		case GL_IMAGE_3D									: 
		case GL_IMAGE_2D_RECT								: 
		case GL_IMAGE_CUBE									: 
		case GL_IMAGE_BUFFER								: 
		case GL_IMAGE_1D_ARRAY								: 
		case GL_IMAGE_2D_ARRAY								: 
		case GL_IMAGE_2D_MULTISAMPLE						: 
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY					: 
		case GL_INT_IMAGE_1D								: 
		case GL_INT_IMAGE_2D								: 
		case GL_INT_IMAGE_3D								: 
		case GL_INT_IMAGE_2D_RECT							: 
		case GL_INT_IMAGE_CUBE								: 
		case GL_INT_IMAGE_BUFFER							: 
		case GL_INT_IMAGE_1D_ARRAY							: 
		case GL_INT_IMAGE_2D_ARRAY							: 
		case GL_INT_IMAGE_2D_MULTISAMPLE					: 
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY				: 
		case GL_UNSIGNED_INT_IMAGE_1D						: 
		case GL_UNSIGNED_INT_IMAGE_2D						: 
		case GL_UNSIGNED_INT_IMAGE_3D						: 
		case GL_UNSIGNED_INT_IMAGE_2D_RECT					: 
		case GL_UNSIGNED_INT_IMAGE_CUBE						: 
		case GL_UNSIGNED_INT_IMAGE_BUFFER					: 
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY					: 
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY					: 
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE			: 
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY		: 
			return sizeof(GLint);
		case GL_UNSIGNED_INT_ATOMIC_COUNTER:    return sizeof(GLuint);
        case GL_BYTE:                           return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE:                  return sizeof(GLubyte);
        case GL_SHORT:                          return sizeof(GLshort);
        case GL_UNSIGNED_SHORT:                 return sizeof(GLushort);
        case GL_INT_2_10_10_10_REV:             return sizeof(GLint);
        case GL_UNSIGNED_INT_2_10_10_10_REV:    return sizeof(GLuint);
        default:
            printf("WARNING!\nGLenum type: %u not supported!!!", type);
            return 0;
    }
}

char* ShaderTypeName(const GLenum typeID)
{
    switch (typeID)
    {
        case GL_VERTEX_SHADER:   return "vertex";
        case GL_FRAGMENT_SHADER: return "fragment";
        case GL_GEOMETRY_SHADER: return "geometry";
        default:
            printf("WARNING!\nGLenum type: %u not supported!!!", typeID);
            return "NONE";
    }
}