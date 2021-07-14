Текущий внесенный коммит рейлиба
	Commits on Jul 12, 2021
	* Use 60 FPS for textures_image_loading example (#1867)


Мои изменения:
=> CloseWindow переменовано в CloseWindowRaylib - иначе ломает компиляцию
=> в raylib.h добавлены дефайны PLATFORM_DESKTOP GRAPHICS_API_OPENGL_33
=> в core.c удален инклюд заголовка Win32 от glfw - ломает компиляцию
=> удален весь код OpenGL 1 и 2 - не нужно
=> в стандартном фрагментном шейдере рейлиба, добавил дискард по альфе "    if (texelColor.a <.02) discard; // My fix             \n"
	сделанно чтобы нормально альфу откидывало, если нужно. Лучше конечно было подключить свой шейдер, но пока лень:)