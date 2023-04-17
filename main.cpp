#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.hpp"
#include "Types.hpp"
#include "VerletSolver.hpp"
#include "Window.hpp"
#include "libs/Shader/Shader.hpp"
#include "libs/stb_image/stb_image.hpp"
#include <OpenGL/OpenGL.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <vector>

using namespace verletSolver;

Window g_window;
uint   VAO_tex;
uint   circle_VAO;
uint   g_texture_id;
uint   g_texture_id_2;
float  texture_mix_val = 0.2f;
float  texture_translate_x = 0.0f;
float  texture_translate_y = 0.0f;

shader::Shader triangle_shader_tex;
shader::Shader circle_shader;

bool init();
void run();
void quit();
void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y);
void create_triangle_tex();
void create_circle();
void process_input(GLFWwindow *window);
void create_texture();

int main(int argc, char *args[]) {
    if (!init()) {
        return 1;
    }
    triangle_shader_tex = {
        "shaders/base_triangle_tex/shader.vs",
        "shaders/base_triangle_tex/shader.fs"};
    circle_shader = {"shaders/circle/shader.vs", "shaders/circle/shader.fs"};

    create_triangle_tex();
    create_circle();

    run();
    quit();
    return 0;
}

bool init() {
    if (!g_window.init()) {
        printf("Error initializing window\n");
        return false;
    }

    return true;
}

void quit() {
    g_window.free();

    glfwTerminate();
}

void run() {
    bool            quit = false;
    Solver          physics_simulation;
    Vector2<double> rect_position(
        ((float)constants::WINDOW_WIDTH - 100) / 2, 0);

    Vector2<double> rect_size(100, 100);

    physics_simulation.add_rect(rect_position, rect_size);

    rect_position = {(((float)constants::WINDOW_WIDTH - 100) / 2) + 50, 150};

    physics_simulation.add_rect(rect_position, rect_size);
    bool simulation_running = false;

    // while (SDL_PollEvent(&event) != 0) {
    //     if (event.type == SDL_QUIT) {
    //         quit = true;
    //     } else if (
    //         event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_s)
    //         { simulation_running = !simulation_running;
    //     } else if (
    //         event.type == SDL_MOUSEBUTTONDOWN &&
    //         event.button.button == SDL_BUTTON_LEFT) {
    //         spawn_rect(physics_simulation, event.button.x,
    //         event.button.y);
    //     }
    //
    //     g_window.handle_event(event, g_renderer);
    // }

    // create transformation matrix and apply some rotation and scale
    //

    glm::vec3 cube_positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(g_window.get_window())) {

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        process_input(g_window.get_window());

        // set color to use when clear function is called
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        circle_shader.use();
        glBindVertexArray(circle_VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 30);

        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(g_window.get_window());
        glfwPollEvents();
    }
}

void create_circle() {
    uint    VBO;
    int     num_points = 30;
    GLfloat vertex_data[6 * num_points];
    uint    current_index = 0;
    int     theta = 0;
    float   radius = 0.5f;

    while (theta < 360) {
        GLfloat x = (GLfloat)radius * cosf(theta * M_PI / 180.0f);
        GLfloat y = (GLfloat)radius * sinf(theta * M_PI / 180.0f);

        // printf("x: %f  y: %f \n\n", x, y);

        vertex_data[current_index++] = x;
        vertex_data[current_index++] = y;

        vertex_data[current_index++] = 0.0f;

        float color_x = 1.0f;
        float color_y = 0.0f;
        float color_z = 0.0f;

        vertex_data[current_index++] = color_x;
        vertex_data[current_index++] = color_y;
        vertex_data[current_index++] = color_z;

        theta += 360 / num_points;
    }

    glGenVertexArrays(1, &circle_VAO);
    glBindVertexArray(circle_VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
        (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y) {
    Vector2<double> rect_position(pos_x, pos_y);
    int             size = 30 + (rand() % 100);
    Vector2<double> rect_size(size, size);

    physics_simulation.add_rect(rect_position, rect_size);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        texture_mix_val += 0.01f;
        texture_mix_val = std::min(texture_mix_val, 1.0f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        texture_mix_val -= 0.01f;
        texture_mix_val = std::max(texture_mix_val, 0.0f);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        texture_translate_x += 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        texture_translate_x -= 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        texture_translate_y += 0.01f;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        texture_translate_y -= 0.01f;
    }
}

void create_texture() {
    int            width, height, channel_count;
    unsigned char *data =
        stbi_load("textures/container.jpg", &width, &height, &channel_count, 0);

    if (!data) {
        printf("Error while loading texture!\n");
        return;
    }

    // gen texture buffers on the GPU
    glGenTextures(1, &g_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture_id);

    // set texture filtering on x and y axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering for mipmaps
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
        data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    /*------------------------------------------*/
    // stbi_set_flip_vertically_on_load(true);
    data = stbi_load(
        "textures/awesomeface.png", &width, &height, &channel_count, 0);

    if (!data) {
        printf("Error while loading awesomeface image\n");
        return;
    }

    glGenTextures(1, &g_texture_id_2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_texture_id_2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void create_triangle_tex() {
    uint VBO, EBO;

    // triangle vertices in normalized positions
    // (opengl operates from -1.0 to 1.0f)
    // in this array we are also declaring colors to use on the vertices
    // GLfloat vertex_data[] = {0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    // 0.0f, 1.0f, 1.0f,
    //                          0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    //                          0.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
    //                          0.0f, 0.0f, -0.5f, 0.5f,  0.0f, 1.0f, 1.0f,
    //                          0.0f, 0.0f, 1.0f};
    GLfloat vertex_data[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    GLuint index_data[] = {0, 1, 3, 1, 2, 3};

    // The VAO saves all the information about the things we want to draw
    // so we declare it before all the VBO, etc creation and setting.
    // Then we unbind it and select it if we want to use it.
    // (Say we want to have multiple triangles and rectangles, we first make the
    // VAOs and VBOs, etc and then we select it for drawing).
    glGenVertexArrays(1, &VAO_tex);
    glBindVertexArray(VAO_tex);

    // set the Vertex Buffer Objects
    // (buffer in GPU memory with vertex positions)
    glGenBuffers(1, &VBO);

    // OpenGL allows different buffers open at the
    // same time as long taht they are of different types
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // load the data in the current buffer
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    // glGenBuffers(1, &EBO);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(
    //     GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data,
    //     GL_STATIC_DRAW);

    // index of attrib, type, normalized, offset between values, starting index
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(
    //     1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    //     (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    create_texture();

    triangle_shader_tex.use();
    triangle_shader_tex.set_int("texture1", 0);
    triangle_shader_tex.set_int("texture2", 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
