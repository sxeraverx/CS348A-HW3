/*********************************************************************************/
/* Programming Project 1: Sample program to visualize a sphere in OpenGL         */
/*********************************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include "pp1.h"
#include "pp1_ui.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define VIEW_X_DEFAULT          5.0       /* default x position of the viewpoint */
#define VIEW_X_DEFAULT_MIN      -8.0      /* min and max values of x position */
#define VIEW_X_DEFAULT_MAX      8.0 

#define VIEW_Y_DEFAULT          5.0       /* default y position of the viewpoint */
#define VIEW_Y_DEFAULT_MIN      -8.0      /* min and max values of y position */
#define VIEW_Y_DEFAULT_MAX      8.0

#define VIEW_Z_DEFAULT          5.0       /* default z position of the viewpoint */
#define VIEW_Z_DEFAULT_MIN      -8.0      /* min and max values of z position */
#define VIEW_Z_DEFAULT_MAX      8.0 

#define VIEW_R_DEFAULT_MIN      0.01      /* min and max values of r position */
#define VIEW_R_DEFAULT_MAX      13.856 

#define VIEW_THETA_DEFAULT_MIN  0.01       /* min and max values of theta position */
#define VIEW_THETA_DEFAULT_MAX  180.0

#define VIEW_PHI_DEFAULT_MIN    -180.0     /* min and max values of phi position */
#define VIEW_PHI_DEFAULT_MAX    180.0

#define TORUS_r_DEFAULT          0.5      /* default r radius of the torus */
#define TORUS_r_DEFAULT_MIN      0.0      /* min and max values of r radius */
#define TORUS_r_DEFAULT_MAX      8.0 

#define TORUS_R_DEFAULT          1.0      /* default R radius of the torus */
#define TORUS_R_DEFAULT_MIN      0.0      /* min and max values of R radius */
#define TORUS_R_DEFAULT_MAX      8.0 

#define TORUS_m_DEFAULT          0.0       /* default m value of the torus */
#define TORUS_m_DEFAULT_MIN      -8.0      /* min and max values of m value */
#define TORUS_m_DEFAULT_MAX      8.0 

GLdouble viewX, viewY, viewZ;             /* view point */

GLUquadricObj *theQuadric;
GLUnurbsObj *theNurb;

GLdouble torusr;
GLdouble torusR;
GLdouble torusm;

