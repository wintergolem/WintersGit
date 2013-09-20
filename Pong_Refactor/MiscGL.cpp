/*  =========================================
    SU-MiscGL.cpp - miscellaneous gl routines
    Copyright (c) Jules Bloomenthal, 2011
    All rights reserved
    =========================================

    Contents
        Support
            Equal
            Normalize
        Query
            glmErrors
            glmFullMatrix
            glmNearestScreen
            glmVisible
        View
            glmReshapeOrtho
            glmScreenMode
            glmRestoreView
        Transformations
            glmTransformPointToEyeSpace
            glmTransformVector
            glmMatrixMultiply
            glmScreenFromWorld
            glmScreenLine
            glmEyeVector
        Initialization
            static SetMatte
            static SetCircle
        Text
            static glmPuts
            glmLabel
        Drawing
            glmSmooth
            glmSoftDisk
            glmDisk
            glmCircle
            glmArrow
            glmArrowV
            glmRect
            glmLine
            glmDash
*/

#include "MiscGL.h"
#include <stdarg.h>
#include <float.h>
#include <math.h>
#include <vector>


//========== Support ===============

bool Equal(double a, double b) {
    double d = a-b;
    return d < DBL_EPSILON && d > -DBL_EPSILON;
}

void Normalize(double *v, int dim = 3)
{
    double magSq = 0;
    for (int i = 0; i < dim; i++)
        magSq += v[i]*v[i];
    if (magSq > DBL_EPSILON) {
        double mag = sqrt(magSq);
        for (int i = 0; i < dim; i++)
            v[i] /= mag;
    }
}


//========== Query ===============

int glmErrors(char *msg)
{
    char buf[1000];
    int nErrors = 0;
    buf[0] = 0;
    for (;;) {
        GLenum n = glGetError();
        if (n == GL_NO_ERROR)
            break;
        sprintf(buf+strlen(buf), "%s%s", !nErrors++? "" : ", ", gluErrorString(n));
            // do not call Debug() while looping through errors, so accumulate in buf
    }
    printf("%s%s\n", msg, nErrors? buf : "no GL errors");
    return nErrors;
}

void glmFullMatrix(double m[][4])
{
    double view[4][4], proj[4][4];
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble *) view);
    glGetDoublev(GL_PROJECTION_MATRIX, (GLdouble *) proj);
    glmMatrixMultiply(view, proj, m);
}

int glmNearestScreen(double pts[][3], int npts, double *s, double m[][4], double *dist)
{
    int index = -1;
    double minD = DBL_MAX;
    for (int i = 0; i < npts; i++) {
        double t[2];
        glmScreenFromWorld(pts[i], t, m);
        double dx = s[0]-t[0], dy = s[1]-t[1], dsq = dx*dx+dy*dy;
        if (dsq < minD) {
            minD = dsq;
            index = i;
        }
    }
    if (dist)
        *dist = (double) sqrt(minD);
    return index;
}

bool glmVisible(double *p, double m[][4], double epsilon)
{
    return true; // assert("glmVisible: this fails" == NULL);
    if (glIsEnabled(GL_DEPTH_TEST) == GL_FALSE)
        return true;
    double z, viewport[4], s[2];
    glGetDoublev(GL_VIEWPORT, viewport);
    glmScreenFromWorld(p, s, m, &z);
    if (s[0] < 0 || s[0] > viewport[2] || s[1] < 0 || s[1] > viewport[3])
        return false;
    if (z < 0.)
        return false;
    GLint x = s[0], y = s[1];
    GLfloat depth;
    // ***** glReadPixels FAILS (causes flicker during Draw!)
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    return (z+1.)/2. < (depth-epsilon);
}


//========== View ===============
        
void glmReshapeOrtho(int x, int y, int width, int height)
{
    double aspectRatio = (double) width / (double) height;
    glViewport(x, y, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1)
        glOrtho(-aspectRatio, aspectRatio, -1, 1, -100, 100);
    else
        glOrtho(-1, 1, -1/aspectRatio, 1/aspectRatio, -100, 100);
}

void glmScreenMode()
{
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();    
    glOrtho(0, viewport[2], 0, viewport[3], -1, 1);
        // in screen mode, wish (0,0) at corner, so ignore viewport[0],[1]
}

void glmRestoreView()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}


//========== Transformations ===============

