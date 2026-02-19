#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

struct Vec2 { float x, y; };
static inline Vec2 vadd(Vec2 a, Vec2 b){ return {a.x+b.x, a.y+b.y}; }
static inline Vec2 vmul(Vec2 a, float s){ return {a.x*s, a.y*s}; }

static int gW = 800, gH = 600;

void framebuffer_size_callback(GLFWwindow*, int w, int h){
    gW = (w>1? w:1); gH = (h>1? h:1);
    glViewport(0,0,gW,gH);
}

void drawSquare(Vec2 c, float halfSize){
    glBegin(GL_LINE_LOOP);
    glVertex2f(c.x - halfSize, c.y - halfSize);
    glVertex2f(c.x + halfSize, c.y - halfSize);
    glVertex2f(c.x + halfSize, c.y + halfSize);
    glVertex2f(c.x - halfSize, c.y + halfSize);
    glEnd();
}

int main(){
    if(!glfwInit()) return 0;

    GLFWwindow* window = glfwCreateWindow(gW, gH, "Square physics", nullptr, nullptr);
    if(!window){ glfwTerminate(); return 0; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    framebuffer_size_callback(window, gW, gH);

    // состояние квадрата
    Vec2 pos{0.0f, 0.0f};
    Vec2 vel{0.0f, 0.0f};

    const float half = 0.06f;    // половина стороны квадрата (NDC)
    const float border = 0.99f;  // граница экрана в NDC
    const float accel = 4.0f;    // ускорение при нажатии (ед/сек^2)
    const float damping = 0.97f; // замедление каждый кадр (0..1), меньше -> сильнее тормозит
    const float bounce = 0.95f;  // упругость отскока (1=идеально, <1 теряет скорость)

    double last = glfwGetTime();

    while(!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        float dt = (float)(now - last);
        last = now;
        if(dt > 0.05f) dt = 0.05f; // защита от больших скачков

        // ввод -> ускорение
        Vec2 a{0,0};
        if(glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) a.x -= accel;
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) a.x += accel;
        if(glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) a.y -= accel;
        if(glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) a.y += accel;

        // физика
        vel = vadd(vel, vmul(a, dt));
        pos = vadd(pos, vmul(vel, dt));

        // отталкивание от стенок (учитываем размер квадрата)
        float minX = -border + half, maxX = border - half;
        float minY = -border + half, maxY = border - half;

        if(pos.x < minX){ pos.x = minX; vel.x = -vel.x * bounce; }
        if(pos.x > maxX){ pos.x = maxX; vel.x = -vel.x * bounce; }
        if(pos.y < minY){ pos.y = minY; vel.y = -vel.y * bounce; }
        if(pos.y > maxY){ pos.y = maxY; vel.y = -vel.y * bounce; }

        // замедление каждый кадр
        vel.x *= damping;
        vel.y *= damping;

        // рисование
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(5.0f);
        glColor3f(0.0f, 0.0f, 0.0f);
        drawSquare(pos, half);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
