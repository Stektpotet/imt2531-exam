#pragma once
#include <overkill/gl_caller.hpp>

#include <overkill/vertexArray.hpp>
#include <overkill/elementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader) const;
};

class EdgeRenderer : public Renderer
{
public:
    void drawEdged(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader, const ShaderProgram& edgeShader) const;
};
