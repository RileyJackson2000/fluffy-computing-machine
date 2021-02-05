#include <cmath>
#include <iostream>
#include <vector>

#include <render/render2d.hpp>
#include <utils/constants.hpp>
#include <utils/utils.hpp>
#include <utils/glew.hpp>
#include <render/vertexBuffer.hpp>
#include <render/vertexBufferLayout.hpp>
#include <render/vertexArray.hpp>

namespace fcm {


void drawCircle(double x, double y, double radius)
{ drawCircle(x, y, radius, 20); }

void drawCircle(double x, double y, double radius, int sides)
{
  glew::GLfloat *v_buffer = new glew::GLfloat[sides * 3];
  for (int i = 0; i < sides; ++i)
  {
    double heading = 2. * pi * i / sides;
    v_buffer[3 * i] = std::cos(heading) * radius + x;
    v_buffer[3 * i] *= 50. / WINDOW_WIDTH; // TODO hack until we get a camera set up to convert world coords to camera coords
    v_buffer[3 * i + 1] = std::sin(heading) * radius + y;
    v_buffer[3 * i + 1] *= 50. / WINDOW_HEIGHT;
    v_buffer[3 * i + 2] = 0.;
  }

  VertexArray va;
  VertexBuffer vb{v_buffer, sides * 3 * sizeof(glew::GLfloat)};

  VertexBufferLayout layout;
  layout.addElem<glew::GLfloat>(3);

  va.addVertexBuffer(vb, layout);
  va.bind();

  glew::glDrawArrays(GL_TRIANGLE_FAN, 0, sides);

  delete[] v_buffer;
}

}
