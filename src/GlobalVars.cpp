#include "Headers\GlobalVars.h"

std::vector<std::unique_ptr<BaseModel>> ObjectVector; // the vector for all Models
//std::vector<std::unique_ptr<BaseModel>> LightVector; // the vector for all Lights 

const int MAIN_GRID = 0; // this is for the editor grid
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


bool ShouldAddCube = false;     // add a new cube
bool ShouldAddPlane = false;    // add a new plane
bool ShouldAddPyramid = false;

bool ShouldUpdateCube = false;  // update the cube
bool ShouldUpdatePlane = false; // update the plane
bool ShouldUpdatePyramid = false; // update the pyramid


//int objectUpdateIndex = -1;
int objectUpdateIndex = 0; 
// #######


unsigned int creatMap = 0;      // this only for the image that shows on the edit window for the selected texture

// ####### Settings
float grid_square = 2;
int grid_size = 40;

// ###### Textures ##############



// ###### Textures ##############



int currentIndex = -1;

int Gridobjidx = 0;
int Cubeobjidx = 0;   // Definition and initialization of the global variable
int Planeobjidx = -1; //0
int Pyramidobjidx = -1; //0
int Sphereobjidx = 0;
int Triangleobjidx = 0;

float object_Pos[3] = { 0.0f, 0.0f, 0.0f };
float object_Scale[3] = { 1.0f, 1.0f, 1.0f };
float object_Rotation[3] = { 0.0f, 0.0f, 0.0f };
