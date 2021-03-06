

#ifdef _WIN32
#define	WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include "MD2.h"
#include "Matrix4x4.h"
#include "mymath.h"
#include "glut.h"
#include <vector>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;


/////////////////////////////////////////////////////////////////////////////
//
// class Md2Model implementation.
//
/////////////////////////////////////////////////////////////////////////////
typedef GLfloat vec2_t[2];
typedef GLfloat vec3_t[3];

// Precomputed normal vector array (162 vectors)
const vec3_t _kAnorms[] = {
#include "Anorms.h"
};

// Magic number (should be 844121161)
const int _kMd2Ident = 'I' + ('D'<<8) + ('P'<<16) + ('2'<<24);

// MD2 format version
const int _kMd2Version = 8;


struct Md2Header_t
{
  int ident;          // Magic number, "IDP2"
  int version;        // Md2 format version, should be 8

  int skinwidth;      // Texture width
  int skinheight;     // Texture height

  int framesize;      // Size of a frame, in bytes

  int num_skins;      // Number of skins
  int num_vertices;   // Number of vertices per frame
  int num_st;         // Number of texture coords
  int num_tris;       // Number of triangles
  int num_glcmds;     // Number of OpenGL commands
  int num_frames;     // Number of frames

  int offset_skins;   // offset to skin data
  int offset_st;      // offset to texture coords
  int offset_tris;    // offset to triangle data
  int offset_frames;  // offset to frame data
  int offset_glcmds;  // offset to OpenGL commands
  int offset_end;     // offset to the end of the file
};

struct Md2Skin_t
{
  char name[64];  // Texture's filename
};

struct Md2TexCoord_t
{
  short s;
  short t;
};

struct Md2Triangle_t
{
  unsigned short vertex[3];  // Triangle's vertex indices
  unsigned short st[3];      // Texture coords. indices
};

struct Md2Vertex_t
{
  unsigned char v[3];         // Compressed vertex position
  unsigned char normalIndex;  // Normal vector index
};


struct Md2Frame_t
{
  vec3_t scale;        // Scale factors
  vec3_t translate;    // Translation vector
  char name[16];       // Frame name
  Md2Vertex_t *verts;  // Frames's vertex list
};

MorphAnimation::MorphAnimation (const char *filename)
  : texCoords (NULL), frames (NULL), tex (NULL)
{
	Md2Header_t header;
	Md2Skin_t *skins;
	Md2TexCoord_t *md2texCoords;
	Md2Frame_t *md2frames;
	Md2Triangle_t *triangles;

	// Open the file
	std::ifstream ifs (filename, std::ios::binary);

	if (ifs.fail ())
		throw std::runtime_error ("Couldn't open file");

	// Read header
	ifs.read (reinterpret_cast<char *>(&header), sizeof (Md2Header_t));

	// Check if ident and version are valid
	if (header.ident != _kMd2Ident)
		throw std::runtime_error ("Bad file ident");

	if (header.version != _kMd2Version)
		throw std::runtime_error ("Bad file version");

	// Memory allocation for model data
	skins = new Md2Skin_t[header.num_skins];
	md2texCoords = new Md2TexCoord_t[header.num_st];
	texCoords = new Vector2f[header.num_st];
	triangles = new Md2Triangle_t[header.num_tris];
	md2frames = new Md2Frame_t[header.num_frames];

	// Read skin names
	ifs.seekg (header.offset_skins, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(skins), sizeof (Md2Skin_t) * header.num_skins);

	// Read texture coords.
	ifs.seekg (header.offset_st, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(md2texCoords), sizeof (Md2TexCoord_t) * header.num_st);

	// Read triangle data
	ifs.seekg (header.offset_tris, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(triangles), sizeof(Md2Triangle_t) * header.num_tris);

  // Read frames
	ifs.seekg (header.offset_frames, std::ios::beg);
	for (int i = 0; i < header.num_frames; ++i)
    {
		// Memory allocation for the vertices of this frame
		md2frames[i].verts = new Md2Vertex_t[header.num_vertices];

		// Read frame data
		ifs.read (reinterpret_cast<char *>(&md2frames[i].scale), sizeof (vec3_t));
		ifs.read (reinterpret_cast<char *>(&md2frames[i].translate),	sizeof (vec3_t));
		ifs.read (reinterpret_cast<char *>(&md2frames[i].name), sizeof (char) * 16);
		ifs.read (reinterpret_cast<char *>(md2frames[i].verts), sizeof (Md2Vertex_t) * header.num_vertices);
    }


	// Close file
	ifs.close();


	numTris = header.num_tris;
	numFrames = header.num_frames;
	numVertices = header.num_vertices;
	frames = new Frame[numFrames];
	vertexIndices = new int[numTris * 3];
	texCoordIndices = new int[numTris * 3];


	for(int i = 0; i < header.num_st; i++)
	{
		texCoords[i].x = (float)md2texCoords[i].s / (float)header.skinwidth;
		texCoords[i].y = 1.0f - (float)md2texCoords[i].t / (float)header.skinheight;
	}

	for (int i = 0; i < numFrames; ++i)
    {
		frames[i].vertices = new Vector3f[header.num_vertices];
		frames[i].normals  = new Vector3f[header.num_vertices];
	}

	Matrix4x4f R = Matrix4x4f::RotationY(PI/2.0f) * Matrix4x4f::RotationX(-PI/2.0);

	for(int i = 0; i < header.num_frames; i++)
		for(int j = 0; j < header.num_vertices; j++)
		{
			Vector3f v, n;

			v.x = md2frames[i].verts[j].v[0] * md2frames[i].scale[0] + md2frames[i].translate[0]; 
			v.y = md2frames[i].verts[j].v[1] * md2frames[i].scale[1] + md2frames[i].translate[1]; 
			v.z = md2frames[i].verts[j].v[2] * md2frames[i].scale[2] + md2frames[i].translate[2]; 

			n = R.TransformVector(_kAnorms[md2frames[i].verts[j].normalIndex]);

			v = R.TransformPoint(v);

			v *= 0.04f; // supposedly quake2 uses tenths of an inch, but the conversion makes the models
						// really short. so this is kind of a magic number   

			frames[i].vertices[j] = v;
			frames[i].normals[j] = n;
		}

	for(int i = 0; i < numTris; i++)
	{
		vertexIndices[i*3]   = triangles[i].vertex[0];
		vertexIndices[i*3+1] = triangles[i].vertex[1];
		vertexIndices[i*3+2] = triangles[i].vertex[2];
		
		texCoordIndices[i*3]   = triangles[i].st[0];
		texCoordIndices[i*3+1] = triangles[i].st[1];
		texCoordIndices[i*3+2] = triangles[i].st[2];
	}

	CalcBoundingVolume();
	CalcEdges();

	for(int i = 0; i < numFrames; i++)
		delete md2frames[i].verts;

	delete [] md2frames;
	delete [] triangles;
	delete [] skins;
	delete [] md2texCoords;
}

