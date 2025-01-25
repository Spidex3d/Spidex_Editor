

#include "Headers\GlobalVars.h"


const int MAIN_GRID = 0; // this is the editor grid
const int OBJ_CUBE = 1;
const int OBJ_PLANE = 2;
const int OBJ_CIRCLE = 3;
const int OBJ_LINE = 4;
const int OBJ_SPHERE = 5;
const int OBJ_CYLINDER = 6;
const int OBJ_TORUS = 7;
const int OBJ_GRID = 8;
const int OBJ_CONE = 9;
const int OBJ_PYRAMID = 10;
const int OBJ_TRIANGEL = 11;


bool ShouldAddCube = false;
bool ShouldAddPlane = false;

bool ShouldUpdateCube = false;

bool shouldUpdateObject = false;
int objectUpdateIndex = -1;

// Object Info
float objPostion[3] =  { 0.0f, 0.0f, 0.0f };
float objScale[3] =    { 1.0f, 1.0f, 1.0f };
float objRotation[3] = { 0.0f, 0.0f, 0.0f };



int currentIndex = -1;

int Gridobjidx = 0;
int Cubeobjidx = 0; // Definition and initialization of the global variable
int Sphereobjidx = 0;
int Triangleobjidx = 0;
int Planeobjidx = 0;
