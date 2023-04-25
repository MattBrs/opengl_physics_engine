#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace shader {
class Shader {
  public:
    Shader(const char *vertex_path, const char *fragment_path);
    Shader();
    ~Shader();

    void use();
    void set_bool(const std::string name, bool value) const;
    void set_int(const std::string name, int value) const;
    void set_float(const std::string name, float value) const;
    void set_mat4f(const std::string name, const glm::mat4 &value) const;
    bool compile_shaders();
    void print_shader_log(GLuint shader);
    void add_shader(
        GLuint shader_program, const char *shader_code, GLenum shader_type);

    unsigned int get_program_id();

  private:
    unsigned int m_shader_id;
};
} // namespace shader
