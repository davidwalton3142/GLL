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
ShaderProgram* unfScaleProg;
ShaderProgram* normScaleProg;
ShaderProgram* biasProg;
Mesh* mesh;

glm::vec4 modelColor(0.6f, 0.6f, 1.0f, 1.0f);
glm::vec4 outlineColor(0.0f, 0.0f, 0.5f, 1.0f);

enum RenderMode {unfScale = 0, normScale = 1, bias = 2};
RenderMode renderMode = unfScale;

float unfScaleAmt = 0.1f;
float normScaleAmt = 0.1f;
float biasAmt = 0.3f;

void addAmt(float x);

/** Backface shell silhouette demo
 *  This demo shows a number of silhouette rendering approaches
 *  using backface shells.
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

	delete unfScaleProg;
	delete normScaleProg;
	delete biasProg;
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
		std::vector<GLuint> unfScale;
		unfScale.push_back(Shader::load(GL_VERTEX_SHADER, "Shell.UnfScale.Vertex"));
		unfScale.push_back(Shader::load(GL_FRAGMENT_SHADER, "Shell.Fragment"));
		unfScaleProg = new ShaderProgram(unfScale);

		std::vector<GLuint> normScale;
		normScale.push_back(Shader::load(GL_VERTEX_SHADER, "Shell.NormScale.Vertex"));
		normScale.push_back(Shader::load(GL_FRAGMENT_SHADER, "Shell.Fragment"));
		normScaleProg = new ShaderProgram(normScale);

		std::vector<GLuint> bias;
		bias.push_back(Shader::load(GL_VERTEX_SHADER, "Shell.Bias.Vertex"));
		bias.push_back(Shader::load(GL_FRAGMENT_SHADER, "Shell.Fragment"));
		biasProg = new ShaderProgram(bias);
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

	if(renderMode == unfScale)
	{
		//Enlarge model and render backfaces in outline color.
		glCullFace(GL_FRONT);
		unfScaleProg->setUniform("scale", 1.f + unfScaleAmt);
		unfScaleProg->setUniform("uColor", outlineColor);
		mesh->render(*unfScaleProg);

		//Render frontfaces as usual over the top.
		glCullFace(GL_BACK);
		unfScaleProg->setUniform("scale", 1.f);
		unfScaleProg->setUniform("uColor", modelColor);
		mesh->render(*unfScaleProg);
	}

	else if(renderMode == normScale)
	{
		//Scale model out along norms and render backfaces in outline color.
		glCullFace(GL_FRONT);
		normScaleProg->setUniform("scale", normScaleAmt);
		normScaleProg->setUniform("uColor", outlineColor);
		mesh->render(*normScaleProg);

		//Render frontfaces as usual over the top.
		glCullFace(GL_BACK);
		normScaleProg->setUniform("scale", 0.f);
		normScaleProg->setUniform("uColor", modelColor);
		mesh->render(*normScaleProg);
	}

	else if(renderMode == bias)
	{
		//Push model forwards and render backfaces in outline color.
		glCullFace(GL_FRONT);
		biasProg->setUniform("bias", biasAmt);
		biasProg->setUniform("uColor", outlineColor);
		mesh->render(*biasProg);

		//Render frontfaces as usual over the top.
		glCullFace(GL_BACK);
		biasProg->setUniform("bias", 0.f);
		biasProg->setUniform("uColor", modelColor);
		mesh->render(*biasProg);
	}

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
	case '=':
	case '+':
		addAmt(0.1f);
		break;
	case '-':
		addAmt(-0.1f);
		break;

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
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		renderMode = (renderMode < 2 ? (RenderMode) (renderMode+1) : (RenderMode) 0);
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

void addAmt(float x)
{
	switch(renderMode)
	{
	case unfScale:
		unfScaleAmt += x;
	case normScale:
		normScaleAmt += x;
	case bias:
		biasAmt -= x;
	}
}
