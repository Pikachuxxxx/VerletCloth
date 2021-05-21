#pragma once

#include <vector>

#include "Constraint.h"

class Cloth
{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
public:
    Cloth(float width, float height, float numParticlesWidth, float numParticlesHeight);

    void TimeStep();
    void AddForce(glm::vec3 force);
    void AddWindForce(glm::vec3 windForce);
    void BallCollision(const glm::vec3 center, const float radius);
    void CalculateClothVertices();
private:
    unsigned int            m_NumParticlesWidth;    // The number of particles in the width direction
    unsigned int            m_NumParticlesHeight;   // The number of particles in the Height direction

    std::vector<Particle>   m_Particles; 
    std::vector<Constraint> m_Constraints;
private:
    void MakeConstraint(Particle* p1, Particle* p2);
    glm::vec3 CalculateTriangleNormal(Particle* p1, Particle* p2, Particle* p3);
    void AddWindForcesToTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3 direction);

    inline Particle* GetParticle(unsigned int x, unsigned int y) { return &m_Particles[y * m_NumParticlesWidth + x]; }
};

