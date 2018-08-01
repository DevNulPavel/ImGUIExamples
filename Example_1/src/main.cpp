// Windows
#ifdef _MSVC_LANG 
    #define NOMINMAX
    #include <windows.h>
#endif

#include <stdio.h>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//#define GLFW_INCLUDE_GLCOREARB 1 // Tell GLFW to include the OpenGL core profile header
//#define GLFW_INCLUDE_GLU
//#define GLFW_INCLUDE_GL3
//#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>




#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

GLFWwindow* window = nullptr;

void glfwErrorCallback(int error, const char* description) {
    printf("OpenGL error = %d\n description = %s\n\n", error, description);
    fflush(stdout);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void glfwMouseButtonCallback(GLFWwindow* window, int button, int state, int mod) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, state, mod);
    
    if (ImGui::IsMouseHoveringAnyWindow() == false) {
        // обработка левой кнопки
        if(button == GLFW_MOUSE_BUTTON_1){
            if(state == GLFW_PRESS){
                printf("Mouse left down\n");
            }else{
                printf("Mouse left up\n");
            }
        }
        // обработка правой кнопки
        if(button == GLFW_MOUSE_BUTTON_2){
            if(state == GLFW_PRESS){
                printf("Mouse right down\n");
            }else{
                printf("Mouse right up\n");
            }
        }
        fflush(stdout);
    }
}

void glfwCursorCallback(GLFWwindow* window, double x, double y) {
}

void glfwScrollCallback(GLFWwindow* window, double scrollByX, double scrollByY) {
    ImGui_ImplGlfw_ScrollCallback(window, scrollByX, scrollByY);
}

#ifndef _MSVC_LANG
int main(int argc, char** argv) {
#else
int local_main(int argc, char** argv) {
#endif
    // GLFW Init
    if (!glfwInit()){
        return 2;
    }
    
    // обработчик ошибок
    glfwSetErrorCallback(glfwErrorCallback);
    
    // Тюнинг GLFW
    #if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    
    // Создаем окно
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ImGUI example", nullptr, nullptr);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Вертикальная синхронизация
    
    // GLEW Init
    glewExperimental = GL_TRUE;
    if (glewInit() != 0){
        return 2;
    }
    
    // Создание ImGUI контекста
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    // Инициализация врапперов ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, false); // НЕ устанавливаем стандартные коллбеки
    ImGui_ImplOpenGL3_Init("#version 150");
    
    // Перегрузка обработки клавиш и прочего
    glfwSetKeyCallback(window, glfwKeyCallback);
    glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
    glfwSetCursorPosCallback(window, glfwCursorCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);
    
    // Тестовые переменные
    bool test1 = false;
    bool test2 = false;
    float sliderValue = 0.0f;
    float color[3] = {};
    
    // Главный цикл отрисовки
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Начало отрисовки ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Тестовое окно
        {
            ImGui::Begin("Hello, world!");
            
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Test 1", &test1);      // Edit bools storing our window open/close state
            ImGui::SameLine();
            ImGui::Checkbox("Test 2", &test2);
            
            ImGui::SliderFloat("float", &sliderValue, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", color); // Edit 3 floats representing a color
            
            if (ImGui::Button("Button")){                            // Buttons return true when clicked (most widgets return true when edited/activated)
                printf("Button pressed\n");
                fflush(stdout);
            }
            ImGui::SameLine();
            ImGui::Text("Slider value = %.1f", sliderValue);
            
            ImGui::Bullet();
            ImGui::Text("Frame time %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::End();
        }
        
        // Вызываем обработку ImGUI
        ImGui::Render();
        
        // Непосредственно сама отрисовка ImGUI
        glfwMakeContextCurrent(window);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }
    
    // Чистка ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // Очищаем GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}

#ifdef _MSVC_LANG
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow) {
	local_main(0, NULL);
	return 0;
}
#endif

