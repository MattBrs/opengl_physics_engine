#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shapes.hpp"
#include "Types.hpp"
#include "VerletCircle.hpp"
#include "VerletObject.hpp"
#include <vector>

namespace verletSolver {
class Solver {
  public:
    ~Solver();

    void update(double delta_time);
    void handle_collisions();
    void add_circle(types::Vector2<double> position, float radius);

    std::vector<verletCircle::VerletCircle *> &get_circles();

  private:
    // std::vector<verletObject::VerletObject *> m_objects;
    types::Vector2<double>                    m_gravity{0.0f, 100.0f};
    types::Vector2<double>                    m_friction{500.0f, 0.0f};
    std::vector<verletCircle::VerletCircle *> m_circles;

    void update_position(double delta_time);
    void apply_gravity();
    void apply_constraint();
};
} // namespace verletSolver
