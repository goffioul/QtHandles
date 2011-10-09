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
#include <QActionGroup>
#include <QApplication>
#include <QEvent>
#include <QFrame>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>

#include "Canvas.h"
#include "Container.h"
#include "Figure.h"
#include "FigureWindow.h"
#include "MouseModeActionGroup.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

#define ABOUT_TEXT "<b>QtHandles</b> - a Qt-based toolkit for <a href=\"http://www.octave.org\">Octave</a>.<br><br>Copyright (C) 2011 Michael Goffioul."

//////////////////////////////////////////////////////////////////////////////

static bool hasUiControlChildren (const figure::properties& fp)
{
  Matrix kids = fp.get_all_children ();

  for (int i = 0; i < kids.numel (); i++)
    {
      graphics_object go (gh_manager::get_object (kids(i)));

      if (go && (go.isa ("uicontrol") || go.isa ("uipanel")
		 || go.isa ("uibuttongroup")))
	return true;
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

Figure* Figure::create (const graphics_object& go)
{
  return new Figure (go, new FigureWindow ());
}

//////////////////////////////////////////////////////////////////////////////

Figure::Figure (const graphics_object& go, QMainWindow* win)
     : Object (go, win), m_blockUpdates (false), m_mouseMode (NoMode),
       m_lastMouseMode (NoMode), m_figureToolBar (0)
{
  m_container = new Container (win);
  win->setCentralWidget (m_container);

  figure::properties& fp = properties<figure> ();

  createFigureToolBar ();
  createMenuBar ();

  int offset = m_figureToolBar->sizeHint ().height ();
  if ((fp.toolbar_is ("auto") && hasUiControlChildren (fp))
      || fp.toolbar_is ("none"))
    {
      m_figureToolBar->hide ();
      offset = 0;
    }
  offset += win->menuBar ()->sizeHint ().height () + 1;

  Matrix bb = fp.get_boundingbox ();
  win->setGeometry (xround (bb(0)), xround (bb(1)) - offset,
		    xround (bb(2)), xround (bb(3)) + offset);
  win->setWindowTitle (Utils::fromStdString (fp.get_title ()));

  if (fp.is_visible ())
    QTimer::singleShot (0, win, SLOT (show ()));
  else
    win->hide ();

  win->installEventFilter (this);
  m_container->installEventFilter (this);
}

//////////////////////////////////////////////////////////////////////////////

Figure::~Figure (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Figure::createFigureToolBar (void)
{
  QMainWindow* win = qWidget<QMainWindow> ();

  m_figureToolBar = win->addToolBar (tr ("Figure ToolBar"));
  m_figureToolBar->setMovable (false);
  m_figureToolBar->setFloatable (false);

  MouseModeActionGroup* mouseModeGroup = new MouseModeActionGroup (win);
  connect (mouseModeGroup, SIGNAL (modeChanged (MouseMode)),
	   SLOT (setMouseMode (MouseMode)));
  m_figureToolBar->addActions (mouseModeGroup->actions ());
}

//////////////////////////////////////////////////////////////////////////////

void Figure::createMenuBar (void)
{
  QMainWindow* win = qWidget<QMainWindow> ();
  QMenuBar* menuBar = win->menuBar ();

  QMenu* helpMenu = menuBar->addMenu (tr ("&Help"));
  connect (helpMenu->addAction (tr ("About QtHandles")),
	   SIGNAL (triggered (void)), this, SLOT (aboutQtHandles (void)));
  connect (helpMenu->addAction (tr ("About Qt")), SIGNAL (triggered (void)),
	   qApp, SLOT (aboutQt (void)));
}

//////////////////////////////////////////////////////////////////////////////

Container* Figure::innerContainer (void)
{
  return m_container;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::redraw (void)
{
  Canvas* canvas = m_container->canvas (m_handle);

  if (canvas)
    {
    canvas->redraw ();
    //canvas->setMouseMode (RotateMode);
    }

  foreach (QFrame* frame,
	   qWidget<QWidget> ()->findChildren<QFrame*> ("UIPanel"))
    {
      Object* obj = Object::fromQObject (frame);

      if (obj)
	obj->slotRedraw ();
    }
}

//////////////////////////////////////////////////////////////////////////////

void Figure::update (int pId)
{
  if (m_blockUpdates)
    return;

  figure::properties& fp = properties<figure> ();
  QMainWindow* win = qWidget<QMainWindow> ();

  m_blockUpdates = true;

  switch (pId)
    {
    case base_properties::ID_BEINGDELETED:
      if (fp.is_beingdeleted ())
	{
	  Canvas* canvas = m_container->canvas (m_handle.value (), false);

	  if (canvas)
	    canvas->blockRedraw (true);
	}
      break;
    case figure::properties::ID_POSITION:
	{
	  Matrix bb = fp.get_boundingbox ();
	  int offset = 0;

	  if (m_figureToolBar->isVisible ())
	    offset = m_figureToolBar->sizeHint ().height ();
	  if (win->menuBar ()->isVisible ())
	    offset += win->menuBar ()->sizeHint ().height () + 1;
	  win->setGeometry (xround (bb(0)), xround (bb(1)) - offset,
			    xround (bb(2)), xround (bb(3)) + offset);
	}
      break;
    case figure::properties::ID_NAME:
    case figure::properties::ID_NUMBERTITLE:
      win->setWindowTitle (Utils::fromStdString (fp.get_title ()));
      break;
    case figure::properties::ID_VISIBLE:
      if (fp.is_visible ())
	QTimer::singleShot (0, win, SLOT (show ()));
      else
	win->hide ();
      break;
    case figure::properties::ID_TOOLBAR:
      if (fp.toolbar_is ("none"))
	showFigureToolBar (false);
      else if (fp.toolbar_is ("figure"))
	showFigureToolBar (true);
      else // "auto"
	showFigureToolBar (! hasUiControlChildren (fp));
      break;
    default:
      break;
    }

  m_blockUpdates = false;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::showFigureToolBar (bool visible)
{
  if (m_figureToolBar->isVisible () != visible)
    {
      int dy = m_figureToolBar->sizeHint ().height ();
      QRect r = qWidget<QWidget> ()->geometry ();

      if (! visible)
	r.adjust (0, dy, 0, 0);
      else
	r.adjust (0, -dy, 0, 0);

      m_blockUpdates = true;
      qWidget<QWidget> ()->setGeometry (r);
      m_figureToolBar->setVisible (visible);
      m_blockUpdates = false;

      if (visible)
	m_mouseMode = m_lastMouseMode;
      else
	{
	  m_lastMouseMode = m_mouseMode;
	  m_mouseMode = NoMode;
	}
    }
}

//////////////////////////////////////////////////////////////////////////////

void Figure::updateBoundingBox (void)
{
  QWidget* win = qWidget<QWidget> ();

  if (win->isVisible ())
    {
      gh_manager::auto_lock lock;
      graphics_object fig = object ();

      if (fig.valid_object ())
	{
	  figure::properties& fp =
	    Utils::properties<figure> (fig);

	  Matrix bb (1, 4);
	  QPoint pt = win->mapToGlobal (m_container->pos ());

	  bb(0) = pt.x ();
	  bb(1) = pt.y ();
	  bb(2) = m_container->width ();
	  bb(3) = m_container->height ();

	  m_blockUpdates = true;

	  fp.set_boundingbox (bb);

	  m_blockUpdates = false;
	}
    }
}

//////////////////////////////////////////////////////////////////////////////

bool Figure::eventFilter (QObject* obj, QEvent* event)
{
  if (! m_blockUpdates)
    {
      if (obj == m_container)
	{
	  switch (event->type ())
	    {
	    case QEvent::Resize:
	      updateBoundingBox ();
	      break;
	    case QEvent::ChildAdded:
	      if (dynamic_cast<QChildEvent*> (event)->child
		  ()->isWidgetType())
		{
		  gh_manager::auto_lock lock;
		  const figure::properties& fp = properties<figure> ();

		  showFigureToolBar (! hasUiControlChildren (fp));
		}
	    default:
	      break;
	    }
	}
      else
	{
	  switch (event->type ())
	    {
	    case QEvent::Move:
	      updateBoundingBox ();
	      break;
	    case QEvent::Close:
	      event->ignore ();
	      gh_manager::post_callback (m_handle, "closerequestfcn");
	      return true;
	    default:
	      break;
	    }
	}
    }

  return false;
}

//////////////////////////////////////////////////////////////////////////////

void Figure::aboutQtHandles (void)
{
  QMessageBox::about (qWidget<QMainWindow> (), tr ("About QtHandles"),
		      ABOUT_TEXT);
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
