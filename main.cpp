#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <GL/glut.h>
#define pi (2*acos(0.0))
#define sqrt2 (sqrt(2))

typedef struct point
{
    double x,y,z;
} Point;

bool drawAxes = true;
float gateAngle = 0;
float doorAngle = 0; //rotate for clr match
float quadAngle = 0;
float boxAngle = 0;
float tyreAngle = 0;
float windowAngle=0;
float uprotate=90;
float boxScaleFactor = 1;
float tyreScaleFactor = 1;
float tinyBoxAngle = 0;
float background = 0;
float winangle=0;
float winangle1=0;

Point pos, u, r, l;
int var;
int refresh =10;
void timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc (refresh,timer,0) ;

}
void displayAxes()
{
    if(drawAxes)
    {
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 0, 0);

            glVertex3f( 500,0,0); // a point on pos x axis
            glVertex3f(-500,0,0); // a point on neg x axis

            glColor3f(0, 1.0, 0);

            glVertex3f(0,-500,0); // a point on pos y axis
            glVertex3f(0, 500,0); // a point on neg y axis

            glColor3f(0, 0, 1.0);

            glVertex3f(0,0, 500);  // a point on pos z axis
            glVertex3f(0,0,-500);   // a point on neg z axix
        }
        glEnd();
    }
}

Point func(Point vect, Point perp, int dir)
{
    double c = cos(pi/180);
    double s = dir * sin(pi/180);
    Point point;
    point.x = c * vect.x + s * perp.x;
    point.y = c * vect.y + s * perp.y;
    point.z = c * vect.z + s * perp.z;
    c = sqrt(point.x*point.x + point.y*point.y + point.z*point.z);
    point.x /= c;
    point.y /= c;
    point.z /= c;
    return point;
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {
    case '1':
    {
        Point l1 = func(l, r, -1);
        r = func(r, l, 1);
        l = l1;
        break;
    }
    case '2':
    {
        Point l1 = func(l, r, 1);
        r = func(r, l, -1);
        l = l1;
        break;
    }

    case '3':
    {
        Point u1 = func(u, l, -1);
        l = func(l, u, 1);
        u = u1;
        break;
    }
    case '4':
    {
        Point u1 = func(u, l, 1);
        l = func(l, u, -1);
        u = u1;
        break;
    }
    case '5':
    {
        Point r1 = func(r, u, -1);
        u = func(u, r, 1);
        r = r1;
        break;
    }
    case '6':
    {
        Point r1 = func(r, u, 1);
        u = func(u, r, -1);
        r = r1;
        break;
    }
    case '7':
    {
        tyreScaleFactor += 0.2;
        break;
    }
    case '8':
    {
        tyreScaleFactor -= 0.2;
        break;
    }
     case '9':
    {
        if (windowAngle > -80  )
            windowAngle -= 5;
        else
            windowAngle = -80;
        break;
    }
    case '0':
    {
        if (windowAngle < 0  )
            windowAngle += 5;
        else
            windowAngle = 0;
        break;
    }


    case  'l' :
    case  'L' :
    {
        boxAngle -= 5;
        break;
    }

    case  'r' :
    case  'R' :
    {
        boxAngle += 5;
        break;
    }
    case 'u':
    case 'U':
    {
        if( int (boxScaleFactor)==2)
            break;
        else
        {
            boxScaleFactor += 0.2;
            break;
        }
    }
    case 'd':
    case 'D':
    {
        if (boxScaleFactor > 0)
        {
            boxScaleFactor -= 0.2;
            if (boxScaleFactor < 0)
                boxScaleFactor =0;
            break;

        }

    }
    default:
    break;
    }
}

point camerapos(Point vect, Point perp, int dir){
vect.x = (vect.x + 5 * dir*perp.x);
vect.y = (vect.y + 5 * dir*perp.y);
vect.z = (vect.z + 5 * dir*perp.z);
return vect;
}
void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN: // down arrow
    {
        pos = camerapos(pos,l,-1);
        break;
    }
    case GLUT_KEY_UP:   // up arrow
    {
        pos = camerapos(pos,l,1);
        break;

    }

    case GLUT_KEY_RIGHT:    // right arrow
    {
       pos = camerapos(pos,r,1);
        break;
    }

    case GLUT_KEY_LEFT:     // left arrow
    {
        pos = camerapos(pos,r,-1);
        break;
    }

    case GLUT_KEY_PAGE_UP:
    {
        pos = camerapos(pos,u,1);
        break;
    }

    case GLUT_KEY_PAGE_DOWN:
    {
        pos = camerapos(pos,u,-1);
        break;
    }
    case GLUT_KEY_HOME:   //Door open
    {
        if (gateAngle > -50  )
            gateAngle -= 5;
        else
            gateAngle = -50;
        break;
    }

    case GLUT_KEY_END:  //door close
    {
        if (gateAngle < 0)
            gateAngle += 5;
        else
            break;
    }
        default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN)
        {
            printf("Mouse Left Button Clicked\n");
        }
        else if(state == GLUT_UP)
        {
            printf("Mouse Left Button Released\n");
        }
        break;
    default:
        break;
    }
}

