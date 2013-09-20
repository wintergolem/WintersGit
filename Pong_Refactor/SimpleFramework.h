// =======================================
// SimpleFramework.h - basic GLUT support
// Copyright (c) Jules Bloomenthal, 2012
// All rights reserved
// ---------------------------------------

bool KeyDown(int key);
    // is the ASCII-numbered key presently down?
    // NB: to know for certain the case of a keyboard character,
    //     one should test the SHIFT key; in the present implementation
    //     the unshifted 'i' key transmits as an upper-case 'I'

void Rectangle(double x1, double y1, double x2, double y2);

void Disk(double x, double y, float radius);

void ClearScreen(float r = 0, float g = .35f, float b = 0);

void Text(int x, int y, char *s);

void Make2dWindow(int x, int y, int w, int h, char *title);
