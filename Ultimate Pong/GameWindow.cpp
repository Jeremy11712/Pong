// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>

// Include GLEW
#include <GL/glew.h>
#include <GL/glut.h>
// Include GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//freetype
#include <ft2build.h>
#include FT_FREETYPE_H
//classes
#include "Paddle.h"
#include "Ball.h"
#include "shader.hpp"
#include "Window.h"
#include "shader_s.h"
#include <chrono>

// ImGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

GLFWwindow *window;
Window gWindow;
const GLuint WIDTH = 800, HEIGHT = 800;

struct Character
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;	// Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;		// Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint VAO, VBO;
void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

Paddle *player1;
Paddle *player2;
Ball ball;
Shader shader;
const int KEY_CT = 349;
bool keysPressed[KEY_CT];
int state = 0;

void registerPressedKeys(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	std::cout << "test";
	if (key == GLFW_KEY_UNKNOWN)
		return;
	if (action == GLFW_PRESS)
	{
		keysPressed[key] = true;
		std::cout << "key pressed = " << key << std::endl;
	}
	else if (action == GLFW_RELEASE)
	{
		keysPressed[key] = false;
		std::cout << "key released = " << key << std::endl;
	}
}

void handleKeyEvent()
{
	for (int i = 0; i < KEY_CT; i++)
	{
		if (keysPressed[i])
		{
			if (i == GLFW_KEY_UP)
			{
				player1->moveUp();
			}
			else if (i == GLFW_KEY_W)
			{
				player2->moveUp();
			}
			else if (i == GLFW_KEY_DOWN)
			{
				player1->moveDown();
			}
			else if (i == GLFW_KEY_S)
			{
				player2->moveDown();
			}
		}
	}
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WIDTH, HEIGHT, "Ultimate Pong", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Register key functions
	glfwSetKeyCallback(window, registerPressedKeys);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint paddle1Program, paddle2Program, ballProgram;
	GLuint paddle1ID, paddle2ID, ballID;
	GLuint paddle1Buffer, paddle2Buffer, ballBuffer;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Freetype
	Shader shader("text.vs", "text.fs");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "arial.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//End freetype
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Font loading (gotta do in init stages, cannot modify between new_frame and end_frame
	ImGuiIO & io = ImGui::GetIO();
	ImFont* ButtonFont = io.Fonts->AddFontFromFileTTF("InconReg.ttf", 25);
	ImFont* TitleFont = io.Fonts->AddFontFromFileTTF("TitleFont.ttf", 60);

	if (state == 1)
	{
		player1 = new Paddle(true);
		player2 = new Paddle(false);
		ball = Ball(&gWindow, player1, player2);

		glGenVertexArrays(1, &paddle1ID);
		glGenVertexArrays(1, &paddle2ID);
		glGenVertexArrays(1, &ballID);

		paddle1Program = LoadShaders("paddlevertex.vs", "paddleframe.fs");
		paddle2Program = LoadShaders("paddlevertex.vs", "paddleframe.fs");
		ballProgram = LoadShaders("ballvertex.vs", "ballframe.fs");

		// 2) Create buffers
		glGenBuffers(1, &paddle1Buffer);
		glGenBuffers(1, &paddle2Buffer);
		glGenBuffers(1, &ballBuffer);

		gWindow = Window();
		gWindow.startTime();
	}
	else if (state == 0) {
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
	}

	// The program Loop
	do
	{

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		if (state == 1)
		{
			//////////////////////////////////////////////
			// Paddle Data:
			GLint paddle1PosAttrib = glGetAttribLocation(paddle1Program, "position");
			GLint paddle2PosAttrib = glGetAttribLocation(paddle2Program, "position");
			GLfloat *paddle1Vertices = player1->getBufferData();
			GLfloat *paddle2Vertices = player2->getBufferData();

			// Player 1 Paddle
			glBindVertexArray(paddle1ID);
			glBindBuffer(GL_ARRAY_BUFFER, paddle1Buffer);
			glBufferData(GL_ARRAY_BUFFER, /* Change this back to sizeof(GLfloat*) */ 32, paddle1Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(paddle1PosAttrib);
			glVertexAttribPointer(paddle1PosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glUseProgram(paddle1Program);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableVertexAttribArray(paddle1PosAttrib);

			// Player 2 Paddle
			glBindVertexArray(paddle2ID);
			glBindBuffer(GL_ARRAY_BUFFER, paddle2Buffer);
			//std::cout << sizeof(vertices[0]) << std::endl;
			glBufferData(GL_ARRAY_BUFFER, /* Change this back to sizeof(GLfloat*) */ 32, paddle2Vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(paddle2PosAttrib);
			glVertexAttribPointer(paddle2PosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glUseProgram(paddle2Program);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableVertexAttribArray(paddle2PosAttrib);

			/////////////////////////////////////////////////////////////
			// End Paddle Data

			//////////////////////////////////////////////
			// Ball Data:
			GLint ballPosAttrib = glGetAttribLocation(ballProgram, "position");
			GLfloat *ballVertices = ball.getBufferData();

			// Ball
			glBindVertexArray(ballID);
			glBindBuffer(GL_ARRAY_BUFFER, ballBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, ballVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(ballPosAttrib);
			glVertexAttribPointer(ballPosAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glUseProgram(ballProgram);

			GLint rgb = glGetUniformLocation(ballProgram, "rgb");
			glUniform4f(rgb, ball.getR(), ball.getG(), ball.getB(), 1.0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glDisableVertexAttribArray(0);

			/////////////////////////////////////////////////////////////
			// End Ball Data
			
			/////////////////////////////////////////////////////////////
			//score_line
			//Runtime
			RenderText(shader, gWindow.getTime(), 365.5f, 770.0f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
			//Score Right
			RenderText(shader, gWindow.getRight(), 730.0f, 770.0f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
			//Score Left
			RenderText(shader, gWindow.getLeft(), 50.0f, 770.0f, 0.5f, glm::vec3(1.0, 1.0f, 1.0f));

			//Dashed Line
			for (int i = 0; i < 25; i++)
			{
				RenderText(shader, "|", 400.0f, 0.0f + (i * 30), 0.5f, glm::vec3(1.0, 1.0f, 1.0f));
			}
			//End score_line
			/////////////////////////////////////////////////////////////
		}
		else if (state == 0) {
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

				state = 1;					//trying to cause entire 'if' stmt block for ImGui to no longer execute
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

		handleKeyEvent();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (state == 1)
		{
			// Move ball
			ball.move();
		}

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	if (state == 1)
	{
		// Cleanup VBO
		glDeleteBuffers(1, &paddle1Buffer);
		glDeleteBuffers(1, &paddle2Buffer);
		glDeleteVertexArrays(1, &paddle1ID);
		glDeleteVertexArrays(1, &paddle2ID);
		glDeleteProgram(paddle1Program);
		glDeleteProgram(paddle2Program);
	}

	//IMGUI cleanup (terminate, etc)
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

void RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{xpos, ypos + h, 0.0, 0.0},
			{xpos, ypos, 0.0, 1.0},
			{xpos + w, ypos, 1.0, 1.0},

			{xpos, ypos + h, 0.0, 0.0},
			{xpos + w, ypos, 1.0, 1.0},
			{xpos + w, ypos + h, 1.0, 0.0}};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}