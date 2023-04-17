#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma once

namespace circle {
class Circle {
  private:
    glm::vec3 m_position;
    float     m_radius;

  public:
    Circle();
    ~Circle();
    glm::vec3 *get_position();
    float      get_radius();
};
} // namespace circle
