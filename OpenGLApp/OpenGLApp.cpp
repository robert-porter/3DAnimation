#include <cmath>
#include <windows.h>
#include <gl\gl.h>
#include "glut.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Camera.h"
#include "Drawing.h"
#include "ParticleSystem.h"
#include <vector>
#include "Terrain.h"
#include "MD2.h"
#include "Entity.h"
#include "Collision.h"
#include "BVTree.h"

Camera camera;
Terrain terrain;
MorphAnimation *pModel = 0;
MorphAnimation *pWeaponModel = 0;
std::vector<Entity *> entities;
std::vector<ParticleSystem *> particleSystems;
SphereTree *pSphereTree = 0;


unsigned char keys[256];
int mouse[3];

float appTime = 0.0f;
float dTime = 0.0f;

void SetLightAndMaterials()
{
	// I just create one material for the whole screen...

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat matAmbient[] = {1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat matDiffuse[] = {1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat matSpecular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat matShininess = 100.0f;

	GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat lightDiffuse[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat lightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightPos[] = { 0.5f, 0.5f, 0.9f, 0.0f };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Draw()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	camera.SetPerspective();

	glMatrixMode(GL_MODELVIEW);

	Vector3f p;
	camera.SetYaw(entities[0]->GetYaw());
	p = entities[0]->GetPosition() + 0.5f * camera.GetForward() + Vector3f(0.0, 1, 0);
	camera.SetPosition(p);
	Vector3f u = camera.GetRight();
	Vector3f v = camera.GetUp();
	Vector3f w = camera.GetForward();
	Vector3f x = camera.GetPosition();


	Matrix4x4f Mview(u.x,  u.y,  u.z,  -Dot(u, x),
					 v.x,  v.y,  v.z,  -Dot(v, x),
					 -w.x, -w.y, -w.z, Dot(w, x),
					 0,    0,    0,    1.0);

	glLoadMatrixf(Mview);

	pSphereTree->Draw();
	
	x = x + camera.GetForward() * 3;
	glPushMatrix();
	glTranslatef(x.x, x.y, x.z);
//	DrawSphereTree(pSphereTreeRoot2, pModel->frames[0].vertices);
	glPopMatrix();

//	if(SphereTreeIntersectSphereTree(pSphereTreeRoot2, pModel->frames[0].vertices, x, Matrix4x4f::Identity(), 
//		                             pSphereTreeRoot, pModel->frames[0].vertices, Vector3f(0,0,0), Matrix4x4f::Identity()))
//	{

//	}
	SetLightAndMaterials();
	
	terrain.Draw();

	for(int i = 1; i < entities.size(); i++)
	{
		entities[i]->Draw();
	}


	for(int i = 0; i < particleSystems.size(); i++)
		particleSystems[i]->Draw(camera);

	glFlush();
	glutSwapBuffers();
}

void Update()
{	
	float newTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	dTime = newTime - appTime;
	appTime = newTime;

	bool bShoot = false;

	if(keys['w'])
		entities[0]->WalkForward(dTime);
	if(keys['s'])
		entities[0]->WalkBackward(dTime);
	if(keys['d'])
		entities[0]->StrafeRight(dTime);
	if(keys['a'])
		entities[0]->StrafeLeft(dTime);
	if(keys['f'])
		entities[0]->Jump();
	if(mouse[GLUT_LEFT] == GLUT_DOWN)
		bShoot = true;


	for(int i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(dTime);

		Vector3f position = entities[i]->GetPosition();
		//Vector3f groundNormal = terrain.GetNormal(position.x, position.z);
		//entities[i]->WalkForward(groundNormal, dTime);
		position = entities[i]->GetPosition();
		position.y = terrain.GetHeight(position.x, position.z) + 1.0;
		entities[i]->SetPosition(position);
		if(bShoot)
		{
			
			float tMin, tMax;
			bool bHit = SegmentIntersectMd2AnimatedModel(entities[i]->pAnimatedModel, entities[i]->GetPosition(), entities[i]->GetRotation(),
				camera.GetPosition(), camera.GetPosition() + camera.GetForward() * 5, tMin, tMax);
			if(bHit)
			{
				entities[i]->Hit();
			}
		}
	}

	for(int i = 0; i < particleSystems.size(); i++)
		particleSystems[i]->Update(dTime);

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int, int)
{
	keys[key] = true;

	if(key == 'q')
		exit(0);
}

void KeyboardUp(unsigned char key, int, int)
{
	keys[key] = false;
}

void PassiveMouse(int x, int y)
{
	if(x == 400 && y == 300)
		return;
	float deltaX = x - 400;
	float deltaY = y - 300;

	float r = sqrtf(deltaX * deltaX + deltaY * deltaY);
	deltaX /= r;
	deltaY /= r;

	if(deltaX > 0)
		entities[0]->TurnRight(dTime);
	if(deltaX < 0)
		entities[0]->TurnLeft(dTime);


	camera.LookUp(1.0f * deltaY * dTime);

	glutWarpPointer(400, 300);
}

void Mouse(int button, int state, int x, int y)
{
	mouse[button] = state;

	PassiveMouse(x, y);
}


void Init()
{
	memset(keys, 0, 256 * sizeof(unsigned char));
	memset(mouse, 0, 3 * sizeof(int));

	appTime = glutGet(GLUT_ELAPSED_TIME)/1000.0f;
	
	glutWarpPointer(400, 300);

	//ps.Init();
	terrain.Init();
	pModel = new MorphAnimation("hueteotl\\tris.md2");
	pModel->SetTexture("hueteotl\\hueteotl.bmp");
	//pWeaponModel = new MorphAnimation("hueteotl\\weapon.md2");

	for(int i = 0; i < 10; i++)
	{
		Entity *pEntity = new Entity();
		pEntity->SetModel(pModel);
		//pEntity->SetWeaponModel(pWeaponModel);
		pEntity->SetPosition(Vector3f(0, 1, i));
		pEntity->TurnRight(-PI/2.0);
		entities.push_back(pEntity);
	}

	pSphereTree = new SphereTree(pModel->frames[0].vertices, pModel->vertexIndices, pModel->numTris);
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("S2");

	glutIdleFunc(Update);
	glutDisplayFunc(Draw);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutPassiveMotionFunc(PassiveMouse);
	glutMotionFunc(PassiveMouse);
	glutMouseFunc(Mouse);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	Init();

	glutMainLoop();
}
