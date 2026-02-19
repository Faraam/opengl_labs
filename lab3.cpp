#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
using namespace std;
int screenW = 800;
int screenH = 600;
double viewerR = 10.0;
double viewerPhi = 0.0;
double viewerTheta = 0.0;

bool isMoving = false;
double mLastX = 0.0, mLastY = 0.0;

const double const1 = 3;
const double const2 = 4;
const double const3 = 5;

struct Vec3
{
    double x;
    double y;
    double z;
};

using SurfaceFunc = Vec3 (*)(double u, double v);

class Surface
{
public:
    SurfaceFunc func;

    double u0, u1;
    double v0, v1;

    int Nu, Nv;

    Surface(SurfaceFunc func,
            double _u0, double _u1, int _Nu,
            double _v0, double _v1, int _Nv)
        : func(func),
          u0(_u0), u1(_u1), Nu(_Nu),
          v0(_v0), v1(_v1), Nv(_Nv)
    {
    }

    void draw() const
    {
        glColor3f(1, 0, 0); 
        double du = (u1 - u0) / Nu;
        double dv = (v1 - v0) / Nv;

        for (int j = 0; j < Nv; ++j)
        {
            double vA = v0 + j * dv;
            double vB = v0 + (j + 1) * dv;

            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i <= Nu; ++i)
            {
                double u = u0 + i * du;

                Vec3 p1 = func(u, vA);
                Vec3 p2 = func(u, vB);

                glVertex3d(p1.x, p1.y, p1.z);
                glVertex3d(p2.x, p2.y, p2.z);
            }
            glEnd();
        }
    }
};

double ellipsoid_r(double phi, double theta)
{
    const double s = sin(theta);
    const double ct = cos(theta);
    const double cp = cos(phi);
    const double sp = sin(phi);

    const double denom =
        (s * s * cp * cp) / (const1 * const1) +
        (s * s * sp * sp) / (const2 * const2) +
        (ct * ct) / (const3 * const3);

    return 1.0 / sqrt(denom);
}
Vec3 ellipsoid_point(double phi, double theta)
{
    double r = ellipsoid_r(phi, theta);

    Vec3 p;
    p.x = r * sin(theta) * cos(phi);
    p.y = r * sin(theta) * sin(phi);
    p.z = r * cos(theta);
    return p;
}
Vec3 cone_point(double x, double y)
{
    Vec3 p;
    p.x = x;
    p.y = y;
    p.z = const3 * sqrt((x * x) / (const1 * const1) + (y * y) / (const2 * const2));
    return p;
}
Vec3 paraboloid_point(double x, double y)
{
    Vec3 p;
    p.x = x;
    p.y = y;
    p.z = 0.5 * ((x * x) / (const1 * const1) + (y * y) / (const2 * const2));
    return p;
}
Vec3 cylinder_point(double phi, double z)
{
    Vec3 p;
    p.x = const1 * cos(phi);
    p.y = const1 * sin(phi);
    p.z = z;
    return p;
}

void setupProjection(int w, int h)
{
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float fov = 60.0f;
    float nearZ = 0.1f, farZ = 100.0f;
    float top = nearZ * tanf((fov * 3.14159265f / 180.0f) * 0.5f);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    glFrustum(left, right, bottom, top, nearZ, farZ);

    glMatrixMode(GL_MODELVIEW);
}

void updateViewport(GLFWwindow *window,
                    int w, int h)
{
    screenW = w;
    screenH = h;
    glViewport(0, 0, w, h);
    setupProjection(screenW, screenH);
}
static void updateMouseClick(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            isMoving = true;
            glfwGetCursorPos(window, &mLastX, &mLastY);
        }
        else if (action == GLFW_RELEASE)
        {
            isMoving = false;
        }
    }
}
static void updateMouseMove(GLFWwindow *window, double x, double y)
{
    if (!isMoving)
        return;

    double dx = x - mLastX;
    double dy = y - mLastY;
    mLastX = x;
    mLastY = y;

    viewerPhi += dx * 0.005;
    viewerTheta += dy * 0.005;

    const double eps = 0.001;
    const double lim = M_PI / 2.0 - eps;
    if (viewerTheta > lim)
        viewerTheta = lim;
    if (viewerTheta < -lim)
        viewerTheta = -lim;
}

static void updateScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    viewerR -= yoffset * 0.8;
    if (viewerR < 1.0)
        viewerR = 1.0;
    if (viewerR > 200.0)
        viewerR = 200.0;
}

void drawCube()
{

    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(-1, 1, 1);
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, 1, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, -1, -1);
    glColor3f(0, 0, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1, -1);
    glColor3f(1, 1, 0);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);
    glColor3f(0, 1, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, -1);
    glColor3f(1, 0, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1);
    glVertex3f(-1, -1, 1);

    glEnd();
}
void updateCamera()
{
    glLoadIdentity();

    glTranslated(0, 0, -viewerR);
    glRotated(viewerTheta * 180.0 / M_PI, 1, 0, 0);
    glRotated(viewerPhi * 180.0 / M_PI, 0, 1, 0);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow *window =
        glfwCreateWindow(screenW, screenH, "lab1", NULL, NULL);
    Surface *surface = nullptr;

    int index;

    cout << "Индекс:\n";
    cout << "0 - Парабалоид\n";
    cout << "1 - Элипс\n";
    cout << "2 - Цилиндр\n";
    cout << "3 - Конус\n";
    cout << "4 - Кубик (для тестов)\n";
    cout << "> ";

    cin >> index;
    // index = 3;
    switch (index)
    {
    case 0:
        surface = new Surface(paraboloid_point, -10, 10, 60, -10, 10, 60);
        break;
    case 1:
        surface = new Surface(ellipsoid_point, 0, 2 * M_PI, 60, 0, 2 * M_PI, 60);
        break;
    case 2:
        surface = new Surface(cylinder_point, 0, 2 * M_PI, 60, -5, 5, 60);
        break;
    case 3:
        surface = new Surface(cone_point, -5, 5, 60, -5, 5, 60);
        break;
    case 4:
        break;
    // case 4:
    //     draw = drawSmth;
    //     break;
    // case 5:
    //     draw = drawHeart;
    //     break;
    default:
        cout << "¯\\_(ツ)_/¯\n";
        return 0;
    }

    glfwMakeContextCurrent(window);
    setupProjection(screenW, screenH);
    glfwSetFramebufferSizeCallback(
        window,
        updateViewport);

    glfwSetFramebufferSizeCallback(window, updateViewport);
    glfwSetMouseButtonCallback(window, updateMouseClick);
    glfwSetCursorPosCallback(window, updateMouseMove);
    glfwSetScrollCallback(window, updateScroll);

    glfwShowWindow(window);

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateCamera();
        surface->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
