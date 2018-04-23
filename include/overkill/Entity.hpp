#pragma once
#include <string>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Config.hpp>

namespace overkill 
{

class Entity
{
protected:
    C::Tag m_tag;
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;

public:
    Entity(C::Tag tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 angVel);
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getVelocity();
    glm::vec3 getAngularVelocity();
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setVelocity(glm::vec3 vel);
    void setAngularVelocity(glm::vec3 angVel); 

    void update(float dt);  
};

struct Transform
{

	const glm::vec3 right{ 1.0f,0.0f,0.0f };
	const glm::vec3 up{ 0.0f,1.0f,0.0f };
	const glm::vec3 forward{ 0.0f,0.0f,1.0f };
	const glm::vec3 one{ 1.0f, 1.0f, 1.0f };
	const glm::vec3 zero{ 0.0f ,0.0f ,0.0f };

	glm::vec3 m_position = zero;
	glm::vec3 m_rotation = zero;
	glm::vec3 m_scale    = one;

	auto modelToWorld()->glm::mat4;
};


struct Light : public Transform
{
	glm::vec3 intensities = one; //a.k.a the color of the light
};

}