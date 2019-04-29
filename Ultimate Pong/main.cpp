#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"


int main(void)
{

	GLFWwindow* window;

	//Initialize glfw
	if (!glfwInit())
		return -1;

	// Create window mode window and its OpenGL context
	window = glfwCreateWindow(700, 700, "Ultimate Pong", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make windows context current 
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) //must be after valid context! (for debug-checking if ok)
		std::cout << "error" << std::endl;

	///////////////////////////////////BG VARS
	const float DEG2RAD = 3.14159 / 180;
	float radius = 0.2;
	float r = 0.0;
	float g = 0.3;
	float b = 0.6;
	float x = 0.5;
	float y = -0.33;
	bool ballLeft = true;
	bool ballDown = true;
	float speed = 0.005;

	//begin IMGUI init
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui::SetNextWindowContentSize(ImVec2(200, 200));
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");
	bool windowActive = true;

	//Font loading (gotta do in init stages, cannot modify between new_frame and end_frame
	ImGuiIO & io = ImGui::GetIO();
	ImFont* ButtonFont = io.Fonts->AddFontFromFileTTF("InconReg.ttf", 25);
	ImFont* TitleFont = io.Fonts->AddFontFromFileTTF("TitleFont.ttf", 60);

	//runs until X is hit or Quit button
	while (!glfwWindowShouldClose(window))
	{
		//Background animation
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		//Movement of the ball
		if (ballLeft) {
			if (x > -1 + radius) {
				x -= speed;
			}
			else {
				ballLeft = false;
			}
		}
		else {
			if (x < 1 - radius) {
				x += speed;
			}
			else {
				ballLeft = true;
			}
		}

		if (ballDown) {
			if (y > -1 + radius) {
				y -= speed;
			}
			else {
				ballDown = false;
			}
		}
		else {
			if (y < 1 - radius) {
				y += speed;
			}
			else {
				ballDown = true;
			}
		}
		//Color change of ball
		r= fmod(r + 0.001, 1);
		g= fmod(g + 0.002, 1);
		b= fmod(b + 0.003, 1);
		//Draw
		glColor3f(r, g, b);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float degInRad = i * DEG2RAD;
			glVertex2f((cos(degInRad)*radius) + x, (sin(degInRad)*radius) + y);
		}
		glEnd();
		
		if (windowActive = true) {

			ImGui_ImplOpenGL3_NewFrame();			//IMGUI new frame put before imgui code
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			//IMGUI window
			ImGui::Begin(" Main Menu");
			ImGui::PushFont(TitleFont);			  //Swtich font without leaving frame?	
			ImGui::Dummy(ImVec2(0.0f, 10.0f));             //padding
			ImGui::Text("  Ultimate Pong");
			ImGui::Dummy(ImVec2(0.0f, 40.0f));
			ImGui::PopFont();                 //pop back to font 1

			ImGui::Dummy(ImVec2(125.0f, 70.0f));	//Quit button padded down here too		
			ImGui::SameLine();
			if (ImGui::Button("Start", ImVec2(200, 50))) {

				windowActive = false;					//trying to cause entire 'if' stmt block for ImGui to no longer execute
				//begin game
			}

			ImGui::Dummy(ImVec2(125.0f, 0.0f));
			ImGui::SameLine();
			if (ImGui::Button("Quit", ImVec2(200, 50))) {

				return 0;						//terminate program
			}
			
			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		//swap buffer; check for events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//IMGUI cleanup (terminate, etc)
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}