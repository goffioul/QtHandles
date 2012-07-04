/*

Copyright (C) 2011 Michael Goffioul.

This file is part of QtHandles.

Foobar is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

QtHandles is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <octave/config.h>
#include "gl-select.h"

#include <iostream>

void
opengl_selector::apply_pick_matrix (void)
{
  GLdouble p_matrix[16];
  GLint viewport[4];

  glGetDoublev (GL_PROJECTION_MATRIX, p_matrix);
  glGetIntegerv (GL_VIEWPORT, viewport);
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  gluPickMatrix (xp, yp, size, size, viewport);
  glMultMatrixd (p_matrix);
  glMatrixMode (GL_MODELVIEW);
}

void
opengl_selector::setup_opengl_transformation (const axes::properties& props)
{
  opengl_renderer::setup_opengl_transformation (props);
  apply_pick_matrix ();
}

void
opengl_selector::init_marker (const std::string& m, double size, float width)  
{
  opengl_renderer::init_marker (m, size, width);
  apply_pick_matrix ();
}

# define BUFFER_SIZE 128

graphics_object
opengl_selector::select (const graphics_object& ax, int x, int y)
{
  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);

  xp = x;
  yp = y;

  GLuint select_buffer[BUFFER_SIZE];

  glSelectBuffer (BUFFER_SIZE, select_buffer);
  glRenderMode (GL_SELECT);
  glInitNames ();

  object_map.clear ();

  draw (ax);

  int hits = glRenderMode (GL_RENDER);
  graphics_object obj;

  if (hits > 0)
    {
      GLuint current_minZ = 0xffffffff;
      GLuint current_name = 0xffffffff;

      for (int i = 0, j = 0; i < hits && j < BUFFER_SIZE-3; i++)
        {
          GLuint n = select_buffer[j++],
                 minZ = select_buffer[j++];

          j++; // skip maxZ
          if (minZ < current_minZ)
            {
              current_minZ = minZ;
              // TODO: scan name stack up to first object with invisible
              //       handle? for the time being, pick up the top-most name
              //       in the stack
              current_name =
                select_buffer[std::min (j + n, GLuint (BUFFER_SIZE)) - 1];
              j += n;
            }
          else
            j += n;
        }

      if (current_name != 0xffffffff)
        obj = object_map[current_name];
    }
  else if (hits < 0)
    warning ("opengl_selector::select: selection buffer overflow");

  object_map.clear ();

  return obj;
}

void
opengl_selector::draw (const graphics_object& go, bool toplevel)
{
  GLuint name = object_map.size ();

  object_map[name] = go;
  glPushName (name);
  opengl_renderer::draw (go, toplevel);
  glPopName ();
}

void
opengl_selector::draw_text (const text::properties& props)
{
  if (props.get_string ().is_empty ())
    return;

  Matrix pos = props.get_data_position ();
  const Matrix bbox = props.get_extent_matrix ();

  ColumnVector xpos, xp1, xp2;

  xpos = get_transform ().transform (pos(0), pos(1),
                                     pos.numel () > 2 ? pos(2) : 0.0);

  xp1 = xp2 = xpos;
  xp1(0) += bbox(0);
  xp1(1) -= bbox(1);
  xp2(0) += (bbox(0) + bbox(2));
  xp2(1) -= (bbox(1) + bbox(3));

  ColumnVector p1, p2, p3, p4;

  p1 = get_transform ().untransform (xp1(0), xp1(1), xp1(2), false);
  p2 = get_transform ().untransform (xp2(0), xp1(1), xp1(2), false);
  p3 = get_transform ().untransform (xp2(0), xp2(1), xp1(2), false);
  p4 = get_transform ().untransform (xp1(0), xp2(1), xp1(2), false);

  glBegin (GL_QUADS);
  glVertex3dv (p1.data ());
  glVertex3dv (p2.data ());
  glVertex3dv (p3.data ());
  glVertex3dv (p4.data ());
  glEnd ();
}
