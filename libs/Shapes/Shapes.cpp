#include "Shapes.hpp"

using namespace circle;

Circle::Circle(glm::vec3 position, float radius) {
    m_position = position;
    m_radius = radius;
}

Circle::Circle() {
    m_position = glm::vec3(0.f);
    m_radius = 1.f;
}

Circle::~Circle() {
    // do nothing
}

float Circle::get_radius() {
    return m_radius;
}

glm::vec3 *Circle::get_position() {
    return &m_position;
}

void Circle::set_position(glm::vec3 new_pos) {
    m_position = new_pos;
}
