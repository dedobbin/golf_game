# GOLF_GAME

**Warning: Jank code**   
Project is bit of a mess. It was used to experiment on running a platforming game in browser using Emscripten, but got a bit of of hand. Had alot of fun working and experimenting with it though.

![screenshot](https://nop.koindozer.org/gh/golf_game/golf_screenshot.png)


**Known problems:**   

- Shooting ball on an elevator that moves up will kill you.
- With very high horizontal speed, spikes sometimes don't kill you?
- Physics are slippery.
- Everything is too damn hard.
- Optimization: Collision for example is solid but not efficient.
- Level editor has some features missing (zoom, level size etc).
- Engine is 'too dynamic', it's better to define it's limits instead of trying to account for each situation.
- Makefile has t be cleaned up
- In-level texts are not contained within level JSON files, hard-coded for demo level.
- Level JSONs end has an entry for each solid block, should probably reuse information to make them grow slower.
## Build
Running `make web` in root will build files and move them to `build/web`. These can be served using a standard webserver.
Makes use of [Emscripten SDK](https://emscripten.org/docs/).

Running `make native` place a binary + resources in `build/native`. Depends on `SDL2`, `SDL2_image` and `SDL2_ttf`