//tyre display
void displayTyre(float tyreRadius, float tyreWidth,int id)
{
    int tyreSlices = 10;
    float sliceWidth = (2 * M_PI * tyreRadius) / tyreSlices;
    float sliceAngle = 360 / tyreSlices;

    float halfTyreWidth = tyreWidth / 2, quartTyreWidth = halfTyreWidth / 2;
    float halfSliceWidth = sliceWidth / 2;

    if(id==1) ///Spikes inside the tyre
    {
        for(int i=0; i<tyreSlices; i++)
        {
            if (i%2 ==0)
            {
                    glColor3f(1, 0, 0);

            }
           /* else  if (i%3 ==0)
            {
                    glColor3f(0, 1, 0);

            }*/
            else
            {
                    glColor3f(0, 0, 1);

            }
            glPushMatrix();
            glRotatef(sliceAngle*i, 1, 0, 0);
           glBegin(GL_QUADS);
            {
                    glVertex3f(halfTyreWidth,-tyreRadius/2, 0);
                    glVertex3f(-halfTyreWidth, -tyreRadius/2, 0);
                    glVertex3f(-halfTyreWidth, -tyreRadius, 0);
                    glVertex3f(halfTyreWidth, -tyreRadius, 0);
            }
    glEnd();
    glPopMatrix();

        }
    }

    for(int i=0; i<tyreSlices; i++)
    {
        glPushMatrix();
        glRotatef(sliceAngle*i, 1, 0, 0);
        glTranslatef(0, tyreRadius, 0);
        glBegin(GL_QUADS);
        {
            if (i%2 ==0)
            {
                    glColor3f(0, 0, 0);
            }
            else
            {
                    glColor3f(1, 1, 1);
            }
            glVertex3f(halfTyreWidth, 0, halfSliceWidth);
            glVertex3f(halfTyreWidth, 0 ,-halfSliceWidth);
            glVertex3f(-halfTyreWidth, 0 ,-halfSliceWidth);
            glVertex3f(-halfTyreWidth,  0, halfSliceWidth);
        }
        glEnd();
        glPopMatrix();
    }
}


void displayBox1(float boxLength, float boxWidth, float boxHeight) // length -> x axis, width -> yaxis, height -> z axis (floor)
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;
    glBegin(GL_QUADS);
    {
        glColor3f(0, 0, .8);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0, 0, 0.5);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}
void displayBox2(float boxLength, float boxWidth, float boxHeight) // length -> x axis, width -> yaxis, height -> z axis wall
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(1, 0, 0);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();

}
void displayBox3(float boxLength, float boxWidth, float boxHeight) // upper stair
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.98, 0.89, 0.12);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.65, 0.65, 0);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}
void displayBox4(float boxLength, float boxWidth, float boxHeight) // middle stair
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.4, 1, 0.8);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.094, 0.623, 0.639);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
    }
    glEnd();
}
void displayBox5(float boxLength, float boxWidth, float boxHeight) // down stair
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.760, 0.792, 0.8);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.490, 0.505, 0.509);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}
void displayBox6(float boxLength, float boxWidth, float boxHeight) // roof
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0, 1, 0);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.129, 0.490, 0.007);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}
void displayBox7(float boxLength, float boxWidth, float boxHeight) // door1 self
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.2, 0.8, 0.8);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.007, 0.458, 0.490);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}
void displayBox8(float boxLength, float boxWidth, float boxHeight) // length -> x axis, width -> yaxis, height -> z axis door2 self
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.2, 0.8, 0.8);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glColor3f(0.007, 0.458, 0.490);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}
void displayBox9(float boxLength, float boxWidth, float boxHeight) // length -> x axis, width -> yaxis, height -> z axis fan wall
{
    float halfBoxLength = boxLength / 2, halfBoxWidth = boxWidth / 2, halfBoxHeight = boxHeight / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0.709, 0.027, 0.631);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}

