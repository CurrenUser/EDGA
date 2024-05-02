#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <iostream>

enum shaders { vertex, fragment };

struct setting {
    GLint width, height;
    const char windowName[];
};

GLuint* CreateShader(shaders shadersType);
GLuint* CreateShaderProgram(GLuint* vertexShader, GLuint* fragmentShader);
void WindowResize(GLFWwindow* pWindow, GLint width, GLint height);
void KeyCallback(GLFWwindow* pWindow, GLint key, GLint scancode, GLint action, GLint mode);

setting windowSetting = { 640,480,"Edga Engine" };

int main(void)
{
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
    ImGui::StyleColorsDark(); // you can also use ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
    ImGui_ImplOpenGL3_Init();


    std::cout << "Video card:" << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version:" << glGetString(GL_VERSION) << "\n";

    GLfloat backgroundColor[4] = { 0,1,0,0 };
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);

    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // Верхний правый угол
         0.5f, -0.5f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f,  // Нижний левый угол
        -0.5f,  0.5f, 0.0f   // Верхний левый угол
    };

    GLuint indices[] = {  
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint *vertexShader = CreateShader(shaders::vertex);
    GLuint *fragmentShader = CreateShader(shaders::fragment);
    GLuint *shaderProgram = CreateShaderProgram(vertexShader, fragmentShader);

    glDeleteShader(*vertexShader);
    glDeleteShader(*fragmentShader);


    while (!glfwWindowShouldClose(pWindow)) {

        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        glClear(GL_COLOR_BUFFER_BIT);

        if(polygonMode) 
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glUseProgram(*shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        ImGui::Begin("Settings");
        static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
        ImGui::Checkbox("glPolygonMode", &polygonMode);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 
        glfwSwapBuffers(pWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

GLuint* CreateShader(shaders shaderType) {

    GLuint* shader = new GLuint;
    GLint success;
    GLchar infoLog[1024];

    const GLchar* vertexSrcShader =
        "#version 460 core\n"
        "layout(location = 0) in vec3 position;\n"
        "void main() {\n"
        "gl_Position = vec4(position.x, position.y, position.z, 1.0);}\0";

    const GLchar* fragmentSrcShader =
        "#version 460 core\n"
        "out vec4 color;\n"
        "void main() {\n"
        "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);}\0";

    switch (shaderType) {
        case shaders::fragment:
            std::cout << "Compile vertex shader\n";
            *shader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(*shader, 1, &vertexSrcShader, NULL);
            glCompileShader(*shader);
            break;
        case shaders::vertex:
            std::cout << "Compile fragment shader\n";
            *shader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(*shader, 1, &fragmentSrcShader, NULL);
            glCompileShader(*shader);
            break;
        default:
            std::cout << "Error, unknown shader type\n";
            delete shader;
            shader = nullptr;
            break;

            if (shader) {
                glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(*shader, 1024, NULL, infoLog);
                    std::cout << "Error compile shader!\n" << infoLog << "\n";
                }
            }
    }
    return shader;
}

GLuint* CreateShaderProgram(GLuint* vertexShader, GLuint* fragmentShader) {

    GLuint* shaderProgram = new GLuint;
    GLint success;
    GLchar infoLog[1024];

    *shaderProgram = glCreateProgram();
    glAttachShader(*shaderProgram, *vertexShader);
    glAttachShader(*shaderProgram, *fragmentShader);
    glLinkProgram(*shaderProgram);
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 1024, NULL, infoLog);
        std::cout << "Error linked shader program!\n" << infoLog << "\n";
        delete shaderProgram;
        shaderProgram = nullptr;
    }

    return shaderProgram;
}