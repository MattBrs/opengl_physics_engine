#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

#include "Constants.hpp"
#include "Window.hpp"
#include <cstdio>
#include <sstream>
#include <sys/types.h>
#include <utility>

using namespace constants;

Window::Window() {
    m_window_width = 0;
    m_window_heigth = 0;
    m_is_fullscreen = false;
    m_is_minimized = false;
    m_mouse_focus = false;
    m_keyboard_focus = false;
}

bool Window::init() {
    // init glfw
    if (!glfwInit()) {
        printf("Error initializing glfw! \n");
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // create gl window
    m_window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT, "Test window", NULL, NULL);

    // we check if there was an error initializing the window
    if (!m_window) {
        printf("Error initializing window\n");
        glfwTerminate();
        return false;
    }

    // set buffer size and context
    glfwGetFramebufferSize(m_window, &m_buffer_width, &m_buffer_heigth);
    glfwMakeContextCurrent(m_window);

    // enable latest features
    glewExperimental = GL_TRUE;

    // init glew and check for errors
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK) {
        printf("Error initializing GLEW\n");
        return false;
    }

    m_window_width = WINDOW_WIDTH;
    m_window_heigth = WINDOW_HEIGHT;

    glViewport(0, 0, m_buffer_width, m_buffer_heigth);

    // when window is resized, adjust viewport size
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetWindowSizeCallback(m_window, window_size_callback);

    unsigned int major =
        glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
    unsigned int minor =
        glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);

    printf("Using opengl version: %d.%d\n", major, minor);

    return true;
}

Window::~Window() {
    free();
}

void Window::free() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

bool Window::has_keyboard_focus() {
    return m_keyboard_focus;
}

bool Window::has_mouse_focus() {
    return m_mouse_focus;
}

bool Window::is_minimized() {
    return m_is_minimized;
}

GLFWwindow *&Window::get_window() {
    return m_window;
}

void Window::framebuffer_size_callback(
    GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::window_size_callback(GLFWwindow *window, int width, int heigth) {
    // do nothing
}

std::pair<int, int> Window::get_window_size() {
    return {m_window_width, m_window_heigth};
}
