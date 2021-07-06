/**********************************************************************************************
*
*   rlgl v3.7 - raylib OpenGL abstraction layer
*
*   rlgl is a wrapper for multiple OpenGL versions (1.1, 2.1, 3.3 Core, ES 2.0) to
*   pseudo-OpenGL 1.1 style functions (rlVertex, rlTranslate, rlRotate...).
*
*   When chosing an OpenGL version greater than OpenGL 1.1, rlgl stores vertex data on internal
*   VBO buffers (and VAOs if available). It requires calling 3 functions:
*       rlglInit()  - Initialize internal buffers and auxiliary resources
*       rlglClose() - De-initialize internal buffers data and other auxiliar resources
*
*   CONFIGURATION:
*
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
*   #define SUPPORT_GL_DETAILS_INFO
*       Show OpenGL extensions and capabilities detailed logs on init
**********************************************************************************************/
#pragma once
#include "raylib.h"         // Required for: Shader, Texture2D
#include "raymath.h"        // Required for: Vector3, Matrix

// Security check in case no GRAPHICS_API_OPENGL_* defined
#if !defined(GRAPHICS_API_OPENGL_33) && \
    !defined(GRAPHICS_API_OPENGL_ES2)
        #define GRAPHICS_API_OPENGL_33
#endif

#define SUPPORT_RENDER_TEXTURES_HINT

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Default internal render batch limits
#ifndef DEFAULT_BATCH_BUFFER_ELEMENTS
    #if defined(GRAPHICS_API_OPENGL_33)
        // This is the maximum amount of elements (quads) per batch
        // NOTE: Be careful with text, every letter maps to a quad
        #define DEFAULT_BATCH_BUFFER_ELEMENTS   8192
    #endif
    #if defined(GRAPHICS_API_OPENGL_ES2)
        // We reduce memory sizes for embedded systems (RPI and HTML5)
        // NOTE: On HTML5 (emscripten) this is allocated on heap,
        // by default it's only 16MB!...just take care...
        #define DEFAULT_BATCH_BUFFER_ELEMENTS   2048
    #endif
#endif
#ifndef DEFAULT_BATCH_BUFFERS
    #define DEFAULT_BATCH_BUFFERS            1      // Default number of batch buffers (multi-buffering)
#endif
#ifndef DEFAULT_BATCH_DRAWCALLS
    #define DEFAULT_BATCH_DRAWCALLS        256      // Default number of batch draw calls (by state changes: mode, texture)
#endif
#ifndef MAX_BATCH_ACTIVE_TEXTURES
    #define MAX_BATCH_ACTIVE_TEXTURES        4      // Maximum number of additional textures that can be activated on batch drawing (SetShaderValueTexture())
#endif

// Internal Matrix stack
#ifndef MAX_MATRIX_STACK_SIZE
    #define MAX_MATRIX_STACK_SIZE           32      // Maximum size of Matrix stack
#endif

// Vertex buffers id limit
#ifndef MAX_MESH_VERTEX_BUFFERS
    #define MAX_MESH_VERTEX_BUFFERS          7      // Maximum vertex buffers (VBO) per mesh
#endif

// Shader and material limits
#ifndef MAX_SHADER_LOCATIONS
    #define MAX_SHADER_LOCATIONS            32      // Maximum number of shader locations supported
#endif
#ifndef MAX_MATERIAL_MAPS
    #define MAX_MATERIAL_MAPS               12      // Maximum number of shader maps supported
#endif

// Projection matrix culling
#ifndef RL_CULL_DISTANCE_NEAR
    #define RL_CULL_DISTANCE_NEAR         0.01      // Default near cull distance
#endif
#ifndef RL_CULL_DISTANCE_FAR
    #define RL_CULL_DISTANCE_FAR        1000.0      // Default far cull distance
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
typedef enum { OPENGL_33, OPENGL_ES_20 } GlVersion;

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
} FramebufferAttachType;

typedef enum {
    RL_ATTACHMENT_CUBEMAP_POSITIVE_X = 0,
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_X,
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Y,
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Y,
    RL_ATTACHMENT_CUBEMAP_POSITIVE_Z,
    RL_ATTACHMENT_CUBEMAP_NEGATIVE_Z,
    RL_ATTACHMENT_TEXTURE2D = 100,
    RL_ATTACHMENT_RENDERBUFFER = 200,
} FramebufferAttachTextureType;

