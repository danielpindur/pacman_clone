# pacman_clone
definitely not PACMAN
Daniel Pindur, 2020
Projekt do předmětu Úvod do programování
------------------------------------------
1. Překlad
----------
Překlad je prováděn pomocí CMake. Překlad vyžaduje knihovny SDL2, SDL2_IMAGE, SDL2_TTF, SDL2_MIXER a math.

2. Fungování programu
---------------------
Při spuštění programu proběhne načtení highscore ze souboru (nebo jeho vytvoření), načtení podoby mapy ze souboru map_data.bit a spuštění Main menu.
V Main menu, je možné se pohybova pomocí U a S (resp. Up a Down). Při najetí na Play a následném stisknutí ENTER, dojde ke spuštění hry.
Při najetí na konfigurační možnosti v menu, je možné pomodí A a D (resp. Right a Left) měnit hodnoty dané možnosti:

a) Difficulty - je možné nastavit obtížnost na Easy nebo Hard, na obtížnost Easy dochází k pohybu duchů jen jednou za dva snímky, hráče
se tedy pohybuje 2x rychleji než duchové, na obtížnost Hard se duchové pohybují stejně rychle jako hráč

b) Number of ghosts - umožňuje zvolit množství nepřátel (min. 1, max 4)

c) Music - umožňuje ztlumit background hudbu

Při spuštění hry má hráč 3 životy a skóre 0. Za každou sebranou minci, se mu přičte skóre 1. Na začátku se čeká, než se začne hráč pohybovat
(do té doby se nepohybují ani duchové). Poté co se hráč pohne, dojde k aktivaci duchů, kteří začnou pronásledovat hráče s využitím A* pathfinding 
algoritmu. Hráč má oproti duchům možnost procháte "teleporty" na pravé a levé straně herního pole, které ko přenesou na opačnou stranu. Hra probíhá 
tak dlouho, než hráč ztratí všechny 3 životy, tedy 3x dojde k chycení hráče duchy. Po chycení hráče dojde k ubrání životu a k resetu pozice všeho na mapě.
Pokud hráč sesbírá všechny mince na mapě, dojde k resetu pozice všeho na mapě, bez ztráty životu hráče.

Po smrti hráče proběhne vyhodnocení, zda nové skóre je větší nebo měnší než highscore. Pokud je větší proběhne zápis daného skóre do souboru jako highscore
a k aktualizaci highscore zobrazeného na obrazovce. Pomocí tlačítka ENTER je hráč schopný se vrátit zpět do Main menu, kde může opět změnit nastavení hry a 
danou hru opět spustit.

3. Memory leaky
---------------
Po doběhnutí programu hlásí adress sanitizer memory leak o velikosti 17 bytu, což by měl být memory leak, který vzniká v rámci knihovny SDL.