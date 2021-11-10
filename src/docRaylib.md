Разное по рейлибу
https://githubmemory.com/repo/Rabios/awesome-raylib


Текущий внесенный коммит рейлиба
	
Commits on Nov 10, 2021
Update parser files 


Мои изменения:
=> CloseWindow переменовано в CloseRaylib - иначе ломает компиляцию
=> в стандартном фрагментном шейдере рейлиба (rlgl.h искать #version 330), добавил дискард по альфе "    if (texelColor.a <.02) discard; // My fix            \n"
	сделанно чтобы нормально альфу откидывало, если нужно. Лучше конечно было подключить свой шейдер, но пока лень:)