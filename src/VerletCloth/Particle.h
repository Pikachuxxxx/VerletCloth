#pragma once

#include <math.h>

#include "core.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

// The Particle class represents a particle of mass that can move around in 3D space
class Particle
{
public:
    Particle(glm::vec3 pos);
    Particle() {}
    ~Particle();

    void TimeStep();
    void MakeUnMovable();
    void AddForce(glm::vec3 force);
    void AddToNormal(glm::vec3 normal);
    void ResetNormal();
    void ResetAcceleration();
    void OffsetPosition(const glm::vec3 pos);

    inline glm::vec3& GetPosition() { return m_Position; }
    inline glm::vec3& GetNormal() { return m_AccumulatedNormal; }
    inline glm::vec3  GetNormalizedNormal() { return glm::normalize(m_AccumulatedNormal); }
private:
    bool        m_Movable;
    float       m_Mass;               // The mass of the particle (is always 1 in this context)
    glm::vec3   m_Position;           // The current position of the particle in 3D space
    glm::vec3   m_OldPosition;        // The position of the particle in the previous time step, used as part of the Verlet numerical integration
    glm::vec3   m_AccumulatedNormal;  // An accumulated normal (i.e. non normalized)
    glm::vec3   m_Acceleration;       // The acceleration of the particle
    
};

