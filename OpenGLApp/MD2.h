#pragma once

#include <windows.h>
#include <GL/gl.h>
#include "Texture.h"
#include "Vector3.h"
#include "Vector2.h"


struct Md2Anim_t
{
  int start;  // first frame index
  int end;    // last frame index
  int fps;
};

struct animState_t
{
    int     startframe;              // first frame
    int     endframe;                // last frame
    int     fps;                     // frame per second for this animation

    float   curr_time;               // current time
    float   old_time;                // old time
    float   interpol;                // percent of interpolation

    int     type;                    // animation type

    int     curr_frame;              // current frame
    int     next_frame;              // next frame

};

const Md2Anim_t animlist[ 21 ] = 
{
    // first, last, fps

    {   0,  39,  9 },   // STAND
    {  40,  45, 10 },   // RUN
    {  46,  53, 10 },   // ATTACK
    {  54,  57,  7 },   // PAIN_A
    {  58,  61,  7 },   // PAIN_B
    {  62,  65,  7 },   // PAIN_C
    {  66,  71,  7 },   // JUMP
    {  72,  83,  7 },   // FLIP
    {  84,  94,  7 },   // SALUTE
    {  95, 111, 10 },   // FALLBACK
    { 112, 122,  7 },   // WAVE
    { 123, 134,  6 },   // POINT
    { 135, 153, 10 },   // CROUCH_STAND
    { 154, 159,  7 },   // CROUCH_WALK
    { 160, 168, 10 },   // CROUCH_ATTACK
    { 196, 172,  7 },   // CROUCH_PAIN
    { 173, 177,  5 },   // CROUCH_DEATH
    { 178, 183,  7 },   // DEATH_FALLBACK
    { 184, 189,  7 },   // DEATH_FALLFORWARD
    { 190, 197,  7 },   // DEATH_FALLBACKSLOW
    { 198, 198,  5 },   // BOOM
};


typedef enum {
    STAND = 0,
    RUN,
    ATTACK,
    PAIN_A,
    PAIN_B,
    PAIN_C,
    JUMP,
    FLIP,
    SALUTE,
    FALLBACK,
    WAVE,
    POINTING,
    CROUCH_STAND,
    CROUCH_WALK,
    CROUCH_ATTACK,
    CROUCH_PAIN,
    CROUCH_DEATH, 
    DEATH_FALLBACK,
    DEATH_FALLFORWARD,
    DEATH_FALLBACKSLOW,
    BOOM,

    MAX_ANIMATIONS

} animType_t;


class MorphAnimation
{
public:
	MorphAnimation (const char *filename);
	~MorphAnimation ();

	void SetTexture (const char *filename);

	void Draw (int frame);
	void Draw (int frameA, int frameB, float interp);
public:

	void CalcBoundingVolume();
	void CalcEdges();

	struct Frame
	{
		Vector3f *vertices;
		Vector3f *normals;
		float radius;	// bounding sphere of all points
	};

	Frame *frames;
	int numFrames;

	int *vertexIndices;
	int numTris;
	int numVertices;

	Vector2f *texCoords;
	int *texCoordIndices;

	int *edges;
	int numEdges;

	GLuint tex;
};



class Md2AnimatedModel
{
public:
	Md2AnimatedModel ();
	Md2AnimatedModel (MorphAnimation *model);
	~Md2AnimatedModel ();

	void SetModel (MorphAnimation *model);

	const MorphAnimation *GetModel () const { return model; }

	void DrawFrame(int frame);
	void Draw();

	void SetAnimation(animType_t animType);
	void SetWalking(bool bIsWalking) { isWalking = bIsWalking; }
	void Update(float dt);

	int GetCurrentFrame() const { return animState.curr_frame; }
private:

	MorphAnimation *model;
	bool isWalking;

public:
	animState_t animState;
};
