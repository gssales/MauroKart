/*
    Universidade Federal do Rio Grande do Sul
             Instituto de Informática
       Departamento de Informática Aplicada

    INF01047 Fundamentos de Computação Gráfica
               Prof. Eduardo Gastal

               TRABALHO FINAL

    Autor: Guilherme Souza Sales 301220
*/

// padrões de C
#include <cmath>
#include <cstdio>
#include <cstdlib>

// específicos de C++
#include <map>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>

// Headers das bibliotecas OpenGL
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Headers da biblioteca GLM: criação de matrizes e vetores.
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "matrices.h"
#include "loaders.h"
#include "camera.h"
#include "scene.h"
#include "model.h"
#include "gamestate.h"

// S.O. callbacks
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

void LoadGpuProgram(const char* vertex_shader_filename, const char* fragment_shader_filename);
GLuint CreateGpuProgram(GLuint vertex_shader_id, GLuint fragment_shader_id);
void DrawVirtualObject(const char* object_name);

GLuint vertex_shader_id;
GLuint fragment_shader_id;
GLuint program_id = 0;
GLint model_uniform;
GLint view_uniform;
GLint projection_uniform;
GLint object_id_uniform;

float g_ScreenRatio = 1.0f;

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
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetScrollCallback(window, ScrollCallback);

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

    ObjModel bunnymodel("../../res/models/bunny.obj");
    AddModelToScene(&bunnymodel);

    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    /* Load game assets and setup logic */
    Camera active_cam;
    active_cam.free = false;
    active_cam.distance = 2.5;

    double curr_time, dt;
    double g_LastCursorPosX, g_LastCursorPosY;
    while (!glfwWindowShouldClose(window))
    {
        curr_time = glfwGetTime();
        dt = curr_time - last_time;

        /* Update game logic */
        // KeyState key_W = input.GetKeyState(GLFW_KEY_W);
        // if (key_W.key == GLFW_KEY_W) {
        //     if (key_W.is_pressed)
        //         printf("W Pressed!\n");
        //     if (key_W.is_down)
        //         printf("W Down!\n");
        //     if (key_W.is_released)
        //         printf("W Released!\n");
        // }

        KeyState left_button = input.GetKeyState(GLFW_MOUSE_BUTTON_LEFT);
        if (left_button.is_pressed) {
            glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
        }
        if (left_button.is_down && !left_button.is_pressed) {
            PairState cursor = input.cursor_state;
            float dx = cursor.xvalue - g_LastCursorPosX;
            float dy = cursor.yvalue - g_LastCursorPosY;

            active_cam.theta -= dx * dt;
            active_cam.phi += dy * dt;

            float phimax = 3.141592f/2;
            float phimin = -phimax;
        
            if (active_cam.phi > phimax)
                active_cam.phi = phimax;
        
            if (active_cam.phi < phimin)
                active_cam.phi = phimin;

            g_LastCursorPosX = cursor.xvalue;
            g_LastCursorPosY = cursor.yvalue;
        }
        if (input.scroll_changed) {
            PairState scroll = input.scroll_state;
            active_cam.distance -= scroll.yvalue * dt;
            
            const float verysmallnumber = std::numeric_limits<float>::epsilon();
            if (active_cam.distance < verysmallnumber)
                active_cam.distance = verysmallnumber;
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
        glm::mat4 model = Matrix_Identity();
        glUniformMatrix4fv(model_uniform, 1 , GL_FALSE , glm::value_ptr(model));

        /* draw element */
        // glUniform1i(object_id_uniform, SPHERE);
        DrawVirtualObject("bunny");

        glfwSwapBuffers(window);

        input.Update();
        glfwPollEvents();

        last_time = curr_time;
    }

    glfwTerminate();

    return 0;
}

void DrawVirtualObject(const char* object_name)
{
    glBindVertexArray(virtualScene[object_name].vertex_array_object_id);

    glDrawElements(
        virtualScene[object_name].rendering_mode,
        virtualScene[object_name].num_indices,
        GL_UNSIGNED_INT,
        (void*)(virtualScene[object_name].first_index * sizeof(GLuint))
    );

    glBindVertexArray(0);
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
    // Se o usuário pressionar a tecla ESC, fechamos a janela.
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    input.KeyCallback(key, action, mod);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    input.KeyCallback(button, action, mods);
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    input.cursor_state.xvalue = xpos;
    input.cursor_state.yvalue = ypos;
    input.cursor_changed = true;
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    input.scroll_state.xvalue = xoffset;
    input.scroll_state.yvalue = yoffset;
    input.scroll_changed = true;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    g_ScreenRatio = (float)width / height;
    printf("%d %d %f\n", width, height, g_ScreenRatio);
}

