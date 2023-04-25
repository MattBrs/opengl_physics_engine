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
    Circle(glm::vec3 position, float radius);
    ~Circle();
    glm::vec3 *get_position();
    float      get_radius();

    void set_position(glm::vec3 new_pos);
};
} // namespace circle
