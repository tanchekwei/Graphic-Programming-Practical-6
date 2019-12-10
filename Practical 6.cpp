#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

#include <cstdio>

int rotate = 0;
bool pyramid = false;
bool light = true;

float diffuseColor[] = {1.0f, 0.0f, 0.0f};
float diffusePosition[] = {0.0f, 1.0f, 0.0f};
float objectColor[] = {0.0f, 1.0f, 0.0f};

double w = 1920;
double h = 1080;
double ar = w / h; // aspect ratio

// use dedicated GPU to run
extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}
extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

void controls(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
        else if (key == GLFW_KEY_SPACE)
        {
            if (light)
                light = false;
            else
                light = true;
        }
        else if (key == GLFW_KEY_UP)
        {
            if (rotate == 0)
                rotate = 1;
            else
                rotate = 0;
        }
        else if (key == GLFW_KEY_DOWN)
        {
            if (rotate == 0)
                rotate = 2;
            else
                rotate = 0;
        }
        else if (key == GLFW_KEY_D)
        {
            diffusePosition[0] += 0.5;
        }
        else if (key == GLFW_KEY_A)
        {
            diffusePosition[0] -= 0.5;
        }
        else if (key == GLFW_KEY_W)
        {
            diffusePosition[1] += 0.5;
        }
        else if (key == GLFW_KEY_S)
        {
            diffusePosition[1] -= 0.5;
        }
        else if (key == GLFW_KEY_E)
        {
            diffusePosition[2] += 0.5;
        }
        else if (key == GLFW_KEY_Q)
        {
            diffusePosition[2] -= 0.5;
        }
        else if (key == GLFW_KEY_O)
        {
            pyramid = false;
        }
        else if (key == GLFW_KEY_P)
        {
            pyramid = true;
        }
}

GLFWwindow *initWindow(const int resX, const int resY)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return NULL;
    }
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

    // Open a window and create its OpenGL context
    // GLFWwindow *window = glfwCreateWindow(resX, resY, "Jaeger", glfwGetPrimaryMonitor(), NULL);
    GLFWwindow *window = glfwCreateWindow(resX, resY, "Practical 6", NULL, NULL);

    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, controls);

    // Get info of GPU and supported OpenGL version
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST); // Depth Testing
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return window;
}

void drawPyramid()
{
	glPushMatrix();
	glColor3ub(180, 255, 145);
	glTranslatef(0, -0.5f, 0);
	//glRotatef(rotateDeg, 1, 1, 1);
	glBegin(GL_POLYGON);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(-0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 1, -0.5);
	glVertex3f(0.5, 0, -1.0);
	glVertex3f(-0.5, 0, -1.0);

	glColor3f(0, 1, 0);
	glVertex3f(-0.5, 0, 0);
	glVertex3f(0.5, 0, 0);
	glVertex3f(0.5, 0, -1.0);
	glVertex3f(-0.5, 0, -1.0);

	glEnd();
	glPopMatrix();
}

void drawSphere(){
        GLUquadricObj *sphere = NULL;
        sphere = gluNewQuadric();
        glMaterialfv(GL_FRONT, GL_AMBIENT, objectColor);
        glLineWidth(4);
        gluQuadricDrawStyle(sphere, GLU_LINE);
        gluSphere(sphere, 0.8, 50, 50);
        gluDeleteQuadric(sphere);

}

void display()
{
    // glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.00, 0.90, 0.90, 0);
    // glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
    // glLightfv(GL_LIGHT0, GL_POSITION, ambPos);
    // glEnable(GL_LIGHT0);

    if (light)
    {
        glPushMatrix();
        {
            glLoadIdentity();
            glEnable(GL_LIGHTING);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
            glLightfv(GL_LIGHT0, GL_POSITION, diffusePosition);
            glEnable(GL_LIGHT0);
        }
        glPopMatrix();
    }

    if (rotate == 1)
    {
        glRotatef(-1, 1, 1, 1);
    }
    else if (rotate == 2)
    {
        glRotatef(1, 1, 1, 1);
    }

    if (!pyramid)
    {
        drawSphere();
    }
    else
    {
        drawPyramid();
    }

    glPushMatrix();
    {
        glLoadIdentity();
        glMaterialfv(GL_FRONT, GL_AMBIENT, objectColor);
        glDisable(GL_LIGHT0);
        GLUquadricObj *sphere = NULL;
        sphere = gluNewQuadric();
        glTranslatef(diffusePosition[0], diffusePosition[1], diffusePosition[2]);
        // glPointSize(2);
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluSphere(sphere, 0.1, 50, 50);
        gluDeleteQuadric(sphere);
    }
    glPopMatrix();
}

int main(int argc, char **argv)
{
    GLFWwindow *window = initWindow(1920, 1080);

    if (NULL != window)
    {
        // handle resolution problem
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double w = 1920;
        double h = 1080;
        double ar = w / h;
        glOrtho(-2 * ar, 2 * ar, -2, 2, -1, 10);

        glMatrixMode(GL_MODELVIEW);

        while (!glfwWindowShouldClose(window))
        {
            // Scale to window size
            GLint windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glViewport(0, 0, windowWidth, windowHeight);

            display();

            // if (rotate)
            //     speed += 1;
            // else
            //     speed = speed;

            // Update Screen
            glfwSwapBuffers(window);

            // Check for any input, or window movement
            glfwPollEvents();
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
