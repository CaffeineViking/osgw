Real-Time Ocean Simulation with Gerstner Waves
==============================================

<p align="center">
    <img width=100% src="/docs/figures/gerstner.gif" alt="Animation of a 4-Wave Gerstner Wave Sum"/>
</p>

A *Gerstner wave* or *trochoidal wave* describes the progression of a wave on the surface of an incompressible fluid of infinite depth. It's suitable for describing the motion of deep-ocean waves. The solution was discovered in 1802 by *F. J. Gerstner*, and has been the workhorse of ocean animation in computer graphics since 1986, after the paper by *Fournier & Reeves* showed very promising results. Then, based on the classic course notes by *Jerry Tessendorf* in 2001, *Mark Finch* implemented the Gerstner wave as a vertex shader in the 2004 *"Effective Water Simulation from Physical Models"*, show that real-time Gerstner wave calculations are indeed possible. They are nowadays extensively in many video games, e.g. in Naughty Dog's *Uncharted*.

In this project, we have implemented *Finch's Gerstner wave shader* in GLSL and provided it under an open-source license. Along with it, an example Gerstner wave visualization program, `osgw`, is bundled. It provides example usage of the shader, and can also be used to play around with the wave parameters. It can also work as a (not very energy-efficient) screensaver. Performance-wise, we're able to simulate and render a 12-wave Gerstner wave sum at 200 FPS (on ~9 year old hardware).

In addition, the `osgw` renderer uses OpenGL's *tessellation shader* to seamlessly switch between geometry LoD, based on the amount of detail the user can see (this reduces the amount of times we need to evaluate the Gerstner wave function). I hope it will be a useful reference for future users, since I've found that there isn't that much documentation on how to setup a *tessellation control and evaluation shader* for OpenGL 4.1 (besides the comprehensive [OpenGL Tessellation Wiki](https://www.khronos.org/opengl/wiki/Tessellation) and the awesome [Prideout Tutorial](http://prideout.net/blog/?p=48)). For the small surface ripples, a two-octave *[Simplex noise](staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)* is used, that is then mixed with a combination of deep and shallow ocean colors.

The Gerstner wave in the animation above was simulated and rendered with the following parameters:

| Wave Number  | Angle | Amplitude | Steepness | Frequency | Speed |
| ------------ | ----- | --------- | --------- | --------- | ----- |
| 1            | 0.32  |  0.64     | 0.64      | 0.64      | 1.28  |
| 2            | 1.28  |  0.08     | 1.28      | 0.64      | 2.56  |
| 3            | 5.12  |  0.16     | 1.28      | 0.32      | 0.64  |
| 4            | 2.56  |  0.32     | 2.56      | 0.16      | 0.32  |

Note: these values were obtained by guesswork, you can probably create something more interesting!

The Gerstner Wave Shader
------------------------

If you are planning on using Gerstner waves in your projects, it can be a bit fiddly to roll your own shader implementation; a lot can go wrong, and it's easy to write non-performant GLSL shaders. Therefore, feel free to use anything you find useful in this repository! Of special interest is probably [gerstner.glsl](https://github.com/CaffeineViking/osgw/blob/master/share/shader/gerstner.glsl), the function `gerstner_wave` is fairly decoupled from most of the project, so you should be able to use it for anything. There are instructions on how to use it in the shader's header, after the license notice (its there so you can just copy-paste the shader without having to think about it). Briefly, here is how to use it:

