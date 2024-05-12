#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "Shader.h"
#include <iostream>
#include <utility>

struct setting {
    GLint width, height;
    const char windowName[];
};

void WindowResize(GLFWwindow* pWindow, GLint width, GLint height);
void KeyCallback(GLFWwindow* pWindow, GLint key, GLint scancode, GLint action, GLint mode);

setting windowSetting = { 640,480,"Edga Engine" };

int main(int argc, char* argv[]) {
    std::string path = argv[0];
    path = path.substr(0, path.find_last_of("\\"));
    std::cout << path << "\n";

    bool polygonMode = false;

    if (!glfwInit()) {
        std::cout << "Error initialization glfw function glfwInit()\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(windowSetting.width, windowSetting.height, windowSetting.windowName, nullptr, nullptr);
    if (!pWindow) {
        std::cout << "Error initialization window\n";
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(pWindow, WindowResize);
    glfwSetKeyCallback(pWindow, KeyCallback);
    glfwMakeContextCurrent(pWindow);


    if (!gladLoadGL()) {
        std::cout << "Error initialization glad\n";
        return -10;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init();


    std::cout << "Video card:" << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version:" << glGetString(GL_VERSION) << "\n";

    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    GLfloat backgroundColor[4] = { 1,1,0,0 };
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    GLfloat verticesOne[] = {
         0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
    };


    GLfloat verticesTwo[] = {
     0.4f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.4f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.4f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
    -0.4f,  0.4f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VAO[2], VBO[2], EBO[2];

    for (int i = 0; i < 2; ++i) {
        glGenVertexArrays(1, &VAO[i]);
        glBindVertexArray(VAO[i]);

        glGenBuffers(1, &VBO[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

        if (i == 0)
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOne), verticesOne, GL_STATIC_DRAW);
        else
            glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTwo), verticesTwo, GL_STATIC_DRAW);


        glGenBuffers(1, &EBO[i]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
    
    Shader shader_one(
        std::pair<const char, std::string> {'v',"vertex.glsl"}, 
        std::pair<const char, std::string> {'f', "fragment2.glsl"}
    );

    Shader shader_two(
        std::pair<const char, std::string> {'v', "vertex.glsl"},
        std::pair<const char, std::string> {'f', "fragment.glsl"}
    );


    GLint fragmentLocationUniform = glGetUniformLocation(shader_two.shaderProgram, "effect");




    while (!glfwWindowShouldClose(pWindow)) {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        glClear(GL_COLOR_BUFFER_BIT);

        if (polygonMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for (int i = 0; i < 2; ++i) {
            if (i == 0) {
                shader_one.Use();
                glBindVertexArray(VAO[i]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            else {
                shader_two.Use();
                glUniform1f(fragmentLocationUniform, (sin(glfwGetTime())) + 0.5);
                glBindVertexArray(VAO[i]);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }

        ImGui::Begin("Settings");
        ImGui::Checkbox("glPolygonMode", &polygonMode);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(pWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    for (int i = 0; i < 2; ++i) {
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteBuffers(1, &VBO[i]);
        glDeleteBuffers(1, &EBO[i]);
    }

    glfwTerminate();
    return 0;
}

void WindowResize(GLFWwindow* pWindow, GLint width, GLint height) {
    windowSetting.width = width;
    windowSetting.height = height;
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow* pWindow, GLint key, GLint scancode, GLint action, GLint mode) {
    std::cout << key << " " << scancode << " " << action << " " << mode << "\n";

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}