#pragma once

#include <GLFW/glfw3.h>
#include <sys/types.h>

class Window {
  public:
    Window();
    ~Window();

    bool init();
    void free();

    int get_window_width();
    int get_window_heigth();

    bool         has_mouse_focus();
    bool         has_keyboard_focus();
    bool         is_minimized();
    GLFWwindow *&get_window();
    void         test_render(uint VAO, uint program_id);

  private:
    int m_window_heigth;
    int m_window_width;

    GLFWwindow *m_window;
    int         m_buffer_width;
    int         m_buffer_heigth;

    bool   m_mouse_focus;
    bool   m_keyboard_focus;
    bool   m_is_fullscreen;
    bool   m_is_minimized;
    GLuint m_program_id = 0;
    GLint  m_vertex_pos_2d_location = -1;
};