MorphAnimation::~MorphAnimation ()
{
	for(int i = 0; i < numFrames; i++)
	{
		delete[] frames[i].vertices;
		delete[] frames[i].normals;
	}
	delete[] frames;
	delete[] texCoords;
	delete[] vertexIndices;
	delete[] texCoordIndices;
}

void MorphAnimation::CalcBoundingVolume()
{
	for(int i = 0; i < numFrames; i++)
	{
		float max_rpow2 = 0;
		for(int j = 0; j < numVertices; j++)
		{
			float rpow2 = frames[i].vertices[j].SquaredLength();
			if(rpow2 > max_rpow2)
				max_rpow2 = rpow2;
		}
		frames[i].radius = sqrtf(max_rpow2);
	}
}

void MorphAnimation::SetTexture (const char *filename)
{
	tex = LoadBitmap(filename);
}

void MorphAnimation::Draw (int frame)
{
	// Check if the frame index is valid
	if ((frame < 0) || (frame > numFrames - 1))
		return;

	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin (GL_TRIANGLES);
	for (int i = 0; i < numTris * 3; ++i)
	{
		Vector2f texCoord = texCoords[texCoordIndices[i]];
		Vector3f vertex = frames[frame].vertices[vertexIndices[i]];
		Vector3f normal = frames[frame].normals[vertexIndices[i]];

		glTexCoord2fv(texCoord);
		glNormal3fv(normal);
		glVertex3fv(vertex);
	}
	glEnd();
}

void MorphAnimation::Draw (int frameA, int frameB, float interp)
{
  // Compute max frame index
	int maxFrame = numFrames - 1;

	// Check if frames are valid
	if ((frameA < 0) || (frameB < 0))
		return;

	if ((frameA > maxFrame) || (frameB > maxFrame))
		return;

	// Bind to model's texture
	glBindTexture(GL_TEXTURE_2D, tex);

	Frame *pFrameA = &frames[frameA];
	Frame *pFrameB = &frames[frameB];

	glBegin (GL_TRIANGLES);
    // Draw each triangle
    for (int i = 0; i < numTris * 3; ++i)
	{	
		Vector2f texCoord = texCoords[texCoordIndices[i]];
		Vector3f vertexA = pFrameA->vertices[vertexIndices[i]];
		Vector3f normalA = pFrameA->normals[vertexIndices[i]];
		Vector3f vertexB = pFrameB->vertices[vertexIndices[i]];
		Vector3f normalB = pFrameB->normals[vertexIndices[i]];

		Vector3f vertex = vertexA + interp * (vertexB - vertexA);
		Vector3f normal = normalA + interp * (normalB - normalA);

		glTexCoord2fv(texCoord);
		glNormal3fv(normal);
		glVertex3fv(vertex);
	}
	glEnd();
	
	glBegin(GL_LINES);
	for(int i = 0; i < numEdges; i++)
	{
		glVertex3fv(pFrameA->vertices[edges[i*2+0]]);
		glVertex3fv(pFrameA->vertices[edges[i*2+1]]);
	}
	glEnd();
}

