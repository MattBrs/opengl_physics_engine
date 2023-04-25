#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include <cstdio>
#include <fstream>
#include <sstream>

using namespace shader;

Shader::~Shader() {
    // do nothing
}
Shader::Shader() {
    // do nothing
}

Shader::Shader(const char *vertex_path, const char *fragment_path) {
    std::string   vertex_code;
    std::string   fragment_code;
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);

        std::stringstream v_shader_stream, f_shader_stream;

        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        v_shader_file.close();
        f_shader_file.close();

        vertex_code = v_shader_stream.str();
        fragment_code = f_shader_stream.str();
    } catch (std::ifstream::failure error) {
        printf("Error while reading shader files!\n");
    }

    const char *v_shader_code = vertex_code.c_str();
    const char *f_shader_code = fragment_code.c_str();

    m_shader_id = glCreateProgram();

    add_shader(m_shader_id, v_shader_code, GL_VERTEX_SHADER);
    add_shader(m_shader_id, f_shader_code, GL_FRAGMENT_SHADER);
    compile_shaders();
}

void Shader::add_shader(
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

bool Shader::compile_shaders() {
    char info_log[512];
    glLinkProgram(m_shader_id);
    GLint programm_success = GL_TRUE;
    glGetProgramiv(m_shader_id, GL_LINK_STATUS, &programm_success);
    if (programm_success != GL_TRUE) {
        glGetProgramInfoLog(m_shader_id, 512, NULL, info_log);
        printf("Error linking program: %s\n", info_log);
        return false;
    }

    glValidateProgram(m_shader_id);
    glGetProgramiv(m_shader_id, GL_VALIDATE_STATUS, &programm_success);
    // if (programm_success != GL_TRUE) {
    //     glGetProgramInfoLog(m_shader_id, 512, NULL, info_log);
    //     printf("Error validating program: %s\n", info_log);
    //     return false;
    // }
    // printf("Finish adding and validating shaders\n");
    return true;
}

void Shader::set_bool(const std::string name, bool value) const {
    glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string name, int value) const {
    glUniform1i(glGetUniformLocation(m_shader_id, name.c_str()), value);
}

void Shader::set_float(const std::string name, float value) const {
    glUniform1f(glGetUniformLocation(m_shader_id, name.c_str()), value);
}

void Shader::use() {
    glUseProgram(m_shader_id);
}

unsigned int Shader::get_program_id() {
    return m_shader_id;
}

void Shader::set_mat4f(const std::string name, const glm::mat4 &value) const {
    glUniformMatrix4fv(
        glGetUniformLocation(m_shader_id, name.c_str()), 1, GL_FALSE,
        glm::value_ptr(value));
}
