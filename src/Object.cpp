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

#include <QVariant>

#include "Backend.h"
#include "Object.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////////////

namespace QtHandles
{

//////////////////////////////////////////////////////////////////////////////

Object::Object (const graphics_object& go, QObject* obj)
  : QObject (), m_handle (go.get_handle ()), m_qobject (0)
{
  gh_manager::auto_lock lock (false);

  if (! lock)
    qCritical ("QtHandles::Object::Object: "
	       "creating Object (h=%g) without a valid lock!!!",
	       m_handle.value ());

  init (obj);
}

//////////////////////////////////////////////////////////////////////////////

void Object::init (QObject* obj, bool)
{
  if (m_qobject)
    qCritical ("QtHandles::Object::init: "
	       "resetting QObject while in invalid state");

  m_qobject = obj;

  if (m_qobject)
    {
      m_qobject->setProperty ("QtHandles::Object",
                              QVariant::fromValue<void*> (this));
      connect (m_qobject, SIGNAL (destroyed (QObject*)),
	       SLOT (objectDestroyed (QObject*)));
    }
}

//////////////////////////////////////////////////////////////////////////////

Object::~Object (void)
{
}

//////////////////////////////////////////////////////////////////////////////

graphics_object Object::object (void) const
{
  gh_manager::auto_lock lock (false);

  if (! lock)
    qCritical ("QtHandles::Object::object: "
	       "accessing graphics object (h=%g) without a valid lock!!!",
	       m_handle.value ());

  return gh_manager::get_object (m_handle);
}

//////////////////////////////////////////////////////////////////////////////

void Object::slotUpdate (int pId)
{
  gh_manager::auto_lock lock;

  switch (pId)
    {
    // Special case for objects being deleted, as it's very likely
    // that the graphics_object already has been destroyed when this
    // is executed (because of the async behavior).
    case base_properties::ID_BEINGDELETED:
      beingDeleted ();
      break;
    default:
      if (object ().valid_object ())
	update (pId);
      break;
    }
}

//////////////////////////////////////////////////////////////////////////////

void Object::slotFinalize (void)
{
  gh_manager::auto_lock lock;

  finalize ();
}

//////////////////////////////////////////////////////////////////////////////

void Object::slotRedraw (void)
{
  gh_manager::auto_lock lock;

  if (object ().valid_object ())
    redraw ();
}

//////////////////////////////////////////////////////////////////////////////

void Object::update (int /* pId */)
{
}

//////////////////////////////////////////////////////////////////////////////

void Object::finalize (void)
{
  if (m_qobject)
    {
      delete m_qobject;
      m_qobject = 0;
    }
  deleteLater ();
}

//////////////////////////////////////////////////////////////////////////////

void Object::redraw (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Object::beingDeleted (void)
{
}

//////////////////////////////////////////////////////////////////////////////

void Object::objectDestroyed (QObject* obj)
{
  if (obj && obj == m_qobject)
    m_qobject = 0;
}

//////////////////////////////////////////////////////////////////////////////

Object* Object::parentObject (const graphics_object& go)
{
  Object* parent = Backend::toolkitObject
    (gh_manager::get_object (go.get_parent ()));

  return parent;
}

//////////////////////////////////////////////////////////////////////////////

Object* Object::fromQObject (QObject* obj)
{
  QVariant v = obj->property ("QtHandles::Object");

  if (v.isValid ())
    return reinterpret_cast<Object *>(v.value<void*>());

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

}; // namespace QtHandles
