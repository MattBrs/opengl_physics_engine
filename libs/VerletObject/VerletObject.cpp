#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.hpp"
#include "Types.hpp"
#include "VerletObject.hpp"
#include <cstdio>

using namespace verletObject;

VerletObject::~VerletObject() {
    // do nothing
}

void VerletObject::update_position(double delta_time) {
    types::Vector2<double> velocity = (m_current_position - m_old_position);
    types::Vector2<double> acc = m_acceleration * delta_time * delta_time;

    velocity.x = velocity.x > 100 ? 100 : velocity.x;
    velocity.y = velocity.y > 100 ? 100 : velocity.y;
    m_old_position = m_current_position;

    printf("velocity: %f  %f\n", velocity.x, velocity.y);
    printf("velocity: %f  %f\n\n", m_acceleration.x, m_acceleration.y);

    m_current_position = m_current_position + velocity +
                         m_acceleration * delta_time * delta_time;

    m_acceleration = {};
}

void VerletObject::accelerate(types::Vector2<double> acc) {
    m_acceleration += acc;
}

types::Vector2<double> VerletObject::get_current_position() {
    return m_current_position;
}
