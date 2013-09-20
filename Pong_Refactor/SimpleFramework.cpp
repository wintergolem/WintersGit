// =======================================
// SimpleFramework.cpp - basic GLUT support
// Copyright (c) Jules Bloomenthal, 2012
// All rights reserved
// ---------------------------------------

#include "SimpleFramework.h"
#include "gl/freeglut.h"

bool KeyDown(int key) {
    // is the ASCII-numbered key presently down?
    // NB: to know for certain the case of a keyboard character,
    //     one should test the SHIFT key; in the present implementation
    //     the unshifted 'i' key transmits as an upper-case 'I'
    static int nShortBits = 8*sizeof(SHORT);
    static SHORT shortMSB = 1 << (nShortBits-1);
    return (GetKeyState(key) & shortMSB) != 0;
}

void Rectangle(double x1, double y1, double x2, double y2)
{
    glBegin(GL_QUADS);
    glVertex2d(x1, y1);
    glVertex2d(x1, y2);
    glVertex2d(x2, y2);
    glVertex2d(x2, y1);
    glEnd();
}

void Disk(double x, double y, float radius)
{
    glPointSize(radius);                               // set GL point diameter, in pixels 
    glBegin(GL_POINTS);
    glVertex2d(x, y);                                  // a single point
    glEnd();
}

void ClearScreen(float r, float g, float b)
{
    glClearColor(r, g, b, 1);
    glClear(GL_COLOR_BUFFER_BIT);                      // clear background
    glDisable(GL_DEPTH_TEST);                          // disable depth buffer
    glEnable(GL_BLEND);                                // enable anti-aliasing
    glEnable(GL_LINE_SMOOTH);                          // smooth lines
    glEnable(GL_POINT_SMOOTH);                         // smooth points
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // type of smoothing
}

void Text(int x, int y, char *s)
    {
    // position
    glMatrixMode(GL_MODELVIEW);                        // access world transform
    glPushMatrix();                                    // save current transform
    glLoadIdentity();                                  // reset transform
    glScaled(.1, .1, 1.);                              // scale transform for text
    glTranslated(x, y, 0);                             // position transform for text

    // display
    glPointSize(1);                                    // set GL point diameter, in pixels 
    for (const char *c = s; *c; c++)                   // display each character
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);    // as caligraphic strokes
    glPopMatrix();                                     // restore current transform
}

void Make2dWindow(int x, int y, int w, int h, char *title)
{
    // initialize GLUT
    int ac = 0;
    glutInitWindowPosition(x, y);
    glutInitWindowSize(w, h);
    glutInit(&ac, NULL);
    glutCreateWindow(title);

    // set default viewport and projection
    glViewport(0, 0, w, h);                            // map GL space into entire app window
    glMatrixMode(GL_PROJECTION);                       // camera view mode
    glLoadIdentity();                                  // no rotations
    glOrtho(0, w, 0, h, 0, -1);                        // GL space in pixels, no perspective
}