// Dynamic vertex buffers (position + texcoords + colors + indices arrays)
typedef struct VertexBuffer {
    int elementsCount;          // Number of elements in the buffer (QUADS)

    int vCounter;               // Vertex position counter to process (and draw) from full buffer
    int tcCounter;              // Vertex texcoord counter to process (and draw) from full buffer
    int cCounter;               // Vertex color counter to process (and draw) from full buffer

    float *vertices;            // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;           // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    unsigned char *colors;      // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
#if defined(GRAPHICS_API_OPENGL_33)
    unsigned int *indices;      // Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#endif
#if defined(GRAPHICS_API_OPENGL_ES2)
    unsigned short *indices;    // Vertex indices (in case vertex data comes indexed) (6 indices per quad)
#endif
    unsigned int vaoId;         // OpenGL Vertex Array Object id
    unsigned int vboId[4];      // OpenGL Vertex Buffer Objects id (4 types of vertex data)
} VertexBuffer;

// Draw call type
// NOTE: Only texture changes register a new draw, other state-change-related elements are not
// used at this moment (vaoId, shaderId, matrices), raylib just forces a batch draw call if any
// of those state-change happens (this is done in core module)
typedef struct DrawCall {
    int mode;                   // Drawing mode: LINES, TRIANGLES, QUADS
    int vertexCount;            // Number of vertex of the draw
    int vertexAlignment;        // Number of vertex required for index alignment (LINES, TRIANGLES)
    //unsigned int vaoId;       // Vertex array id to be used on the draw -> Using RLGL.currentBatch->vertexBuffer.vaoId
    //unsigned int shaderId;    // Shader id to be used on the draw -> Using RLGL.currentShader.id
    unsigned int textureId;     // Texture id to be used on the draw -> Use to create new draw call if changes

    //Matrix projection;        // Projection matrix for this draw -> Using RLGL.projection by default
    //Matrix modelview;         // Modelview matrix for this draw -> Using RLGL.modelview by default
} DrawCall;

// RenderBatch type
typedef struct RenderBatch {
    int buffersCount;           // Number of vertex buffers (multi-buffering support)
    int currentBuffer;          // Current buffer tracking in case of multi-buffering
    VertexBuffer *vertexBuffer; // Dynamic buffer(s) for vertex data

    DrawCall *draws;            // Draw calls array, depends on textureId
    int drawsCounter;           // Draw calls counter
    float currentDepth;         // Current depth value for next draw
} RenderBatch;

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif

//------------------------------------------------------------------------------------
// Functions Declaration - Matrix operations
//------------------------------------------------------------------------------------
void rlMatrixMode(int mode);                    // Choose the current matrix to be transformed
void rlPushMatrix(void);                        // Push the current matrix to stack
void rlPopMatrix(void);                         // Pop lattest inserted matrix from stack
void rlLoadIdentity(void);                      // Reset current matrix to identity matrix
void rlTranslatef(float x, float y, float z);   // Multiply the current matrix by a translation matrix
void rlRotatef(float angleDeg, float x, float y, float z);  // Multiply the current matrix by a rotation matrix
void rlScalef(float x, float y, float z);       // Multiply the current matrix by a scaling matrix
void rlMultMatrixf(float *matf);                // Multiply the current matrix by another matrix
void rlFrustum(double left, double right, double bottom, double top, double znear, double zfar);
void rlOrtho(double left, double right, double bottom, double top, double znear, double zfar);
void rlViewport(int x, int y, int width, int height); // Set the viewport area

//------------------------------------------------------------------------------------
// Functions Declaration - Vertex level operations
//------------------------------------------------------------------------------------
void rlBegin(int mode);                         // Initialize drawing mode (how to organize vertex)
void rlEnd(void);                               // Finish vertex providing
void rlVertex2i(int x, int y);                  // Define one vertex (position) - 2 int
void rlVertex2f(float x, float y);              // Define one vertex (position) - 2 float
void rlVertex3f(float x, float y, float z);     // Define one vertex (position) - 3 float
void rlTexCoord2f(float x, float y);            // Define one vertex (texture coordinate) - 2 float
void rlNormal3f(float x, float y, float z);     // Define one vertex (normal) - 3 float
void rlColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a);  // Define one vertex (color) - 4 byte
void rlColor3f(float x, float y, float z);          // Define one vertex (color) - 3 float
void rlColor4f(float x, float y, float z, float w); // Define one vertex (color) - 4 float

//------------------------------------------------------------------------------------
// Functions Declaration - OpenGL style functions (common to 1.1, 3.3+, ES2)
// NOTE: This functions are used to completely abstract raylib code from OpenGL layer,
// some of them are direct wrappers over OpenGL calls, some others are custom
//------------------------------------------------------------------------------------

// Vertex buffers state
bool rlEnableVertexArray(unsigned int vaoId);     // Enable vertex array (VAO, if supported)
void rlDisableVertexArray(void);                  // Disable vertex array (VAO, if supported)
void rlEnableVertexBuffer(unsigned int id);       // Enable vertex buffer (VBO)
void rlDisableVertexBuffer(void);                 // Disable vertex buffer (VBO)
void rlEnableVertexBufferElement(unsigned int id);// Enable vertex buffer element (VBO element)
void rlDisableVertexBufferElement(void);          // Disable vertex buffer element (VBO element)
void rlEnableVertexAttribute(unsigned int index); // Enable vertex attribute index
void rlDisableVertexAttribute(unsigned int index);// Disable vertex attribute index

