#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
using namespace std;
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

struct Vec3
{
    double x;
    double y;
    double z;
};
double ellipsoid_r(double a, double b, double c, double phi, double theta)
{
    const double s = sin(theta);
    const double ct = cos(theta);
    const double cp = cos(phi);
    const double sp = sin(phi);

    const double denom =
        (s * s * cp * cp) / (a * a) +
        (s * s * sp * sp) / (b * b) +
        (ct * ct) / (c * c);

    return 1.0 / sqrt(denom);
}
Vec3 ellipsoid_point(double a, double b, double c, double phi, double theta)
{
    double r = ellipsoid_r(a, b, c, phi, theta);

    Vec3 p;
    p.x = r * sin(theta) * cos(phi);
    p.y = r * sin(theta) * sin(phi);
    p.z = r * cos(theta);
    return p;
}
Vec3 cone_point(double x, double y, double a, double b, double c)
{
    Vec3 p;
    p.x = x;
    p.y = y;
    p.z = c * sqrt((x * x) / (a * a) + (y * y) / (b * b));
    return p;
}
Vec3 paraboloid_point(double x, double y, double a, double b)
{
    Vec3 p;
    p.x = x;
    p.y = y;
    p.z = 0.5 * ((x * x) / (a * a) + (y * y) / (b * b));
    return p;
}
Vec3 cylinder_point(double x, double c, double h)
{
    Vec3 p;
    p.x = x;
    p.y = sqrt(c * c - x * x);
    p.z = h;
    return p;
}

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

void drawCube()
{

    glBegin(GL_QUADS);

    // Передняя грань (красная)
    glColor3f(1, 0, 0);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(-1, 1, 1);

    // Задняя (зелёная)
    glColor3f(0, 1, 0);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, 1, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, -1, -1);

    // Левая (синяя)
    glColor3f(0, 0, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1, -1);

    // Правая (жёлтая)
    glColor3f(1, 1, 0);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);

    // Верх (голубая)
    glColor3f(0, 1, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, -1);

    // Низ (пурпурная)
    glColor3f(1, 0, 1);
    glVertex3f(-1, -1, -1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, -1, 1);
    glVertex3f(-1, -1, 1);

    glEnd();
}
void updateCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslated(0, 0, -viewerR);
    glRotated(viewerTheta * 180.0 / M_PI, 1, 0, 0);
    glRotated(viewerPhi   * 180.0 / M_PI, 0, 1, 0);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "lab1", NULL, NULL);
    void (*draw)() = nullptr;

    int index;

    cout << "Индекс:\n";
    cout << "0 - Парабалоид\n";
    cout << "1 - Элипс\n";
    cout << "2 - Цилиндр\n";
    cout << "3 - Конус\n";
    cout << "> ";

    cin >> index;
    // index = 3;
    switch (index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
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
        drawCube();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
