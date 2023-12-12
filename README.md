# SDL tests

I've used this demo to observe and document how SDL functions on different platforms
with respect to three features: Relative Mouse Movement, Window grab and Full screen.

I'm trying to find out if there's any _reliable_ way  of understanding which features are supported, without running into silent failures or irreversible state changes.

The platforms are Windows (11, as of Dec 2023), Linux (under WSL2) and Emscripten (under both Firefox & Chrome).

Hopefully `make` will build these demos, assuming you have 

## Relative Mouse Movement

`SDL_SetRelativeMouseMode` seems to have an effect on all platforms, but always returns SDL_FALSE.

* Windows ✔️: Pointer disappears, cannot be moved outside of window.
* Emscripten (Firefox and Chrome) ✔️: Pointer disappears, cannot be moved outside of window.
* Linux (under WSL2) ❌: When switched on, mouse movement seems clamped to the bottom-right and no real movement is detected. Can be switched off again.

## Window grab

`SDL_SetWindowGrab` does not report failure, but reflects its state in `SDL_GetWindowGrab`.

* Windows: ✔️: Pointer is constrained to window. Works independently of relative mouse movement.
* Emscripten (Firefox and Chrome) ❌: Flag is set/unset but nothing happens.
* Linux (under WSL2) ❌: Flag is set/unset but nothing happens.

## Full screen

Using `SDL_SetWindowFullscreen` to toggle between windowed, SDL_WINDOW_FULLSCREEN_DESKTOP (soft full screen) and SDL_WINDOW_FULLSCREEN (hard, with a mode change).

* Windows ✔️: Original windowed size seems to get lost, 
* Emscripten (Firefox) ❌: Soft fullscreen expands to fill window, but does not fill screen. No further changes are possible, despite call succeeding.
* Emscripten (Chrome) ✔️: No difference between soft and hard full screen mode, but otherwise survives.
* Linux (under SDL2) ❌: Soft full screen is convincing, hard full screen doesn't change the screen mode (so small display in bottom-left).

## Emscripten's shell's "Full Screen" button?

Not sure how this interacts with SDL, seems to be more reliable but also do something different.