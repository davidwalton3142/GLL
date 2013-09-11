#include <GL/glew.h>
#include <GL/freeglut.h>

#include <GLL/Shader.hpp>
#include <GLL/Mesh.hpp>
#include <GLL/Camera.hpp>

#include <iostream>

using namespace GLL;

int init();
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void passiveMotion(int x, int y);

Viewer* viewer;
ShaderProgram* prog;
Mesh* mesh;

/** Backface shell silhouette demo
 *  This demo shows a dead simple method for rendering silhouettes.
 */

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Backface Shell Silhouette Demo");
	glewInit();

	int initializedCorrectly = init();
	if(!initializedCorrectly) return 1;

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutMainLoop();

	delete prog;
	delete mesh;

	return 0;
}

int init()
{
	float clearColor[] = { .8f, 1.f, .8f, 1.f }; //Pastel Green
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
	glEnable(GL_CULL_FACE);

	viewer = new Viewer();

	try
	{
		std::vector<GLuint> shell;
		shell.push_back(Shader::load(GL_VERTEX_SHADER, "Shell.Vertex"));
		shell.push_back(Shader::load(GL_FRAGMENT_SHADER, "Shell.Fragment"));
		prog = new ShaderProgram(shell);
	}
	catch(ShaderException e)
	{
		std::cerr << e.msg;
		std::cin.get();
		return 0;
	}

	try
	{
		mesh = new Mesh("../models/stanford.ctm");
	}
	catch(MeshException e)
	{
		std::cerr << e.msg;
		std::cin.get();
		return 0;
	}

	return 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Enlarge model and render backfaces in outline color.
	glCullFace(GL_FRONT);
	prog->setUniform("scale", 1.1f);
	prog->setUniform("uColor", glm::vec4(0.0f, 0.0f, 0.5f, 1.0f));
	mesh->render(*prog);

	//Render frontfaces as usual over the top.
	glCullFace(GL_BACK);
	prog->setUniform("scale", 1.0f);
	prog->setUniform("uColor", glm::vec4(0.6f, 0.6f, 1.0f, 1.0f));
	mesh->render(*prog);

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int width, int height)
{
	float aspect = (float) width / (float) height;
	viewer->setAspect(aspect);
	glViewport(0, 0, width, height);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27: //ESC
		exit(0);
		return;
	}
}

/* Callback at each mouse button press.
 */
void mouse(int button, int state, int x, int y)
{
	viewer->mouse(button, state, x, y);
}

/* Callback on mouse motion with one or more buttons depressed.
 */
void motion(int x, int y)
{
	viewer->motion(x, y);
}

/* Callback on mouse motion with no buttons depressed.
 */
void passiveMotion(int x, int y)
{

}