void displayBox10(float boxLength, float boxWidth, float boxHeight) // length -> x axis, width -> yaxis, height
{
    float halfBoxLength = boxLength, halfBoxWidth = boxWidth, halfBoxHeight = boxHeight;

    glBegin(GL_QUADS);
    {
        glColor3f(0, 0, 1);
        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, halfBoxHeight);

        glVertex3f(halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, halfBoxWidth, -halfBoxHeight);
        glVertex3f(-halfBoxLength, -halfBoxWidth, -halfBoxHeight);
        glVertex3f(halfBoxLength, -halfBoxWidth, -halfBoxHeight);

    }
    glEnd();
}




void displaybackdoor(float quadLength, float quadWidth, int id ) // width along x axis, length along z axis
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;
    float r,g,b,r1,r2,r3;
    r=0,g=1,b=0;
    r1=1,r2=1,r3=1;
    glBegin(GL_QUADS);
    {
        if(id ==1)
        {
            glColor3f(r1,r2, r3);
            glVertex3f(-quadWidth, 0, halfQuadLength);
            glVertex3f(-quadWidth, 0, -halfQuadLength);
            glColor3f(r,g,b);
            glVertex3f(0,0, -halfQuadLength);
            glVertex3f(0, 0, halfQuadLength);
        }
        else
        {
            glColor3f(r1,r2, r3);
            glVertex3f(quadWidth, 0, halfQuadLength);
            glVertex3f(quadWidth, 0, -halfQuadLength);
            glColor3f(r,g,b);
            glVertex3f(0,0, -halfQuadLength);
            glVertex3f(0, 0, halfQuadLength);
        }
    }
    glEnd();

}
// door display
void Door(float quadLength, float quadWidth) // width along x axis, length along z axis  // front door
{
    float halfQuadWidth = quadWidth / 2, halfQuadLength = quadLength / 2;

    glBegin(GL_QUADS);
    {
        glColor3f(0, 0.2, 0.8);
        glVertex3f(halfQuadWidth, 0, halfQuadLength);
        glVertex3f(halfQuadWidth, 0, -halfQuadLength);

        glColor3f(0, 1, 1);
        glVertex3f(-halfQuadWidth, 0, -halfQuadLength);
        glVertex3f(-halfQuadWidth, 0, halfQuadLength);

    }
    glEnd();

}

void Window() // width along x axis, length along z axis
{

   glBegin(GL_QUADS);
    {
        glColor3f(1, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -70, 0);
        glVertex3f(0,-70, -65);
        glVertex3f(0, 0, -65);

    }
    glEnd();
}

