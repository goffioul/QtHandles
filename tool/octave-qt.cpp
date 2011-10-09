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
#include <QThread>

#include <octave/config.h>
#include <octave/octave.h>

//////////////////////////////////////////////////////////////////////////////

class OctaveThread : public QThread
{
public:
  OctaveThread (int argc, char** argv)
    : m_argc (argc), m_argv (argv), m_result (0)
    { }

  int result (void) const { return m_result; }

protected:
  void run (void)
    {
      m_result = octave_main (m_argc, m_argv, 0);
      QApplication::exit (m_result);
    }

private:
  int m_argc;
  char** m_argv;
  int m_result;
};

//////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv)
{
  QApplication app (argc, argv);
  OctaveThread mainThread (argc, argv);

  app.setQuitOnLastWindowClosed (false);
  mainThread.start ();

  return app.exec ();
}