// Textures state
void rlActiveTextureSlot(int slot);               // Select and active a texture slot
void rlEnableTexture(unsigned int id);            // Enable texture
void rlDisableTexture(void);                      // Disable texture
void rlEnableTextureCubemap(unsigned int id);     // Enable texture cubemap
void rlDisableTextureCubemap(void);               // Disable texture cubemap
void rlTextureParameters(unsigned int id, int param, int value); // Set texture parameters (filter, wrap)

// Shader state
void rlEnableShader(unsigned int id);             // Enable shader program
void rlDisableShader(void);                       // Disable shader program

// Framebuffer state
void rlEnableFramebuffer(unsigned int id);        // Enable render texture (fbo)
void rlDisableFramebuffer(void);                  // Disable render texture (fbo), return to default framebuffer

// General render state
void rlEnableDepthTest(void);                     // Enable depth test
void rlDisableDepthTest(void);                    // Disable depth test
void rlEnableDepthMask(void);                     // Enable depth write
void rlDisableDepthMask(void);                    // Disable depth write
void rlEnableBackfaceCulling(void);               // Enable backface culling
void rlDisableBackfaceCulling(void);              // Disable backface culling
void rlEnableScissorTest(void);                   // Enable scissor test
void rlDisableScissorTest(void);                  // Disable scissor test
void rlScissor(int x, int y, int width, int height); // Scissor test
void rlEnableWireMode(void);                      // Enable wire mode
void rlDisableWireMode(void);                     // Disable wire mode
void rlSetLineWidth(float width);                 // Set the line drawing width
float rlGetLineWidth(void);                       // Get the line drawing width
void rlEnableSmoothLines(void);                   // Enable line aliasing
void rlDisableSmoothLines(void);                  // Disable line aliasing
void rlEnableStereoRender(void);                  // Enable stereo rendering
void rlDisableStereoRender(void);                 // Disable stereo rendering
bool rlIsStereoRenderEnabled(void);               // Check if stereo render is enabled

void rlClearColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a); // Clear color buffer with color
void rlClearScreenBuffers(void);                  // Clear used screen buffers (color and depth)
void rlCheckErrors(void);                         // Check and log OpenGL error codes
void rlSetBlendMode(int mode);                    // Set blending mode
void rlSetBlendFactors(int glSrcFactor, int glDstFactor, int glEquation); // Set blending mode factor and equation (using OpenGL factors)

//------------------------------------------------------------------------------------
// Functions Declaration - rlgl functionality
//------------------------------------------------------------------------------------
// rlgl initialization functions
void rlglInit(int width, int height);           // Initialize rlgl (buffers, shaders, textures, states)
void rlglClose(void);                           // De-inititialize rlgl (buffers, shaders, textures)
void rlLoadExtensions(void *loader);            // Load OpenGL extensions (loader function required)
int rlGetVersion(void);                         // Get current OpenGL version
int rlGetFramebufferWidth(void);                // Get default framebuffer width
int rlGetFramebufferHeight(void);               // Get default framebuffer height

Shader rlGetShaderDefault(void);                // Get default shader
Texture2D rlGetTextureDefault(void);            // Get default texture

// Render batch management
// NOTE: rlgl provides a default render batch to behave like OpenGL 1.1 immediate mode
// but this render batch API is exposed in case of custom batches are required
RenderBatch rlLoadRenderBatch(int numBuffers, int bufferElements);  // Load a render batch system
void rlUnloadRenderBatch(RenderBatch batch);                        // Unload render batch system
void rlDrawRenderBatch(RenderBatch *batch);                         // Draw render batch data (Update->Draw->Reset)
void rlSetRenderBatchActive(RenderBatch *batch);                    // Set the active render batch for rlgl (NULL for default internal)
void rlDrawRenderBatchActive(void);                                 // Update and draw internal render batch
bool rlCheckRenderBatchLimit(int vCount);                           // Check internal buffer overflow for a given number of vertex
void rlSetTexture(unsigned int id);           // Set current texture for render batch and check buffers limits

//------------------------------------------------------------------------------------------------------------------------