void glmTransformPointToEyeSpace(double *p, double m[][4], double *s, double *z)
{
    if (z)
        *z = p[0]*m[0][2]+p[1]*m[1][2]+p[2]*m[2][2]+m[3][2];
    s[0] = p[0]*m[0][0]+p[1]*m[1][0]+p[2]*m[2][0]+m[3][0];
    s[1] = p[0]*m[0][1]+p[1]*m[1][1]+p[2]*m[2][1]+m[3][1];
    double w = p[0]*m[0][3]+p[1]*m[1][3]+p[2]*m[2][3]+m[3][3];
    if (!Equal(w, 1.)) {
        s[0] /= w;
        s[1] /= w;
        if (z)
            *z /= w;
    }
}

void glmTransformPointToEyeSpace(float *p, double m[][4], double *s, double *z)
{
    if (z)
        *z = p[0]*m[0][2]+p[1]*m[1][2]+p[2]*m[2][2]+m[3][2];
    s[0] = p[0]*m[0][0]+p[1]*m[1][0]+p[2]*m[2][0]+m[3][0];
    s[1] = p[0]*m[0][1]+p[1]*m[1][1]+p[2]*m[2][1]+m[3][1];
    double w = p[0]*m[0][3]+p[1]*m[1][3]+p[2]*m[2][3]+m[3][3];
    if (!Equal(w, 1.)) {
        s[0] /= w;
        s[1] /= w;
        if (z)
            *z /= w;
    }
}

void glmTransformVector(double *v, double m[][4], double *xv)
{
    for (int i = 0; i < 3; i++)
        xv[i] = v[0]*m[0][i]+v[1]*m[1][i]+v[2]*m[2][i];
}

void glmMatrixMultiply(double m1[][4], double m2[][4], double result[][4])
{
    // compute m1*m2, given row-ordered matrices (ie, m[i][j] refers to row i, column j)
    // if column-ordered, result = m2*m1
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            result[i][j] = m1[i][0]*m2[0][j]+  
                           m1[i][1]*m2[1][j]+
                           m1[i][2]*m2[2][j]+
                           m1[i][3]*m2[3][j];
}

void glmScreenFromWorld(double *p, double *s, double m[][4], double *z)
{
    double vp[4], t[2];
    glGetDoublev(GL_VIEWPORT, vp);
    glmTransformPointToEyeSpace(p, m, t, z);
    // ignore vp[0],[1] so point is wrt (0,0)
    s[0] = (t[0]*vp[2]+vp[2])/2.;
    s[1] = (t[1]*vp[3]+vp[3])/2.;
}

void glmScreenFromWorld(float *p, double *s, double m[][4], double *z)
{
    double vp[4], t[2];
    glGetDoublev(GL_VIEWPORT, vp);
    glmTransformPointToEyeSpace(p, m, t, z);
    // ignore vp[0],[1] so point is wrt (0,0)
    s[0] = (t[0]*vp[2]+vp[2])/2.; // +vp[0]
    s[1] = (t[1]*vp[3]+vp[3])/2.; // +vp[1]
}

void glmScreenLine(double xscreen, double yscreen, double p1[], double p2[])
{
    // compute 3D world space line, given by p1 and p2, that transforms
    // to a line perpendicular to the screen at (xscreen, yscreen)
	int vp[4];
    double view[16], proj[16];
	glGetIntegerv(GL_VIEWPORT, vp);
	glGetDoublev(GL_MODELVIEW_MATRIX, view);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
    // **** should cache inverse matrix
	if (gluUnProject(xscreen, yscreen, .25, view, proj, vp, &p1[0], &p1[1], &p1[2]) == GL_FALSE)
        printf("UnProject false\n");
	if (gluUnProject(xscreen, yscreen, .50, view, proj, vp, &p2[0], &p2[1], &p2[2]) == GL_FALSE)
        printf("UnProject false\n");
}

void glmScreenLine(int xscreen, int yscreen, double p1[], double p2[])
{
    glmScreenLine((double)xscreen, (double)yscreen, p1, p2);
}

void glmScreenVector(double p[3], double fullMatrix[4][4], double v[3])
{
    double scr[2], p1[3], p2[3];
    glmScreenFromWorld(p, scr, fullMatrix);
    glmScreenLine(scr[0], scr[1], p1, p2);
    for (int i = 0; i < 3; i++)
        v[i] = p2[i]-p1[i];
    Normalize(v);
}

void glmEyeVector(double *eye)
{
    double vp[4];
	glGetDoublev(GL_VIEWPORT, vp);
    double x = vp[0]+.5*vp[2], y = vp[1]+.5*vp[3], p1[3], p2[3];
    glmScreenLine((int)x, (int)y, p1, p2);
    for (int i = 0; i < 3; i++)
        eye[i] = p1[i]-p2[i];
    Normalize(eye);
}

