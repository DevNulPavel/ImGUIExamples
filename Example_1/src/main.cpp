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
#include <Helpers.h>

//#define GLFW_INCLUDE_GLCOREARB 1 // Tell GLFW to include the OpenGL core profile header
//#define GLFW_INCLUDE_GLU
//#define GLFW_INCLUDE_GL3
//#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>




#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

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
    
    // Инициализация отладки
    if(glDebugMessageCallback){
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        
        // Коллбек ошибок OpenGL
        glDebugMessageCallback((GLDEBUGPROC)glDebugOut, 0);
        
        // Более высокий уровень отладки
        // GLuint unusedIds = 0;
        // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, true);
    }
    
    const unsigned char* version = glGetString(GL_VERSION);
    printf("OpenGL version = %s\n", version);
    fflush(stdout);
    
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
    ImGuiWindowFlags windowFlags = 0;
    bool windowOpened = true;
    bool test1 = false;
    bool test2 = false;
    float sliderValue = 0.0f;
    float color[3] = {};
    bool openFilePressed = false;
    bool pastePressed = false;
    int radioButtonValue = 0;
    int arrowsCounter = 0;
    
    // Главный цикл отрисовки
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        // Начало отрисовки ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        // Тестовое окно
        {
            ImGui::SetNextWindowPos(ImVec2(20, 10), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(400, 650), ImGuiCond_Once);
            
            //windowFlags |= ImGuiWindowFlags_NoTitleBar;
            //windowFlags |= ImGuiWindowFlags_NoScrollbar;
            windowFlags |= ImGuiWindowFlags_MenuBar;
            //windowFlags |= ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoResize;
            windowFlags |= ImGuiWindowFlags_NoCollapse;
            windowFlags |= ImGuiWindowFlags_NoNav;
            
            ImGui::Begin("Hello, world!", &windowOpened, windowFlags);
            
            // Меню на самом верху окна
            if (ImGui::BeginMenuBar()){
                if (ImGui::BeginMenu("File")){
                    ImGui::MenuItem("Open file", NULL, &openFilePressed);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Edit")){
                    ImGui::MenuItem("Paste", NULL, &pastePressed);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            ImGui::Spacing();
            
            // Сворачивающееся меню
            if (ImGui::CollapsingHeader("Help")){
                ImGui::TextWrapped("Wrapped text.\n");
                if (ImGui::TreeNode("Three node 1")){
                    ImGui::Text("Test text 1");
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Three node 2")){
                    ImGui::Text("Test text 2");
                    ImGui::TreePop();
                }
            }
            ImGui::Separator();
            ImGui::Spacing();
            
            //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);
            
            // Пример RadioButton
            ImGui::RadioButton("Radio a", &radioButtonValue, 0);
            ImGui::SameLine();
            ImGui::RadioButton("Radio b", &radioButtonValue, 1);
            ImGui::SameLine();
            ImGui::RadioButton("Radio c", &radioButtonValue, 2);
            ImGui::Separator();
            ImGui::Spacing();
            
            // Цветные кнопки
            for (int i = 0; i < 3; i++){
                if (i > 0) ImGui::SameLine();
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(i/7.0f, 0.7f, 0.7f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8, 0.8f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(i/7.0f, 0.8f, 0.8f));
                ImGui::Button("Click");
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
            ImGui::Separator();
            ImGui::Spacing();
            
            // Стрелочные кнопки
            float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::PushButtonRepeat(true);
            if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { arrowsCounter--; }
            ImGui::SameLine(0.0f, spacing);
            if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { arrowsCounter++; }
            ImGui::PopButtonRepeat();
            ImGui::SameLine();
            ImGui::Text("%d", arrowsCounter);
            ImGui::Separator();
            ImGui::Spacing();
            
            // Всплывающее окно
            ImGui::Text("Hover over me");
            if (ImGui::IsItemHovered()){
                ImGui::SetTooltip("I am a tooltip");
            }
            
            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Test 1", &test1);
            ImGui::SameLine();
            ImGui::Checkbox("Test 2", &test2);
            
            ImGui::SliderFloat("float", &sliderValue, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", color);
            
            ImGui::Separator();
            
            if (ImGui::Button("Button")){
                printf("Button pressed\n");
                fflush(stdout);
            }
            ImGui::SameLine();
            ImGui::Text("Slider value = %.1f", sliderValue);
            
            ImGui::Bullet();
            ImGui::Text("Frame time %.1f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::BulletText("Bullet text example");
            
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

