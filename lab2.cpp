#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
#include <array>
#include <vector>
using namespace std;
struct Vec2
{
    double x;
    double y;
};

Vec2 vector_sum(Vec2 a, Vec2 b)
{
    return {a.x + b.x, a.y + b.y};
}

Vec2 vector_multiply(Vec2 a, double s)
{
    return {a.x * s, a.y * s};
}

static inline Vec2 rotate(Vec2 p, double ang)
{
    double c = cos(ang), s = sin(ang);
    return {p.x * c - p.y * s, p.x * s + p.y * c};
}

double frand(double a, double b)
{
    return a + (b - a) * (rand() / (double)RAND_MAX);
}

const float PI = 3.1415f;

array<Vec2, 4> calcRect(double x, double y, double w, double h)
{
    return {{{x, y}, {x + w, y}, {x + w, y + h}, {x, y + h}}};
}
array<Vec2, 4> calcSquareFromCenter(double x, double y, double s)
{
    return {{{x + s, y + s}, {x + s, y - s}, {x - s, y - s}, {x - s, y + s}}};
}

double w = 1.0 / 6.0;
double h = 1.0 / 12.0;
enum Wall
{
    BOTTOM = 0,
    RIGHT = 1,
    TOP = 2,
    LEFT = 3
};

const double border = 0.99;
struct Rect
{
    Vec2 base;
    Wall wall;
    double speed;
    float r;
    float g;
    float b;
};
vector<Rect> rects;
void initMoovingRects(int n)
{
    rects.clear();

    for (int i = 0; i < n; i++)
    {
        Rect r;

        r.wall = (Wall)(int)frand(0.0, 4.0);

        r.speed = frand(0.003, 0.013);

        r.r = frand(0.0, 1.0);
        r.g = frand(0.0, 1.0);
        r.b = frand(0.0, 1.0);

        double t = frand(0.0, 2.0);

        switch (r.wall)
        {
        case BOTTOM:
            r.base = {-border + t, -border};
            break;
        case RIGHT:
            r.base = {border - w, -border + t};
            break;
        case TOP:
            r.base = {border - w - t, border - h};
            break;
        case LEFT:
            r.base = {-border, border - h - t};
            break;
        }

        rects.push_back(r);
    }
}

void movingRects()
{
    for (auto &r : rects)
    {
        switch (r.wall)
        {
        case BOTTOM:
            r.base.x += r.speed;
            r.base.y = -border;
            if (r.base.x >= border - w)
            {
                r.base.x = border - w;
                r.wall = RIGHT;
            }
            break;

        case RIGHT:
            r.base.y += r.speed;
            r.base.x = border - w;
            if (r.base.y >= border - h)
            {
                r.base.y = border - h;
                r.wall = TOP;
            }
            break;

        case TOP:
            r.base.x -= r.speed;
            r.base.y = border - h;
            if (r.base.x <= -border)
            {
                r.base.x = -border;
                r.wall = LEFT;
            }
            break;

        case LEFT:
            r.base.y -= r.speed;
            r.base.x = -border;
            if (r.base.y <= -border)
            {
                r.base.y = -border;
                r.wall = BOTTOM;
            }
            break;
        }

        auto pts = calcRect(r.base.x, r.base.y, w, h);

        glColor3f(r.r, r.g, r.b);
        glBegin(GL_LINE_LOOP);
        for (auto &p : pts)
            glVertex2f((float)p.x, (float)p.y);
        glEnd();
    }
}

struct Tri
{
    Vec2 pos;
    Vec2 velocity;
    double ang;
    double angVelocity;

    array<Vec2, 3> local;

    double radius;

    float r;
    float g;
    float b;
};

vector<Tri> tris;

