#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "texture.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int width, height, counter=0, index = 0;
float pos_x=0,pos_z=0;
bool flagWireFrame = true, flagTexture = true, left,right,up,down;
float l0_color[][4] = {{1,0,0,1},{0,1,0,1},{1,1,0,1}};
float l0_pos[] = {0,2.5,10,1} ; 

Camera  cam1(0,5,1, 0, 0, 1 );
GLUquadricObj *quadric;
texture t1,t2,t3,t4;


void loop(int val)
{
	glutTimerFunc(20,loop,0);

	int wall_limit = 7;

	//Limit movement
	if(left && (pos_x >= -wall_limit-pos_z) )
	{
		pos_x -= .1; pos_z -= .1;
	}
	if(right && (pos_x <= wall_limit - pos_z) )
	{
		pos_x += .1; pos_z += .1;
	}
	if(up && (pos_z >= -wall_limit + pos_x) )
	{
		pos_z -= .1; pos_x += .1;
	}
	if(down && (pos_z <= wall_limit+pos_x) )
	{ 
		pos_z += .1; pos_x -= .1;
	}

	//Change Color
	if(counter%100 == 0)
	{
		index = counter / 100 % 3;
		glLightfv( GL_LIGHT0,GL_DIFFUSE,l0_color[ index ] );
	}
	counter++;
}

void drawWall()
{
	for(float i=0;i<10;i++)
	{
		for(float k=0;k<10;k++)
		{
			glPushMatrix();		
			  glBegin( GL_QUADS );
				glNormal3f( 0,0,1 ) ;
				glTexCoord2f(k*0.1,i*0.1) ;
				glVertex3f(k,i,0);
		  
				glTexCoord2f(0.1*(k+1),0.1*i);
				glVertex3f(k+1,i,0);
				
				glTexCoord2f(0.1*(k+1),0.1*(i+1));
				glVertex3f(k+1, i+1,0);
				
				glTexCoord2f(0.1*(k),0.1*(i+1));
				glVertex3f(k, i+1,0);
			glEnd();
			glPopMatrix();
		}
	}
}

void drawLamb()
{
	if(!flagWireFrame)
		glDisable(GL_TEXTURE_2D);
	if(!flagTexture)
		glEnable(GL_TEXTURE_2D);
	
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glColor3f(l0_color[index][0],l0_color[index][1],l0_color[index][2]);
		glBindTexture(GL_TEXTURE_2D,t3.texName);
		glTranslatef(0,2,10);
		glRotatef(-90,1,0,0);
		gluSphere(quadric,1,20,20);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glBindTexture(GL_TEXTURE_2D,t3.texName);
		glTranslatef(0,.2,10);
		glRotatef(-90,1,0,0);
		gluCylinder(quadric,1,1,.5,20,20);
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D,t4.texName);
		glTranslatef(0,.5,10);
		glRotatef(-90,1,0,0);
		gluDisk(quadric,0.1f,1,20,20);
	glPopMatrix();
	
	for(int i=0;i<5;i++)
	{
		glPushMatrix();
			glDisable(GL_LIGHTING);
			glBindTexture(GL_TEXTURE_2D,t3.texName);
			glTranslatef( 0,.5+(i*0.1),10);
			glRotatef(-90, 1,0,0);
			gluCylinder(quadric,.2,.2,.1,20,20);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}


	glPushMatrix();
		glLightfv( GL_LIGHT0, GL_POSITION, l0_pos ) ;
	glPopMatrix();
 	
	if(!flagWireFrame)
		glEnable(GL_TEXTURE_2D);
	if(!flagTexture)
		glDisable(GL_TEXTURE_2D);
}

