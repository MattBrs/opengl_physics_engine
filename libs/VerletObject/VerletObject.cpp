#include "VerletObject.hpp"
#include "Types.hpp"
#include <cstdio>

using namespace verletObject;

VerletObject::~VerletObject() {
	// do nothing
}

void VerletObject::update_position(double delta_time) {
	types::Vector2<double> velocity = 
		m_current_position - m_old_position;

	velocity.x = velocity.x > 50 ? 50:velocity.x;
	velocity.y = velocity.y > 50 ? 50:velocity.y;
	m_old_position = m_current_position;

	m_current_position = 
		m_current_position + velocity 
		+ m_acceleration * delta_time * delta_time;

	m_acceleration = {};
}

void VerletObject::accelerate(types::Vector2<double> acc) {
	m_acceleration += acc;
}

void VerletObject::render(int *renderer) {
	// nothing to render because its not a shape but a base implementation
}
