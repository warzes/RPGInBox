Текущий внесенный коммит рейлиба
	
Commits on Sep 24, 2021
    [models] Animation System Fix Part 1 


Мои изменения:
=> CloseWindow переменовано в CloseWindowRaylib - иначе ломает компиляцию
=> в raylib.h добавлены дефайны PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33
=> в core.c удален инклюд заголовка Win32 от glfw (#include "GLFW/glfw3native.h" )- ломает компиляцию
=> в стандартном фрагментном шейдере рейлиба (rlgl.h искать #version 330), добавил дискард по альфе "    if (texelColor.a <.02) discard; // My fix             \n"
	сделанно чтобы нормально альфу откидывало, если нужно. Лучше конечно было подключить свой шейдер, но пока лень:)