void initMoovingTriangles(int n)
{
    tris.clear();
    tris.reserve(n);

    for (int i = 0; i < n; i++)
    {
        Tri t{};

        double sx = frand(0.03, 0.15);
        double sy = frand(0.03, 0.15);

        t.local[0] = {frand(-1.0, 1.0) * sx, frand(-1.0, 1.0) * sy};
        t.local[1] = {frand(-1.0, 1.0) * sx, frand(-1.0, 1.0) * sy};
        t.local[2] = {frand(-1.0, 1.0) * sx, frand(-1.0, 1.0) * sy};
        t.radius = 0.0;
        for (auto &p : t.local)
        {
            double d = sqrt(p.x * p.x + p.y * p.y);
            if (d > t.radius)
                t.radius = d;
        }
        t.pos = {frand(-border + t.radius, border - t.radius),
                 frand(-border + t.radius, border - t.radius)};

        double sp = frand(0.007, 0.02);
        double sxdir = (rand() % 2) ? 1.0 : -1.0;
        double sydir = (rand() % 2) ? 1.0 : -1.0;

        double k = frand(0.6, 1.4);
        t.velocity = {sxdir * sp * k, sydir * sp};

        t.ang = frand(0.0, 6.28318530718);
        t.angVelocity = frand(-0.05, 0.05);

        t.r = (float)frand(0.0, 1.0);
        t.g = (float)frand(0.0, 1.0);
        t.b = (float)frand(0.0, 1.0);

        tris.push_back(t);
    }
}
void drawMoovingTriangles()
{
    double now = glfwGetTime();
    glLineWidth(3.0f);

    for (auto &t : tris)
    {
        t.pos = vector_sum(t.pos, t.velocity);
        t.ang += t.angVelocity;

        if (t.pos.x + t.radius > border)
        {
            t.pos.x = border - t.radius;
            t.velocity.x *= -1;
        }
        else if (t.pos.x - t.radius < -border)
        {
            t.pos.x = -border + t.radius;
            t.velocity.x *= -1;
        }
        if (t.pos.y + t.radius > border)
        {
            t.pos.y = border - t.radius;
            t.velocity.y *= -1;
        }
        else if (t.pos.y - t.radius < -border)
        {
            t.pos.y = -border + t.radius;
            t.velocity.y *= -1;
        }

        array<Vec2, 3> pts;
        for (int i = 0; i < 3; i++)
        {
            Vec2 rp = rotate(t.local[i], t.ang);
            pts[i] = vector_sum(t.pos, rp);
        }

        glColor3f(t.r, t.g, t.b);

        glBegin(GL_LINE_LOOP);
        for (auto &p : pts)
            glVertex2d(p.x, p.y);
        glEnd();
    }
}
int screenW = 800;
int screenH = 600;
static bool drawing = false;
static vector<Vec2> path;
const double circleSizeBase = 0.1;
const double circleMax = circleSizeBase * 2;
const double circleGrow = 0.001;
double circleSize = 0.1;

void drawCircle(const Vec2 &c, float r)
{
    const int segments = 48;
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i)
    {
        float a = (float)(2.0 * M_PI * i / segments);
        glVertex2f(c.x + r * cosf(a), c.y + r * sinf(a));
    }
    glEnd();
}

static Vec2 pxToCoordinates(double sx, double sy)
{
    float x = (float)(2.0 * sx / screenW - 1.0);
    float y = (float)(1.0 - 2.0 * sy / screenH);
    return {x, y};
}

static float distance(const Vec2 &a, const Vec2 &b)
{
    float dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static void updateMouseClick(GLFWwindow *window, int button, int action, int)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
        drawing = true;
        circleSize = circleSizeBase;
        path.clear();

        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        path.push_back(pxToCoordinates(x, y));
    }
    else if (action == GLFW_RELEASE)
    {
        drawing = false;
    }
}

static void updateMouseMove(GLFWwindow *window, double x, double y)
{
    if (!drawing)
        return;

    Vec2 p = pxToCoordinates(x, y);

    const float minDist = 4.0f / max({screenW, screenH, 800});
    if (path.empty() || distance(path.back(), p) > minDist * minDist)
    {
        path.push_back(p);
    }
}
static void drawPath()
{
    if (path.size() < 2)
        return;

    glLineWidth(2.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);
    for (auto &p : path)
        glVertex2f(p.x, p.y);
    glEnd();
}

