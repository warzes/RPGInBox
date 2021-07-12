Commits on Jul 12, 2021

* Use 60 FPS for textures_image_loading example (#1867)




Внесены изменения:

=> CloseWindow переменовано в CloseWindowRaylib

=> в raylib.h добавлены дефайны PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33

=> в core.c удалена зависимость Win32 glfw

=> удален весь код OpenGL 1 и 2