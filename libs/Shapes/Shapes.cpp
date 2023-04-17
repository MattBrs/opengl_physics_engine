#include "Shapes.hpp"

using namespace circle;

Circle::Circle() {}

Circle::~Circle() {
    // do nothing
}

float Circle::get_radius() {
    return m_radius;
}

glm::vec3 *Circle::get_position() {
    return &m_position;
}
