#pragma once
#include <vector>
#include <memory>
#include "../Ecs/BaseModel.h"

extern std::vector<std::unique_ptr<BaseModel>> ObjectVector; // the vector for all Models
//extern std::vector<std::shared_ptr<BaseModel>> ObjectVector; // the vector for all Models
//extern std::vector<std::unique_ptr<BaseModel>> LightVector; // the vector for all Lights 

extern char nameBuffer[128];

extern const int MAIN_GRID;
extern const int OBJ_CUBE;
extern const int OBJ_PLANE;
extern const int OBJ_CIRCLE;
extern const int OBJ_LINE;
extern const int OBJ_SPHERE;
extern const int OBJ_CYLINDER;
extern const int OBJ_TORUS;
extern const int OBJ_GRID;
extern const int OBJ_CONE;
extern const int OBJ_PYRAMID;
extern const int OBJ_TRIANGEL;
// .gltf files
extern const int GLTF_MODEL;
extern bool ShouldAddglTFModel;
extern bool ShouldUpdateglTFModel;
extern int glTFModelIndex;

// .obj files
extern const int OBJ_OBJ_MODEL;
extern bool ShouldAddObjModel;
extern bool ShouldUpdateObjModel;
extern int ModleObjidx;

// ##### Lighting ########### 
extern int LIGHT_SUN;
extern int LIGHT_POINT;
extern int LIGHT_SPOT;
extern int LIGHT_AREA;
extern int MAX_SUN_LIGHTS;
extern int MAX_POINT_LIGHTS;
extern int MAX_LIGHT_SPOT;
extern int MAX_LIGHT_AREA;
extern int LightIdx;
extern int LightSelector;  // This is used in the main screen menue
//extern int LightUpdateSelector;  // This is used in the Light Editor to set which light to update
extern bool ShouldAddSunLight;
extern bool ShouldAddPointLight;
extern bool ShouldAddSpotLight;
extern bool ShouldAddAreaLight;
extern bool ShouldUpdateSunLight;
extern bool ShouldUpdatePointLight;
extern bool ShouldUpdateSpotLight;
extern bool ShouldUpdateAreaLight;
extern bool ShowLightEditor;
extern float SunLightCol[4]; 
extern float SunLightIntensity;
extern float PointLightCol[4];
extern float PointLightIntensity;
extern float PointLightRadius;
extern float AreaLightCol[4];
extern float AreaLightIntensity;
extern float AreaLightArea;


// ##### End Lighting ########
// 
// #####  Terrain ############
extern const int TERRAIN_FLOOR;
extern const int TERRAIN_TERRAIN;
extern const int TERRAIN_WATER;
extern int TerrainIdx;
extern bool ShouldAddFloor;
extern bool ShouldAddTerrain;
extern bool ShouldAddWater;
extern bool ShouldUpdateTerrain;
extern bool ShouldUpdateWater;
extern bool ShouldUpdateFloor;
extern bool showTerrainEditor;

extern float terrain_Pos[3];
extern float terrain_Scale[3];

extern unsigned int creatTerrainTex;	// this only for the image that shows on the terrain editor window for editing
extern float tilingFactor; // Try values between 4–64 depending on your terrain size



// ##### End Terrain #########
//
// adding Objects
extern bool ShouldAddCube;		// add a cube
extern bool ShouldAddSphere;		// add a cube
extern bool ShouldAddPlane;		// add a plane
extern bool ShouldAddPyramid;	// add a pyramid
// updating Objects
extern bool ShouldUpdateCube;	// update the cube
extern bool ShouldUpdateSphere;	// update the sphere
extern bool ShouldUpdatePlane;	// update the plane
extern bool ShouldUpdatePyramid;	// update the plane


extern int objectUpdateIndex; // this is in use

// Selecting objects
extern bool SelectedObject;

//############# objects maps
extern unsigned int creatMap;	// this only for the image that shows on the edit window

//#############

// ###### For use with Textures and obj Files ##############
extern bool IsTexture; 
extern std::string modelPath;
extern bool dialogType;  // sets is textured or obj file for the opendialog box
// ###### Textures ##############

extern int currentIndex;

// indevidual object index's
extern int Gridobjidx; // Declaration of the global variable
extern int Cubeobjidx; // Declaration of the global variable
extern int Sphereobjidx;
extern int Triangleobjidx;
extern int Planeobjidx;
extern int Pyramidobjidx;



// Settings Window
extern float grid_square; // grid square size
extern int grid_size; // grid size x, z
extern bool show_settings_window;

// For the Cube
extern glm::vec3 newCubePosition;
extern glm::vec3 newCubeScale;
extern glm::vec3 newCubeRotation;

extern float object_Pos[3];
extern float object_Scale[3];
extern float object_Rotation[3];
// End of Cube
