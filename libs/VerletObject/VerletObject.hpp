#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types.hpp"
#include "Utils.hpp"
#include <vector>

namespace verletObject {
class VerletObject {
  public:
    virtual ~VerletObject();
    types::Vector2<double> m_current_position;
    types::Vector2<double> m_old_position;
    types::Vector2<double> m_acceleration;

    void update_position(double delta_time);
    void accelerate(types::Vector2<double> acc);

    types::Vector2<double> get_current_position();

  private:
};
} // namespace verletObject
