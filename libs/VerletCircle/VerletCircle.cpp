#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shapes.hpp"
#include "Types.hpp"
#include "VerletCircle.hpp"

using namespace verletCircle;

VerletCircle::VerletCircle(types::Vector2<double> position, float radius) {
    m_radius = radius;
    m_current_position = position;
    m_old_position = position;
}

VerletCircle::~VerletCircle() {
    // do nothing
}

float VerletCircle::get_radius() {
    return m_radius;
}
