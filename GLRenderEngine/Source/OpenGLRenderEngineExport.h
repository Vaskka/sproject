#ifdef OPENGL_RENDER_ENGINE_EXPORTS
#define OPENGL_RENDER_ENGINE_DLL_EXPORT __declspec(dllexport)
#else 
#define OPENGL_RENDER_ENGINE_DLL_EXPORT __declspec(dllimport)
#endif