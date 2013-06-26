EnvelopeCollide
===============

Project developed originally by Przemysław Bągard for master's thesis
at the Faculty of Mathematics, Informatics, and Mechanics 
at University of Warsaw.

------------------------------------------------------------------------------
Project written in C++.
Application uses:
- Qt libraries in version 4 (including qmake),
- openGL.

------------------------------------------------------------------------------
Programs:
- example1 (CollisionVisualisator):
Application used to check collision detection functions correctness.
- example2 (CollisionExample):
Simple engine using EnvelopeCollide library.
There are 'hard' shapes (filled) - obstacles, 
and 'soft' shapes (wireframed), which triggers something when character 
enters/exits those shapes.
- performance.

In collision/bin folder will be created static library 'collision'
(qt is not essential for 'collision' library, however program uses qmake).
Example1 and example2 programs need qt dynamic libraries to work.

------------------------------------------------------------------------------
Tested on both Windows and Ubuntu Linux.

------------------------------------------------------------------------------
In a linux console, type:

qmake-qt4 Aplikacja.pro
make

Remarks:
- in some linux distributions qmake or qmake4 command is used 
instead of qmake-qt4,
- if system variable QMAKESPEC is not defined (user receives error message
about lack of MAKESPEC), type in qmake-qt4 command additional 
parameter '-spec linux-g++' (program creates makefile for g++ compiler).

------------------------------------------------------------------------------
License

I grant MIT license for 'collision' library (see 'License.txt' for details).

Qt libraries are used on LGPL 2.1 license
(see file: 'QtUsedLicense_lgpl-2.1.txt'. Content of license
is also available on: http://www.gnu.org/licenses/lgpl-2.1.html ).

------------------------------------------------------------------------------

![example1_1](/screenshots/example1_1.png "example1 - CollisionVisualisator")
![example1_2](/screenshots/example1_2.png)
![example2_1](/screenshots/example2.png "example2 - CollisionExample")
