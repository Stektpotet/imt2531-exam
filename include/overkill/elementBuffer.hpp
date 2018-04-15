#pragma once

#include <overkill/gl_caller.hpp>

namespace overkill 
{

class ElementBuffer
{
private:
    unsigned int m_rendererID;
    unsigned int m_Count;
    
public:
    ElementBuffer() = default;
    ElementBuffer(const unsigned int* data, unsigned int count);
    
    
    void clean();

    void bind() const;
    void unbind() const;

    inline unsigned int count() const { return m_Count; }
};

}