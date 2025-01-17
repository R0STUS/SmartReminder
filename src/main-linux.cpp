#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"
#include "dbe16.hpp"

struct RemindDay {
    unsigned long time;
    bool isReminded;
    std::string name;
    int typeOfDay;
};

struct Config {
	float defVolume;
};
/*
Json getJson(const std::string& path) {
	Json output;
	std::ifstream file(path);
	try {
		output = Json::parse(file);
	} catch (const Json::parse_error& e) {
		std::cout << "ERROR! " << e.what() << std::endl;
	}
	return output;
}

Config jtconfig(Json& jconfig) {
	Config output;
    output.defVolume = jconfig["defVolume"];
	return output;
}*/

int main() {
    std::cout << "Initialising core..." << std::endl;
	//Json jconfig = getJson("res/config.json");
	//Config config = jtconfig(jconfig);
    std::cout << "Initialising GLFW..." << std::endl;
    if (!glfwInit()) {
        fprintf(stderr, "Error: Cannot init GLFW\n");
        return EXIT_FAILURE;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "SmartReminder", NULL, NULL);
    std::cout << "Creating window GLFW..." << std::endl;
    if (!window) {
        fprintf(stderr, "Error: Cannot create window GLFW\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    std::cout << "Initialising GLAD..." << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Error: Cannot init GLAD\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    std::cout << "Initialising GLT..." << std::endl;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    if (!gltInit()) {
        fprintf(stderr, "Error: Cannot init glText\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    GLTtext* text = gltCreateText();
    gltSetText(text, "hello, world!");
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        gltBeginDraw();
        gltColor(1.0f, 1.0f, 1.0f, 1.0f);
        gltDrawText2DAligned(text, (float)(width / 2), (float)(height / 2), 2.0f, GLT_CENTER, GLT_CENTER);
        gltEndDraw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    gltDeleteText(text);
    gltTerminate();
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
// It's only alpha-test! In the future this will be replaced
