#include <overkill/gl_util.hpp>

void GLPrintMaxConstants()
{
    // @doc list of available constants https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGet.xhtml 18.04.2018
    GLint out;
  //  GLCall(glGetIntegerv(GL_MAX_ELEMENT_INDICIES, &out));
   // LOG_INFO("GL_MAX_ELEMENT_INDICIES: %d", out);

//    GLCall(glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &out));
 //   LOG_INFO("GL_MAX_ELEMENTS_VERTICES: %d", out);

  //  GLCall(glGetIntegerv(GL_MAX_ELEMENT_INDEX, &out));
   // LOG_INFO("GL_MAX_ELEMENT_INDEX: %d", out);

    GLCall(glGetIntegerv(GL_MAX_DRAW_BUFFERS, &out));
    LOG_INFO("GL_MAX_DRAW_BUFFERS: %d", out);

 //   GLCall(glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, &out));
  //  LOG_INFO("GL_MAX_FRAMEBUFFER_WIDTH: %d", out);

  //  GLCall(glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, &out));
   // LOG_INFO("GL_MAX_FRAMEBUFFER_HEIGHT: %d", out);

  //  GLCall(glGetIntegerv(GL_MAX_LABEL_LENGTH, &out));
   // LOG_INFO("GL_MAX_LABEL_LENGTH: %d", out);

    // @note not supported opengl 4.1
   // GLCall(glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &out));
   // LOG_INFO("GL_MAX_COMPUTE_UNIFORM_BLOCKS: %d", out);

    // @note not supported opengl 4.1
    //  GLCall(glGetIntegerv(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &out));
    // LOG_INFO("GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS: %d", out);

    GLCall(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &out));
    LOG_INFO("GL_MAX_TEXTURE_SIZE: %d", out);

    GLCall(glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &out));
    LOG_INFO("GL_MAX_3D_TEXTURE_SIZE: %d", out);

    GLCall(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &out));
    LOG_INFO("GL_MAX_ARRAY_TEXTURE_LAYERS: %d", out); 

 //   GLCall(glGetIntegerv(GL_TEXTURE_IMAGE_UNITS, &out));
  //  LOG_INFO("GL_TEXTURE_IMAGE_UNITS: %d", out); 

  //  GLCall(glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &out));
   // LOG_INFO("GL_MAX_UNIFORM_LOCATIONS: %d", out); 

       
}


// @TODO replace with a "magical" template specifier i.e. template<>GLTypeSize<GL_FLOAT>(){return sizeof(GLfloat);}
auto GLTypeSize(const GLenum type) -> GLsizei 
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
        case GL_NONE:                           return 1;
        default:
            printf("WARNING!\nGLenum type: %u not supported!!!", type);
            return 0;
    }
}

std::string ShaderTypeName(const GLenum typeID)
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