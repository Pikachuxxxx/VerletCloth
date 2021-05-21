#include "Cloth.h"
#include <iostream>
/*
The cloth is seen as consisting of triangles for four particles in the grid as follows:

  0             1
(x,y)   *--* (x+1,y)
        | /|
        |/ |
(x,y+1) *--* (x+1,y+1)
   3             2
*/

Cloth::Cloth(float width, float height, float numParticlesWidth, float numParticlesHeight)
    : m_NumParticlesWidth(numParticlesWidth), m_NumParticlesHeight(numParticlesHeight)
{
    m_Particles.resize(m_NumParticlesWidth * m_NumParticlesHeight);

    // Creating Particles in a grid from (0,0,0) to (width,-height,0)
    for (unsigned int x = 0; x < m_NumParticlesWidth; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight; y++)
        {
            glm::vec3 pos = glm::vec3(width * ((float)x / m_NumParticlesWidth), -height * ((float)y / m_NumParticlesHeight), 0.0f);
            m_Particles[y * m_NumParticlesWidth + x] = Particle(pos);
        }
    }

    uv.resize((m_NumParticlesWidth - 1) * (m_NumParticlesHeight - 1) * 6);

    // Connecting immediate neighboring particles with constraints (Distance 1 and sqrt(2))
    for (unsigned int x = 0; x < m_NumParticlesWidth; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight; y++)
        {
            if (x < m_NumParticlesWidth  - 1) MakeConstraint(GetParticle(x, y), GetParticle(x + 1, y));
            if (y < m_NumParticlesHeight - 1) MakeConstraint(GetParticle(x, y), GetParticle(x, y + 1));
            if (x < m_NumParticlesWidth  - 1 && y < m_NumParticlesHeight - 1) MakeConstraint(GetParticle(x, y)    , GetParticle(x + 1, y + 1));
            if (x < m_NumParticlesWidth  - 1 && y < m_NumParticlesHeight - 1) MakeConstraint(GetParticle(x + 1, y), GetParticle(x, y + 1));
        }
    }

    // Connecting secondary neighboring particles with constraints (Distance 2 and sqrt(4))
    for (unsigned int x = 0; x < m_NumParticlesWidth; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight; y++)
        {
            if (x < m_NumParticlesWidth  - 2) MakeConstraint(GetParticle(x, y), GetParticle(x + 2, y));
            if (y < m_NumParticlesHeight - 2) MakeConstraint(GetParticle(x, y), GetParticle(x, y + 2));
            if (x < m_NumParticlesWidth  - 2 && y < m_NumParticlesHeight - 2) MakeConstraint(GetParticle(x, y)    , GetParticle(x + 2, y + 2));
            if (x < m_NumParticlesWidth  - 2 && y < m_NumParticlesHeight - 2) MakeConstraint(GetParticle(x + 2, y), GetParticle(x, y + 2));
        }
    }

    // Making the top left and right most particles unmovable
    for (int i = 0; i < 3; i++)
    {
        GetParticle(0 + i, 0)->MakeUnMovable(); // left
        GetParticle(m_NumParticlesWidth - 1 - i, 0)->MakeUnMovable(); // right
    }
}

void Cloth::TimeStep()
{
    std::vector<Constraint>::iterator constraint;
    for (int i = 0; i < CONSTRAINT_ITERATIONS; ++i)
    {
        for (constraint = m_Constraints.begin(); constraint != m_Constraints.end(); constraint++)
        {
            (*constraint).SatisfyStiffnessConstraint();
        }
    }

    std::vector<Particle>::iterator particle;
    for (particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
    {
        (*particle).TimeStep();
    }
}

void Cloth::AddForce(glm::vec3 force)
{
    std::vector<Particle>::iterator particle;
    for (particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
    {
        (*particle).AddForce(force);
    }
}

void Cloth::AddWindForce(glm::vec3 windForce)
{
    for (unsigned int x = 0; x < m_NumParticlesWidth - 1; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight - 1; y++)
        {
            AddWindForcesToTriangle(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1), windForce); // 1 0 3
            AddWindForcesToTriangle(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1), windForce); // 2 1 3
        }
    }
}

