#pragma once


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


extern bool ShouldAddCube; // add a cube
extern bool ShouldAddPlane;
extern bool ShouldUpdateCube;

extern bool shouldUpdateObject;
extern int objectUpdateIndex;

extern float objPostion[3];
extern float objScale[3];
extern float objRotation[3];

extern int currentIndex;

// indevidual object index's
extern int Gridobjidx; // Declaration of the global variable
extern int Cubeobjidx; // Declaration of the global variable
extern int Sphereobjidx;
extern int Triangleobjidx;
extern int Planeobjidx;


