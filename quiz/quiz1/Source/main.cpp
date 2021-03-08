#include "../Include/Common.h"
#include "../Include/ViewManager.h"
#include "../Include/Scene.h"

#define MENU_Sale 1
#define MENU_Shrink 2
#define MENU_EXIT   3

using namespace glm;
using namespace std;

float	width = 600;
float   height = 600;
float	aspect;	
float	interval = 16.0f;

Scene *scene;

void My_Init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	scene = new Scene();
}

// GLUT callback. Called to draw the scene.
void My_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->Render();
    glutSwapBuffers();
}

//Call to resize the window
void My_Reshape(int width, int height)
{
	scene->GetCamera()->SetWindowSize(width, height);
	glViewport(0, 0, width, height);
}

//Timer event
void My_Timer(int val)
{
	scene->Update(interval);
	glutPostRedisplay();
	glutTimerFunc(interval, My_Timer, val);
}

//Mouse event
void My_Mouse(int button, int state, int x, int y)
{
	scene->MouseEvent(button, state, x, y);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			printf("Mouse %d is pressed at (%d, %d)\n", button, x, y);
		}
		else if (state == GLUT_UP)
		{
			printf("Mouse %d is released at (%d, %d)\n", button, x, y);
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		printf("Mouse %d is pressed\n", button);
	}
}

//Keyboard event
void My_Keyboard(unsigned char key, int x, int y)
{
	scene->KeyBoardEvent(key);
}

//Special key event
void My_SpecialKeys(int key, int x, int y)
{
	scene->KeyBoardEvent(key);
}

//Menu event
void My_Menu(int id)
{
	scene->MenuEvent(id);

	switch(id)
	{
	case MENU_EXIT:
		exit(0);
		break;
	default:
		break;
	}
}



void My_Mouse_Moving(int x, int y) {
	scene->GetCamera()->mouseMoveEvent(x, y);
}

int main(int argc, char *argv[])
{
#ifdef __APPLE__
    //Change working directory to source code path
    chdir(__FILEPATH__("/../Assets/"));
#endif
	// Initialize GLUT and GLEW, then create a window.
	////////////////////
	glutInit(&argc, argv);
#ifdef _MSC_VER
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Framework"); // You cannot use OpenGL functions before this line; The OpenGL context must be created first by glutCreateWindow()!
#ifdef _MSC_VER
	glewInit();
#endif

	//Print debug information 
	Common::DumpInfo();

	//Call custom initialize function
	My_Init();
	
	//定義選單
	////////////////////
	int menu_main = glutCreateMenu(My_Menu);
	int menu_entry = glutCreateMenu(My_Menu);

	glutSetMenu(menu_main);
	glutAddSubMenu("Scale", menu_entry);
	glutAddMenuEntry("Exit", MENU_EXIT);

	glutSetMenu(menu_entry);
	glutAddMenuEntry("*2.0", MENU_Sale);
	glutAddMenuEntry("*0.5", MENU_Shrink);

	glutSetMenu(menu_main);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	////////////////////

	//Register GLUT callback functions
	////////////////////
	glutDisplayFunc(My_Display);
	glutReshapeFunc(My_Reshape);
	glutMouseFunc(My_Mouse);
	glutKeyboardFunc(My_Keyboard);
	glutSpecialFunc(My_SpecialKeys);
	glutTimerFunc(16, My_Timer, 0); 
	glutPassiveMotionFunc(My_Mouse_Moving);           
	glutMotionFunc(My_Mouse_Moving);     
	////////////////////

	//進入主迴圈
	glutMainLoop();

	return 0;
}

