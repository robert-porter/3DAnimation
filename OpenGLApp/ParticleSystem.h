#pragma once

#include "Vector3.h"
#include "mymath.h"
#include "Texture.h"
#include "Camera.h"


Vector3f RandomVector(float rMin, float rMax)
{
	float phi = PI * float(rand()) / float(RAND_MAX);
	float theta = 2 * PI * float(rand()) / float(RAND_MAX);
	float r = rMin + (rMax - rMin) * float(rand()) / float(RAND_MAX);

	return Vector3f(r * cos(theta)*sin(phi), r * sin(theta) * sin(phi), r * cos(phi));
}

class ParticleSystem
{
public:
	ParticleSystem()
	{
		lifetime = 2.0f;
		spawnPoint     = Vector3f(0,0,0);
		numParticles   = 60;
		particles      = new Particle[numParticles];

		for(int i = 0; i < numParticles; i++)
		{
			particles[i].age = lifetime * float(rand()) / float(RAND_MAX);
			particles[i].position = spawnPoint + RandomVector(0.3, 2);
			particles[i].velocity = Vector3f(0,10 * float(rand())/RAND_MAX,0);
			particles[i].acceleration = Vector3f(0, -9.8, 0);
		}

	}

	void Init()
	{
		textureID	   = LoadBitmap("particle.bmp");

	}
	void Update(float dt)
	{
		for(int i = 0; i < numParticles; i++)
		{
			particles[i].age += dt;
			if(particles[i].age > lifetime)
			{
				particles[i].age = 0.0f;
				particles[i].position = spawnPoint;
				particles[i].velocity = Vector3f(float(rand())/RAND_MAX,2 * float(rand())/RAND_MAX,float(rand())/RAND_MAX);
				particles[i].acceleration = Vector3f(0, -9.8, 0);
			}
			else
			{
				particles[i].velocity += particles[i].acceleration * dt;
				particles[i].position += particles[i].velocity * dt;
			}
		}
	}
	void Draw(const Camera &camera)
	{
		glPointSize(50.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glDisable(GL_DEPTH_TEST);

		glBegin(GL_QUADS);
		for(int i = 0; i < numParticles; i++)
		{
			//glVertex3fv(particles[i].position);
			
			glTexCoord2f(0,1);
			glVertex3fv(particles[i].position + Vector3f(-0.5, 0, 0.5));
			glTexCoord2f(1,1);
			glVertex3fv(particles[i].position + Vector3f(0.5, 0, 0.5));
			glTexCoord2f(1,0);
			glVertex3fv(particles[i].position + Vector3f(0.5, 0, -0.5));
			glTexCoord2f(0,0);
			glVertex3fv(particles[i].position + Vector3f(-0.5, 0, -0.5));
			
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
	}
private:

	struct Particle
	{
		Vector3f position;
		Vector3f velocity;
		Vector3f acceleration;
		float age;
	};

	Particle *particles;
	Vector3f spawnPoint;
	int numParticles;
	float lifetime;

	GLint textureID;
};