#include "Headers\GlobalVars.h"

std::vector<std::unique_ptr<BaseModel>> ObjectVector; // the vector for all Models
//std::vector<std::shared_ptr<BaseModel>> ObjectVector; // the vector for all Models
//std::vector<std::unique_ptr<BaseModel>> LightVector; // the vector for all Lights 

char nameBuffer[128] = "";

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

const int GLTF_MODEL = 14;
bool ShouldAddglTFModel = false;
bool ShouldUpdateglTFModel = false;
int glTFModelIndex = 0;

const int OBJ_OBJ_MODEL = 12;
bool ShouldAddObjModel = false;
bool ShouldUpdateObjModel = false;
int ModleObjidx = 0;

// ##### Lighting ###########
int LIGHT_SUN = 20;
int LIGHT_POINT = 21;
int LIGHT_SPOT = 22;
int LIGHT_AREA = 23;
int MAX_SUN_LIGHTS = 0;
int MAX_POINT_LIGHTS = 10;
int MAX_LIGHT_SPOT = 10;
int MAX_LIGHT_AREA = 10;
int LightIdx = 0;
int LightSelector =  0;
//int LightUpdateSelector =  0;
bool ShouldAddSunLight = false;
bool ShouldAddPointLight = false;
bool ShouldAddSpotLight = false;
bool ShouldAddAreaLight = false;
bool ShouldUpdateSunLight = false;
bool ShouldUpdatePointLight = false;
bool ShouldUpdateSpotLight = false;
bool ShouldUpdateAreaLight = false;
bool ShowLightEditor = false;
float SunLightCol[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float SunLightIntensity = 0.4f;
float PointLightRadius = 0.0f;
float PointLightCol[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
float PointLightIntensity = 0.5f;
float AreaLightCol[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
float AreaLightIntensity = 0.5f;
float AreaLightArea = 1.0f;

// ##### End Lighting ########

// #####  Terrain ############
const int TERRAIN_FLOOR = 25;
const int TERRAIN_TERRAIN = 26;
const int TERRAIN_WATER = 27;
int TerrainIdx = 0;
bool ShouldAddFloor = false;
bool ShouldAddTerrain = false;
bool ShouldAddWater = false;
bool ShouldUpdateTerrain = false;
bool ShouldUpdateBlendMap = false;
bool ShouldUpdateWater = false;
bool ShouldUpdateFloor = false;
bool showTerrainEditor = false;

float terrain_Pos[3] = { 0.0f, 15.0f, 0.0f };
float terrain_Scale[3] = { 1.0f, 1.0f, 1.0f };

unsigned int creatTerrainTex = 0;
float tilingFactor = 4.0f; // Try values between 4–64 depending on your terrain size
float slopeStart = 0.6f; // steepness begins
float slopeEnd = 0.9f; // fully steep
float heightBlendRange = 0.1f; // fully steep

// ########### End Terrain start Sky #########
 const int SKY_BOX = 30;
 const int SKY_BOX_HEM = 31;
 const int SKY_BOX_HDRI = 32;
 int SkyIdx = 0;
 unsigned int creatSkyTex = 0;
 bool SkyBoxSet = false;   // this is used so we can only load 1 skyBox
 bool ShowSkyEditor = false;
 bool ShouldAddSkyBox = false;
 bool ShouldAddSkyHem = false;
 bool ShouldAddSkyHDIR = false;
 bool ShouldUpdateSkyBox = false;
// ################ End Sky ##################
// 
// ################ Edit Object ##################
 const int HALF_EDGE_MESH = 15;
 int HalfEdgeIdx = 0;
 int selectedFaceIndex = -1;
 bool ShouldAddEditMesh = false;
 bool ShouldUpdateEditMesh = false;
 bool ShowHalfEdgeEditor = false;
// ################ End Edit Object ##################

bool ShouldAddCube = false;     // add a new cube
bool ShouldAddPlane = false;    // add a new plane
bool ShouldAddSphere = false;    // add a new sphere
bool ShouldAddPyramid = false;

bool ShouldUpdateCube = false;  // update the cube
bool ShouldUpdateSphere = false;  // update the sphere
bool ShouldUpdatePlane = false; // update the plane
bool ShouldUpdatePyramid = false; // update the pyramid

//int objectUpdateIndex = -1;
int objectUpdateIndex = 0; 
// #######

// Selecting objects
bool SelectedObject = false;

unsigned int creatMap = 0;      // this only for the image that shows on the edit window for the selected texture

// ####### Settings
float grid_square = 2;
int grid_size = 40;

// ###### Textures ##############
bool IsTexture = true; // sets is textured or obj file for opendialog
std::string modelPath = "";
bool dialogType = false; // false = dialog Image, true = dialog model

// ###### End Textures ##############

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