double glmScreenDistSq(int x, int y, double *p, double m[][4])
{
    double s[2];
    glmScreenFromWorld(p, s, m);
    double dx = x-s[0], dy = y-s[1];
    return dx*dx+dy*dy;
}

double glmScreenDistSq(int x, int y, float *p, double m[][4])
{
    double s[2];
    glmScreenFromWorld(p, s, m);
    double dx = x-s[0], dy = y-s[1];
    return dx*dx+dy*dy;
}


//========== Initialization ===============

#define N_CIRCLE_POINTS 12
static double circle[N_CIRCLE_POINTS][2];

unsigned char diskMatte[16][16][4]; // raster of 4-byte rgba
unsigned char ringMatte[16][16][4];

unsigned char ringAlpha[16][16] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   4,  18,  18,   4,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,  56, 197, 241, 241, 197,  56,   0,   0,   0,   0,   0,
    0,   0,   0,   0,  56, 242, 255, 255, 255, 255, 242,  56,   0,   0,   0,   0,
    0,   0,   0,   4, 197, 255, 255, 255, 255, 255, 255, 197,   4,   0,   0,   0,
    0,   0,   0,  18, 241, 255, 255, 255, 255, 255, 255, 241,  18,   0,   0,   0,
    0,   0,   0,  18, 241, 255, 255, 255, 255, 255, 255, 241,  18,   0,   0,   0,
    0,   0,   0,   4, 197, 255, 255, 255, 255, 255, 255, 197,   4,   0,   0,   0,
    0,   0,   0,   0,  56, 242, 255, 255, 255, 255, 242,  56,   0,   0,   0,   0,
    0,   0,   0,   0,   0,  56, 197, 241, 241, 197,  56,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   4,  18,  18,   4,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

unsigned char diskAlpha[16][16] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   1,  26,  92, 139, 139,  92,  26,   1,   0,   0,   0,   0,
    0,   0,   0,  11, 139, 241, 254, 255, 255, 254, 241, 139,  11,   0,   0,   0,
    0,   0,  11, 184, 254, 255, 255, 255, 255, 255, 255, 254, 184,  11,   0,   0,
    0,   1, 139, 254, 255, 255, 255, 255, 255, 255, 255, 255, 254, 139,   1,   0,
    0,  26, 241, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 241,  26,   0,
    0,  92, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254,  92,   0,
    0, 139, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 139,   0,
    0, 139, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 139,   0,
    0,  92, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254,  92,   0,
    0,  26, 241, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 241,  26,   0,
    0,   1, 139, 254, 255, 255, 255, 255, 255, 255, 255, 255, 254, 139,   1,   0,
    0,   0,  11, 184, 254, 255, 255, 255, 255, 255, 255, 254, 184,  11,   0,   0,
    0,   0,   0,  11, 139, 241, 254, 255, 255, 254, 241, 139,  11,   0,   0,   0,
    0,   0,   0,   0,   1,  26,  92, 139, 139,  92,  26,   1,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

static bool SetDiskMatte()
{
    unsigned char *a = (unsigned char *) diskAlpha;
    unsigned char *m = (unsigned char *) diskMatte;
    unsigned char white[] = {255, 255, 255};
    for (int i = 0; i < 16*16; i++) {
        memcpy(m, white, 3);
        m += 3;
        *m++ = *a++;
    }
    return true;
}

static bool SetRingMatte()
{
    unsigned char *a = (unsigned char *) diskAlpha;
    unsigned char *r = (unsigned char *) ringAlpha;
    unsigned char *m = (unsigned char *) ringMatte;
    for (int i = 0; i < 16*16; i++) {
        *m++ = *r;
        *m++ = *r;
        *m++ = *r++;
        *m++ = *a++;
    }
    return true;
}

static bool SetCircle()
{
    #define _PI   3.14159265358979323846264338
    for (int i = 0; i < N_CIRCLE_POINTS; i++) {
        double angle = 2*_PI*i/N_CIRCLE_POINTS, *c = circle[i];
        c[0] = (double) cos(angle);
        c[1] = (double) sin(angle);
    }
    return true;
}

static bool diskMatteSet = SetDiskMatte();
static bool ringMatteSet = SetRingMatte();
static bool circleSet    = SetCircle();


//========== Text ===============

static void glmPuts(double x, double y, const char *s, double scale)
{
    #if defined(__glut_h__)
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();
        glScaled(scale, scale, 1);
        glTranslated(x, y, 0);
        for (const char *c = s; *c; c++)
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        glPopMatrix();
        glPopAttrib();
    #endif
}

