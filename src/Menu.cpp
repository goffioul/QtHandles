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

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

#include "Menu.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

Menu* Menu::create (const graphics_object& go)
{
  Object* parent = Object::parentObject (go);

  if (parent)
    {
      QObject* qObj = parent->qObject ();

      if (qObj)
	return new Menu (go, new QAction (qObj), parent);
    }

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

Menu::Menu (const graphics_object& go, QAction* action, Object* parent)
    : Object (go, action)
{
  uimenu::properties& up = properties<uimenu> ();

  action->setText (Utils::fromStdString (up.get_label ()));
  if (parent->object ().isa ("figure"))
    parent->qWidget<QMainWindow> ()->menuBar ()->addAction (action);
  else
    {
      Menu* parentMenu = dynamic_cast<Menu*> (parent);

      if (parentMenu)
	parentMenu->menu ()->addAction (action);
    }

  connect (action, SIGNAL (triggered (bool)), SLOT (actionTriggered (bool)));
}

//////////////////////////////////////////////////////////////////////////////

Menu::~Menu (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Menu::update (int pId)
{
  uimenu::properties& up = properties<uimenu> ();
  QAction* action = qWidget<QAction> ();

  switch (pId)
    {
    case uimenu::properties::ID_LABEL:
      action->setText (Utils::fromStdString (up.get_label ()));
      break;
    default:
      Object::update (pId);
      break;
    }
}

//////////////////////////////////////////////////////////////////////////////

QMenu* Menu::menu (void)
{
  QAction* action = qWidget<QAction> ();
  QMenu* _menu = action->menu ();

  if (! _menu)
    {
      _menu = new QMenu (action->parentWidget ());
      action->setMenu (_menu);
    }

  return _menu;
}

//////////////////////////////////////////////////////////////////////////////

void Menu::actionTriggered (bool checked)
{
  gh_manager::post_callback (m_handle, "callback");
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
