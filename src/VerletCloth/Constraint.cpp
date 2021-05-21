#include "Constraint.h"

Constraint::Constraint(Particle* p1, Particle* p2)
    : p1(p1), p2(p2)
{
    glm::vec3 dist = p1->GetPosition() - p2->GetPosition();
    m_RestDistance = glm::length(dist);
}

Constraint::~Constraint() {}

void Constraint::SatisfyStiffnessConstraint()
{
    glm::vec3 p1Top2 = p2->GetPosition() - p1->GetPosition();
    float currentDistance = glm::length(p1Top2);
    glm::vec3 correctionVector = p1Top2 * (1.0f - m_RestDistance / currentDistance); // The offset vector that could moves p1 into a distance of rest_distance to p2
    glm::vec3 correctionVectorHalf = correctionVector * 0.5f; // Lets make it half that length, so that we can move BOTH p1 and p2
    p1->OffsetPosition(correctionVectorHalf); // correctionVectorHalf is pointing from p1 to p2, so the length should move p1 half the length needed to satisfy the constraint
    p2->OffsetPosition(-correctionVectorHalf);  // we must move p2 the negative direction of correctionVectorHalf since it points from p2 to p1, and not p1 to p2
}