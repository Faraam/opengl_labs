// g++ main.cpp -lglfw -lGL -ldl -lpthread -o cube
// (на Windows/MSVC линковка будет другой)

#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

static void setup_projection(int w, int h)
{
    if (h == 0)
        h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov = 60.0f;
    float nearZ = 0.1f, farZ = 100.0f;
    float top = nearZ * std::tan((fov * 3.14159265f / 180.0f) * 0.5f);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;
    glFrustum(left, right, bottom, top, nearZ, farZ);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void draw_cube(float s = 1.0f)
{
    float h = s * 0.5f;

    glBegin(GL_QUADS);

    // Front (z = +h) - red
    glColor3f(1, 0, 0);
    glVertex3f(-h, -h, +h);
    glVertex3f(+h, -h, +h);
    glVertex3f(+h, +h, +h);
    glVertex3f(-h, +h, +h);

    // Back (z = -h) - green
    glColor3f(0, 1, 0);
    glVertex3f(+h, -h, -h);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, +h, -h);
    glVertex3f(+h, +h, -h);

    // Left (x = -h) - blue
    glColor3f(0, 0, 1);
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, -h, +h);
    glVertex3f(-h, +h, +h);
    glVertex3f(-h, +h, -h);

    // Right (x = +h) - yellow
    glColor3f(1, 1, 0);
    glVertex3f(+h, -h, +h);
    glVertex3f(+h, -h, -h);
    glVertex3f(+h, +h, -h);
    glVertex3f(+h, +h, +h);

    // Top (y = +h) - magenta
    glColor3f(1, 0, 1);
    glVertex3f(-h, +h, +h);
    glVertex3f(+h, +h, +h);
    glVertex3f(+h, +h, -h);
    glVertex3f(-h, +h, -h);

    // Bottom (y = -h) - cyan
    glColor3f(0, 1, 1);
    glVertex3f(-h, -h, -h);
    glVertex3f(+h, -h, -h);
    glVertex3f(+h, -h, +h);
    glVertex3f(-h, -h, +h);

    glEnd();
}
int screenW = 800;
int screenH = 600;
double viewerR = 10.0;
double viewerPhi = 0.0;
double viewerTheta = 0.0;

static bool gDragging = false;
static double gLastX = 0.0, gLastY = 0.0;

static const double ROT_SPEED = 0.005;
static const double ZOOM_SPEED = 0.8;
static const double R_MIN = 1.0;
static const double R_MAX = 200.0;
void updateViewport(GLFWwindow *window,
                    int w, int h)
{
    screenW = w;
    screenH = h;
    glViewport(0, 0, w, h);
}
static void updateMouseClick(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            gDragging = true;
            glfwGetCursorPos(window, &gLastX, &gLastY);
        }
        else if (action == GLFW_RELEASE)
        {
            gDragging = false;
        }
    }
}
static void updateMouseMove(GLFWwindow *window, double x, double y)
{
    if (!gDragging)
        return;

    double dx = x - gLastX;
    double dy = y - gLastY;
    gLastX = x;
    gLastY = y;

    viewerPhi += dx * ROT_SPEED;
    viewerTheta += dy * ROT_SPEED;

    const double eps = 0.001;
    const double lim = M_PI / 2.0 - eps;
    if (viewerTheta > lim)
        viewerTheta = lim;
    if (viewerTheta < -lim)
        viewerTheta = -lim;
}

static void updateScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    viewerR -= yoffset * ZOOM_SPEED;
    if (viewerR < R_MIN)
        viewerR = R_MIN;
    if (viewerR > R_MAX)
        viewerR = R_MAX;
}
void updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(0, 0, -viewerR);
    glRotated(viewerTheta * 180.0 / M_PI, 1, 0, 0);
    glRotated(viewerPhi * 180.0 / M_PI, 0, 1, 0);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "glfwInit failed\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *win = glfwCreateWindow(800, 600, "GLFW OpenGL Cube", nullptr, nullptr);
    if (!win)
    {
        std::cerr << "glfwCreateWindow failed\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    int w, h;
    glfwGetFramebufferSize(win, &w, &h);
    setup_projection(w, h);

    glfwSetMouseButtonCallback(win, updateMouseClick);
    glfwSetCursorPosCallback(win, updateMouseMove);
    glfwSetScrollCallback(win, updateScroll);
    glfwSetFramebufferSizeCallback(win, [](GLFWwindow *, int ww, int hh)
                                   { setup_projection(ww, hh); });

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(win))
    {
        glfwPollEvents();
        updateCamera();

        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(win, GLFW_TRUE);

        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        draw_cube(1.0f);

        glfwSwapBuffers(win);
    }

    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
