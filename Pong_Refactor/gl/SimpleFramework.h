// =======================================
// SimpleFramework.h - basic GLUT support
// Copyright (c) Jules Bloomenthal, 2012
// All rights reserved
// ---------------------------------------

void Rectangle(double x1, double y1, double x2, double y2);

void Disk(double x, double y, float radius);

void ClearScreen(float r = 0, float g = .35f, float b = 0);

void Text(int x, int y, char *s);

void Make2dWindow(int x, int y, int w, int h, char *title);
