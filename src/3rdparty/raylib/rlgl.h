/**********************************************************************************************
*
*   rlgl v4.0
*
*   An abstraction layer for multiple OpenGL versions (1.1, 2.1, 3.3 Core, ES 2.0)
*   that provides a pseudo-OpenGL 1.1 immediate-mode style API (rlVertex, rlTranslate, rlRotate...)
*
*   When chosing an OpenGL backend different than OpenGL 1.1, some internal buffer are
*   initialized on rlglInit() to accumulate vertex data.
*
*   When an internal state change is required all the stored vertex data is renderer in batch,
*   additioanlly, rlDrawRenderBatchActive() could be called to force flushing of the batch.
*
*   Some additional resources are also loaded for convenience, here the complete list:
*      - Default batch (RLGL.defaultBatch): RenderBatch system to accumulate vertex data
*      - Default texture (RLGL.defaultTextureId): 1x1 white pixel R8G8B8A8
*      - Default shader (RLGL.State.defaultShaderId, RLGL.State.defaultShaderLocs)
*
*   Internal buffer (and additional resources) must be manually unloaded calling rlglClose().
*
*
*   CONFIGURATION:
*
*   #define GRAPHICS_API_OPENGL_11
*   #define GRAPHICS_API_OPENGL_21
*   #define GRAPHICS_API_OPENGL_33
*   #define GRAPHICS_API_OPENGL_ES2
*       Use selected OpenGL graphics backend, should be supported by platform
*       Those preprocessor defines are only used on rlgl module, if OpenGL version is
*       required by any other module, use rlGetVersion() to check it
*
*   #define RLGL_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
*
*   #define SUPPORT_RENDER_TEXTURES_HINT
*       Enable framebuffer objects (fbo) support (enabled by default)
*       Some GPUs could not support them despite the OpenGL version
*
*   #define SUPPORT_GL_DETAILS_INFO
*       Show OpenGL extensions and capabilities detailed logs on init
*
*   rlgl capabilities could be customized just defining some internal
*   values before library inclusion (default values listed):
*
*   #define RL_DEFAULT_BATCH_BUFFER_ELEMENTS   8192    // Default internal render batch elements limits
*   #define RL_DEFAULT_BATCH_BUFFERS              1    // Default number of batch buffers (multi-buffering)
*   #define RL_DEFAULT_BATCH_DRAWCALLS          256    // Default number of batch draw calls (by state changes: mode, texture)
*   #define RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS    4    // Maximum number of textures units that can be activated on batch drawing (SetShaderValueTexture())
*
*   #define RL_MAX_MATRIX_STACK_SIZE             32    // Maximum size of internal Matrix stack
*   #define RL_MAX_SHADER_LOCATIONS              32    // Maximum number of shader locations supported
*   #define RL_CULL_DISTANCE_NEAR              0.01    // Default projection matrix near cull distance
*   #define RL_CULL_DISTANCE_FAR             1000.0    // Default projection matrix far cull distance
*
*   When loading a shader, the following vertex attribute and uniform
*   location names are tried to be set automatically:
*
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_POSITION     "vertexPosition"    // Binded by default to shader location: 0
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD     "vertexTexCoord"    // Binded by default to shader location: 1
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_NORMAL       "vertexNormal"      // Binded by default to shader location: 2
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_COLOR        "vertexColor"       // Binded by default to shader location: 3
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_TANGENT      "vertexTangent"     // Binded by default to shader location: 4
*   #define RL_DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2    "vertexTexCoord2"   // Binded by default to shader location: 5
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_MVP         "mvp"               // model-view-projection matrix
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_VIEW        "matView"           // view matrix
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_PROJECTION  "matProjection"     // projection matrix
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_MODEL       "matModel"          // model matrix
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_NORMAL      "matNormal"         // normal matrix (transpose(inverse(matModelView))
*   #define RL_DEFAULT_SHADER_UNIFORM_NAME_COLOR       "colDiffuse"        // color diffuse (base tint color, multiplied by texture color)
*   #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0  "texture0"          // texture0 (texture slot active 0)
*   #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1  "texture1"          // texture1 (texture slot active 1)
*   #define RL_DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2  "texture2"          // texture2 (texture slot active 2)
*
*   DEPENDENCIES:
*
*      - OpenGL libraries (depending on platform and OpenGL version selected)
*      - GLAD OpenGL extensions loading library (only for OpenGL 3.3 Core)
*
*
*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2021 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#pragma once

#define GRAPHICS_API_OPENGL_33



#define SUPPORT_RENDER_TEXTURES_HINT

#ifndef RLAPI
#define RLAPI   // We are building or using rlgl as a static library (or Linux shared library)
#endif

#if defined(_WIN32)
#if defined(BUILD_LIBTYPE_SHARED)
#define RLAPI __declspec(dllexport)         // We are building raylib as a Win32 shared library (.dll)
#elif defined(USE_LIBTYPE_SHARED)
#define RLAPI __declspec(dllimport)         // We are using raylib as a Win32 shared library (.dll)
#endif
#endif

// Support TRACELOG macros
#ifndef TRACELOG
#define TRACELOG(level, ...) (void)0
#define TRACELOGD(...) (void)0
#endif

// Allow custom memory allocators
#ifndef RL_MALLOC
#define RL_MALLOC(sz)     malloc(sz)
#endif
#ifndef RL_CALLOC
#define RL_CALLOC(n,sz)   calloc(n,sz)
#endif
#ifndef RL_REALLOC
#define RL_REALLOC(n,sz)  realloc(n,sz)
#endif
#ifndef RL_FREE
#define RL_FREE(p)        free(p)
#endif

// Security check in case no GRAPHICS_API_OPENGL_* defined
#if !defined(GRAPHICS_API_OPENGL_11) && \
!defined(GRAPHICS_API_OPENGL_21) && \
!defined(GRAPHICS_API_OPENGL_33) && \
!defined(GRAPHICS_API_OPENGL_ES2)
#define GRAPHICS_API_OPENGL_33
#endif

// Security check in case multiple GRAPHICS_API_OPENGL_* defined
#if defined(GRAPHICS_API_OPENGL_11)
#if defined(GRAPHICS_API_OPENGL_21)
#undef GRAPHICS_API_OPENGL_21
#endif
#if defined(GRAPHICS_API_OPENGL_33)
#undef GRAPHICS_API_OPENGL_33
#endif
#if defined(GRAPHICS_API_OPENGL_ES2)
#undef GRAPHICS_API_OPENGL_ES2
#endif
#endif

// OpenGL 2.1 uses most of OpenGL 3.3 Core functionality
// WARNING: Specific parts are checked with #if defines
#if defined(GRAPHICS_API_OPENGL_21)
#define GRAPHICS_API_OPENGL_33
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------

// Default internal render batch elements limits
#ifndef RL_DEFAULT_BATCH_BUFFER_ELEMENTS
#if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
// This is the maximum amount of elements (quads) per batch
// NOTE: Be careful with text, every letter maps to a quad
#define RL_DEFAULT_BATCH_BUFFER_ELEMENTS   8192
#endif
#if defined(GRAPHICS_API_OPENGL_ES2)
// We reduce memory sizes for embedded systems (RPI and HTML5)
// NOTE: On HTML5 (emscripten) this is allocated on heap,
// by default it's only 16MB!...just take care...
#define RL_DEFAULT_BATCH_BUFFER_ELEMENTS   2048
#endif
#endif
#ifndef RL_DEFAULT_BATCH_BUFFERS
#define RL_DEFAULT_BATCH_BUFFERS            1      // Default number of batch buffers (multi-buffering)
#endif
#ifndef RL_DEFAULT_BATCH_DRAWCALLS
#define RL_DEFAULT_BATCH_DRAWCALLS        256      // Default number of batch draw calls (by state changes: mode, texture)
#endif
#ifndef RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS
#define RL_DEFAULT_BATCH_MAX_TEXTURE_UNITS  4      // Maximum number of textures units that can be activated on batch drawing (SetShaderValueTexture())
#endif

// Internal Matrix stack
#ifndef RL_MAX_MATRIX_STACK_SIZE
#define RL_MAX_MATRIX_STACK_SIZE           32      // Maximum size of Matrix stack
#endif

// Shader limits
#ifndef RL_MAX_SHADER_LOCATIONS
#define RL_MAX_SHADER_LOCATIONS            32      // Maximum number of shader locations supported
#endif

// Projection matrix culling
#ifndef RL_CULL_DISTANCE_NEAR
#define RL_CULL_DISTANCE_NEAR            0.01      // Default near cull distance
#endif
#ifndef RL_CULL_DISTANCE_FAR
#define RL_CULL_DISTANCE_FAR           1000.0      // Default far cull distance
#endif

// Texture parameters (equivalent to OpenGL defines)
#define RL_TEXTURE_WRAP_S                       0x2802      // GL_TEXTURE_WRAP_S
#define RL_TEXTURE_WRAP_T                       0x2803      // GL_TEXTURE_WRAP_T
#define RL_TEXTURE_MAG_FILTER                   0x2800      // GL_TEXTURE_MAG_FILTER
#define RL_TEXTURE_MIN_FILTER                   0x2801      // GL_TEXTURE_MIN_FILTER

#define RL_TEXTURE_FILTER_NEAREST               0x2600      // GL_NEAREST
#define RL_TEXTURE_FILTER_LINEAR                0x2601      // GL_LINEAR
#define RL_TEXTURE_FILTER_MIP_NEAREST           0x2700      // GL_NEAREST_MIPMAP_NEAREST
#define RL_TEXTURE_FILTER_NEAREST_MIP_LINEAR    0x2702      // GL_NEAREST_MIPMAP_LINEAR
#define RL_TEXTURE_FILTER_LINEAR_MIP_NEAREST    0x2701      // GL_LINEAR_MIPMAP_NEAREST
#define RL_TEXTURE_FILTER_MIP_LINEAR            0x2703      // GL_LINEAR_MIPMAP_LINEAR
#define RL_TEXTURE_FILTER_ANISOTROPIC           0x3000      // Anisotropic filter (custom identifier)

#define RL_TEXTURE_WRAP_REPEAT                  0x2901      // GL_REPEAT
#define RL_TEXTURE_WRAP_CLAMP                   0x812F      // GL_CLAMP_TO_EDGE
#define RL_TEXTURE_WRAP_MIRROR_REPEAT           0x8370      // GL_MIRRORED_REPEAT
#define RL_TEXTURE_WRAP_MIRROR_CLAMP            0x8742      // GL_MIRROR_CLAMP_EXT

// Matrix modes (equivalent to OpenGL)
#define RL_MODELVIEW                            0x1700      // GL_MODELVIEW
#define RL_PROJECTION                           0x1701      // GL_PROJECTION
#define RL_TEXTURE                              0x1702      // GL_TEXTURE

// Primitive assembly draw modes
#define RL_LINES                                0x0001      // GL_LINES
#define RL_TRIANGLES                            0x0004      // GL_TRIANGLES
#define RL_QUADS                                0x0007      // GL_QUADS

// GL equivalent data types
#define RL_UNSIGNED_BYTE                        0x1401      // GL_UNSIGNED_BYTE
#define RL_FLOAT                                0x1406      // GL_FLOAT

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
	OPENGL_11 = 1,
	OPENGL_21,
	OPENGL_33,
	OPENGL_ES_20
} rlGlVersion;

typedef enum {
	RL_ATTACHMENT_COLOR_CHANNEL0 = 0,
	RL_ATTACHMENT_COLOR_CHANNEL1,
	RL_ATTACHMENT_COLOR_CHANNEL2,
	RL_ATTACHMENT_COLOR_CHANNEL3,
	RL_ATTACHMENT_COLOR_CHANNEL4,
	RL_ATTACHMENT_COLOR_CHANNEL5,
	RL_ATTACHMENT_COLOR_CHANNEL6,
	RL_ATTACHMENT_COLOR_CHANNEL7,
	RL_ATTACHMENT_DEPTH = 100,
	RL_ATTACHMENT_STENCIL = 200,
} rlFramebufferAttachType;

typedef enum {
	RL_ATTACHMENT_CUBEMAP_POSITIVE_X = 0,
	RL_ATTACHMENT_CUBEMAP_NEGATIVE_X,
	RL_ATTACHMENT_CUBEMAP_POSITIVE_Y,
	RL_ATTACHMENT_CUBEMAP_NEGATIVE_Y,
	RL_ATTACHMENT_CUBEMAP_POSITIVE_Z,
	RL_ATTACHMENT_CUBEMAP_NEGATIVE_Z,
	RL_ATTACHMENT_TEXTURE2D = 100,
	RL_ATTACHMENT_RENDERBUFFER = 200,
} rlFramebufferAttachTextureType;

// Dynamic vertex buffers (position + texcoords + colors + indices arrays)
typedef struct rlVertexBuffer {
	int elementsCount;          // Number of elements in the buffer (QUADS)

	int vCounter;               // Vertex position counter to process (and draw) from full buffer
	int tcCounter;              // Vertex texcoord counter to process (and draw) from full buffer
	int cCounter;               // Vertex color counter to process (and draw) from full buffer

	float* vertices;            // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
	float* texcoords;           // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
	unsigned char* colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
#if defined(GRAPHICS_API_OPENGL_11) || defined(GRAPHICS_API_OPENGL_33)
	unsigned int* indices;      // Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#endif
#if defined(GRAPHICS_API_OPENGL_ES2)
	unsigned short* indices;    // Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#endif
	unsigned int vaoId;         // OpenGL Vertex Array Object id
	unsigned int vboId[4];      // OpenGL Vertex Buffer Objects id (4 types of vertex data)
} rlVertexBuffer;

// Draw call type
// NOTE: Only texture changes register a new draw, other state-change-related elements are not
// used at this moment (vaoId, shaderId, matrices), raylib just forces a batch draw call if any
// of those state-change happens (this is done in core module)
typedef struct rlDrawCall {
	int mode;                   // Drawing mode: LINES, TRIANGLES, QUADS
	int vertexCount;            // Number of vertex of the draw
	int vertexAlignment;        // Number of vertex required for index alignment (LINES, TRIANGLES)
	//unsigned int vaoId;       // Vertex array id to be used on the draw -> Using RLGL.currentBatch->vertexBuffer.vaoId
	//unsigned int shaderId;    // Shader id to be used on the draw -> Using RLGL.currentShaderId
	unsigned int textureId;     // Texture id to be used on the draw -> Use to create new draw call if changes

	//Matrix projection;      // Projection matrix for this draw -> Using RLGL.projection by default
	//Matrix modelview;       // Modelview matrix for this draw -> Using RLGL.modelview by default
} rlDrawCall;

// rlRenderBatch type
typedef struct rlRenderBatch {
	int buffersCount;           // Number of vertex buffers (multi-buffering support)
	int currentBuffer;          // Current buffer tracking in case of multi-buffering
	rlVertexBuffer* vertexBuffer; // Dynamic buffer(s) for vertex data

	rlDrawCall* draws;          // Draw calls array, depends on textureId
	int drawsCounter;           // Draw calls counter
	float currentDepth;         // Current depth value for next draw
} rlRenderBatch;

#if defined(__STDC__) && __STDC_VERSION__ >= 199901L
#include <stdbool.h>
#elif !defined(__cplusplus) && !defined(bool) && !defined(RL_BOOL_TYPE)
// Boolean type
//typedef enum bool{ false, true } bool;
#endif

// Trace log level
// NOTE: Organized by priority level
typedef enum {
	RL_LOG_ALL = 0,            // Display all logs
	RL_LOG_TRACE,              // Trace logging, intended for internal use only
	RL_LOG_DEBUG,              // Debug logging, used for internal debugging, it should be disabled on release builds
	RL_LOG_INFO,               // Info logging, used for program execution info
	RL_LOG_WARNING,            // Warning logging, used on recoverable failures
	RL_LOG_ERROR,              // Error logging, used on unrecoverable failures
	RL_LOG_FATAL,              // Fatal logging, used to abort program: exit(EXIT_FAILURE)
	RL_LOG_NONE                // Disable logging
} rlTraceLogLevel;

// Texture formats (support depends on OpenGL version)
typedef enum {
	RL_PIXELFORMAT_UNCOMPRESSED_GRAYSCALE = 1,     // 8 bit per pixel (no alpha)
	RL_PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA,        // 8*2 bpp (2 channels)
	RL_PIXELFORMAT_UNCOMPRESSED_R5G6B5,            // 16 bpp
	RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8,            // 24 bpp
	RL_PIXELFORMAT_UNCOMPRESSED_R5G5B5A1,          // 16 bpp (1 bit alpha)
	RL_PIXELFORMAT_UNCOMPRESSED_R4G4B4A4,          // 16 bpp (4 bit alpha)
	RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,          // 32 bpp
	RL_PIXELFORMAT_UNCOMPRESSED_R32,               // 32 bpp (1 channel - float)
	RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32,         // 32*3 bpp (3 channels - float)
	RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,      // 32*4 bpp (4 channels - float)
	RL_PIXELFORMAT_COMPRESSED_DXT1_RGB,            // 4 bpp (no alpha)
	RL_PIXELFORMAT_COMPRESSED_DXT1_RGBA,           // 4 bpp (1 bit alpha)
	RL_PIXELFORMAT_COMPRESSED_DXT3_RGBA,           // 8 bpp
	RL_PIXELFORMAT_COMPRESSED_DXT5_RGBA,           // 8 bpp
	RL_PIXELFORMAT_COMPRESSED_ETC1_RGB,            // 4 bpp
	RL_PIXELFORMAT_COMPRESSED_ETC2_RGB,            // 4 bpp
	RL_PIXELFORMAT_COMPRESSED_ETC2_EAC_RGBA,       // 8 bpp
	RL_PIXELFORMAT_COMPRESSED_PVRT_RGB,            // 4 bpp
	RL_PIXELFORMAT_COMPRESSED_PVRT_RGBA,           // 4 bpp
	RL_PIXELFORMAT_COMPRESSED_ASTC_4x4_RGBA,       // 8 bpp
	RL_PIXELFORMAT_COMPRESSED_ASTC_8x8_RGBA        // 2 bpp
} rlPixelFormat;

// Texture parameters: filter mode
// NOTE 1: Filtering considers mipmaps if available in the texture
// NOTE 2: Filter is accordingly set for minification and magnification
typedef enum {
	RL_TEXTURE_FILTER_POINT = 0,               // No filter, just pixel aproximation
	RL_TEXTURE_FILTER_BILINEAR,                // Linear filtering
	RL_TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
	RL_TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
	RL_TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
	RL_TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
} rlTextureFilter;

// Color blending modes (pre-defined)
typedef enum {
	RL_BLEND_ALPHA = 0,                // Blend textures considering alpha (default)
	RL_BLEND_ADDITIVE,                 // Blend textures adding colors
	RL_BLEND_MULTIPLIED,               // Blend textures multiplying colors
	RL_BLEND_ADD_COLORS,               // Blend textures adding colors (alternative)
	RL_BLEND_SUBTRACT_COLORS,          // Blend textures subtracting colors (alternative)
	RL_BLEND_CUSTOM                    // Belnd textures using custom src/dst factors (use SetBlendModeCustom())
} rlBlendMode;

// Shader location point type
typedef enum {
	RL_SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
	RL_SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
	RL_SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
	RL_SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
	RL_SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
	RL_SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
	RL_SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
	RL_SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view (camera transform)
	RL_SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
	RL_SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
	RL_SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
	RL_SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
	RL_SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
	RL_SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
	RL_SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
	RL_SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: RL_SHADER_LOC_MAP_DIFFUSE)
	RL_SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: RL_SHADER_LOC_MAP_SPECULAR)
	RL_SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
	RL_SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
	RL_SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
	RL_SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
	RL_SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
	RL_SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
	RL_SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
	RL_SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
	RL_SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
} rlShaderLocationIndex;

#define RL_SHADER_LOC_MAP_DIFFUSE      RL_SHADER_LOC_MAP_ALBEDO
#define RL_SHADER_LOC_MAP_SPECULAR     RL_SHADER_LOC_MAP_METALNESS

// Shader uniform data type
typedef enum {
	RL_SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
	RL_SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
	RL_SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
	RL_SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
	RL_SHADER_UNIFORM_INT,             // Shader uniform type: int
	RL_SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
	RL_SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
	RL_SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
	RL_SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
} rlShaderUniformDataType;

// Shader attribute data types
typedef enum {
	RL_SHADER_ATTRIB_FLOAT = 0,        // Shader attribute type: float
	RL_SHADER_ATTRIB_VEC2,             // Shader attribute type: vec2 (2 float)
	RL_SHADER_ATTRIB_VEC3,             // Shader attribute type: vec3 (3 float)
	RL_SHADER_ATTRIB_VEC4              // Shader attribute type: vec4 (4 float)
} rlShaderAttributeDataType;

//------------------------------------------------------------------------------------
// Functions Declaration - Matrix operations
//------------------------------------------------------------------------------------
#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

	RLAPI void rlMatrixMode(int mode);                    // Choose the current matrix to be transformed
	RLAPI void rlPushMatrix(void);                        // Push the current matrix to stack
	RLAPI void rlPopMatrix(void);                         // Pop lattest inserted matrix from stack
	RLAPI void rlLoadIdentity(void);                      // Reset current matrix to identity matrix
	RLAPI void rlTranslatef(float x, float y, float z);   // Multiply the current matrix by a translation matrix
	RLAPI void rlRotatef(float angle, float x, float y, float z);  // Multiply the current matrix by a rotation matrix
	RLAPI void rlScalef(float x, float y, float z);       // Multiply the current matrix by a scaling matrix
	RLAPI void rlMultMatrixf(float* matf);                // Multiply the current matrix by another matrix
	RLAPI void rlFrustum(double left, double right, double bottom, double top, double znear, double zfar);
	RLAPI void rlOrtho(double left, double right, double bottom, double top, double znear, double zfar);
	RLAPI void rlViewport(int x, int y, int width, int height); // Set the viewport area

	//------------------------------------------------------------------------------------
	// Functions Declaration - Vertex level operations
	//------------------------------------------------------------------------------------
	RLAPI void rlBegin(int mode);                         // Initialize drawing mode (how to organize vertex)
	RLAPI void rlEnd(void);                               // Finish vertex providing
	RLAPI void rlVertex2i(int x, int y);                  // Define one vertex (position) - 2 int
	RLAPI void rlVertex2f(float x, float y);              // Define one vertex (position) - 2 float
	RLAPI void rlVertex3f(float x, float y, float z);     // Define one vertex (position) - 3 float
	RLAPI void rlTexCoord2f(float x, float y);            // Define one vertex (texture coordinate) - 2 float
	RLAPI void rlNormal3f(float x, float y, float z);     // Define one vertex (normal) - 3 float
	RLAPI void rlColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);  // Define one vertex (color) - 4 byte
	RLAPI void rlColor3f(float x, float y, float z);          // Define one vertex (color) - 3 float
	RLAPI void rlColor4f(float x, float y, float z, float w); // Define one vertex (color) - 4 float

	//------------------------------------------------------------------------------------
	// Functions Declaration - OpenGL style functions (common to 1.1, 3.3+, ES2)
	// NOTE: This functions are used to completely abstract raylib code from OpenGL layer,
	// some of them are direct wrappers over OpenGL calls, some others are custom
	//------------------------------------------------------------------------------------

	// Vertex buffers state
	RLAPI bool rlEnableVertexArray(unsigned int vaoId);     // Enable vertex array (VAO, if supported)
	RLAPI void rlDisableVertexArray(void);                  // Disable vertex array (VAO, if supported)
	RLAPI void rlEnableVertexBuffer(unsigned int id);       // Enable vertex buffer (VBO)
	RLAPI void rlDisableVertexBuffer(void);                 // Disable vertex buffer (VBO)
	RLAPI void rlEnableVertexBufferElement(unsigned int id);// Enable vertex buffer element (VBO element)
	RLAPI void rlDisableVertexBufferElement(void);          // Disable vertex buffer element (VBO element)
	RLAPI void rlEnableVertexAttribute(unsigned int index); // Enable vertex attribute index
	RLAPI void rlDisableVertexAttribute(unsigned int index);// Disable vertex attribute index
#if defined(GRAPHICS_API_OPENGL_11)
	RLAPI void rlEnableStatePointer(int vertexAttribType, void* buffer);    // Enable attribute state pointer
	RLAPI void rlDisableStatePointer(int vertexAttribType);                 // Disable attribute state pointer
#endif

	// Textures state
	RLAPI void rlActiveTextureSlot(int slot);               // Select and active a texture slot
	RLAPI void rlEnableTexture(unsigned int id);            // Enable texture
	RLAPI void rlDisableTexture(void);                      // Disable texture
	RLAPI void rlEnableTextureCubemap(unsigned int id);     // Enable texture cubemap
	RLAPI void rlDisableTextureCubemap(void);               // Disable texture cubemap
	RLAPI void rlTextureParameters(unsigned int id, int param, int value); // Set texture parameters (filter, wrap)

	// Shader state
	RLAPI void rlEnableShader(unsigned int id);             // Enable shader program
	RLAPI void rlDisableShader(void);                       // Disable shader program

	// Framebuffer state
	RLAPI void rlEnableFramebuffer(unsigned int id);        // Enable render texture (fbo)
	RLAPI void rlDisableFramebuffer(void);                  // Disable render texture (fbo), return to default framebuffer
	RLAPI void rlActiveDrawBuffers(int count);              // Activate multiple draw color buffers

	// General render state
	RLAPI void rlEnableColorBlend(void);                    // Enable color blending
	RLAPI void rlDisableColorBlend(void);                   // Disable color blending
	RLAPI void rlEnableDepthTest(void);                     // Enable depth test
	RLAPI void rlDisableDepthTest(void);                    // Disable depth test
	RLAPI void rlEnableDepthMask(void);                     // Enable depth write
	RLAPI void rlDisableDepthMask(void);                    // Disable depth write
	RLAPI void rlEnableBackfaceCulling(void);               // Enable backface culling
	RLAPI void rlDisableBackfaceCulling(void);              // Disable backface culling
	RLAPI void rlEnableScissorTest(void);                   // Enable scissor test
	RLAPI void rlDisableScissorTest(void);                  // Disable scissor test
	RLAPI void rlScissor(int x, int y, int width, int height); // Scissor test
	RLAPI void rlEnableWireMode(void);                      // Enable wire mode
	RLAPI void rlDisableWireMode(void);                     // Disable wire mode
	RLAPI void rlSetLineWidth(float width);                 // Set the line drawing width
	RLAPI float rlGetLineWidth(void);                       // Get the line drawing width
	RLAPI void rlEnableSmoothLines(void);                   // Enable line aliasing
	RLAPI void rlDisableSmoothLines(void);                  // Disable line aliasing
	RLAPI void rlEnableStereoRender(void);                  // Enable stereo rendering
	RLAPI void rlDisableStereoRender(void);                 // Disable stereo rendering
	RLAPI bool rlIsStereoRenderEnabled(void);               // Check if stereo render is enabled

	RLAPI void rlClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Clear color buffer with color
	RLAPI void rlClearScreenBuffers(void);                  // Clear used screen buffers (color and depth)
	RLAPI void rlCheckErrors(void);                         // Check and log OpenGL error codes
	RLAPI void rlSetBlendMode(int mode);                    // Set blending mode
	RLAPI void rlSetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation); // Set blending mode factor and equation (using OpenGL factors)

	//------------------------------------------------------------------------------------
	// Functions Declaration - rlgl functionality
	//------------------------------------------------------------------------------------
	// rlgl initialization functions
	RLAPI void rlglInit(int width, int height);           // Initialize rlgl (buffers, shaders, textures, states)
	RLAPI void rlglClose(void);                           // De-inititialize rlgl (buffers, shaders, textures)
	RLAPI void rlLoadExtensions(void* loader);            // Load OpenGL extensions (loader function required)
	RLAPI int rlGetVersion(void);                         // Get current OpenGL version
	RLAPI int rlGetFramebufferWidth(void);                // Get default framebuffer width
	RLAPI int rlGetFramebufferHeight(void);               // Get default framebuffer height

	RLAPI unsigned int rlGetTextureIdDefault(void);       // Get default texture id
	RLAPI unsigned int rlGetShaderIdDefault(void);        // Get default shader id
	RLAPI int* rlGetShaderLocsDefault(void);              // Get default shader locations

	// Render batch management
	// NOTE: rlgl provides a default render batch to behave like OpenGL 1.1 immediate mode
	// but this render batch API is exposed in case of custom batches are required
	RLAPI rlRenderBatch rlLoadRenderBatch(int numBuffers, int bufferElements);  // Load a render batch system
	RLAPI void rlUnloadRenderBatch(rlRenderBatch batch);                        // Unload render batch system
	RLAPI void rlDrawRenderBatch(rlRenderBatch* batch);                         // Draw render batch data (Update->Draw->Reset)
	RLAPI void rlSetRenderBatchActive(rlRenderBatch* batch);                    // Set the active render batch for rlgl (NULL for default internal)
	RLAPI void rlDrawRenderBatchActive(void);                                   // Update and draw internal render batch
	RLAPI bool rlCheckRenderBatchLimit(int vCount);                             // Check internal buffer overflow for a given number of vertex
	RLAPI void rlSetTexture(unsigned int id);           // Set current texture for render batch and check buffers limits

	//------------------------------------------------------------------------------------------------------------------------

	// Vertex buffers management
	RLAPI unsigned int rlLoadVertexArray(void);                               // Load vertex array (vao) if supported
	RLAPI unsigned int rlLoadVertexBuffer(void* buffer, int size, bool dynamic);            // Load a vertex buffer attribute
	RLAPI unsigned int rlLoadVertexBufferElement(void* buffer, int size, bool dynamic);     // Load a new attributes element buffer
	RLAPI void rlUpdateVertexBuffer(unsigned int bufferId, void* data, int dataSize, int offset);    // Update GPU buffer with new data
	RLAPI void rlUnloadVertexArray(unsigned int vaoId);
	RLAPI void rlUnloadVertexBuffer(unsigned int vboId);
	RLAPI void rlSetVertexAttribute(unsigned int index, int compSize, int type, bool normalized, int stride, void* pointer);
	RLAPI void rlSetVertexAttributeDivisor(unsigned int index, int divisor);
	RLAPI void rlSetVertexAttributeDefault(int locIndex, const void* value, int attribType, int count); // Set vertex attribute default value
	RLAPI void rlDrawVertexArray(int offset, int count);
	RLAPI void rlDrawVertexArrayElements(int offset, int count, void* buffer);
	RLAPI void rlDrawVertexArrayInstanced(int offset, int count, int instances);
	RLAPI void rlDrawVertexArrayElementsInstanced(int offset, int count, void* buffer, int instances);

	// Textures management
	RLAPI unsigned int rlLoadTexture(void* data, int width, int height, int format, int mipmapCount); // Load texture in GPU
	RLAPI unsigned int rlLoadTextureDepth(int width, int height, bool useRenderBuffer);               // Load depth texture/renderbuffer (to be attached to fbo)
	RLAPI unsigned int rlLoadTextureCubemap(void* data, int size, int format);                        // Load texture cubemap
	RLAPI void rlUpdateTexture(unsigned int id, int offsetX, int offsetY, int width, int height, int format, const void* data);  // Update GPU texture with new data
	RLAPI void rlGetGlTextureFormats(int format, unsigned int* glInternalFormat, unsigned int* glFormat, unsigned int* glType);  // Get OpenGL internal formats
	RLAPI const char* rlGetPixelFormatName(unsigned int format);              // Get name string for pixel format
	RLAPI void rlUnloadTexture(unsigned int id);                              // Unload texture from GPU memory
	RLAPI void rlGenTextureMipmaps(unsigned int id, int width, int height, int format, int* mipmaps); // Generate mipmap data for selected texture
	RLAPI void* rlReadTexturePixels(unsigned int id, int width, int height, int format);              // Read texture pixel data
	RLAPI unsigned char* rlReadScreenPixels(int width, int height);           // Read screen pixel data (color buffer)

	// Framebuffer management (fbo)
	RLAPI unsigned int rlLoadFramebuffer(int width, int height);              // Load an empty framebuffer
	RLAPI void rlFramebufferAttach(unsigned int fboId, unsigned int texId, int attachType, int texType, int mipLevel);  // Attach texture/renderbuffer to a framebuffer
	RLAPI bool rlFramebufferComplete(unsigned int id);                        // Verify framebuffer is complete
	RLAPI void rlUnloadFramebuffer(unsigned int id);                          // Delete framebuffer from GPU

	// Shaders management
	RLAPI unsigned int rlLoadShaderCode(const char* vsCode, const char* fsCode);    // Load shader from code strings
	RLAPI unsigned int rlCompileShader(const char* shaderCode, int type);           // Compile custom shader and return shader id (type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
	RLAPI unsigned int rlLoadShaderProgram(unsigned int vShaderId, unsigned int fShaderId); // Load custom shader program
	RLAPI void rlUnloadShaderProgram(unsigned int id);                              // Unload shader program
	RLAPI int rlGetLocationUniform(unsigned int shaderId, const char* uniformName); // Get shader location uniform
	RLAPI int rlGetLocationAttrib(unsigned int shaderId, const char* attribName);   // Get shader location attribute
	RLAPI void rlSetUniform(int locIndex, const void* value, int uniformType, int count); // Set shader value uniform
	RLAPI void rlSetUniformMatrix(int locIndex, Matrix mat);                      // Set shader value matrix
	RLAPI void rlSetUniformSampler(int locIndex, unsigned int textureId);           // Set shader value sampler
	RLAPI void rlSetShader(unsigned int id, int* locs);                             // Set shader currently active (id and locations)

	// Matrix state management
	RLAPI Matrix rlGetMatrixModelview(void);                                  // Get internal modelview matrix
	RLAPI Matrix rlGetMatrixProjection(void);                                 // Get internal projection matrix
	RLAPI Matrix rlGetMatrixTransform(void);                                  // Get internal accumulated transform matrix
	RLAPI Matrix rlGetMatrixProjectionStereo(int eye);                        // Get internal projection matrix for stereo render (selected eye)
	RLAPI Matrix rlGetMatrixViewOffsetStereo(int eye);                        // Get internal view offset matrix for stereo render (selected eye)
	RLAPI void rlSetMatrixProjection(Matrix proj);                            // Set a custom projection matrix (replaces internal projection matrix)
	RLAPI void rlSetMatrixModelview(Matrix view);                             // Set a custom modelview matrix (replaces internal modelview matrix)
	RLAPI void rlSetMatrixProjectionStereo(Matrix right, Matrix left);      // Set eyes projection matrices for stereo rendering
	RLAPI void rlSetMatrixViewOffsetStereo(Matrix right, Matrix left);      // Set eyes view offsets matrices for stereo rendering

	// Quick and dirty cube/quad buffers load->draw->unload
	RLAPI void rlLoadDrawCube(void);     // Load and draw a cube
	RLAPI void rlLoadDrawQuad(void);     // Load and draw a quad
#if defined(__cplusplus)
}
#endif