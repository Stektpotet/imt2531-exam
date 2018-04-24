#pragma once
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <overkill/Config.hpp>

namespace overkill 
{

class Entity
{
protected:
    C::Tag m_tag;
    int m_entityID;                 // ID scene uses to keep track of all entities. Also parentID.
    std::vector<int> m_childIDs;      // The ids of all the entity's children.
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_velocity;
    glm::vec3 m_angularVelocity;
    void update(float dt);  

public:
    Entity(C::Tag tag, int entityID, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 angVel);
    int getEntityID();
    glm::vec3 getPosition();
    glm::vec3 getRotation();
    glm::vec3 getVelocity();
    glm::vec3 getAngularVelocity();
    void addChild(int childID);
    void setEntityID(int ID);
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setVelocity(glm::vec3 vel);
    void setAngularVelocity(glm::vec3 angVel); 

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