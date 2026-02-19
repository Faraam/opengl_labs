#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <array>
using namespace std;
struct Vec2
{
    float x;
    float y;
};

const float PI = 3.1415f;
array<Vec2, 3> calcRayPoints(int i, int N,
                             float innerR, float outerR, float rotateFactor = +PI / 2)
{

    float w = 2.0f * PI / N;
    float a = i * w + rotateFactor;
    float aL = a - w * 0.5f;
    float aR = a + w * 0.5f;

    float xL = innerR * cos(aL);
    float yL = innerR * sin(aL);
    float xR = innerR * cos(aR);
    float yR = innerR * sin(aR);
    float xT = outerR * cos(a);
    float yT = outerR * sin(a);

    return {{{xL, yL}, {xR, yR}, {xT, yT}}};
}

void drawStar1()
{
    glLineWidth(6.0f);
    int N = 5;
    for (int i = 0; i < N; ++i)
    {
        auto pts = calcRayPoints(i, N, 0.25f, 0.80f);
        auto pts2 = calcRayPoints(i, N, 0.1f, 0.25f);
        glColor3f(1.0f, 0.5f, 0.1f);
        glBegin(GL_TRIANGLES);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glVertex2f(pts[2].x, pts[2].y);

        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glVertex2f(0, 0);
        glEnd();
        glColor3f(0.9f, 0.1f, 0.1f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[2].x, pts[2].y);
        glVertex2f(pts2[2].x, pts2[2].y);
        glEnd();
        glColor3f(0.6f, 0.3f, 0.1f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[2].x, pts[2].y);
        glVertex2f(pts[1].x, pts[1].y);
        glEnd();

        glBegin(GL_LINE_STRIP);
        glVertex2f(pts2[0].x, pts2[0].y);
        glVertex2f(pts2[2].x, pts2[2].y);
        glVertex2f(pts2[1].x, pts2[1].y);
        glEnd();
    }
}

void drawStar2()
{
    glLineWidth(6.0f);
    int N = 5;
    for (int i = 0; i < N; ++i)
    {
        auto pts = calcRayPoints(i, N, 0.1f, 0.25f);
        auto pts2 = calcRayPoints(i, N, 0.2f, 0.40f);
        auto pts3 = calcRayPoints(i, N, 0.3f, 0.8f);

        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[2].x, pts[2].y);
        glVertex2f(pts[1].x, pts[1].y);
        glEnd();
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts2[0].x, pts2[0].y);
        glVertex2f(pts2[2].x, pts2[2].y);
        glVertex2f(pts2[1].x, pts2[1].y);
        glEnd();
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts3[0].x, pts3[0].y);
        glVertex2f(pts3[2].x, pts3[2].y);
        glVertex2f(pts3[1].x, pts3[1].y);
        glEnd();
    }
}

void drawStar3()
{
    glLineWidth(6.0f);
    int N = 4;
    for (int i = 0; i < N; ++i)
    {
        auto pts = calcRayPoints(i, N, 0.05f, 0.25f);
        auto pts2 = calcRayPoints(i, N, 0.15f, 0.40f);
        auto pts3 = calcRayPoints(i, N, 0.22f, 0.8f);

        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[2].x, pts[2].y);
        glVertex2f(pts[1].x, pts[1].y);
        glEnd();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts2[0].x, pts2[0].y);
        glVertex2f(pts2[2].x, pts2[2].y);
        glVertex2f(pts2[1].x, pts2[1].y);
        glEnd();
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts3[0].x, pts3[0].y);
        glVertex2f(pts3[2].x, pts3[2].y);
        glVertex2f(pts3[1].x, pts3[1].y);
        glEnd();
    }
}
void drawSomeangle()
{
    glLineWidth(6.0f);
    int N = 6;
    for (int i = 0; i < N; ++i)
    {
        auto pts = calcRayPoints(i, N, 0.8f, 0.0f);
        auto pts2 = calcRayPoints(i, N, 0.55f, 0.0f);
        auto pts3 = calcRayPoints(i, N, 0.3f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glEnd();

        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts2[0].x, pts2[0].y);
        glVertex2f(pts2[1].x, pts2[1].y);
        glEnd();

        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts3[0].x, pts3[0].y);
        glVertex2f(pts3[1].x, pts3[1].y);
        glEnd();
    }
}