#define FormatString(buffer, maxBufferSize, format) {  \
    if (format) {                                      \
        va_list ap;                                    \
        va_start(ap, format);                          \
        _vsnprintf(buffer, maxBufferSize, format, ap); \
        va_end(ap);                                    \
    }                                                  \
    else                                               \
        (buffer)[0] = 0;                               \
}

int glmLabel(double x, double y, double scale, const char *format, ...)
{
    // GL must be in screen space (x, y in pixels)
    // place text at screen-space (x, y)
    if (scale < DBL_EPSILON)
        return 0;
    char buf[500];
    FormatString(buf, 500, format);
    int ySpace = 12; // should be function of scale
    int nLine = 0;
    const char *b = buf;
    char line[500], *endLine = NULL;
    for (; *b; nLine++, b = endLine+1) {
        double yy = y+ySpace*nLine;
        while (*b == '\n')
            b++;
        endLine = strchr(b, '\n');
        if (endLine == NULL) {
            glmPuts(x/scale, yy/scale, b, scale);
            break;
        }
        int nChars = endLine-b;
        strncpy(line, b, nChars);
        line[nChars] = 0;
        glmPuts(x, yy, line, scale);
    }
    return nLine+1;
}
        
int glmLabel(double *p, double scale, double fullMatrix[][4], const char *format, ... )
{
    // place text at 3D world point p
    // GL must be in world space
    if (glmVisible(p, fullMatrix)) {
        glPushAttrib(GL_DEPTH_TEST);
        glDisable(GL_DEPTH_TEST);
        double s[2]; 
        glmScreenFromWorld(p, s, fullMatrix);
        char buf[500];
        FormatString(buf, 500, format);
        glmScreenMode();
        int nlines = glmLabel(s[0], s[1], scale, buf);
        glmRestoreView();
        glPopAttrib();
        return nlines;
    }
    return 0;
}


//========== 2D Display ===============

// draw matte onto unit square scaled by dia, centered at s
void DrawMatte(double *s, double dia, void *matte)
{
    // save GL state, disable zbuffer, enable smooth texture
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);

    // enable texture and set parameters
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, matte);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // enable alpha-blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // multiply diskMatte by current color and alpha-blend onto square
    // screen coordinates (x-r,y-r) to (x+r,y+r), r (radius) in pixels
    // texture coordinates (0,0) to (1,1)
    double r = dia/2., x0 = s[0]-r, x1 = s[0]+r, y0 = s[1]-r, y1 = s[1]+r;
    glPolygonMode(GL_FRONT, GL_FILL);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(x0, y0);
    glTexCoord2d(1, 0);
    glVertex2d(x1, y0);
    glTexCoord2d(1, 1);
    glVertex2d(x1, y1);
    glTexCoord2d(0, 1);
    glVertex2d(x0, y1);
    glEnd();

    // restore GL state
    glPopAttrib();
}

// draw smooth disk with black ring
// void glmRingDisk(double *s, double dia)
// {
//     DrawMatte(s, dia, ringMatte);
// }

