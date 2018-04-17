#pragma once
#include <overkill/gl_caller.hpp>

#include <overkill/VertexArray.hpp>
#include <overkill/ElementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Model.hpp>

namespace overkill
{

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader) const;
    void draw(const Model& model) const;


};

class EdgeRenderer : public Renderer
{
public:
    void drawEdged(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader, const ShaderProgram& edgeShader) const;
};

}
