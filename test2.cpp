#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <string>
#include <iostream>

struct Vec3 { float x,y,z; };
static inline Vec3 v3(float x,float y,float z){ return {x,y,z}; }

static int gW=900, gH=700;

// -------- мини-текст (цифры + минус + точка) линиями в 2D --------
// Рисуем в NDC поверх сцены. Это “без заморочек”, но выполняет “подписи значений”.
static void drawDigitLines(float x, float y, float s, char ch)
{
    // 7-seg: segments A,B,C,D,E,F,G
    //  A
    // F B
    //  G
    // E C
    //  D
    bool A=0,B=0,C=0,D=0,E=0,F=0,G=0;
    if(ch=='0'){A=B=C=D=E=F=1;}
    else if(ch=='1'){B=C=1;}
    else if(ch=='2'){A=B=G=E=D=1;}
    else if(ch=='3'){A=B=G=C=D=1;}
    else if(ch=='4'){F=G=B=C=1;}
    else if(ch=='5'){A=F=G=C=D=1;}
    else if(ch=='6'){A=F=G=C=D=E=1;}
    else if(ch=='7'){A=B=C=1;}
    else if(ch=='8'){A=B=C=D=E=F=G=1;}
    else if(ch=='9'){A=B=C=D=F=G=1;}
    else if(ch=='-'){G=1;}
    else if(ch=='.'){
        glBegin(GL_LINES);
        glVertex2f(x + 0.80f*s, y - 1.05f*s);
        glVertex2f(x + 0.90f*s, y - 1.05f*s);
        glEnd();
        return;
    } else {
        return;
    }

    auto seg = [&](float x1,float y1,float x2,float y2){
        glVertex2f(x + x1*s, y + y1*s);
        glVertex2f(x + x2*s, y + y2*s);
    };

    glBegin(GL_LINES);
    if(A) seg(0.10f, 1.00f, 0.90f, 1.00f);
    if(B) seg(0.90f, 1.00f, 0.90f, 0.00f);
    if(C) seg(0.90f, 0.00f, 0.90f,-1.00f);
    if(D) seg(0.10f,-1.00f, 0.90f,-1.00f);
    if(E) seg(0.10f, 0.00f, 0.10f,-1.00f);
    if(F) seg(0.10f, 1.00f, 0.10f, 0.00f);
    if(G) seg(0.10f, 0.00f, 0.90f, 0.00f);
    glEnd();
}

static void drawText2D(float x, float y, float s, const std::string& text)
{
    // x,y in NDC
    float cx = x;
    for(char ch: text){
        drawDigitLines(cx, y, s, ch);
        cx += 1.2f*s;
    }
}

// -------- эллипсоид: аналитика -> треугольники --------
struct Vtx { Vec3 p; Vec3 n; };

static std::vector<Vtx> mesh;
static int stacks = 40;
static int slices = 60;

static void buildEllipsoid(float a, float b, float c)
{
    mesh.clear();
    mesh.reserve(stacks*slices*6);

    for(int i=0;i<stacks;i++){
        float v0 = (float)i / stacks;      // 0..1
        float v1 = (float)(i+1) / stacks;  // 0..1
        float th0 = v0 * (float)M_PI;      // 0..pi
        float th1 = v1 * (float)M_PI;

        for(int j=0;j<slices;j++){
            float u0 = (float)j / slices;      // 0..1
            float u1 = (float)(j+1) / slices;
            float ph0 = u0 * 2.0f*(float)M_PI; // 0..2pi
            float ph1 = u1 * 2.0f*(float)M_PI;

            auto pos = [&](float th,float ph){
                // x=a sin(th) cos(ph), y=b sin(th) sin(ph), z=c cos(th)
                return v3(a*sinf(th)*cosf(ph), b*sinf(th)*sinf(ph), c*cosf(th));
            };

            auto normal = [&](Vec3 p){
                // Для эллипсоида нормаль пропорциональна (x/a^2, y/b^2, z/c^2)
                float nx = p.x/(a*a);
                float ny = p.y/(b*b);
                float nz = p.z/(c*c);
                float len = std::sqrt(nx*nx+ny*ny+nz*nz);
                if(len<1e-6f) return v3(0,0,1);
                return v3(nx/len, ny/len, nz/len);
            };

            Vec3 p00 = pos(th0, ph0);
            Vec3 p10 = pos(th0, ph1);
            Vec3 p01 = pos(th1, ph0);
            Vec3 p11 = pos(th1, ph1);

            // 2 треугольника на “квад”
            mesh.push_back({p00, normal(p00)});
            mesh.push_back({p01, normal(p01)});
            mesh.push_back({p11, normal(p11)});

            mesh.push_back({p00, normal(p00)});
            mesh.push_back({p11, normal(p11)});
            mesh.push_back({p10, normal(p10)});
        }
    }
}

static void framebuffer_size_callback(GLFWwindow*, int w, int h){
    gW = (w>1? w:1); gH=(h>1? h:1);
    glViewport(0,0,gW,gH);
}

static void setupProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // простая перспектива без GLU:
    float aspect = (float)gW/(float)gH;
    float fovy = 60.0f;
    float n = 0.1f, f = 50.0f;
    float t = n * tanf((fovy*0.5f) * (float)M_PI/180.0f);
    float b = -t;
    float r = t*aspect;
    float l = -r;

    // frustum matrix
    float M[16] = {
        2*n/(r-l), 0,           0,                 0,
        0,         2*n/(t-b),   0,                 0,
        (r+l)/(r-l),(t+b)/(t-b),-(f+n)/(f-n),     -1,
        0,         0,           -2*f*n/(f-n),      0
    };
    glLoadMatrixf(M);

    glMatrixMode(GL_MODELVIEW);
}

