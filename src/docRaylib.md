Разное по рейлибу
https://githubmemory.com/repo/Rabios/awesome-raylib


Текущий внесенный коммит рейлиба
	
Commits on Nov 6, 2021
    Updated the raylib.v binding to the latest version (#2103) 


Мои изменения:
=> CloseWindow переменовано в CloseRaylib - иначе ломает компиляцию
=> в стандартном фрагментном шейдере рейлиба (rlgl.h искать #version 330), добавил дискард по альфе "    if (texelColor.a <.02) discard; // My fix            \n"
	сделанно чтобы нормально альфу откидывало, если нужно. Лучше конечно было подключить свой шейдер, но пока лень:)