void displayScene()
{
    float quadLength = 130, quadWidth = 55; //front door
    float quadLength1 = 110, quadWidth1 = 10,quadHeight1=10;//front doorupside
    float quadLength4 = 70, quadWidth4 = 10,quadHeight4=10;   //left window upside height
    float quadLength2 = 130, quadWidth2 = 55; //back door
    float quadLength3 = 110, quadWidth3 = 10,quadHeight3=10;;//back door upside
    float largeBoxLength = 300, largeBoxWidth = 300, largeBoxHeight = 30;// floor
    float smallBoxLength = 250, smallBoxWidth = 250, smallBoxHeight = 200;  //wall
    float sBoxLength = 100, sBoxWidth = 70, sBoxHeight = 10;         //stair
    float sBoxLength1 = 110, sBoxWidth1 = 80, sBoxHeight1 = 10;     // middle stair
    float sBoxLength2 = 120, sBoxWidth2 = 90, sBoxHeight2 = 10;           /** down stair **/
    float uBoxLength = 260, uBoxWidth = 270, uBoxHeight = 8;         /** roof measurement **/
     float uBoxLength1 = 120, uBoxWidth1 = 30, uBoxHeight1 = 150;      /** fan wall measurement **/
     float tyreRadius = 50, tyreWidth = 20;


    displayAxes();


   //rotation house
    glScalef(boxScaleFactor, boxScaleFactor, boxScaleFactor);
    glRotatef(boxAngle, 0, 0, 1); // roytates with respect to z axis

    /** floor display **/
    displayBox1(largeBoxLength, largeBoxWidth, largeBoxHeight);

    //wall
    glPushMatrix();
    glTranslatef(0, 0, (smallBoxHeight/2) + (largeBoxHeight/2)); // places the red box on top of the blue box
    displayBox2(smallBoxLength, smallBoxWidth, smallBoxHeight); // red box or wall
    glPopMatrix();

    //stairs
    glPushMatrix();
    glTranslatef(0, (largeBoxWidth/2), ((largeBoxHeight/2)-6) );
    displayBox3(sBoxLength,sBoxWidth, sBoxHeight);
    glPopMatrix();
    //middle stair
    glPushMatrix();
    glTranslatef(0, (largeBoxWidth/2), 0 );
    //glTranslatef((0,largeBoxWidth/2),0 , 0 );
    displayBox4(sBoxLength1,sBoxWidth1, sBoxHeight1);
    glPopMatrix();

    //upsideBox (down stair)
    glPushMatrix();
    glTranslatef(0, (largeBoxWidth/2), -((largeBoxHeight/2)-6) );
    displayBox5(sBoxLength2,sBoxWidth2, sBoxHeight2);
    //displayBox5(sBoxLength2,sBoxWidth2, sBoxHeight2);
    glPopMatrix();

    //roof
    glPushMatrix();
    glTranslatef(0, 0, smallBoxHeight+21 );
    displayBox6(uBoxLength,uBoxWidth, uBoxHeight);
    glPopMatrix();
    // fan wall
    glPushMatrix();
    glTranslatef(0, 0, smallBoxHeight+90 );
    displayBox9(uBoxLength1,uBoxWidth1, uBoxHeight1);
    glPopMatrix();

//left window
     glPushMatrix();
     glTranslatef(128,50,130);
    // glTranslatef(0,50,0);
    // glTranslatef(0,0,130);
     glRotatef(windowAngle, 0,1,0);  // window up/down
     Window(); //left window display
     glPopMatrix();

    // left window upside
     glPushMatrix();
     glRotatef(uprotate,0,0,1);
    glTranslatef(13, -130, 135 );
    //glTranslatef(0, 0, 135 );
    //glTranslatef(0, -130, 0 );
    displayBox7(quadLength4,quadWidth4, quadHeight4);
    glPopMatrix();

    //back/ right window
    glPushMatrix();
    glTranslatef(-128,50,130);
    //glTranslatef(0,50,0);
   // glTranslatef(0,0,130);
    glTranslatef (0.0,0.0,winangle);  //window up z axes
    //glRotatef(winangle1=180,0,1,0);
    Window();
    glPopMatrix();

    //back/ right window up
    glPushMatrix();
     glRotatef(uprotate,0,0,1);
    glTranslatef(15, 0, 0 );
    glTranslatef(0, 0, 135 );
    glTranslatef(0, 130, 0 );
    displayBox7(quadLength4,quadWidth4, quadHeight4);
    glPopMatrix();


    //front door1 right part
    glPushMatrix();
    glTranslatef( -26,(quadLength/2.5) + (smallBoxHeight/2.5), 84);  //right part position show
    glTranslatef (gateAngle,0.0,0.0);  // right part slide off
    glRotatef(doorAngle=-180, 0,0,1);  // color combination
    Door(quadLength, quadWidth);
    glPopMatrix();
//front door1 left part
    glPushMatrix();
    glTranslatef( 26,(quadLength/2.5) + (smallBoxHeight/2.5), 84);   //left part position show
    glTranslatef (-gateAngle,0.0,0.0);   // left part slide off
    Door(quadLength, quadWidth);                    //left part function call
    glPopMatrix();

    //frontdoor upside
    glPushMatrix();
    glTranslatef(0, quadWidth+75, quadLength+24 );
    displayBox7(quadLength1,quadWidth1, quadHeight1);
    glPopMatrix();

    //back door2 left part
    glPushMatrix();
    glTranslatef( -55,-((quadLength2/2.5) + (smallBoxHeight/2.5)),84);
    glRotatef(quadAngle, 0, 0,-1);   //left side rotate
    displaybackdoor(quadLength2, quadWidth2,2);  // display back door left
    glPopMatrix();
//back door2 right part
    glPushMatrix();
    glTranslatef( 55,-((quadLength2/2.5) + (smallBoxHeight/2.5)),84);
    glRotatef(quadAngle, 0,0,1);   // right side rotate
    displaybackdoor(quadLength2, quadWidth2,1);  // // display back door right
    glPopMatrix();


    //backdoor upside
    glPushMatrix();
    glTranslatef(0, -(quadWidth+75), quadLength+24 );
    displayBox8(quadLength3,quadWidth3, quadHeight3);
    glPopMatrix();


   //tyre large display
    glPushMatrix();
    glTranslatef(0,40,0);
    glTranslatef(0,0,290);// places the tyre on top of both boxes
    glRotatef(tyreAngle, 0, 1, 0); //large tyre rotate
     glRotatef(90, 0, 0, 1); // large tyre angle
    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
    displayTyre(tyreRadius, tyreWidth,1); // tyre
    glPopMatrix();

    //small tyre display
    glPushMatrix();
    glTranslatef(0,40,0);
    glTranslatef(0,0,290);// places the tyre on top of both boxes
    glRotatef(tyreAngle, 0, 1, 0);   //small tyre rotate
    glRotatef(90, 0, 0, 1); //small tyre angle
    glScalef(tyreScaleFactor, tyreScaleFactor, tyreScaleFactor);
    displayTyre(tyreRadius/2, tyreWidth,2); // tyre
    glPopMatrix();

}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background, background, background,0);	//sets background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // sets the current matrix to identity
    gluLookAt(pos.x, pos.y, pos.z, pos.x+l.x, pos.y+l.y, pos.z+l.z, u.x, u.y, u.z); // sets camera position and direction
    displayScene();
    glutSwapBuffers();
    glutPostRedisplay();
}
void animate()
{
    if(var==1)
    {
      if (winangle >= 38  )
            winangle =38 ;
        else
            winangle += .05;
    }
    else if(var==2)
    {
        if (winangle <= 0 )
            winangle = 0;
        else
            winangle -= .05;
    }

    if (var == 3)
    {
        if (quadAngle>=145)
            quadAngle=145;
        else
            quadAngle +=0.1;
    }
    else if (var == 4)
    {
        if (quadAngle<=0)
            quadAngle=0;
        else
            quadAngle -=0.1;
    }

    if (var ==5)
        tyreAngle -=1;
    else if (var == 6)
        tyreAngle +=1;
    else if (var == 7)
        tyreAngle=tyreAngle;

    glutPostRedisplay();
}