// Vertex buffers management
unsigned int rlLoadVertexArray(void);                               // Load vertex array (vao) if supported
unsigned int rlLoadVertexBuffer(void *buffer, int size, bool dynamic);            // Load a vertex buffer attribute
unsigned int rlLoadVertexBufferElement(void *buffer, int size, bool dynamic);     // Load a new attributes element buffer
void rlUpdateVertexBuffer(int bufferId, void *data, int dataSize, int offset);    // Update GPU buffer with new data
void rlUnloadVertexArray(unsigned int vaoId);
void rlUnloadVertexBuffer(unsigned int vboId);
void rlSetVertexAttribute(unsigned int index, int compSize, int type, bool normalized, int stride, void *pointer);
void rlSetVertexAttributeDivisor(unsigned int index, int divisor);
void rlSetVertexAttributeDefault(int locIndex, const void *value, int attribType, int count); // Set vertex attribute default value
void rlDrawVertexArray(int offset, int count);
void rlDrawVertexArrayElements(int offset, int count, void *buffer);
void rlDrawVertexArrayInstanced(int offset, int count, int instances);
void rlDrawVertexArrayElementsInstanced(int offset, int count, void *buffer, int instances);

// Textures management
unsigned int rlLoadTexture(void *data, int width, int height, int format, int mipmapCount); // Load texture in GPU
unsigned int rlLoadTextureDepth(int width, int height, bool useRenderBuffer);               // Load depth texture/renderbuffer (to be attached to fbo)
unsigned int rlLoadTextureCubemap(void *data, int size, int format);                        // Load texture cubemap
void rlUpdateTexture(unsigned int id, int offsetX, int offsetY, int width, int height, int format, const void *data);  // Update GPU texture with new data
void rlGetGlTextureFormats(int format, unsigned int *glInternalFormat, unsigned int *glFormat, unsigned int *glType);  // Get OpenGL internal formats
const char *rlGetPixelFormatName(unsigned int format);              // Get name string for pixel format
void rlUnloadTexture(unsigned int id);                              // Unload texture from GPU memory
void rlGenerateMipmaps(Texture2D *texture);                         // Generate mipmap data for selected texture
void *rlReadTexturePixels(Texture2D texture);                       // Read texture pixel data
unsigned char *rlReadScreenPixels(int width, int height);           // Read screen pixel data (color buffer)

// Framebuffer management (fbo)
unsigned int rlLoadFramebuffer(int width, int height);              // Load an empty framebuffer
void rlFramebufferAttach(unsigned int fboId, unsigned int texId, int attachType, int texType, int mipLevel);  // Attach texture/renderbuffer to a framebuffer
bool rlFramebufferComplete(unsigned int id);                        // Verify framebuffer is complete
void rlUnloadFramebuffer(unsigned int id);                          // Delete framebuffer from GPU

// Shaders management
unsigned int rlLoadShaderCode(const char *vsCode, const char *fsCode);    // Load shader from code strings
unsigned int rlCompileShader(const char *shaderCode, int type);           // Compile custom shader and return shader id (type: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
unsigned int rlLoadShaderProgram(unsigned int vShaderId, unsigned int fShaderId); // Load custom shader program
void rlUnloadShaderProgram(unsigned int id);                              // Unload shader program
int rlGetLocationUniform(unsigned int shaderId, const char *uniformName); // Get shader location uniform
int rlGetLocationAttrib(unsigned int shaderId, const char *attribName);   // Get shader location attribute
void rlSetUniform(int locIndex, const void *value, int uniformType, int count); // Set shader value uniform
void rlSetUniformMatrix(int locIndex, Matrix mat);                        // Set shader value matrix
void rlSetUniformSampler(int locIndex, unsigned int textureId);           // Set shader value sampler
void rlSetShader(Shader shader);                                    // Set shader currently active

// Matrix state management
Matrix rlGetMatrixModelview(void);                                  // Get internal modelview matrix
Matrix rlGetMatrixProjection(void);                                 // Get internal projection matrix
Matrix rlGetMatrixTransform(void);                                  // Get internal accumulated transform matrix
Matrix rlGetMatrixProjectionStereo(int eye);                        // Get internal projection matrix for stereo render (selected eye)
Matrix rlGetMatrixViewOffsetStereo(int eye);                        // Get internal view offset matrix for stereo render (selected eye)
void rlSetMatrixProjection(Matrix proj);                            // Set a custom projection matrix (replaces internal projection matrix)
void rlSetMatrixModelview(Matrix view);                             // Set a custom modelview matrix (replaces internal modelview matrix)
void rlSetMatrixProjectionStereo(Matrix right, Matrix left);        // Set eyes projection matrices for stereo rendering
void rlSetMatrixViewOffsetStereo(Matrix right, Matrix left);        // Set eyes view offsets matrices for stereo rendering

// Quick and dirty cube/quad buffers load->draw->unload
void rlLoadDrawCube(void);     // Load and draw a cube
void rlLoadDrawQuad(void);     // Load and draw a quad
#if defined(__cplusplus)
}
#endif