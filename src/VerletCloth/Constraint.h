#pragma once

#include "Particle.h"

class Constraint
{
public:
    Particle* p1;
    Particle* p2;
public:
    Constraint(Particle* p1, Particle* p2);
    ~Constraint();

    void SatisfyStiffnessConstraint();
private:
    float m_RestDistance; // The distance between particles in rest mode

};