void Cloth::BallCollision(const glm::vec3 center, const float radius)
{
    std::vector<Particle>::iterator particle;
    for (particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
    {
        glm::vec3 pv = (*particle).GetPosition() - center;
        float l = glm::length(pv);
        if (l < radius)
            (*particle).OffsetPosition(glm::normalize(pv) * (radius - l)); // project the particle to the surface of the ball
    }
}

void Cloth::MakeConstraint(Particle* p1, Particle* p2)
{
    m_Constraints.push_back(Constraint(p1, p2));
}

glm::vec3 Cloth::CalculateTriangleNormal(Particle* p1, Particle* p2, Particle* p3)
{
    glm::vec3 pos1 = p1->GetPosition();
    glm::vec3 pos2 = p2->GetPosition();
    glm::vec3 pos3 = p3->GetPosition();

    glm::vec3 v1 = pos2 - pos1;
    glm::vec3 v2 = pos3 - pos1;

    return glm::cross(v1, v2);
}

void Cloth::AddWindForcesToTriangle(Particle* p1, Particle* p2, Particle* p3, const glm::vec3 direction)
{
    glm::vec3 normal = CalculateTriangleNormal(p1, p2, p3);
    glm::vec3 d = glm::normalize(normal);
    glm::vec3 force = normal * glm::dot(d, direction);
    p1->AddForce(force);
    p2->AddForce(force);
    p3->AddForce(force);
}


void Cloth::CalculateClothVertices()
{
    //Clear the verts and normals
    vertices.clear();
    normals.clear();

    // Reset normals (which where written to last frame)
    std::vector<Particle>::iterator particle;
    for (particle = m_Particles.begin(); particle != m_Particles.end(); particle++)
    {
        (*particle).ResetNormal();
    }

    // Create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
    for (unsigned int x = 0; x < m_NumParticlesWidth - 1; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight - 1; y++)
        {
            glm::vec3 normal = CalculateTriangleNormal(GetParticle(x + 1, y), GetParticle(x, y), GetParticle(x, y + 1));
            GetParticle(x + 1, y)->AddToNormal(normal);
            GetParticle(x, y)->AddToNormal(normal);
            GetParticle(x, y + 1)->AddToNormal(normal);

            normal = CalculateTriangleNormal(GetParticle(x + 1, y + 1), GetParticle(x + 1, y), GetParticle(x, y + 1));
            GetParticle(x + 1, y + 1)->AddToNormal(normal);
            GetParticle(x + 1, y)->AddToNormal(normal);
            GetParticle(x, y + 1)->AddToNormal(normal);
        }
    }

    int offset = 0;
    for (unsigned int x = 0; x < m_NumParticlesWidth - 1; x++)
    {
        for (unsigned int y = 0; y < m_NumParticlesHeight - 1; y++)
        {
            // Vertices
            vertices.push_back(GetParticle(x + 1, y)->GetPosition()); 
            vertices.push_back(GetParticle(x, y)->GetPosition());
            vertices.push_back(GetParticle(x, y + 1)->GetPosition());

            vertices.push_back(GetParticle(x + 1, y + 1)->GetPosition());
            vertices.push_back(GetParticle(x + 1, y)->GetPosition());
            vertices.push_back(GetParticle(x, y + 1)->GetPosition());

            // Normals
            normals.push_back(GetParticle(x + 1, y)->GetNormal());
            normals.push_back(GetParticle(x, y)->GetNormal());
            normals.push_back(GetParticle(x, y + 1)->GetNormal());

            normals.push_back(GetParticle(x + 1, y + 1)->GetNormal());
            normals.push_back(GetParticle(x + 1, y)->GetNormal());
            normals.push_back(GetParticle(x, y + 1)->GetNormal());

            // UV Coordinates
            uv[offset + 0] = glm::vec2((float)(x + 1)   / (m_NumParticlesWidth - 1), (float)y         / (m_NumParticlesHeight - 1));
            uv[offset + 1] = glm::vec2((float)(x)       / (m_NumParticlesWidth - 1), (float)y         / (m_NumParticlesHeight - 1));
            uv[offset + 2] = glm::vec2((float)(x)       / (m_NumParticlesWidth - 1), (float)(y + 1)   / (m_NumParticlesHeight - 1));
                                                                                                                                 
            uv[offset + 3] = glm::vec2((float)(x + 1)   / (m_NumParticlesWidth - 1), (float)(y + 1)   / (m_NumParticlesHeight - 1));
            uv[offset + 4] = glm::vec2((float)(x + 1)   / (m_NumParticlesWidth - 1), (float)y         / (m_NumParticlesHeight - 1));
            uv[offset + 5] = glm::vec2((float)(x)       / (m_NumParticlesWidth - 1), (float)(y + 1)   / (m_NumParticlesHeight - 1));
            offset += 6;
        }
    }

}