static void drawAxesWithLabels(bool on)
{
    if(!on) return;

    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);

    // Оси в 3D
    glBegin(GL_LINES);
    // X red
    glColor3f(1,0,0);
    glVertex3f(0,0,0); glVertex3f(1.2f,0,0);
    // Y green
    glColor3f(0,1,0);
    glVertex3f(0,0,0); glVertex3f(0,1.2f,0);
    // Z blue
    glColor3f(0,0,1);
    glVertex3f(0,0,0); glVertex3f(0,0,1.2f);
    glEnd();

    // “Подписи значений” — как 2D-оверлей (NDC).
    // Пишем только цифры, чтобы было просто.
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0,0,0);
    drawText2D(-0.95f,  0.90f, 0.02f, "1.0"); // пример шкалы
    drawText2D(-0.95f,  0.84f, 0.02f, "0.5");
    drawText2D(-0.95f,  0.78f, 0.02f, "0.0");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LIGHTING);
}

int main()
{
    if(!glfwInit()) return 0;
    GLFWwindow* win = glfwCreateWindow(gW,gH,"Ellipsoid",nullptr,nullptr);
    if(!win){ glfwTerminate(); return 0; }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
    framebuffer_size_callback(win,gW,gH);

    // Параметры эллипсоида (аналитика)
    float a=1.0f, b=0.7f, c=0.4f;
    buildEllipsoid(a,b,c);

    // Трансформации
    float rotX=20, rotY=-30, rotZ=0;
    float scale=1.0f;
    float sx=1.0f, sy=1.0f, sz=1.0f;

    // Цвет/свет
    float color[3] = {0.2f, 0.6f, 0.9f};
    float lightDiffuse = 0.9f;
    float lightSpec    = 0.6f;
    float shininess    = 32.0f;

    bool axesOn = true;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // важно при scale

    while(!glfwWindowShouldClose(win))
    {
        // ---- управление ----
        auto down = [&](int key){ return glfwGetKey(win,key)==GLFW_PRESS; };

        if(down(GLFW_KEY_LEFT))  rotY -= 1.2f;
        if(down(GLFW_KEY_RIGHT)) rotY += 1.2f;
        if(down(GLFW_KEY_UP))    rotX -= 1.2f;
        if(down(GLFW_KEY_DOWN))  rotX += 1.2f;
        if(down(GLFW_KEY_Q))     rotZ -= 1.2f;
        if(down(GLFW_KEY_E))     rotZ += 1.2f;

        if(down(GLFW_KEY_W)) scale *= 1.01f;
        if(down(GLFW_KEY_S)) scale *= 0.99f;

        if(down(GLFW_KEY_A)) sx *= 0.99f;
        if(down(GLFW_KEY_D)) sx *= 1.01f;
        if(down(GLFW_KEY_Z)) sy *= 0.99f;
        if(down(GLFW_KEY_C)) sy *= 1.01f;
        if(down(GLFW_KEY_R)) sz *= 1.01f;
        if(down(GLFW_KEY_F)) sz *= 0.99f;

        if(down(GLFW_KEY_1)){ color[0]=1; color[1]=0.2f; color[2]=0.2f; }
        if(down(GLFW_KEY_2)){ color[0]=0.2f; color[1]=1; color[2]=0.2f; }
        if(down(GLFW_KEY_3)){ color[0]=0.2f; color[1]=0.2f; color[2]=1; }

        if(down(GLFW_KEY_I)) lightDiffuse = std::min(1.5f, lightDiffuse + 0.01f);
        if(down(GLFW_KEY_K)) lightDiffuse = std::max(0.0f, lightDiffuse - 0.01f);

        if(down(GLFW_KEY_O)) lightSpec = std::min(1.5f, lightSpec + 0.01f);
        if(down(GLFW_KEY_L)) lightSpec = std::max(0.0f, lightSpec - 0.01f);

        if(down(GLFW_KEY_P)) shininess = std::min(128.0f, shininess + 1.0f);
        if(down(GLFW_KEY_SEMICOLON)) shininess = std::max(1.0f, shininess - 1.0f);

        static bool xPrev=false;
        bool xNow = down(GLFW_KEY_X);
        if(xNow && !xPrev) axesOn = !axesOn;
        xPrev = xNow;

        // ---- рисование ----
        glClearColor(1,1,1,1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupProjection();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0,0,-3.2f);

        // свет
        float lightPos[4] = {2.0f, 2.0f, 2.0f, 1.0f};
        float diff[4] = {lightDiffuse, lightDiffuse, lightDiffuse, 1.0f};
        float spec[4] = {lightSpec, lightSpec, lightSpec, 1.0f};
        float amb[4]  = {0.15f, 0.15f, 0.15f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  diff);
        glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);

        // материал
        float matDiff[4] = {color[0], color[1], color[2], 1.0f};
        float matSpec[4] = {1,1,1,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

        // трансформации эллипсоида
        glRotatef(rotX,1,0,0);
        glRotatef(rotY,0,1,0);
        glRotatef(rotZ,0,0,1);
        glScalef(scale*sx, scale*sy, scale*sz);

        // оси (в мировых координатах до меша — можно рисовать и после, но так нагляднее)
        drawAxesWithLabels(axesOn);

        // сам эллипсоид (треугольники)
        glBegin(GL_TRIANGLES);
        for(const auto& v : mesh){
            glNormal3f(v.n.x, v.n.y, v.n.z);
            glVertex3f(v.p.x, v.p.y, v.p.z);
        }
        glEnd();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
