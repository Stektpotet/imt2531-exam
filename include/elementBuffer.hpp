#pragma once

#include <gl_caller.hpp>

class ElementBuffer
{
private:
    unsigned int m_rendererID;
    unsigned int m_Count;
public:
    ElementBuffer(const unsigned int* data, unsigned int count);
    ~ElementBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int count() const { return m_Count; }
};