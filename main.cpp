#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constants.hpp"
#include "Types.hpp"
#include "VerletSolver.hpp"
#include "Window.hpp"
#include <OpenGL/OpenGL.h>
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
GLuint program_id;
uint   VBO, VAO, EBO;

bool init();
void run();
void quit();
void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y);
void add_shader(
    GLuint shader_program, const char *shader_code, GLenum shader_type);

bool create_triangle();
bool compile_shaders();
void print_shader_log(GLuint shader);
void process_input(GLFWwindow *window);

int main(int argc, char *args[]) {
    if (!init()) {
        return 1;
    }

    create_triangle();
    compile_shaders();

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

    while (!glfwWindowShouldClose(g_window.get_window())) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        process_input(g_window.get_window());

        // set color to use when clear function is called
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program_id);
        glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(g_window.get_window());
        glfwPollEvents();

        /*
         * Try to make a couple triangles with different VBOs and then call
         * bindVertexArray and draw them
         * */
    }
}

void spawn_rect(Solver &physics_simulation, int pos_x, int pos_y) {
    Vector2<double> rect_position(pos_x, pos_y);
    int             size = 30 + (rand() % 100);
    Vector2<double> rect_size(size, size);

    physics_simulation.add_rect(rect_position, rect_size);
}

bool create_triangle() {
    // triangle vertices in normalized positions
    // (opengl operates from -1.0 to 1.0f)
    GLfloat vertex_data[] = {-0.5f, -0.3f, 0.0f, 0.0f, 0.5f,  0.0f,
                             0.0f,  -0.2f, 0.0f, 0.5f, -0.3f, 0.0f};

    GLuint index_data[] = {0, 1, 2, 1, 2, 3};

    // The VAO saves all the information about the things we want to draw
    // so we declare it before all the VBO, etc creation and setting.
    // Then we unbind it and select it if we want to use it.
    // (Say we want to have multiple triangles and rectangles, we first make the
    // VAOs and VBOs, etc and then we select it for drawing).
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // set the Vertex Buffer Objects
    // (buffer in GPU memory with vertex positions)
    glGenBuffers(1, &VBO);

    // OpenGL allows different buffers open at the
    // same time as long taht they are of different types
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // load the data in the current buffer
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data,
        GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, 3 * sizeof(float), 0, (void *)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    return true;
}

void add_shader(
    GLuint shader_program, const char *shader_code, GLenum shader_type) {

    // GL_VERTEX_SHADER
    char   info_log[512];
    GLuint shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &shader_code, NULL);
    glCompileShader(shader);

    GLint v_shader_compiled = GL_TRUE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &v_shader_compiled);
    if (v_shader_compiled != GL_TRUE) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        printf("Unable to compile shader: %s\n", info_log);
        return;
    }

    glAttachShader(shader_program, shader);
    glDeleteShader(shader);
}

bool compile_shaders() {
    program_id = glCreateProgram();
    char info_log[512];

    const char   *vertex_shader_source[] = {" \n\
        #version 330 core \n\
        layout (location = 0) in vec3 pos; \n\
        void main()\n\
        {\n\
            gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n\
        }\n\
    "};
    const GLchar *fragment_shader_source[] = {" \n\
        #version 330 core \n\
        out vec4 fragColor; \n\
        void main()\n {\n fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n }\n"};

    add_shader(program_id, *vertex_shader_source, GL_VERTEX_SHADER);
    add_shader(program_id, *fragment_shader_source, GL_FRAGMENT_SHADER);

    glLinkProgram(program_id);
    GLint programm_success = GL_TRUE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &programm_success);
    if (programm_success != GL_TRUE) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        printf("Error linking program: %s\n", info_log);
        return false;
    }

    glValidateProgram(program_id);
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &programm_success);
    if (programm_success != GL_TRUE) {
        glGetProgramInfoLog(program_id, 512, NULL, info_log);
        printf("Error validating program: %s\n", info_log);
        return false;
    }
    printf("Finish adding and validating shaders\n");
    return true;
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