void glmSmooth()
    {
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// draw smooth disk: texture-map disk matte onto square
void glmSoftDisk(double *s, double dia, double *color)
{
    if (color)
        glColor3dv(color);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPointSize(dia);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2dv(s);
    glEnd();
    glPopAttrib();
}

// void glmSoftDisk(double *s, double dia)
// {
//     glPushAttrib(GL_ALL_ATTRIB_BITS);
//     glPointSize(dia);
//     glEnable(GL_BLEND);
//     glEnable(GL_POINT_SMOOTH);
//     glBegin(GL_POINTS);
//     glVertex2dv(s);
//     glEnd();
//     glPopAttrib();
//     DrawMatte(s, dia, diskMatte);
// }

void glmCircle(double p[3], double fullMatrix[4][4], double dia, bool solid)
{
    double scr[2];
    glmScreenFromWorld(p, scr, fullMatrix);
    glmScreenMode();
    glmCircle(scr, dia, solid);
    glmRestoreView();
}

void glmCircle(double s[2], double dia, bool solid)
{
    // produce hard-edged disk (no anti-aliasing)
    double radius = dia/2;
    glLineWidth(.5);
    if (solid)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBegin(solid? GL_POLYGON : GL_LINE_LOOP);
    for (int i = 0; i < N_CIRCLE_POINTS; i++) {
        double *c = circle[i];
        glVertex2d(s[0]+radius*c[0], s[1]+radius*c[1]);
    }
    glEnd();
}

void glmArrow(double *base, double *head, char *label, double headSize)
{
    glBegin(GL_LINES);
    glVertex2dv(base);
    glVertex2dv(head);
    if (headSize > 0) {
        double v1[] = {head[0]-base[0], head[1]-base[1]};
        Normalize(v1, 2);
        v1[0] *= headSize;
        v1[1] *= headSize;
        double v2[] = {v1[1]/2., -v1[0]/2.}, tip1[2], tip2[2];
        for (int i = 0; i < 2; i++) {
            tip1[i] = head[i]-v1[i]+v2[i];
            tip2[i] = head[i]-v1[i]-v2[i];
        }
        glVertex2dv(head);
        glVertex2dv(tip1);
        glVertex2dv(head);
        glVertex2dv(tip2);
    }
    if (label)
        glmLabel(head[0], head[1], .12, label);
    glEnd();
}

void glmRect(int x1, int y1, int x2, int y2, double *color, bool solid)
{
    if (color)
        glColor3dv(color);
    glBegin(solid? GL_QUADS : GL_LINE_LOOP);
    glVertex2d(x1, y1);
    glVertex2d(x1, y2);
    glVertex2d(x2, y2);
    glVertex2d(x2, y1);
    glEnd();
}

void glmLine(int x1, int y1, int x2, int y2, double *color)
{
    if (color)
        glColor3dv(color);
    glBegin(GL_LINES);
    glVertex2i(x1, y1);
    glVertex2i(x2, y2);
    glEnd();
}

void glmDash(int x1, int y1, int x2, int y2, double *color)
{
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
    glmLine(x1, y1, x2, y2, color);
    glDisable(GL_LINE_STIPPLE);
}


//========== 3D Display ===============

void glmDisk(double *p, double dia, double *color)
{
    if (color)
        glColor3dv(color);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPointSize(dia);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex3dv(p);
    glEnd();
    glPopAttrib();
}

void glmArrowV(double *base,
               double  fullMatrix[][4],
               double *vec,
               double  scale,
               char   *label,
               double  headSize)
{
    double head[3], lineWidth;
    glGetDoublev(GL_LINE_WIDTH, &lineWidth);
    for (int i = 0; i < 3; i++)
        head[i] = base[i]+scale*vec[i];
    glBegin(GL_LINES);
    glVertex3dv(base);
    glVertex3dv(head);
    glEnd();
    if (headSize > 0 && glmVisible(head, fullMatrix)) {
        double sbase[2], shead[2];
        glmScreenFromWorld(base, sbase, fullMatrix);
        glmScreenFromWorld(head, shead, fullMatrix);
        double v1[] = {shead[0]-sbase[0], shead[1]-sbase[1]};
        Normalize(v1, 2);
        v1[0] *= headSize;
        v1[1] *= headSize;
        double v2[] = {v1[1]/2, -v1[0]/2}, tip1[2], tip2[2];
        for (int i = 0; i < 2; i++) {
            tip1[i] = shead[i]-v1[i]+v2[i];
            tip2[i] = shead[i]-v1[i]-v2[i];
        }
        glmScreenMode();
        glLineWidth(.5);
        glBegin(GL_LINES);
        glVertex2dv(shead);
        glVertex2dv(tip1);
        glVertex2dv(shead);
        glVertex2dv(tip2);
        glEnd();
        glLineWidth(lineWidth);
        if (label)
            glmLabel(shead[0], shead[1], .12, label);
        glmRestoreView();
    }
}


//========== Raster I/O ===============

bool glmWritePPM(char *name, int width, int height)
{
    // open file
    FILE *f = fopen(name, "wb");
    if (!f) {
        printf("can't open %s\n", name);
        return false;
    }

    char *magic = "P6", *cr = "\n", *max = "255", buf[100];

    // write magic number
    fwrite(magic, 1, 2, f);
    fwrite(cr, 1, 1, f);

    // write width and height
    sprintf(buf, "%d %d", width, height);
    fwrite(buf, strlen(buf), 1, f);
    fwrite(cr, 1, 1, f);

    // write max value
    fwrite(max, strlen(max), 1, f);
    fwrite(cr, 1, 1, f);

    // allocate buffer to hold single line
    char *linebuf = (char *) calloc(1, 3*width);
    if (!linebuf) {
        printf ("can't allocate memory\n");
        return false;
    }

    // write 24-bit data one line at a time, bottom to top (per Windows)
    for (int y = height-1; y >= 0; y--) {
        // fill linebuf with data for line y
        glReadPixels(0, y, width, 1, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) linebuf);
        fwrite(linebuf, 1, 3*width, f);
    }

    fclose(f);
    free(linebuf);
    return true;
}
