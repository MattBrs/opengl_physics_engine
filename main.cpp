#include "VerletCircle.hpp"
#include "libs/Shapes/Shapes.hpp"
#include <utility>
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
int    g_circle_num_points = 30;
bool   g_sim_running = false;
Solver g_physics_simulation;

shader::Shader triangle_shader_tex;
shader::Shader circle_shader;

bool init();
void run();
void quit();
void spawn_circle(double pos_x, double pos_y);
void create_triangle_tex();
void create_circle();
void process_input(GLFWwindow *window);
void create_texture();
void mouse_button_callback(
    GLFWwindow *window, int button, int action, int mods);
float normalize_value(
    float value, float range_min_1, float range_max_1, float range_min_2,
    float range_max_2);

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

    glfwSetMouseButtonCallback(g_window.get_window(), mouse_button_callback);

    return true;
}

void quit() {
    g_window.free();

    glfwTerminate();
}

void run() {
    bool quit = false;

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

    g_physics_simulation.add_circle({480.f, 360.f}, 20.0f);
    g_physics_simulation.add_circle({482.f, 180.f}, 20.0f);

    glEnable(GL_DEPTH_TEST);

    double old_time = 0.f;
    while (!glfwWindowShouldClose(g_window.get_window())) {
        double time = glfwGetTime();
        double delta_time = time - old_time;
        old_time = time;

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        process_input(g_window.get_window());

        // set color to use when clear function is called
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        circle_shader.use();
        glBindVertexArray(circle_VAO);

        std::pair<int, int> window_size = g_window.get_window_size();
        float aspect = (float)window_size.first / window_size.second;

        // for (int i = 0; i < circles.size(); ++i) {
        //     float circle_radius = circles[i].get_radius();

        //     // set matrix to scale object in world coordinates
        //     glm::mat4 model = glm::mat4(1.0f);
        //     model = glm::translate(model, *circles[i].get_position());
        //     model = glm::scale(
        //         model,
        //         glm::vec3(circle_radius / 10.f, circle_radius / 10.f, 1.0f));

        //     // set matrix for object translation
        //     glm::mat4 view = glm::mat4(1.0f);
        //     view = glm::translate(view, glm::vec3(0.f, 0.f, 0.f));

        //     // set projection matrix to preserve aspect ratio after window
        //     size
        //     // changes
        //     glm::mat4 projection = glm::mat4(1.0f);
        //     projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);

        //     circle_shader.set_mat4f("model", model);
        //     circle_shader.set_mat4f("view", view);
        //     circle_shader.set_mat4f("projection", projection);

        //     glDrawArrays(GL_TRIANGLE_FAN, 0, circle_num_points);
        // }
        std::vector<verletCircle::VerletCircle *> verlet_circles =
            g_physics_simulation.get_circles();
        for (int i = 0; i < verlet_circles.size(); ++i) {

            // set matrix to scale object in world coordinates
            glm::mat4 model = glm::mat4(1.0f);

            glm::vec3 position = glm::vec3(
                verlet_circles[i]->m_current_position.x,
                verlet_circles[i]->m_current_position.y, 0.0f);

            position.x = -1.0f + 2.0f * (position.x / constants::WINDOW_WIDTH);
            position.y = 1.0f - 2.0f * (position.y / constants::WINDOW_HEIGHT);

            model = glm::translate(model, position);
            model = glm::scale(model, glm::vec3(0.0490f, 0.0490f, 1.0f));

            // printf(
            //     "\n\nclamped:  %f\n\n",
            //     glm::clamp(verlet_circles[i]->get_radius(), -1.0f, 1.0f));

            // set matrix for object translation
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, glm::vec3(0.f, 0.f, 0.f));

            // set projection matrix to preserve aspect ratio after window size
            // changes
            //

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f);
            // projection = glm::ortho(
            //     -((float)constants::WINDOW_WIDTH / 2.0f),
            //     (float)constants::WINDOW_WIDTH / 2.0f,
            //     (float)constants::WINDOW_HEIGHT / 2.0f,
            //     -((float)constants::WINDOW_HEIGHT / 2.0f));

            circle_shader.set_mat4f("model", model);
            circle_shader.set_mat4f("view", view);
            circle_shader.set_mat4f("projection", projection);

            glDrawArrays(GL_TRIANGLE_FAN, 0, g_circle_num_points);
        }
        glUseProgram(0);
        glBindVertexArray(0);

        glfwSwapBuffers(g_window.get_window());
        glfwPollEvents();
        if (g_sim_running) {

            g_physics_simulation.update(delta_time);
        }
    }
}

void create_circle() {
    uint    VBO;
    int     num_points = g_circle_num_points;
    GLfloat vertex_data[6 * num_points];
    uint    current_index = 0;
    int     theta = 0;
    float   radius = 1.f;

    while (theta < 360) {
        GLfloat x = (GLfloat)radius * cosf(theta * M_PI / 180.0f);
        GLfloat y = (GLfloat)radius * sinf(theta * M_PI / 180.0f);

        vertex_data[current_index++] = x;
        vertex_data[current_index++] = y;

        vertex_data[current_index++] = 0.5f;

        vertex_data[current_index++] = 1.0f;
        vertex_data[current_index++] = 1.0f;
        vertex_data[current_index++] = 1.0f;

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

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        g_sim_running = true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        g_sim_running = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        printf("");
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
    uint VBO;

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

    // index of attrib, type, normalized, offset between values, starting index
    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

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

void spawn_circle(double pos_x, double pos_y) {
    for (int i = 0; i < 10; ++i) {
        g_physics_simulation.add_circle({pos_x - i * 5, pos_y - i * 5}, 20.f);
    }
    // g_physics_simulation.add_circle({pos_x, pos_y}, 20.f);
}

void mouse_button_callback(
    GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double pos_x, pos_y;

        glfwGetCursorPos(g_window.get_window(), &pos_x, &pos_y);

        spawn_circle(pos_x, pos_y);
    }
}

float normalize_value(
    float value, float range_min_1, float range_max_1, float range_min_2,
    float range_max_2) {
    return ((range_max_2 - range_min_2) * (value - range_min_1) /
            (range_max_1 - range_min_1)) +
           range_min_2;
}
