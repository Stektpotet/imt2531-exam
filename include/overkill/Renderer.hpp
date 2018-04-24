#pragma once
#include <overkill/gl_caller.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <overkill/VertexArray.hpp>
#include <overkill/ElementBuffer.hpp>
#include <overkill/ShaderProgram.hpp>
#include <overkill/Model.hpp>
#include <overkill/EntityModel.hpp>

namespace overkill
{

class EntityModel;

class Renderer
{
public:
    static void clear();
    static void draw(const VertexArray& va, const ElementBuffer& eb, const ShaderProgram& shader);
    static void draw(const Model& model, glm::mat4 modelMatrix = glm::mat4(1));   // Model matrix is translation rotation and scale of the model.
   
    static void draw(const Model& model, glm::mat4 modelMatrix, float t);
    //static void draw( EntityModel& entity, float t);
};


}
