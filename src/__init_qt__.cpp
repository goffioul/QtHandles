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

#include <QApplication>
#include <QMetaType>
#include <QPalette>

#include <octave/oct.h>
#include <octave/graphics.h>
#include <octave/toplev.h>

#include "Backend.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

static bool qtHandlesInitialized = false;

//////////////////////////////////////////////////////////////////////////////

bool __init__ (void)
{
  if (! qtHandlesInitialized)
    {
      if (qApp)
	{
	  qRegisterMetaType<graphics_object> ("graphics_object");

	  gh_manager::enable_event_processing (true);

	  graphics_toolkit tk (new Backend ());
	  graphics_toolkit::register_toolkit (tk);

	  octave_add_atexit_function ("__shutdown_qt__");

	  // Change some default settings to use Qt default colors
	  QPalette p;
	  graphics_object root = gh_manager::get_object (0);

	  /*
	  root.set ("defaultfigurecolor",
		    octave_value (Utils::toRgb (p.color (QPalette::Window))));
	  */
	  root.set ("defaultuicontrolbackgroundcolor",
		    octave_value (Utils::toRgb (p.color (QPalette::Window))));
	  root.set ("defaultuicontrolforegroundcolor",
		    octave_value (Utils::toRgb
				  (p.color (QPalette::WindowText))));
	  root.set ("defaultuipanelbackgroundcolor",
		    octave_value (Utils::toRgb (p.color (QPalette::Window))));
	  root.set ("defaultuipanelforegroundcolor",
		    octave_value (Utils::toRgb
				  (p.color (QPalette::WindowText))));
	  root.set ("defaultuipanelhighlightcolor",
		    octave_value (Utils::toRgb (p.color (QPalette::Light))));
	  root.set ("defaultuipanelshadowcolor",
		    octave_value (Utils::toRgb (p.color (QPalette::Dark))));

	  qtHandlesInitialized = true;

	  return true;
	}
      else
	error ("__init_qt__: QApplication object must exist.");
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

bool __shutdown__ (void)
{
  if (qtHandlesInitialized)
    {
      octave_add_atexit_function ("__shutdown_qt__");

      graphics_toolkit::unregister_toolkit ("qt");

      gh_manager::enable_event_processing (false);

      qtHandlesInitialized = false;

      return true;
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles

//////////////////////////////////////////////////////////////////////////////

DEFUN_DLD (__init_qt__, , , "")
{
  if (QtHandles::__init__ ())
    mlock ();

  return octave_value ();
}

//////////////////////////////////////////////////////////////////////////////

DEFUN_DLD (__shutdown_qt__, , , "")
{
  if (QtHandles::__shutdown__ ())
    munlock ("__init_qt__");

  return octave_value ();
}
