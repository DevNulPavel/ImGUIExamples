#include <inttypes.h>

typedef unsigned int uint;

int checkOpenGLerror(const char *file, int line);
void glDebugOut(uint source, uint type, uint id, uint severity, int length, const char* message, void* userParam);
