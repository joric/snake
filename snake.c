/* omg teh rule snake by joric
win: cl snake.c /link glut32.lib; penguin: gcc snake.c -lglut
used to be $20 rentacoder task, outbid and failed by Gulfam Shabbir
*/

#define FREEGLUT_STATIC
#include <GL/glut.h>

float zoom = 35.0f;
float rotx = 45.0f;
float roty = 0.001f;
float tx = 0;
float ty = 0;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = { 0 };

static const int w = 20;
static const int h = 20;

int data[20][20];

#define W 640
#define H 480
#define FOOD -1

int cx, cy, dx, dy, timerrate, items, level, length;

void display(void)
{
    int i, j;

    float c1[] = { 1, 1, 0 };
    float c2[] = { 1, 0, 0 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glTranslatef(0, 0, -zoom);
    glTranslatef(tx, ty, 0);
    glRotatef(rotx, 1, 0, 0);
    glRotatef(roty, 0, 1, 0);

    glDisable(GL_LIGHTING);

    /* draw grid */
    {
        int i,j;

        glColor3f(0.2f, 0.2f, 0.2f);
        glBegin(GL_LINES);
        for (i = -w / 2; i <= w / 2; ++i)
        {
            glVertex3f(i*1.0f, -0.5f, -w / 2.0f);
            glVertex3f(i*1.0f, -0.5f, w / 2.0f);
        }

        for (j = -h / 2; j <= h / 2; ++j)
        {
            glVertex3f(h / 2.0f, -0.5f, j*1.0f);
            glVertex3f(-h / 2.0f, -0.5f, j*1.0f);
        }
        glEnd();
    }

    glEnable(GL_LIGHTING);

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            int n = data[i][j];

            float x = i - w / 2 + 0.5f;
            float y = j - h / 2 + 0.5f;

            glPushMatrix();
            glTranslatef(y, 0, x);

            if (n > 0)
            {
                glLightfv(GL_LIGHT0, GL_AMBIENT, c1);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, c1);
            }

            if (n == FOOD)
            {
                glLightfv(GL_LIGHT0, GL_AMBIENT, c2);
                glLightfv(GL_LIGHT0, GL_DIFFUSE, c2);
            }

            if (n != 0)
                /* glutSolidCube(1);
                glutSolidSphere(0.5, 20, 10); */
                glutSolidTeapot(0.5);

            glPopMatrix();
        }
    }

    glutSwapBuffers();
}

void loadlevel(int level)
{
    int i, j;
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            data[i][j] = 0;
        }
    }
}

void reset()
{
    level = 1;
    loadlevel(level);
    length = 1;
    items = 0;
    cx = w / 2;
    cy = h / 2;
    dx = 0;
    dy = 0;
    timerrate = 125;
}

void tick(int value)
{
    int i, j;

    cx += dx;
    cy += dy;

    if (cx >= w)
        cx = 0;
    if (cx < 0)
        cx = w - 1;

    if (cy >= h)
        cy = 0;
    if (cy < 0)
        cy = h - 1;

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            if (data[i][j] > 0)
                data[i][j]--;
        }
    }

    if (data[cy][cx] == FOOD)
    {
        length++;
        items--;
    }
    else if (data[cy][cx] != 0 && (dx != 0 || dy != 0))
    {
        reset();
    }

    data[cy][cx] = length;

    if (items == 0)
    {
        int x, y;
        int i = 0;
        do
        {
            x = rand() % w;
            y = rand() % h;
            i++;
        }
        while (data[y][x] != 0 && i < 5);

        data[y][x] = FOOD;

        items++;
    }

    glutTimerFunc(timerrate, tick, 1);
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    if (w == 0)
        h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w / h, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void motion(int x, int y)
{
    int diffx = x - lastx;
    int diffy = y - lasty;
    lastx = x;
    lasty = y;

    if (Buttons[2])
    {
        zoom -= (float)0.05f *(diffx - diffy);
    }
    else if (Buttons[0])
    {
        rotx += (float)0.5f *diffy;
        roty += (float)0.5f *diffx;
    }
    else if (Buttons[1])
    {
        tx += (float)0.05f *diffx;
        ty -= (float)0.05f *diffy;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;
    }
}

void special(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            dx = 0;
            dy = -1;
            break;
        case GLUT_KEY_DOWN:
            dx = 0;
            dy = 1;
            break;
        case GLUT_KEY_LEFT:
            dx = -1;
            dy = 0;
            break;
        case GLUT_KEY_RIGHT:
            dx = 1;
            dy = 0;
            break;
    }
}

void mouse(int b, int s, int x, int y)
{
    lastx = x;
    lasty = y;
    switch (b)
    {
        case GLUT_LEFT_BUTTON:
            Buttons[0] = ((GLUT_DOWN == s) ? 1 : 0);
            break;
        case GLUT_MIDDLE_BUTTON:
            Buttons[1] = ((GLUT_DOWN == s) ? 1 : 0);
            break;
        case GLUT_RIGHT_BUTTON:
            Buttons[2] = ((GLUT_DOWN == s) ? 1 : 0);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - W) / 2,
        (glutGet(GLUT_SCREEN_HEIGHT) - H) / 2);
    glutInitWindowSize(W, H);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    reset();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glutTimerFunc(timerrate, tick, 1);
    glutMainLoop();
    return 0;
}
