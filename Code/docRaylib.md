
Commits on Jul 6, 2021
    Update raylib.vcxproj



Внесены изменения:

=> CloseWindow переменовано в CloseWindowRaylib

=> в raylib.h добавлены дефайны PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33

=> в core.c удалена зависимость Win32 glfw

=> удален весь код OpenGL 1 и 2