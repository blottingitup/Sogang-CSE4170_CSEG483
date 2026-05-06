#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <deque>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
// #include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

int win_width = 0, win_height = 0;
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;


#include "objects.h"

#define LEVEL1_LEN 300.0f
#define TRAIL_MAX_SIZE 100  // max size of trail deque

int leftbuttonpressed = 0;
int prev_x, prev_y;
int level1 = 1, level4 = 1;  // Both level 1 and 4 are activated at init
unsigned int timestamp = 0, one_clock = 0, four_clock = 0;

// Speed of revolution and rotation for some levels
float rev1 = 0.8f;
float rev2 = 0.8f;
float rev3 = 1.2f;
float rev4 = 2.0f;
float rot2 = 6.0f;
float rot3 = 4.0f;
float rot4 = 6.0f;

float scale3 = 1.0f;
float scaling3[2] = { 1.01f, 0.99f };
int scale3_idx = 0;

// Save trail of level 4 objects
std::deque<glm::mat4> trail4;
#define TRAIL_MAX_SIZE 100

void timer(int value) {
	timestamp = (timestamp + 1) % UINT_MAX;
	if (level1) one_clock = (one_clock + 1) % UINT_MAX;  // Clock for level 1
	if (level4) four_clock = (four_clock + 1) % UINT_MAX;  // Clock for level 4

	// Scaling at level3: max 2.4x, min 0.8x
	if (scale3 < 0.8f) scale3_idx = 0;
	else if (scale3 > 2.4f) scale3_idx = 1;
	scale3 *= scaling3[scale3_idx];

	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

/*
* level 0: house
* level 1: car
* level 2: book
* level 3: shield
* level 4: sword
*/

void display(void) {
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	// 0. Draw house in the center of the axes, and move along the x-axis with leftmousebutton pressed
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(centerx, 0.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_house(); // in MC

	// 1. Car(4): Revolution in diamond orbit, no rotation
	for (int i = 0; i < 4; i++) {
		float dir[4][2] = { {-1.0f, 1.0f}, {-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f} };
		int temp_lane = (int)(rev1 * one_clock / LEVEL1_LEN) % 4;  // Lane 0, 1, 2, 3 calculated by total moving time
		int actual_lane = (temp_lane + i) % 4;  // Lane 0, 1, 2, 3 calculated considering the starting position
		float cur_pos = rev1 * one_clock - (int)(rev1 * one_clock / LEVEL1_LEN) * LEVEL1_LEN;  // Position in lane
		float car_translatex = LEVEL1_LEN * cosf(actual_lane * 90.0f * TO_RADIAN) + dir[actual_lane][0] * cur_pos + centerx;
		float car_translatey = LEVEL1_LEN * sinf(actual_lane * 90.0f * TO_RADIAN) + dir[actual_lane][1] * cur_pos;

		// T(centerx, 0.0f, 0.0f)*T(move to position in diamond)*R(i*90deg)*T(LEVEL1_LEN, 0.0f, 0.0f)*R(-45deg)
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car_translatex, car_translatey, 0.0f));

		// Rotation in direction of movement should only be done in level 1
		glm::mat4 CarMatrix = ModelMatrix;
		CarMatrix = glm::rotate(CarMatrix, (actual_lane * 90.0f - 45.0f) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * CarMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_car();

		// 2. Book(1): Revolution in elliptic orbit with rotation(anti-clockwise) + sinewave
		float wave2 = 20.0f * sinf(10.0f * timestamp * TO_RADIAN);

		glm::mat4 BookMatrix = ModelMatrix;
		BookMatrix = glm::rotate(BookMatrix, 45.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		BookMatrix = glm::translate(BookMatrix, glm::vec3((wave2 + 120.0f) * cosf((rev2 * timestamp) * TO_RADIAN),
			(wave2 + 60.0f) * sinf((rev2 * timestamp) * TO_RADIAN), 0.0f));

		/* BookMatrix = glm::rotate(BookMatrix, 45.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		BookMatrix = glm::translate(BookMatrix, glm::vec3((wave2 + 120.0f) * cosf((rev2 * timestamp) * TO_RADIAN),
			(wave2 + 60.0f) * sinf((rev2 * timestamp) * TO_RADIAN), 0.0f));
		BookMatrix = glm::rotate(BookMatrix, rot2 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f)); */

		// Rotation should only done in level 2
		glm::mat4 BookRotateMatrix = glm::rotate(BookMatrix, rot2 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * BookRotateMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_book();

		// 3. Shield(1): Revolution in circular orbit with rotation(clockwise) and scaling
		glm::mat4 ShieldMatrix = BookMatrix;
		ShieldMatrix = glm::rotate(ShieldMatrix, rev3 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ShieldMatrix = glm::translate(ShieldMatrix, glm::vec3(50.0f, 0.0f, 0.0f));

		/* ShieldMatrix = glm::rotate(ShieldMatrix, rev3 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ShieldMatrix = glm::translate(ShieldMatrix, glm::vec3(50.0f, 0.0f, 0.0f));
		ShieldMatrix = glm::rotate(ShieldMatrix, -rot3 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f)); */

		// Scaling and rotation should only done in level 3
		glm::mat4 ShieldScaleMatrix = glm::rotate(ShieldMatrix, -rot3 * timestamp * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		ShieldScaleMatrix = glm::scale(ShieldScaleMatrix, glm::vec3(scale3, scale3, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ShieldScaleMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_shield();

		// 4. Sword(3): Revolution in circular orbit with rotation(anti-clockwise), each sword having different orbital radius
		for (int j = 0; j < 3; j++) {
			float rev_angle4 = (rev4 * (float)(3 - j) * four_clock + j * 60.0f) * TO_RADIAN;
			glm::mat4 SwordMatrix = ShieldMatrix;
			SwordMatrix = glm::translate(SwordMatrix, glm::vec3(cosf(rev_angle4) * (j + 1) * 50.0f, sinf(rev_angle4) * (j + 1) * 50.0f, 0.0f));
			SwordMatrix = glm::rotate(SwordMatrix, (rot4 * four_clock + rev4 * (float)(3 - j) * four_clock + j * 60.0f) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));

			/* SwordMatrix = glm::rotate(SwordMatrix, (rev4 * (float)(3 - j) * four_clock + j * 60.0f) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			SwordMatrix = glm::translate(SwordMatrix, glm::vec3((j + 1) * 50.0f, 0.0f, 0.0f));
			SwordMatrix = glm::rotate(SwordMatrix, rot4 * four_clock * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));*/

			ModelViewProjectionMatrix = ViewProjectionMatrix * SwordMatrix;
			glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
			draw_sword(0, 1);

			// Create trail of level 4 objects
			if (trail4.size() >= TRAIL_MAX_SIZE) trail4.pop_back();
			if (level4) trail4.push_front(SwordMatrix);
			for (int k = 1; k < trail4.size(); k++) {
				float scale4trail = 1.0f - 1.0f * k / TRAIL_MAX_SIZE;
				glm::mat4 NewSwordMatrix = glm::scale(trail4[k], glm::vec3(scale4trail, scale4trail, 1.0f));
				ModelViewProjectionMatrix = ViewProjectionMatrix * NewSwordMatrix;
				glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
				draw_sword(1, k);
			}
		}
	}

	glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'F':
		case 'f':
			level1 = 1 - level1;
			glutPostRedisplay();
			break;
		case 'G':
		case 'g':
			level4 = 1 - level4;
			if (!level4) trail4.clear();
			glutPostRedisplay();
			break;
		case 27: // ESC key
			glutLeaveMainLoop(); // Incur destuction callback for cleanups.
			break;
	}
}

void mouse(int button, int state, int x, int y) {
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		leftbuttonpressed = 1;
		prev_x = x, prev_y = y;
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}

void motion(int x, int y) {
	if (leftbuttonpressed) {
		centerx = x - win_width / 2.0f;
		centerx = centerx < win_width / 2.5f ? centerx : win_width / 2.5f;
		centerx = centerx > -win_width / 2.5f ? centerx : -win_width / 2.5f;
		// centery = (win_height - y) - win_height/2.0f;
		glutPostRedisplay();
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	update_axes();
	update_line();

	glutPostRedisplay();
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);

	glDeleteVertexArrays(1, &VAO_line);
	glDeleteBuffers(1, &VBO_line);

	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);

	glDeleteVertexArrays(1, &VAO_car);
	glDeleteBuffers(1, &VBO_car);

	glDeleteVertexArrays(1, &VAO_book);
	glDeleteBuffers(1, &VBO_book);

	glDeleteVertexArrays(1, &VAO_shield);
	glDeleteBuffers(1, &VBO_shield);

	glDeleteVertexArrays(1, &VAO_sword);
	glDeleteBuffers(1, &VBO_sword);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glClearColor(44 / 255.0f, 180 / 255.0f, 49 / 255.0f, 1.0f);
	glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_line();
	prepare_house();
	prepare_car();
	prepare_book();
	prepare_shield();
	prepare_sword();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n");
	fprintf(stdout, "  Modified by SeJoon Kim for Sogang CSE4170 HW2\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 2DObjects_GLSL_3.0.1.3_HW2";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'f', 'g', 'ESC'"
		"    - Mouse used: L-click and move"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}