#include <overkill/gl_util.hpp>

void GLPrintMaxContants()
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


GLuint GLTypeSize(const GLenum type)
{
    switch (type)
    {
        case GL_BOOL:                           return sizeof(GLboolean);
        case GL_BYTE:                           return sizeof(GLbyte);
        case GL_UNSIGNED_BYTE:                  return sizeof(GLubyte);
        case GL_SHORT:                          return sizeof(GLshort);
        case GL_UNSIGNED_SHORT:                 return sizeof(GLushort);
        case GL_INT:                            return sizeof(GLint);
        case GL_INT_2_10_10_10_REV:             return sizeof(GLint);
        case GL_UNSIGNED_INT:                   return sizeof(GLuint);
        case GL_UNSIGNED_INT_2_10_10_10_REV:    return sizeof(GLuint);
        case GL_FLOAT:                          return sizeof(GLfloat);
        case GL_DOUBLE:                         return sizeof(GLdouble);
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
