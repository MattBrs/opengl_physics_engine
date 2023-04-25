#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shapes.hpp"
#include "Types.hpp"
#include "VerletObject.hpp"

namespace verletCircle {
class VerletCircle : public verletObject::VerletObject {
  private:
    float m_radius;

  public:
    VerletCircle(types::Vector2<double> position, float radius);
    ~VerletCircle();

    float get_radius();
};
} // namespace verletCircle