void initMoovingCircle(GLFWwindow *window)
{
    glfwSetMouseButtonCallback(window, updateMouseClick);
    glfwSetCursorPosCallback(window, updateMouseMove);
}

static void drawMoovingCircle()
{
    if (path.size() < 2)
        return;

    glLineWidth(4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);
    for (auto &p : path)
        glVertex2f(p.x, p.y);
    glEnd();

    if (!drawing)
    {
        if (!path.empty())
        {
            drawCircle(path[0], min(circleSize, circleMax));
            circleSize += circleGrow;
            path.erase(path.begin());
        }
    }
}
Vec2 squarePos{0.0f, 0.0f};
Vec2 squareVelocity{0.0f, 0.0f};

const float halfside = 0.06f;
const float accel = 0.005f;
const float speedMp = 0.97f;
const float bounceMp = 0.95f;

GLFWwindow *global_window; // да, костыль, знаю

void drawSquare()
{
    glLineWidth(5.0f);
    Vec2 a{0, 0};
    if (glfwGetKey(global_window, GLFW_KEY_LEFT) == GLFW_PRESS)
        a.x -= accel;
    if (glfwGetKey(global_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        a.x += accel;
    if (glfwGetKey(global_window, GLFW_KEY_DOWN) == GLFW_PRESS)
        a.y -= accel;
    if (glfwGetKey(global_window, GLFW_KEY_UP) == GLFW_PRESS)
        a.y += accel;

    squareVelocity = vector_sum(squareVelocity, a);
    squarePos = vector_sum(squarePos, squareVelocity);

    float minX = -border + halfside, maxX = border - halfside;
    float minY = -border + halfside, maxY = border - halfside;

    if (squarePos.x < minX)
    {
        squarePos.x = minX;
        squareVelocity.x = -squareVelocity.x * bounceMp;
    }
    if (squarePos.x > maxX)
    {
        squarePos.x = maxX;
        squareVelocity.x = -squareVelocity.x * bounceMp;
    }
    if (squarePos.y < minY)
    {
        squarePos.y = minY;
        squareVelocity.y = -squareVelocity.y * bounceMp;
    }
    if (squarePos.y > maxY)
    {
        squarePos.y = maxY;
        squareVelocity.y = -squareVelocity.y * bounceMp;
    }

    squareVelocity.x *= speedMp;
    squareVelocity.y *= speedMp;

    glColor3f(0.0f, 0.0f, 0.0f);
    auto pts = calcSquareFromCenter(squarePos.x, squarePos.y, halfside);
    glBegin(GL_LINE_LOOP);
    for (auto &p : pts)
        glVertex2f((float)p.x, (float)p.y);
    glEnd();
}

void updateViewport(GLFWwindow *window,
                    int w, int h)
{
    screenW = w;
    screenH = h;
    glViewport(0, 0, w, h);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow *window =
        glfwCreateWindow(800, 600, "lab1", NULL, NULL);
    void (*draw)() = nullptr;

    int index;

    cout << "Индекс:\n";
    cout << "0 - Прямоугольники\n";
    cout << "1 - Треугольники (вроде похожи)\n";
    cout << "2 - Пакман\n";
    cout << "3 - Квадрат (радиоупраляемый)\n";
    cout << "4 - Психоделический камешек\n";
    cout << "5 - Люблю численные методы\n";
    cout << "> ";

    cin >> index;
    // index = 3;
    switch (index)
    {
    case 0:
        initMoovingRects(20);
        draw = movingRects;
        break;
    case 1:
        initMoovingTriangles(20);
        draw = drawMoovingTriangles;
        break;
    case 2:
        initMoovingCircle(window);
        draw = drawMoovingCircle;
        break;
    case 3:
        global_window = window;
        draw = drawSquare;
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
    glfwShowWindow(window);
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