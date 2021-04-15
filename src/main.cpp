/*
    Universidade Federal do Rio Grande do Sul
             Instituto de Inform�tica
       Departamento de Inform�tica Aplicada

    INF01047 Fundamentos de Computa��o Gr�fica
               Prof. Eduardo Gastal

               TRABALHO FINAL

    Autor: Guilherme Souza Sales 301220
*/

// padr�es de C
#include <cmath>
#include <cstdio>
#include <cstdlib>

// espec�ficos de C++
#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers da biblioteca GLM: cria��o de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "loaders.h"
#include "input.h"
#include "camera.h"

// S.O. callbacks
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

void LoadGpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);

GLuint vertex_shader_id;
GLuint fragment_shader_id;
GLuint program_id = 0;
GLint model_uniform;
GLint view_uniform;
GLint projection_uniform;
GLint object_id_uniform;

float g_ScreenRatio = 16/9;

Input input;

int main()
{
    int success = glfwInit();
    if (!success)
    {
        fprintf(stderr, "ERROR: glfwInit() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(1280, 720, "MauroKart -- by gssales", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
        std::exit(EXIT_FAILURE);
    }

    double last_time = glfwGetTime();


    glfwSetKeyCallback(window, KeyCallback);

    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetWindowSize(window, 1280, 720);

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    const GLubyte *vendor      = glGetString(GL_VENDOR);
    const GLubyte *renderer    = glGetString(GL_RENDERER);
    const GLubyte *glversion   = glGetString(GL_VERSION);
    const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

    LoadGpuProgram("../../res/shaders/shader_vertex.glsl", "../../res/shaders/shader_fragment.glsl");

    glEnable(GL_DEPTH_TEST);

    /* Load game assets and setup logic */
    Camera active_cam;

    double curr_time, dt;
    while (!glfwWindowShouldClose(window))
    {
        curr_time = glfwGetTime();
        dt = curr_time - last_time;

        /* Update game logic */
        KeyState key_W = input.GetKeyState(GLFW_KEY_W);
        if (key_W.key == GLFW_KEY_W) {
            if (key_W.is_pressed)
                printf("W Pressed!\n");
            if (key_W.is_down)
                printf("W Down!\n");
            if (key_W.is_released)
                printf("W Released!\n");
        }

        active_cam.Update();

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_id);

        /* draw/render */

        glm::mat4 view = active_cam.View();
        glm::mat4 projection = active_cam.Projection(g_ScreenRatio);
        glUniformMatrix4fv(view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
        glUniformMatrix4fv(projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));

        /* build matrix */

        /* draw element */

        glfwSwapBuffers(window);

        input.Update();
        glfwPollEvents();

        last_time = curr_time;
    }

    glfwTerminate();

    return 0;
}

GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id)
{
    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);

    glLinkProgram(program_id);

    GLint linked_ok = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &linked_ok);

    if ( linked_ok == GL_FALSE )
    {
        GLint log_length = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);

        GLchar* log = new GLchar[log_length];

        glGetProgramInfoLog(program_id, log_length, &log_length, log);

        std::string output;

        output += "ERROR: OpenGL linking of program failed.\n";
        output += "== Start of link log\n";
        output += log;
        output += "\n== End of link log\n";

        delete [] log;

        fprintf(stderr, "%s", output.c_str());
    }

    return program_id;
}

void LoadGpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename)
{
    vertex_shader_id = LoadShader_Vertex("../../res/shaders/shader_vertex.glsl");
    fragment_shader_id = LoadShader_Fragment("../../res/shaders/shader_fragment.glsl");

    if ( program_id != 0 )
        glDeleteProgram(program_id);

    program_id = CreateGpuProgram(vertex_shader_id, fragment_shader_id);

    model_uniform           = glGetUniformLocation(program_id, "model");
    view_uniform            = glGetUniformLocation(program_id, "view");
    projection_uniform      = glGetUniformLocation(program_id, "projection");
    object_id_uniform       = glGetUniformLocation(program_id, "object_id");
}

void ErrorCallback(int error, const char* description)
{
    fprintf(stderr, "ERROR: GLFW: %s\n", description);
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    // Se o usu�rio pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);


    input.KeyCallback(key, action, mod);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    g_ScreenRatio = (float)width / height;
}

