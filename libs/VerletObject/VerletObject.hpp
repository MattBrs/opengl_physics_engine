#pragma once

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
		types::Vector2<double> m_size;
		shapes::Circle m_collider;
		int m_box_collider;

		void update_position(double delta_time);
		void accelerate(types::Vector2<double> acc);
		virtual void render(int* renderer);
	private:
	};
}
