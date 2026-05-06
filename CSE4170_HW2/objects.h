#pragma once
GLfloat axes[4][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

#define TRAIL_MAX_SIZE 100

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.5f; axes[0][1] = 0.0f;
	axes[1][0] = win_width / 2.5f; axes[1][1] = 0.0f;
	axes[2][0] = 0.0f; axes[2][1] = -win_height / 2.5f;
	axes[3][0] = 0.0f; axes[3][1] = win_height / 2.5f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.25f; axes[1][0] = win_width / 2.25f;
	axes[2][1] = -win_height / 2.25f;
	axes[3][1] = win_height / 2.25f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 4);
	glBindVertexArray(0);
}

GLfloat line[2][2];
GLfloat line_color[3] = { 1.0f, 0.0f, 0.0f };
GLuint VBO_line, VAO_line;

void prepare_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_line);
	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_line(void) { 	// y = x - win_height/4
	line[0][0] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height;
	line[0][1] = (1.0f / 4.0f - 1.0f / 2.5f) * win_height - win_height / 4.0f;
	line[1][0] = win_width / 2.5f;
	line[1][1] = win_width / 2.5f - win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_line(void) { // Draw line in its MC.
	// y = x - win_height/4
	glUniform3fv(loc_primitive_color, 1, line_color);
	glBindVertexArray(VAO_line);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
	{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
	{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel) + sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}

// sword
#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_sword(bool is_trail, int ratio) {
	// When level4 is moving, trail goes from green to blue
	GLfloat new_color[7][3];

	if (is_trail) {
		for (int i = 0; i < 7; i++) {
			new_color[i][0] = 0.5f * sword_color[i][0];
			new_color[i][1] = (1.0f - 1.0f * ratio / TRAIL_MAX_SIZE) * sword_color[i][1];
			new_color[i][2] = (1.0f * ratio / TRAIL_MAX_SIZE) * sword_color[i][2];
		}
	}
	else {
		for (int i = 0; i < 7; i++) {
			new_color[i][0] = sword_color[i][0];
			new_color[i][1] = sword_color[i][1];
			new_color[i][2] = sword_color[i][2];
		}
	}

	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}

// shield
#define SHIELD_BODY 0
#define SHIELD_HEAD1 1
#define SHIELD_HEAD2 2
#define SHIELD_BOTTOM 3
#define SHIELD_MARK 4
#define SHIELD_LINE 5
GLfloat shield_body[4][2] = { { 8.0, 8.0 }, { -8.0, 8.0 }, { -8.0, -8.0 }, { 8.0, -8.0 } };
GLfloat shield_head1[3][2] = { { 8.0, 8.0 }, { 8.0, 10.0 }, { 6.0, 8.0 } };
GLfloat shield_head2[3][2] = { { -8.0, 8.0 }, { -8.0, 10.0 }, { -6.0, 8.0 } };
GLfloat shield_bottom[3][2] = { { -8.0, -8.0 }, { 0.0, -12.0 }, { 8.0, -8.0 } };
GLfloat shield_mark[4][2] = { { 4.0, 0.0 }, { 0.0, 4.0 }, { -4.0, 0.0 }, { 0.0, -4.0 } };
GLfloat shield_line[6][2] = { { 2.0, 2.0 }, { -2.0, 2.0 }, { -2.0, 0.0 }, { 2.0, -0.0 }, { 2.0, -2.0 }, { -2.0, -2.0 } };
GLfloat shield_color[6][3] = {
	{ 81 / 255.0f, 57 / 255.0f, 40 / 255.0f },  // shield body
	{ 97 / 255.0f, 12 / 255.0f, 4 / 255.0f },  // shield head1
	{ 97 / 255.0f, 12 / 255.0f, 4 / 255.0f },  // shield head2
	{ 35 / 255.0f, 79 / 255.0f, 18 / 255.0f },  // shield bottom
	{ 1.0f, 0.0f, 77 / 255.0f },  // shield mark
	{ 35 / 255.0f, 79 / 255.0f, 18 / 255.0f }  // shield line
};

GLuint VBO_shield, VAO_shield;
void prepare_shield() {
	GLsizeiptr buffer_size = sizeof(shield_body) + sizeof(shield_head1) + sizeof(shield_head2) + sizeof(shield_bottom)
		+ sizeof(shield_mark) + sizeof(shield_line);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_shield);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shield);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shield_body), shield_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(shield_body), sizeof(shield_head1), shield_head1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(shield_body) + sizeof(shield_head1), sizeof(shield_head2), shield_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(shield_body) + sizeof(shield_head1) + sizeof(shield_head2), sizeof(shield_bottom),
		shield_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(shield_body) + sizeof(shield_head1) + sizeof(shield_head2) + sizeof(shield_bottom),
		sizeof(shield_mark), shield_mark);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(shield_body) + sizeof(shield_head1) + sizeof(shield_head2) + sizeof(shield_bottom)
		+ sizeof(shield_mark), sizeof(shield_line), shield_line);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_shield);
	glBindVertexArray(VAO_shield);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_shield);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_shield() {
	glBindVertexArray(VAO_shield);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_HEAD1]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 3);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 3);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 10, 3);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_MARK]);
	glDrawArrays(GL_TRIANGLE_FAN, 13, 4);

	glUniform3fv(loc_primitive_color, 1, shield_color[SHIELD_LINE]);
	glDrawArrays(GL_LINE_STRIP, 17, 6);

	glBindVertexArray(0);
}