void drawSmth()
{
    glLineWidth(6.0f);
    int N = 5;
    for (int i = 0; i < N; ++i)
    {
        auto pts = calcRayPoints(i, N, 0.8f, 0.0f, -PI / 2);
        glColor3f(0.0f, 1.0f, 1.0f);

        glBegin(GL_TRIANGLES);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glVertex2f(0, 0);
        glEnd();
        glColor3f(1.0f, 0.5f, 0.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glEnd();
    }
    glLineWidth(2.0f);
    int N2 = 4;
    for (int i = 0; i < N2; ++i)
    {
        auto pts = calcRayPoints(i, N2, 0.4f, 0.0f, PI / 4);
        glColor3f(1.0f, 0.3f, 0.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(pts[1].x, pts[1].y);
        glVertex2f(0, 0);
        glEnd();
        glBegin(GL_LINE_STRIP);
        glVertex2f(pts[0].x, pts[0].y);
        glVertex2f(0, 0);
        glEnd();
    }
}
double F(double r, double phi, double p, double psqrt, double k)
{

    return pow(r * r - p, 3.0) - pow(r, 5.0) * psqrt * k;
}

double getR(double phi, double p)
{
    double psqrt = sqrt(p);
    double c = cos(phi);
    double s = sin(phi);
    double k = (c * c) * (s * s * s);
    double eps = 1e-12;
    if (abs(k) < eps)
    {
        return psqrt;
    }
    double a = 0;
    double b = 1;
    double fa = F(a, phi, p, psqrt, k);
    for (int i = 0; i < 70; i++)
    {
        double m = 0.5 * (a + b);
        double fm = F(m, phi, p, psqrt, k);
        if (abs(fm) <= eps)
        {
            return 0.5 * (a + b);
        }
        if (fa * fm <= 0.0)
        {
            b = m;
        }
        else
        {
            a = m;
            fa = fm;
        }
    }
    return 0;
}

array<Vec2, 100> calcHeartPoints(double p)
{
    array<Vec2, 100> result;
    int N = 100;
    double minphi = -PI / 2;
    double maxphi = PI / 2;
    for (int i = 0; i < N; ++i)
    {
        double phi = minphi + (maxphi - minphi) * i / (N - 1);
        float r = getR(phi, p);
        cout << r << "\n";
        result[i] = {
            (float)(r * cos(phi)),
            (float)(r * sin(phi))};
    }
    return result;
}

void drawHeart()
{
    glLineWidth(6.0f);

    auto pts = calcHeartPoints(0.1);
    auto pts2 = calcHeartPoints(0.25);
    auto pts3 = calcHeartPoints(0.45);
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts){
        glVertex2f(p.x, p.y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts){
        glVertex2f(-p.x, p.y);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts2){
        glVertex2f(p.x, p.y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts2){
        glVertex2f(-p.x, p.y);
    }
    glEnd();

    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts3){
        glVertex2f(p.x, p.y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for(auto &p : pts3){
        glVertex2f(-p.x, p.y);
    }
    glEnd();
}

void movingRectangle(){}

void updateViewport(GLFWwindow *window,
                    int w, int h)
{
    glViewport(0, 0, w, h);
}

int main()
{
    void (*draw)() = nullptr;

    int index;

    cout << "Индекс:\n";
    cout << "0 - Звезда\n";
    cout << "1 - Звезда 2\n";
    cout << "2 - Опять звезда, но неправильная\n";
    cout << "3 - Скольки-то угольник\n";
    cout << "4 - Психоделический камешек\n";
    cout << "5 - Люблю численные методы\n";
    cout << "> ";

    cin >> index;
    // index = 3;
    switch (index)
    {
    case 0:
        draw = drawStar1;
        break;
    case 1:
        draw = drawStar2;
        break;
    case 2:
        draw = drawStar3;
        break;
    case 3:
        draw = drawSomeangle;
        break;
    case 4:
        draw = drawSmth;
        break;
    case 5:
        draw = drawHeart;
        break;
    default:
        cout << "¯\\_(ツ)_/¯\n";
        return 0;
    }

    glfwInit();

    GLFWwindow *window =
        glfwCreateWindow(800, 600, "lab1", NULL, NULL);

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        updateViewport);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(1, 1, 1, 1); // белый фон
        glClear(GL_COLOR_BUFFER_BIT);

        // вызов через указатель
        draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}