void display()
{
	glClearColor( 0, 0 , 0 , 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

    cam1.LookAt();
    glTranslatef(0,0,-20);

	//Right Wall
	glPushMatrix();
		glBindTexture( GL_TEXTURE_2D, t1.texName ) ;
		glRotatef(-45,0,1,0);
		drawWall();
	glPopMatrix();
		
	//Left Wall
	glPushMatrix();
		glBindTexture( GL_TEXTURE_2D, t1.texName ) ;
		glRotatef(45,0,1,0);
		glTranslatef(-10,0,0);
		drawWall();
	glPopMatrix();
		
	//Ground Wall
	glPushMatrix();
		glBindTexture( GL_TEXTURE_2D, t2.texName ) ;
		glTranslatef(0,0,14.1);
		glRotatef(270,1,0,0);
		glRotatef(45,0,0,1);
		drawWall();
	glPopMatrix();

	//Lamb
	glPushMatrix();
		glScalef(.8,.8,.8);
		glTranslatef(pos_x,.5,pos_z);
		drawLamb();
	glPopMatrix();

    glutSwapBuffers();
    glutPostRedisplay();
}

void ASCIIKeyDown(unsigned char key, int x, int y )
{
	if( key == 27 )
		exit(0);
}

void ASCIIKeyUp ( unsigned char key, int x, int y )
{
}

void SpecialKeyDown( int key, int x, int y )
{
	switch(key)
	{
		case GLUT_KEY_UP: up = true; break ;
		case GLUT_KEY_DOWN: down = true;  break ;
		case GLUT_KEY_LEFT: left = true; break;
		case GLUT_KEY_RIGHT: right = true; break;
		case GLUT_KEY_F1: if(!flagTexture)  glEnable( GL_TEXTURE_2D ) ; else  glDisable( GL_TEXTURE_2D ) ; flagTexture = !flagTexture; break;
		case GLUT_KEY_F2: if(flagWireFrame) { glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) ;gluQuadricDrawStyle(quadric,GL_LINE); } else { glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );gluQuadricDrawStyle(quadric,GL_FILL); } flagWireFrame = !flagWireFrame;  break;
	}
}

void SpecialKeyUp( int key , int x, int y )
{
	switch(key)
	{
		case GLUT_KEY_UP: up = false; break ;
		case GLUT_KEY_DOWN: down = false;  break ;
		case GLUT_KEY_LEFT: left = false; break;
		case GLUT_KEY_RIGHT: right = false; break;
	}
}

void MouseClick( int button, int stat, int x, int y )
{

}

void MouseMove( int x, int y )
{
}

void MouseDownMove( int x, int y )
{
}

void reshape( int w, int h )
{
	width = w ;
	height = h ;
	glViewport( 0,0,w,h) ;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 70, 1.0*w/h, 1, 4000 ) ;
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();

}

void init() 
{
	glEnable( GL_NORMALIZE );
    glEnable( GL_DEPTH_TEST ) ;
	glEnable( GL_TEXTURE_2D ) ; 
	glEnable( GL_LIGHT0 );
	glEnable( GL_LIGHTING );

    t1.Create( "images/wall1.bmp" ) ;
	t2.Create( "images/floor.bmp" );
	t3.Create( "images/lamb1.bmp" );
	t4.Create( "images/text.bmp" );
	
	quadric = gluNewQuadric() ;
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);}

int main( int argc, char *argv[] )
{

	glutInit(&argc, argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow( "Lamp Example" ) ;

	glutDisplayFunc( display ) ;
    glutReshapeFunc( reshape );
	//
	// keyboard registration
	//
	glutKeyboardFunc( ASCIIKeyDown ) ;
	glutKeyboardUpFunc( ASCIIKeyUp ) ;

	glutSpecialFunc( SpecialKeyDown ) ;
	glutSpecialUpFunc( SpecialKeyUp ) ;
	//
	// mouse registration
	//
    glutMouseFunc( MouseClick ) ;
    glutPassiveMotionFunc( MouseMove ) ;
	glutMotionFunc( MouseDownMove ) ;
	
	glutTimerFunc(20,loop,0);
    init();

	glutMainLoop();
}
