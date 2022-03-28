#include <iostream>
#include "GLFW/glfw3.h"

int main() {
    using namespace std;

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    };

    cout << "Hello world" << endl;

    glfwTerminate();
    return 0;
}