void init()
{
    glClearColor(background, background, background,0); 	//sets background color

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity(); // sets the current matrix to identity
    gluPerspective(80,	1,	1,	1000.0);

    // to understand the following values, read the specification pdf
    double d = 1 / sqrt2;
    pos = {300, 300, 150};
    u = {0, 0, 1};
    r = {-d, d, 0};
    l = {-d, -d, 0};
}

void subMenu1(int id)
{
    if(id == 1)
        var = 1;
    else if(id == 2)
        var = 2;

}

void subMenu2(int id)
{
    if(id == 3)
        var = 3;
    else if(id == 4)
        var = 4;

}

void subMenu3(int id)  // exhaust fan function
{
    if(id == 5)
        var = 5;
    else if(id == 6)
        var = 6;
    else if(id == 7)
        var = 7;

}
void mainMenu(int id)
{
    if(id == 8)
    {
       gateAngle = 0; //first door
       //doorAngle = 0; //rotate for clr match
       quadAngle = 0;
       boxAngle = 0; //home angle
       tyreAngle = 0;
       boxScaleFactor = 1;// home scale
       tyreScaleFactor = 1;//fan scale
        windowAngle=0;
        //uprotate=90;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
     glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    int subMenuNo1 = glutCreateMenu(subMenu1); // sliding window
    glutAddMenuEntry("Slide Up", 1);
    glutAddMenuEntry("Slide Down", 2);

    int subMenuNo2 = glutCreateMenu(subMenu2); // rotating door
    glutAddMenuEntry("Open", 3);
    glutAddMenuEntry("Close", 4);

    int subMenuNo3 = glutCreateMenu(subMenu3); // exhaust fan
    glutAddMenuEntry("Clockwise", 5);
    glutAddMenuEntry("Anti Clockwise", 6);
    glutAddMenuEntry("Stop", 7);

    glutCreateMenu(mainMenu);

    //glutAddMenuEntry("Axes On", 1); // axes hide
    // glutAddMenuEntry("Axes Off", 2);

    glutAddSubMenu("Sliding Window", subMenuNo1);
    glutAddSubMenu("Rotating Door", subMenuNo2);
    glutAddSubMenu("Exhaust Fan", subMenuNo3);
    glutAddMenuEntry("Orginal Size ", 8);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutTimerFunc(0, timer, 0) ;
    glutMainLoop();		//The main loop of OpenGL, this function never returns
    return 0;
}
