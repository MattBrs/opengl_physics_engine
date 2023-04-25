#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VerletCircle.hpp"
#include "VerletSolver.hpp"

#include "Constants.hpp"
#include "Types.hpp"
#include "Utils.hpp"
#include "VerletObject.hpp"
#include "VerletRect.hpp"
#include <cmath>
#include <cstdio>
#include <vector>

using namespace verletSolver;

Solver::~Solver() {
    for (int i = 0; i < m_circles.size(); ++i) {
        delete m_circles[i];
    }
}

void Solver::apply_gravity() {
    for (auto &object : m_circles) {
        object->accelerate(m_gravity);
    }
}

void Solver::update(double delta_time) {
    const int    sub_steps = 2;
    const double sub_delta_time = delta_time / sub_steps;

    for (int i = 0; i < sub_steps; ++i) {
        apply_gravity();
        apply_constraint();
        handle_collisions();
        update_position(sub_delta_time);
    }
}

void Solver::update_position(double delta_time) {
    for (auto &object : m_circles) {
        object->update_position(delta_time);
    }
}

void Solver::apply_constraint() {
    for (auto &object : m_circles) {
        if (object->m_current_position.y > 1.0f - object->get_radius()) {
            object->m_current_position.y = 1.0f - object->get_radius();
            object->m_old_position.y = object->m_current_position.y;
        } else if (
            object->m_current_position.y < -1.0f + object->get_radius()) {
            object->m_current_position.y = -1.0f + object->get_radius();
            object->m_old_position.y = object->m_current_position.y;
        }

        if (object->m_current_position.x > 1.0f - object->get_radius()) {
            object->m_current_position.x = 1.0f - object->get_radius();
            object->m_old_position.x = object->m_current_position.x;
        } else if (object->m_current_position.x < -1.f + object->get_radius()) {
            object->m_current_position.x = -1.f + object->get_radius();
            object->m_old_position.x = object->m_current_position.x;
        }
    }
}

void Solver::handle_collisions() {

    const float response_coeff = 0.75f;

    for (int i = 0; i < m_circles.size(); ++i) {
        verletCircle::VerletCircle &obj_1 = *m_circles[i];

        for (int j = i + 1; j < m_circles.size(); ++j) {
            verletCircle::VerletCircle &obj_2 = *m_circles[j];

            Vector2<double> v =
                obj_1.m_current_position - obj_2.m_current_position;
            const float distance = v.x * v.x + v.y * v.y;
            const float min_distance = obj_1.get_radius() + obj_2.get_radius();

            if (distance <= min_distance * min_distance) {

                const float     new_distance = sqrt(distance);
                Vector2<double> n = v / new_distance;

                const float mass_ratio_1 =
                    obj_1.get_radius() /
                    (obj_1.get_radius() + obj_2.get_radius());

                const float mass_ratio_2 =
                    obj_2.get_radius() /
                    (obj_1.get_radius() + obj_2.get_radius());

                const float delta =
                    response_coeff * 0.5f * (new_distance - min_distance);

                obj_1.m_current_position -= n * (mass_ratio_2 * delta);
                obj_2.m_current_position += n * (mass_ratio_1 * delta);
            }
        }
    }
}

void Solver::add_circle(Vector2<double> position, float radius) {
    verletCircle::VerletCircle *object =
        new verletCircle::VerletCircle(position, radius);
    m_circles.push_back(object);
}

std::vector<verletCircle::VerletCircle *> &Solver::get_circles() {
    return m_circles;
}

void Solver::set_gravity(types::Vector2<double> new_gravity) {
    m_gravity = new_gravity;
}
void Solver::reset_gravity() {
    m_gravity = {0.0f, -2.f};
}
