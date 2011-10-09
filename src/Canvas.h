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

#ifndef __QtHandles_Canvas__
#define __QtHandles_Canvas__ 1

#include <QPoint>

#include <octave/oct.h>
#include <octave/graphics.h>

#include "Figure.h"

class QMouseEvent;
class QWidget;

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

class Canvas
{
public:
  virtual ~Canvas (void) { }

  void redraw (bool sync = false);
  void blockRedraw (bool block = true);

  virtual QWidget* qWidget (void) = 0;

  static Canvas* create (const std::string& name, QWidget* parent,
			 const graphics_handle& handle);

protected:
  virtual void draw (const graphics_handle& handle) = 0;
  virtual void drawZoomBox (const QPoint& p1, const QPoint& p2) = 0;
  virtual void resize (int x, int y, int width, int height) = 0;

protected:
  Canvas (const graphics_handle& handle)
    : m_handle (handle),
      m_redrawBlocked (false),
      m_mouseMode (NoMode)
    { }

  void canvasPaintEvent (void);
  void canvasMouseMoveEvent (QMouseEvent* event);
  void canvasMousePressEvent (QMouseEvent* event);
  void canvasMouseReleaseEvent (QMouseEvent* event);

private:
  graphics_handle m_handle;
  bool m_redrawBlocked;
  MouseMode m_mouseMode;
  QPoint m_mouseAnchor;
  QPoint m_mouseCurrent;
  graphics_handle m_mouseAxes;
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
