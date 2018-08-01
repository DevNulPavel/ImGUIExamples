#include "Helpers.h"
#include <iostream>

#define GLFW_INCLUDE_GLU
#include <GL/glew.h>        // для поддержки расширений, шейдеров и так далее

using namespace std;

int checkOpenGLerror(const char* file, int line) {
    GLenum glErr = GL_NO_ERROR;
    int retCode = 0;
    glErr = glGetError();
    if (glErr != GL_NO_ERROR) {
        cout << "OpenGL Error in file " << file << " at " << line << ": " << glewGetErrorString(glErr) << endl;
        cout.flush();
        retCode = 1;
    }
    return retCode;
}

void glDebugOut(unsigned int source, uint type, uint id, uint severity, int length, const char* message, void* userParam){
    cout << "###### opengl-callback BEGIN ######" << endl;
    cout << "message: "<< message;
    cout << "type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            cout << "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            cout << "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            cout << "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            cout << "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            cout << "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            cout << "OTHER";
            break;
    }
    cout << endl;
    
    cout << "id: " << id << endl;
    cout << "severity: ";
    switch (severity){
        case GL_DEBUG_SEVERITY_LOW:
            cout << "LOW";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            cout << "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            cout << "HIGH";
            break;
    }
    cout << endl << endl;
    cout.flush();
}
