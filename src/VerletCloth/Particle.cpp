#include "Particle.h"

Particle::Particle(glm::vec3 pos)
    : m_Movable(true), m_Mass(1.0f), m_Position(pos), m_OldPosition(pos), m_AccumulatedNormal(glm::vec3(0.0f)), m_Acceleration(glm::vec3(0.0f))
{

}

Particle::~Particle()
{

}

void Particle::TimeStep()
{
    if (m_Movable)
    {
        glm::vec3 temp = m_Position;
        // Verlet Integration (with damping over time)
        m_Position = m_Position + (m_Position - m_OldPosition) * (1.0f - DAMPING) + m_Acceleration * TIME_STEPSIZE2;
        m_OldPosition = temp;
        m_Acceleration = glm::vec3(0.0f); // acceleration is reset since it HAS been translated into a change in position (and implicitly into velocity)	
    }
}

void Particle::MakeUnMovable()
{
    m_Movable = false;
}

void Particle::AddForce(glm::vec3 force)
{
    m_Acceleration = force / m_Mass;
}

void Particle::AddToNormal(glm::vec3 normal)
{
    m_AccumulatedNormal += glm::normalize(normal);
}

void Particle::OffsetPosition(const glm::vec3 pos)
{
    if (m_Movable) m_Position += pos;
}

void Particle::ResetNormal()
{
    m_AccumulatedNormal = glm::vec3(0.0f);
}

void Particle::ResetAcceleration()
{
    m_Acceleration = glm::vec3(0.0f);
}