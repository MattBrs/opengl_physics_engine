#pragma once

#include "Types.hpp"
#include "VerletObject.hpp"
#include <vector>

using types::Vector2;

namespace verletSolver {
	class Solver {
	public:
		~Solver();

		void update(double delta_time);
		bool add_rect(
			const Vector2<double> &position, 
			const Vector2<double> &size
		);
		void render(int* renderer);
		void handle_collisions();
		Vector2<double> calc_closest_point(
			const verletObject::VerletObject &obj_1,
			const verletObject::VerletObject &obj_2
		);

	private:
		std::vector<verletObject::VerletObject*> m_objects;
		types::Vector2<double> m_gravity {0.0f, 1000.0f};
		Vector2<double> m_friction {500.0f, 0.0f};

		void update_position(double delta_time);
		void apply_gravity();
		void apply_constraint();
	};
}
