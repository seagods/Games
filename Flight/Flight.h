#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
/*
 * In  /usr/include/GL/  */

#include <GL/gl.h>
#include <GL/glu.h>
#include "/usr/include/SDL/SDL.h"

#include "/usr/local/include/XFiles/D3Dvec.h"
#include "/usr/local/include/XFiles/Triangle.h"
#include "/usr/local/include/XFiles/throwcatch.h"

const char* WindowTitle="Flight Game";


typedef unsigned char byte;
typedef unsigned short int USHORT;
typedef unsigned int UINT;
typedef unsigned int DWORD;  //yep, a DWORD is the same as a UINT...
                             //we use it for stuff that has DWORD
			     //or glEnum types.

const USHORT WIDTH=800, HEIGHT=600;
const USHORT SCREEN_DEPTH=16;

extern const double twopi,pi;  //initialised externally

/*********************************************************/
/*           for periodic boundary condition special case */
bool startoffx=true;
bool startoffy=true;
bool launch=true;
/*************************************************************/
bool periodic_xz=true;

bool collisiontest=true;
bool foggy=true;
bool Gouraud=true;
bool drawlines=false;
bool picking=false;  //enable us to pick things in the scene
bool PickTri=true;  //if false, we pick nodes and not triangles
int ipicked;
int hits;           //number of hits if picking
/********************MODES***********************************/
bool twodee=false;  // camera moves in xz only

                      //One, but ONE ONLY  of these three
		      //MUST be true
bool freedom=true;      // if true viewer is free to move anywhere
bool tethered=false;   //  if true viewer is attached to fixed point by a boom
bool staked=false;    //   if true viewer is not free to move, but can look about

bool wander=false;    //one of these must be set true if free is true
bool flight=true;

bool aeroplane=true;   // if true we have aeroplane roll - pitch control

                        
bool boom=false;      //  needed for tethered only 
                      //  if boom true  make exwhyzed false
bool exwhyzed=false;   //  if tethered make boom false if exwhyzed is true
                      //  if wander must always be true
		      //  if flight can be true or false
		      //  make false if aeroplane true
		      //
bool hidemousecursor=true;      

/*************************************************************/
int iside, ihalfside;   // iside is the number of segments per side
             //not the number of nodes!
double xlength, xside, scalefac;
int ix, iz;
int ixold, izold; 
int diffx, diffz;
int diffoldx, diffoldz;
int yshiftedge, xshiftedge;
double **xshift, **yshift;
int **index1, **index2;

void SetShiftX(int);
void SetShiftY(int);

/**********************************************************************/
D3Dvec *NodeV;
Triangle* Triangles;
D3Dvec* edges1;
D3Dvec* edges2;
D3Dvec* Normals;
D3Dvec* NodeNormals;
#include "/usr/local/include/XFiles/List.h"
#include "/usr/local/include/XFiles/Node1.h"
typedef CMyNode1 <int>  ListNode;
typedef CMyLinkedList<ListNode> intList;
int const MD=4;  //MAXIMUM DEPTH OF TREE (depth ranges from 0 to MD
    intList** CollideTri; 
    int nlists;
    intList**  NodeTris;

#include "/usr/local/include/XFiles/TriOctNode.h"
#include "/usr/local/include/XFiles/TriOctTree.h"
typedef CMyOctNode <D3Dvec> ONode; //An ONode shall hold a vector
                                   //describing the compare point
				   // and a list of integers
				   // (Triangle array indexes)
typedef CMyTriOctTree<ONode> OTree;   //OctTree consists of nodes
        OTree* CollideTree;
/**********************************************************************/
#include "/usr/local/include/XFiles/Dmatrix.h"
//#include "/usr/local/include/XFiles/Solver.h"
//Solver replaced by myGaussElim in libmylib
/**********************************************************************/
//void Solver(int &dim, double &maxerr, Dmatrix& Matrix, Dvector& rhs, Dvector& SolveVec);
int myGaussElim(int& dimen , double** Matr , double* VexRhs, double* VexLhs );
/**********************************************************************/

D3Dvec RealCamPos;  //periodic boundary conditions
                    //CamPos thinks it  wanders in infinite space
		    //But RealCamPos knows about the pbc.
double X1, X2, Y1, Y2, Z1, Z2;
#include "/usr/local/include/XFiles/Camera.h"
#include "/usr/local/include/XFiles/gl2ps.h"

extern int VideoFlags;
extern SDL_Surface * MainWindow;

extern double speed;
extern const double convert;

int arg_count;
char **arg_list;
const int nplotsmax=1;
int ntri, istart, n_nodes, istop, ishow;

double speed=50.0, angspeed=0.05, speed_diff=.5;


// Prototypes for functions
//void EventLoop(CCam  & Camera1);
void EventLoop();
void RenderScene(CCam  & Camera1);
void PickObjects();
UINT* TriBuffer;
void ProcessHitsLP(int, UINT *);
void ProcessHitsLC(int, UINT *);
void ProcessHitsMC(int, UINT *);
int  ProcessTriangleHits(int, UINT *); //processing in Flight.cpp
int  ProcessNodeHits(int, UINT *);
void ProcessTBoxHits(int, UINT *); //processing in DrawXBox
void ProcessNBoxHits(int, UINT *);
void SetUp();
void ToggleWindow(void);
void MakeWindow(const char *Name, 
		const USHORT WIDTH,
	        const USHORT HEIGHT,
		 int VideoFlags);
void SizeOpenGLScreen(const USHORT, const USHORT);
void Init();
void InitialiseGL(int, int);
void HandleKeyPress(SDL_keysym * );
void HandleKeyRelease(SDL_keysym *);
void CheckMove(CCam &);
int Screenshot(SDL_Surface *screen, char *filename);

void CreateTexture(unsigned int textureArray[], char *strFilename, int textureID);

void OrthoMode(double , double , double, double);
void PerspectiveMode();
void DrawLightPos(bool);
void DrawLightCol(bool);
void DrawMatCol(bool);
void DrawTriHits(bool);
void DrawNodeHits(bool);

extern UINT base; //bitmap font functions
void buildFont(void); 
void QuitFont(int );    
void KillFont();
void glPrint( char *);

void SetShiftX(int);
void SetShiftY(int);

void DrawScene(bool);

#include "/usr/local/include/XFiles/Init.h"