Md2AnimatedModel::Md2AnimatedModel ()
  : model (NULL)
{
	memset(&animState, 0, sizeof(animState));
	SetAnimation(PAIN_A);
}


Md2AnimatedModel::Md2AnimatedModel (MorphAnimation *model)
  : model (model)
{
	SetModel (model);
  	memset(&animState, 0, sizeof(animState));
	SetAnimation(PAIN_A);
}

Md2AnimatedModel::~Md2AnimatedModel()
{
}

void Md2AnimatedModel::Draw()
{
	glPushAttrib (GL_POLYGON_BIT);
	glFrontFace (GL_CW);

	model->Draw(animState.curr_frame, animState.next_frame, animState.interpol);

	glPopAttrib();
}

void Md2AnimatedModel::DrawFrame (int frame)
{
	glPushAttrib (GL_POLYGON_BIT);
	glFrontFace (GL_CW);

	model->Draw (frame);

	glPopAttrib();
}

void Md2AnimatedModel::SetModel (MorphAnimation *m)
{
	model = m;
}

void Md2AnimatedModel::SetAnimation(animType_t animType)
{
	if(animState.type == animType)
		return;

	if( (animType < 0) || (animType > MAX_ANIMATIONS) )
        animType = (animType_t)0;
	
    animState.startframe   = animlist[ animType ].start;
    animState.endframe     = animlist[ animType ].end;
    animState.next_frame   = animlist[ animType ].start + 1;
    animState.fps          = animlist[ animType ].fps;
    animState.type         = animType;
}

void Md2AnimatedModel::Update(float dt)
{
	animState.old_time = animState.curr_time;
	animState.curr_time += dt;

	animState.interpol += dt * animState.fps;
	while(animState.interpol > 1.0f)
	{
		animState.curr_frame = animState.next_frame;
		animState.next_frame++;
		if(animState.next_frame > animState.endframe )
		{
			if(animState.type == RUN && isWalking || animState.type == STAND)
				animState.next_frame = animState.startframe;
			else if(isWalking)
				SetAnimation(RUN);
			else
				SetAnimation(STAND);
		}
		animState.interpol -= 1.0f;
	}
}


void MorphAnimation::CalcEdges()
{
	// I start with all the edges, sort them and remove the duplicates

	struct XEdge	// this is so I can use the stl algorithms
	{
		int e[2];
		XEdge(int e0, int e1) 
		{ 
			e[0] = min(e0, e1); 
			e[1] = max(e0, e1); 
		}
		XEdge() { }
		XEdge& operator=(const XEdge &rhs) { e[0]=rhs.e[0]; e[1]=rhs.e[1]; return *this; }
		bool operator<(const XEdge &rhs) 
		{ 
			if(e[0] < rhs.e[0]) return true; 
			else if (rhs.e[0] < e[0]) return false;// assert(p)
			if (e[1] < rhs.e[1]) return true;
			else return false;
		}
		bool operator==(const XEdge &rhs) { return e[0] == rhs.e[0] && e[1] == rhs.e[1]; }
	};

	XEdge *tempEdges = new XEdge[numTris * 3];

	for(int i = 0; i < numTris; i++)
	{
		tempEdges[i*3 + 0] = XEdge(vertexIndices[i*3+0], vertexIndices[i*3+1]);
		tempEdges[i*3 + 1] = XEdge(vertexIndices[i*3+1], vertexIndices[i*3+2]);
		tempEdges[i*3 + 2] = XEdge(vertexIndices[i*3+2], vertexIndices[i*3+0]);
	}

	std::sort(tempEdges, tempEdges + numTris * 3);
	XEdge *uniqueEnd = std::unique(tempEdges, tempEdges + numTris * 3);
	numEdges = int(uniqueEnd - tempEdges);
	edges = new int[numEdges * 2];

	for(int i = 0; i < numEdges; i++)
	{
		edges[i*2+0] = tempEdges[i].e[0];
		edges[i*2+1] = tempEdges[i].e[1];
	}

	delete[] tempEdges;
}