Setup a tessellated 2-D plane-like mesh, either by uploading the vertices manually, or by generating them with a geometry or tessellation shader (you can see examples on how to do this in [gerstner.tesc](https://github.com/CaffeineViking/osgw/blob/master/share/shader/gerstner.tesc) and [gerstner.tese](https://github.com/CaffeineViking/osgw/blob/master/share/shader/gerstner.tese)). Thereafter, you want to displace each vertex according to the wave. You do this by copy-pasting [gerstner.glsl](https://github.com/CaffeineViking/osgw/blob/master/share/shader/gerstner.glsl) and calling `gerstner_wave` like so:

```glsl
position = gerstner_wave(position.xz, elapsed_time, normal);
```

where `position` is the world-coordinate vertex position, `normal` is the normal of that vertex (presumably pointing up), and `elapsed_time` is the time since the simulation started (usually in seconds). The function will return where the new position of the vertex should be after the Gerstner wave displacement, and the `normal` you provided will have been overwritten with the correct normal after the Gerstner wave displacement (i.e you don't need to set it yourself). If you want to change the wave parameters, you can either feed the `gerstner_waves`-array directly, or upload the data to the array dynamically by uniforms:

```C++
glUniform2f(glUniformLocation(shader_program, "gerstner_waves[0].direction"), 1.0f, 0.0f);
glUniform1f(glUniformLocation(shader_program, "gerstner_waves[0].amplitude"), 1.0);
glUniform1f(glUniformLocation(shader_program, "gerstner_waves[0].steepness"), 0.5);
glUniform1f(glUniformLocation(shader_program, "gerstner_waves[0].frequency"), 1.0);
glUniform1f(glUniformLocation(shader_program, "gerstner_waves[0].speed"), 1.0);
glUniform1ui(glUniformLocation(shader_program, "gerstner_waves_length"), 1);
```

Note that you need to provide how many waves the shader should consider in `gerstner_waves_length`, otherwise it won't evaluate all of the waves you have provided. Performance-wise, the `gerstner_wave` function can have around 8-12 waves as parameters before it starts to become too costly. But your mileage may vary. I don't claim to distribute the fastest Gerstner wave GLSL implementation, but seeing as there aren't that many around (publicly speaking), it seems to get the work done.

Compiling and Installation
--------------------------

1. Place yourself in the root directory of this project.
2. Acquire the latest version of the `premake5` build system.
3. Thereafter, execute `premake5 gmake` if building on Make.
4. Finally, issue the command `make -j8 -C build` and wait.
5. When complete, either `bin` or `lib` have been built.
6. **Shortcuts:** `make run` to build and run the demo.

Usage and Documents
-------------------

Start the demo with `bin/osgw`, there are zero command-line arguments here, pinky promise. Then use:

* **Hold down the left mouse button and move the mouse:** to rotate the camera around the scene.
* **Hold down the middle mouse button and move the mouse:** pan the camera accross the scene.
* **Hold down the right mouse button and move the mouse:** zoom the camera in/out of the scene.
* **Pressing the F key:** toggles between fullscreen and windowed mode (only tested on X11 targets).
* **Pressing the P key:** pauses the simulation time so you can change wave parameters more easily.
* **Pressing the W key:** will toggle wireframe rendering, which is good for inspecting the tessellation.
* **Pressing the Q key:** will exit the application. I thought about using Escape, but that's for normies.

Then, if you want to change the parameters of the Gerstner waves at run-time, you can use the following:

* **Press 1, 2, 3, 4, 5, 6, 7 or 8 on the keyboard:** to select the current Gerstner wave we want to modify.
* **Press the same number again:** to enable or disable the Gerstner wave from the final Gerstner sum.
* **The up and down arrow keys:** are used to select the parameter of the wave that is being changed.
* The parameter selection cycles between: *angle*, *amplitude*, *steepness*, *frequency* and *phase speed*.
* **Finally, the left and right keys:** decrease or increases the current wave's selected wave parameter.

You can get a glimpse of what you are currently changing by looking at the title bar of the `osgw` window. By default, you'll only get a single default-initialized Gerstner wave enabled. As you enable more waves, please note that the amplitude of them will be zero at first, so you need to manually increase it to see any difference in the final wave. Disabling all but one wave layer is a good strategy when composing complex wave motions, since you can concentrate on changing only one wave at a time, and once you are happy, you can combine it with the rest of the waves by enabling the other waves again.

A short paper has been written on the subject, if you want to compile it yourself, see the `docs` directory.

Or search for [*Real-Time Ocean Simulation and Rendering Using Gerstner Waves*](https://eriksvjansson.net/share/papers/osgw.pdf) by Erik S. V. Jansson.

System Requirements
-------------------

Should run on almost anything, even on a toaster-like potato-ish hardware.

Your toaster must support and have drivers for `OpenGL 4.1` or later though.

Dependencies
------------

There aren't any dependencies besides: `glfw3` and `png` for now.

Structure
---------

* `bin`: contains the built software and any utilities needed as well.
* `build`: stores intermediate object files and generated GNU Make files.
    * `obj`: has all of the generated object files given under compilation.
    * `Makefile`: automatically generated by executing `premake5 gmake`.
    * `*.make`: program specific make config for augmenting `Makefile`.
* `docs`: any generated documentation for this project is over here.
* `foreign`: external headers and source for libraries and Catch.
    * `include`: the external header files should go here.
    * `src`: the external source code should go over here.
* `include`: only internal headers from this project should go here.
    * `project directories`: internal headers for the project.
* `license.md`: please look through this very carefully.
* `premake5.lua`: configuration file for the build system.
* `readme.md`: this file, contains information on the project.
* `share`: any extra data that needs to be bundled should be here.
* `src`: all source code for the project should be located below here.
    * `project directories`: source code for specific project build.
    * `main.cc`:
* `utils`: any sort of helper scripts or similar should be over here.

Contributing
------------

1. See if there any updates upstream: `git remote update`
2. If you are already working on a feature branch, jump to step 4.
3. Create a new branch for your feature: `git branch <feature>`
4. Change to the feature branch by: `git checkout <feature>`
5. Start/continue building the feature. Jump to 7 if done.
6. Push your changes to your remote feature branch with:
   * `git add -A` (everything) or `git add -u` (updates).
   * `git commit -m "Message describing the additions."`
   * Publish remotely: `git push origin <feature>`
   * Go back to step 1 or 5 (if working quickly).
7. Your feature is done (single/many commits).
8. Fetch the changes from upstream first:
   * Go to master: `git checkout -b master`
   * Pull changes: `git pull origin master`
8. Rebase your changes on top of `master`:
   * Back to feature: `git checkout <feature>`
   * Thereafter, issue: `git rebase master`
   * Fix any conflicts that may happen...
   * Changes now on top of the Git tree.
9. Finally, go to GitHub and issue PR.
   * Listen in on other's feedback!
   * Make changes if necessary.
10. Back to the master: `git checkout master`
11. Pat yourself on the back, then goto 1 again.

Reporting Bugs
--------------

There are definitely no known bugs in this software at this time.
