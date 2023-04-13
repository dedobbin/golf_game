# GOLF_GAME

This project start as an experiment to run a platforming/golf physics game in browser using Emscripten.   
Since i was running a (self-imposed) deadline the game needs finetuning to actually be fun, but fun is secondary anyway right.    

Also the graphics are classic "programmer art" :^)

![screenshot](https://nop.koindozer.org/gh/golf_game/golf_screenshot.png)


**Known problems:**   

- Shooting ball on an elevator that moves up will kill you.
- Optimization: Collision for example is solid but not efficient.
- Level editor has some features missing (zoom, level size etc).
- Engine is 'too dynamic', it's better to define it's limits instead of trying to account for each situation.
- Level JSONs end has an entry for each solid block, should probably reuse information to make them grow slower.
- Physics are slippery.
- Everything is too damn hard.
- In-level texts are not contained within level JSON files, hard-coded for demo level.
- Makefile has to be cleaned up.- With very high horizontal speed, spikes sometimes don't kill you?
## Build
Running `make web` in root will build files and move them to `build/web`. These can be served using a standard webserver.
Makes use of [Emscripten SDK](https://emscripten.org/docs/).

Running `make native` place a binary + resources in `build/native`. Depends on `SDL2`, `SDL2_image` and `SDL2_ttf`
