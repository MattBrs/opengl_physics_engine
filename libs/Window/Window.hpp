#pragma once

#include <GLFW/glfw3.h>
#include <sys/types.h>
#include <utility>

class Window {
  public:
    Window();
    ~Window();

    bool init();
    void free();

    bool                has_mouse_focus();
    bool                has_keyboard_focus();
    bool                is_minimized();
    GLFWwindow        *&get_window();
    std::pair<int, int> get_window_size();

  private:
    int m_buffer_width;
    int m_buffer_heigth;
    int m_window_heigth;
    int m_window_width;

    GLFWwindow *m_window;

    bool   m_mouse_focus;
    bool   m_keyboard_focus;
    bool   m_is_fullscreen;
    bool   m_is_minimized;
    GLuint m_program_id = 0;
    GLint  m_vertex_pos_2d_location = -1;

    static void
    framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void window_size_callback(GLFWwindow *window, int width, int heigth);
};
