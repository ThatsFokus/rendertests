#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

#include <string>



#define ERROR_STREAM std::cerr
#define glsl_version "#version 130"

void setupGLFW(GLFWerrorfun callback = nullptr, int swapinterval = 1) {
	if (glfwInit() == GLFW_FALSE) {
		ERROR_STREAM << "Failed GLFW Init" << std::endl;
		exit(1);
	}
	std::cout << "GLFW Init success" << std::endl;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwSwapInterval(swapinterval); // Enable vsync
	if (callback) glfwSetErrorCallback(callback);
}
void setupGL3W() {
	if (gl3wInit() != 0)
	{
		ERROR_STREAM << "Failed to initialize OpenGL loader!" << std::endl;
		exit(2);
	}
	std::cout << "GL3W Init success" << std::endl;
}
void glfwEnd(GLFWwindow* window) {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void imguiStart(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
void imguiEnd() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}



void doStuff(GLFWwindow* window) {


    std::vector<PDU> pdus;
    PcapReader pcapreader;

    pcapreader.open("C:\\Users\\Joshua\\Desktop\\test.pcap");
    pcapreader.beginRead(&pdus);

    bool showtest = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    ImVec2 siztest = ImVec2(400, 400);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Test Window", &showtest);

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (showtest) {
            ImGui::Begin("Test Window", &showtest);
            ImGui::Text("Hello");
            ImGui::SetWindowSize(siztest);

            for (int i = 0; i < pdus.size(); i++) {
                std::string a = std::to_string(pdus[i].pkhdr->incl_len);
                ImGui::Text(a.c_str());
            }



            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}