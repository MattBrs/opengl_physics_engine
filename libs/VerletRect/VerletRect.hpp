#pragma once

#include "Types.hpp"
#include "VerletObject.hpp"

using types::Vector2;

namespace verletRect {
	class VerletRect: public verletObject::VerletObject {
	public:
		VerletRect(
			const Vector2<double> &position, 
			const Vector2<double> &size
		);
		~VerletRect();

		void render(int* renderer) override;

	private:
		int m_rect;
	};
}
