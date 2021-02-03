#include <cmath>
#include <iostream>
#include <vector>

#include <render/render2d.hpp>
#include <utils/constants.hpp>
#include <utils/glew.hpp>
#include <utils/utils.hpp>

namespace fcm {

void drawCircle(double x, double y, double radius) {
  drawCircle(x, y, radius, 20);
}

void drawCircle(double x, double y, double radius, int sides) {
  glew::GLfloat *v_buffer = new glew::GLfloat[sides * 3];
  for (int i = 0; i < sides; ++i) {
    double heading = 2. * pi * i / sides;
    v_buffer[3 * i] = std::cos(heading) * radius + x;
    v_buffer[3 * i] *=
        50. / WINDOW_WIDTH; // TODO hack until we get a camera set up to convert
                            // world coords to camera coords
    v_buffer[3 * i + 1] = std::sin(heading) * radius + y;
    v_buffer[3 * i + 1] *= 50. / WINDOW_HEIGHT;
    v_buffer[3 * i + 2] = 0.;
  }

  glew::GLuint vertexbuffer;
  glew::glGenBuffers(1, &vertexbuffer);
  glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glew::glBufferData(GL_ARRAY_BUFFER, sides * 3 * sizeof(glew::GLfloat),
                     v_buffer, GL_STATIC_DRAW);

  glew::glEnableVertexAttribArray(0);
  glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

  glew::glVertexAttribPointer(0, // attribute 0. No particular reason for 0, but
                                 // must match the layout in the shader.
                              3, // size
                              GL_FLOAT, // type
                              GL_FALSE, // normalized?
                              0,        // stride
                              (void *)0 // array buffer offset
  );
  glew::glDrawArrays(GL_TRIANGLE_FAN, 0, sides);

  glew::glDisableVertexAttribArray(0);
  delete[] v_buffer;
  glew::glDeleteBuffers(1, &vertexbuffer);
}

void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
  glew::GLfloat *v_buffer = new glew::GLfloat[3 * 3];
  v_buffer[0] = x1;
  v_buffer[1] = y1;
  v_buffer[2] = 0;
  v_buffer[3] = x2;
  v_buffer[4] = y2;
  v_buffer[5] = 0;
  v_buffer[6] = x3;
  v_buffer[7] = y3;
  v_buffer[8] = 0;

  glew::GLuint vertexbuffer;
  glew::glGenBuffers(1, &vertexbuffer);
  glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glew::glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(glew::GLfloat), v_buffer, GL_STATIC_DRAW);

  glew::glEnableVertexAttribArray(0);
  glew::glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

  glew::glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );
  glew::glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

  glew::glDisableVertexAttribArray(0);
  delete[] v_buffer;
  glew::glDeleteBuffers(1, &vertexbuffer);

}

} // namespace fcm