// book
#define BOOK_BODY 0
#define BOOK_TOP 1
#define BOOK_SIDE 2
#define BOOK_PAGE1 3
#define BOOK_PAGE2 4
#define BOOK_MARK 5
GLfloat book_body[4][2] = { { 5.0, 10.0 }, { -5.0, 10.0 }, { -5.0, -10.0 }, { 5.0, -10.0 } };
GLfloat book_top[4][2] = { { 5.0, 10.0 }, { 11.0, 16.0 }, { 1.0, 16.0 }, { -5.0, 10.0} };
GLfloat book_side[4][2] = { { 11.0, 16.0 }, { 5.0, 10.0 }, { 5.0, -10.0 }, { 11.0, -4.0 } };
GLfloat book_page1[3][2] = { { 7.0, -8.0 }, { 7.0, 12.0 }, { -3.0, 12.0 } };
GLfloat book_page2[3][2] = { { 9.0, -6.0 }, { 9.0, 14.0 }, { -1.0, 14.0 } };
GLfloat book_mark[8][2] = { { 0.0, 2.0 }, { 2.0, 2.0 }, { 4.0, 0.0 }, { 1.0, -2.0 }, { 0.0, -4.0 }, { -1.0, -2.0 }, { -4.0, 0.0 }, { -2.0, 2.0 } };
GLfloat book_color[6][3] = {
	{ 40 / 255.0f, 35 / 255.0f, 29 / 255.0f },  // book body
	{ 245 / 255.0f, 245 / 255.0f, 245 / 255.0f },  // book top
	{ 245 / 255.0f, 245 / 255.0f, 245 / 255.0f },  // book side
	{ 0.0f, 128 / 255.0f, 254 / 255.0f },  // book page1
	{ 0.0f, 128 / 255.0f, 254 / 255.0f },  // book page2
	{ 1.0f, 0.0f, 77 / 255.0f }  // book mark
};

GLuint VBO_book, VAO_book;
void prepare_book() {
	GLsizeiptr buffer_size = sizeof(book_body) + sizeof(book_top) + sizeof(book_side) + sizeof(book_page1)
		+ sizeof(book_page2) + sizeof(book_mark);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_book);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_book);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(book_body), book_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(book_body), sizeof(book_top), book_top);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(book_body) + sizeof(book_top), sizeof(book_side), book_side);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(book_body) + sizeof(book_top) + sizeof(book_side), sizeof(book_page1),
		book_page1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(book_body) + sizeof(book_top) + sizeof(book_side) + sizeof(book_page1),
		sizeof(book_page2), book_page2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(book_body) + sizeof(book_top) + sizeof(book_side) + sizeof(book_page1)
		+ sizeof(book_page2), sizeof(book_mark), book_mark);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_book);
	glBindVertexArray(VAO_book);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_book);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_book() {
	glBindVertexArray(VAO_book);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_TOP]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_SIDE]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glLineWidth(0.5f);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_PAGE1]);
	glDrawArrays(GL_LINE_STRIP, 12, 3);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_PAGE2]);
	glDrawArrays(GL_LINE_STRIP, 15, 3);

	glUniform3fv(loc_primitive_color, 1, book_color[BOOK_MARK]);
	glDrawArrays(GL_LINE_STRIP, 18, 8);

	glBindVertexArray(0);
}