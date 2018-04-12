#include <overkill/gl_util.hpp>

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