GLfloat ctrlpoints[3][3][4] = {
  {{0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 1}},
  {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
  {{0, 0, 0, 1}, {0, 0, 0, 0}, {0, 0, 0, 1}}
};

void DefineLight();
void DefineMaterial();
void DefineViewPointMain();


void recalcCtrlPoints()
{
  ctrlpoints[0][0][0] = torusr + torusR;
  ctrlpoints[0][1][2] = torusr + torusR;
  ctrlpoints[0][2][0] = -torusr - torusR;
  ctrlpoints[1][0][1] = torusr;
  ctrlpoints[1][1][2] = torusm*torusR;
  ctrlpoints[1][1][1] = torusm*torusr;
  ctrlpoints[1][1][3] = torusm;
  ctrlpoints[1][2][1] = torusr;
  ctrlpoints[2][0][0] = -torusr + torusR;
  ctrlpoints[2][1][2] = -torusr + torusR;
  ctrlpoints[2][2][0] = torusr - torusR;
}

void recalcPolarSliders()
{
  double r = sqrt(pow(viewX, 2) + pow(viewY, 2) + pow(viewZ, 2));
  double theta = acos(viewY/r);
  double phi = atan(viewZ/viewX);

  ViewPointRSlider->value(r);
  ViewPointThetaSlider->value(theta*180/M_PI);
  ViewPointPhiSlider->value(phi*180/M_PI);

  DefineViewPointMain();
  DefineLight();
}
void recalcCartesianSliders()
{
  ViewPointXSlider->value(viewX);
  ViewPointYSlider->value(viewY);
  ViewPointZSlider->value(viewZ);

  DefineViewPointMain();
  DefineLight();
}

void nurbsError(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf (stderr, "Nurbs Error: %s\n", estring);
   exit (0);
}

void DrawQuadrant() {
  GLfloat knots[6] = {0, 0, 0, 1, 1, 1};

  gluBeginSurface(theNurb);
  gluNurbsSurface(theNurb,
		  6, knots, 6, knots,
		  3 * 4, 4, &ctrlpoints[0][0][0],
		  3, 3, GL_MAP2_VERTEX_4);
  gluEndSurface(theNurb);
}

void DrawScene(){
  // Replace with your code to draw the torus
  //gluSphere(theQuadric, 3.0, 50, 50);

  theNurb = gluNewNurbsRenderer();
  gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
  gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
  gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
  int i, j;

  DrawQuadrant();
  
  glPushMatrix();
  glRotatef(180, 0, 0, 1);
  DrawQuadrant();
  
  glRotatef(180, 0, 1, 0);
  DrawQuadrant();

  glRotatef(180, 0, 0, 1);
  DrawQuadrant();
  
  glPopMatrix();
  glFlush();
    
  gluDeleteNurbsRenderer(theNurb);
}

// Callback for the slider that defines viewpoint x
void ViewPointXCallback(Fl_Value_Slider *ob, long data){
	viewX = ob->value();
	canvas->redraw();
	canvas2->redraw();
	recalcPolarSliders();
}

// Callback for the slider that defines viewpoint y
void ViewPointYCallback(Fl_Value_Slider *ob, long data){
	viewY = ob->value();
	canvas->redraw();
	canvas2->redraw();
	recalcPolarSliders();
}

// Callback for the slider that defines viewpoint z
void ViewPointZCallback(Fl_Value_Slider *ob, long data){
	viewZ = ob->value();
	canvas->redraw();
	canvas2->redraw();
	recalcPolarSliders();
}

// Callback for the slider that defines viewpoint r
void ViewPointRCallback(Fl_Value_Slider *ob, long data){
  double r = sqrt(pow(viewX, 2) + pow(viewZ, 2) + pow(viewY, 2));
  double theta = acos(viewY/r);
  double phi = atan(viewZ/viewX);

  r = ob->value();
  viewX = r * cos(phi) * sin(theta);
  viewZ = r * sin(phi) * sin(theta);
  viewY = r * cos(theta);
  canvas->redraw();
  canvas2->redraw();
  recalcCartesianSliders();
}

// Callback for the slider that defines viewpoint Theta
void ViewPointThetaCallback(Fl_Value_Slider *ob, long data){
  double r = sqrt(pow(viewX, 2) + pow(viewZ, 2) + pow(viewY, 2));
  double theta = ob->value()*M_PI/180;
  double phi = atan(viewZ / viewX);

  viewX = r * cos(phi) * sin(theta);
  viewZ = r * sin(phi) * sin(theta);
  viewY = r * cos(theta);

  if(phi<-M_PI/2 || phi>M_PI/2)
  {
      viewX = -viewX;
      viewZ = -viewZ;
  }

  canvas->redraw();
  canvas2->redraw();
  recalcCartesianSliders();
}

// Callback for the slider that defines viewpoint Phi
void ViewPointPhiCallback(Fl_Value_Slider *ob, long data){
  double r = sqrt(pow(viewX, 2) + pow(viewZ, 2) + pow(viewY, 2));
  double theta = acos(viewY/r);
  double phi = ob->value()*M_PI/180;
  viewX = r * cos(phi) * sin(theta);
  viewZ = r * sin(phi) * sin(theta);
  viewY = r * cos(theta);
  canvas->redraw();
  canvas2->redraw();
  recalcCartesianSliders();
}

void TorusrCallback(Fl_Value_Slider *ob, long data){
  torusr = ob->value();
  recalcCtrlPoints();
  canvas->redraw();
  canvas2->redraw();
}

void TorusRCallback(Fl_Value_Slider *ob, long data){
  torusR = ob->value();
  recalcCtrlPoints();
  canvas->redraw();
  canvas2->redraw();
}

void TorusmCallback(Fl_Value_Slider *ob, long data){
  torusm = ob->value();
  recalcCtrlPoints();
  canvas->redraw();
  canvas2->redraw();
}


// Callback for the button that controls the exit of the program
void QuitProgram(Fl_Button *ob, long data){
	exit(0);
}

// Define the default values of the interface
void InitInterfaceDefaults(void){
	ViewPointRSlider->bounds(VIEW_R_DEFAULT_MIN, VIEW_R_DEFAULT_MAX);

	ViewPointThetaSlider->bounds(VIEW_THETA_DEFAULT_MIN, VIEW_THETA_DEFAULT_MAX);

	ViewPointPhiSlider->bounds(VIEW_PHI_DEFAULT_MIN, VIEW_PHI_DEFAULT_MAX);

	ViewPointXSlider->bounds(VIEW_X_DEFAULT_MIN, VIEW_X_DEFAULT_MAX);
	ViewPointXSlider->value(VIEW_X_DEFAULT);

	ViewPointYSlider->bounds(VIEW_Y_DEFAULT_MIN, VIEW_Y_DEFAULT_MAX);
	ViewPointYSlider->value(VIEW_Y_DEFAULT);

	ViewPointZSlider->bounds(VIEW_Z_DEFAULT_MIN, VIEW_Z_DEFAULT_MAX);
	ViewPointZSlider->value(VIEW_Z_DEFAULT);

	TorusrSlider->bounds(TORUS_r_DEFAULT_MIN, TORUS_r_DEFAULT_MAX);
	TorusrSlider->value(TORUS_r_DEFAULT);
	
	TorusRSlider->bounds(TORUS_R_DEFAULT_MIN, TORUS_R_DEFAULT_MAX);
	TorusRSlider->value(TORUS_R_DEFAULT);
	
	TorusmSlider->bounds(TORUS_m_DEFAULT_MIN, TORUS_m_DEFAULT_MAX);
	TorusmSlider->value(TORUS_m_DEFAULT);
  
	viewX      = VIEW_X_DEFAULT ;
	viewY      = VIEW_Y_DEFAULT ;
	viewZ      = VIEW_Z_DEFAULT ;

	torusr     = TORUS_r_DEFAULT ;
	torusR     = TORUS_R_DEFAULT ;
	torusm     = TORUS_m_DEFAULT ;

	recalcPolarSliders();
}

void MyInit(void){
	theQuadric = gluNewQuadric();
	recalcCtrlPoints();
}


int main(int argc, char *argv[]){
	Fl_Double_Window *ui = create_the_forms();
	Fl::visual(FL_DOUBLE|FL_INDEX);
	ui->label("Sphere");
	ui->show(argc, argv);

	InitInterfaceDefaults();

	ViewPointXSlider->redraw();
	ViewPointYSlider->redraw();
	ViewPointZSlider->redraw();

	TorusrSlider->redraw();
	TorusRSlider->redraw();
	TorusmSlider->redraw();
	MyInit();

	Fl::run();
	return 0;
}

///////////////////////////////////////////////////////////////////////
/////////// You should not need to edit anything below here ///////////
///////////////////////////////////////////////////////////////////////

// Define viewpoint of main window
void DefineViewPointMain(){
	glLoadIdentity();
	gluLookAt(
		viewX, viewY, viewZ,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

// Define viewpoint of the secondary window
void DefineViewPointSecondary(){
	glLoadIdentity();
	gluLookAt(
		6.0, 10.0, 13.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
}

// Create the axes
void DrawAxes(void){
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glBegin (GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 0.0, 0.0, 10.0 );
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 0.0, 10.0, 0.0 );
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f( 0.0, 0.0, 0.0 );   glVertex3f( 10.0, 0.0, 0.0 );
	glEnd();
	glEnable(GL_LIGHTING);
}

// Create the viewpoint lines and spheres
void DrawViewpoint(void){
	glDisable(GL_LIGHTING);
	glLineWidth(3.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin (GL_LINES);
	glVertex3f( 0.0, 0.0, 0.0 ); glVertex3f( viewX, viewY, viewZ );
	glEnd();
	glTranslatef(viewX, viewY, viewZ);
	glEnable(GL_LIGHTING);
	gluSphere(theQuadric, 0.5, 10, 10);
}

// Define the light sources
void DefineLight(void){
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient); 
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse); 

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
}

// Define the material
void DefineMaterial(void){
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

MainCanvas::MainCanvas(int X, int Y, int W, int H)
 : Fl_Gl_Window(X, Y, W, H, "")
{
}

int MainCanvas::handle(int event){
	return Fl_Gl_Window::handle(event);
}

void MainCanvas::draw(){
	/* Draw in the main window */
    DefineViewPointMain();
	if (!valid()) {
	  glViewport(0,0, (GLint)w(), (GLint)h());
	  glMatrixMode( GL_PROJECTION );
	  glLoadIdentity();
	  gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 20.0);
	  glMatrixMode( GL_MODELVIEW );
	  DefineLight();
	  DefineMaterial();
	}
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DefineViewPointMain();

	DrawScene();
}

CameraPositionCanvas::CameraPositionCanvas(int X, int Y, int W, int H)
 : Fl_Gl_Window(X, Y, W, H, "")
{
}

int CameraPositionCanvas::handle(int event){
	return Fl_Gl_Window::handle(event);
}
void CameraPositionCanvas::draw(){
    DefineViewPointSecondary();
	if (!valid()) {
		glViewport(0,0, (GLint)w(), (GLint)h());
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective (75.0, (GLdouble)w()/(GLdouble)h(), 0.1, 20.0);
		glMatrixMode( GL_MODELVIEW );
		DefineLight();
		DefineMaterial();
	}
	/* Draw in the secondary window */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DefineViewPointSecondary();

	DrawScene();
	
	DrawAxes();
	DrawViewpoint();
}
