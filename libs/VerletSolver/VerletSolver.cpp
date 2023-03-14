#include "VerletSolver.hpp"

#include "Constants.hpp"
#include "Types.hpp"
#include "Utils.hpp"
#include "VerletObject.hpp"
#include "VerletRect.hpp"
#include <cmath>
#include <cstdio>

using namespace verletSolver;

Solver::~Solver() {
	for (int i = 0; i < m_objects.size(); ++i) {
		delete m_objects[i];
	}
}

void Solver::apply_gravity() {
	for (auto &object: m_objects) {
		object->accelerate(m_gravity);
	}
}

void Solver::update(double delta_time) {
	const int sub_steps = 2;
	const double sub_delta_time = delta_time / sub_steps;

	for (int i = 0; i < sub_steps; ++i) {
		apply_gravity();
		apply_constraint();
		handle_collisions();
		update_position(sub_delta_time);
	}
}

void Solver::update_position(double delta_time) {
	for (auto &object: m_objects) {
		object->update_position(delta_time);
	}
}

bool Solver::add_rect(
	const Vector2<double> &position, const Vector2<double> &size) {
	verletRect::VerletRect* rect = new verletRect::VerletRect(position, size);

	m_objects.push_back(rect);

	return true;
}

void Solver::render(int* renderer) {
	for (auto &object: m_objects) {
		//object->render(renderer);
	}
}

void Solver::apply_constraint() {
	for (auto &object: m_objects) {
		if (
			object->m_current_position.y 
			> constants::WINDOW_HEIGHT - object->m_size.y
		) {

			object->m_current_position.y = 
				constants::WINDOW_HEIGHT - object->m_size.y;
			object->m_old_position.y = object->m_current_position.y;
		} else if (object->m_current_position.y < 0) {
			object->m_current_position.y = 0;
			object->m_old_position.y = object->m_current_position.y;
		}

		if (
			object->m_current_position.x
			> constants::WINDOW_WIDTH - object->m_size.x
		) {

			object->m_current_position.x = 
				constants::WINDOW_WIDTH - object->m_size.x;
			object->m_old_position.x = object->m_current_position.x;
		} else if (object->m_current_position.x < 0) {
			object->m_current_position.x = 0;
			object->m_old_position.x = object->m_current_position.x;
		}		
	}
}

void Solver::handle_collisions() {
	/*
	const float response_coeff = 0.75f;

	for (int i = 0; i < m_objects.size(); ++i) {
		verletObject::VerletObject* obj_1 = m_objects[i];

		for (int j = i + 1; j < m_objects.size(); ++j) {
			verletObject::VerletObject* obj_2 = m_objects[j];

			Vector2<double> v = 
				obj_1->m_current_position - obj_2->m_current_position;
			const double distance = v.x * v.x + v.y * v.y;
			const double min_distance = 
				obj_1->m_collider.radius + obj_2->m_collider.radius;

			if (distance < min_distance * min_distance) {
				const double new_distance = sqrt(distance);
				Vector2<double> n = v / new_distance;

				const double mass_ratio_1 = 
					obj_1->m_collider.radius / 
					(obj_1->m_collider.radius + obj_2->m_collider.radius);

				const double mass_ratio_2 = 
					obj_2->m_collider.radius / 
					(obj_2->m_collider.radius + obj_1->m_collider.radius);

				const double delta = 
					response_coeff * 0.5f * (new_distance - min_distance);

				obj_1->m_current_position -= n * (mass_ratio_2 + delta);
				obj_2->m_current_position += n * (mass_ratio_1 + delta);


			}
		}
	}
	*/
	
}


Vector2<double> Solver::calc_closest_point(
	const verletObject::VerletObject &obj_1,
	const verletObject::VerletObject &obj_2
) {
	Vector2<double> closest_point = {};

	Vector2<double> obj_1_vertex[8];
	Vector2<double> obj_2_vertex[8];

	// vertexes of first rect
	obj_1_vertex[0] = {obj_1.m_current_position};
	obj_1_vertex[1] = {
		obj_1.m_current_position.x + (obj_1.m_size.x / 2),
		obj_1.m_current_position.y
	};
	obj_1_vertex[2] = {
		obj_1.m_current_position.x + obj_1.m_size.x,
		obj_1.m_current_position.y
	};
	obj_1_vertex[3] = {
		obj_1.m_current_position.x + obj_1.m_size.x,
		obj_1.m_current_position.y + (obj_1.m_size.y / 2)
	};
	obj_1_vertex[4] = {
		obj_1.m_current_position.x + obj_1.m_size.x,
		obj_1.m_current_position.y + obj_1.m_size.y
	};
	obj_1_vertex[5] = {
		obj_1.m_current_position.x + (obj_1.m_size.x / 2),
		obj_1.m_current_position.y + obj_1.m_size.y
	};
	obj_1_vertex[6] = {
		obj_1.m_current_position.x,
		obj_1.m_current_position.y + obj_1.m_size.y	
	};
	obj_1_vertex[7] = {
		obj_1.m_current_position.x,
		obj_1.m_current_position.y + (obj_1.m_size.y / 2)
	};



	// vertexes of second rect
	obj_2_vertex[0] = {obj_2.m_current_position};
	obj_2_vertex[1] = {
		obj_2.m_current_position.x + (obj_2.m_size.x / 2),
		obj_2.m_current_position.y
	};
	obj_2_vertex[2] = {
		obj_2.m_current_position.x + obj_2.m_size.x,
		obj_2.m_current_position.y
	};
	obj_2_vertex[3] = {
		obj_2.m_current_position.x + obj_2.m_size.x,
		obj_2.m_current_position.y + (obj_2.m_size.y / 2)
	};
	obj_2_vertex[4] = {
		obj_2.m_current_position.x + obj_2.m_size.x,
		obj_2.m_current_position.y + obj_2.m_size.y
	};
	obj_2_vertex[5] = {
		obj_2.m_current_position.x + (obj_2.m_size.x / 2),
		obj_2.m_current_position.y + obj_2.m_size.y
	};
	obj_2_vertex[6] = {
		obj_2.m_current_position.x,
		obj_2.m_current_position.y + obj_2.m_size.y	
	};
	obj_2_vertex[7] = {
		obj_2.m_current_position.x,
		obj_2.m_current_position.y + (obj_2.m_size.y / 2)
	};

	float min_distance = 10000;
	for (int i = 0; i < 8; ++i) {
		for (int j= 0; j < 8; ++j) {
			float diff_x = obj_2_vertex[j].x - obj_1_vertex[i].x;
			float diff_y = obj_2_vertex[j].y - obj_1_vertex[i].y;
			float distance = sqrt((diff_x * diff_x) + (diff_y * diff_y));
			if (distance < min_distance) {
				// todo
			}
			min_distance = distance < min_distance ? distance : min_distance; 
		}
	}

	printf("Min distance:  %f\n", min_distance);

	return closest_point;
}