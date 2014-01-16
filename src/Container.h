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

#ifndef __QtHandles_Container__
#define __QtHandles_Container__ 1

#include <QWidget>

#include "GenericEventNotify.h"

class octave_handle;
typedef octave_handle graphics_handle;

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

DECLARE_GENERICEVENTNOTIFY_SENDER(ContainerBase, QWidget);

class Canvas;

class Container : public ContainerBase
{
public:
  Container (QWidget* parent);
  ~Container (void);

  Canvas* canvas (const graphics_handle& handle, bool create = true);

protected:
  void resizeEvent (QResizeEvent* event);

private:
  Canvas* m_canvas;
};

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

#endif
