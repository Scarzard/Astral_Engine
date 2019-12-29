#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <list>
#include <vector>

#include "Json Parser/nlohmann/json.hpp"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

#define ALIGN_CLASS_TO_16 \
    void* operator new(size_t i) { return _aligned_malloc(i,16); }\
    void operator delete(void* p) { _aligned_free(p); }

#ifdef NULL
#undef NULL
#endif
#define NULL  0

// Deletes a buffer
#define RELEASE( x ) \
    {                        \
    if( x != NULL )        \
	    {                      \
      delete x;            \
	  x = NULL;              \
	    }                      \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x ) \
    {                              \
    if( x != NULL )              \
	    {                            \
      delete[] x;                \
	  x = NULL;                    \
	    }                            \
                              \
    }


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -------------------------------------
#define VSYNC true

#define ASSETS_FOLDER "/Assets/"
#define ASSETS_SCENES_FOLDER "Assets/Scenes/"
#define ASSETS_TEXTURES_FOLDER "Assets/Textures/"
#define ASSETS_MODELS_FOLDER "Assets/Models/"
#define LIBRARY_FOLDER "/Library/"
#define LIBRARY_TEXTURES_FOLDER "/Library/Textures/"
#define LIBRARY_MESH_FOLDER "/Library/Meshes/"
#define LIBRARY_ANIMS_FOLDER "/Library/Animations/"
#define LIBRARY_BONES_FOLDER "/Library/Bones/"
#define LIBRARY_TRANSF_FOLDER "/Library/Transforms/"
#define LIBRARY_MODELS_FOLDER "/Library/Models/"