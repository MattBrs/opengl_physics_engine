#include "VerletRect.hpp"
#include "Constants.hpp"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "Types.hpp"
#include <cstdio>

using namespace verletRect;

VerletRect::VerletRect(
    const Vector2<double> &position, const Vector2<double> &size) {

    // m_current_position = position;
    /*
    m_current_position = position;
    m_old_position = position;
    m_size = size;

    m_rect.x = position.x;
    m_rect.y = position.y;

    m_rect.w = size.x;
    m_rect.h = size.y;

    m_collider.pos_x = (float)(position.x + m_rect.w) / 2;
    m_collider.pos_y = (float)(position.y + m_rect.h) / 2;
    m_collider.radius = (float)m_rect.w / 2;

    m_box_collider = m_rect;
    */
}
VerletRect::~VerletRect() {
    // do nothing
}
