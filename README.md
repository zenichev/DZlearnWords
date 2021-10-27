# DZlearnWords

## Introduction

A simple program, which allows you to learn new words / phrases of the language you desire to learn.
It's meant to be used under Debian/Ubuntu based desktop systems.

It's not bound to any language, you can freely learn even other things using that.
The source information for learning (the list of things to learn) is defined by you in advance,
it must be a simple file of .txt extension, with words and translations.

Syntax example: `<word>:<translation>`

Example of the file:
```
$bash> cat /tmp/my_words_list.txt
die Vermittlung:a mediation
die Vorwarnung:a warning
die Einschätzung:an estimation
```

The method for learning implemented in this program,
gives you a possibility to focus on your daily work,
meanwhile the program between-times pops up (with a random time step)
and shows you some word/phrase from the list (which you previously prepared),
and then you can either set it as learned or set this as a forgotten.

In the end of the program run you get the summary of what you have learned so far,
and what has been left to repeat again.

The main idea is, that your brain faster learns, in case the material to repeat/learn appears
randomly, and you have to quickly answer (retrieve the information from your mind).
Hence after several iterations of the same list of words, you can quickly achieve the target.

Remark: please take into account, this program is under development and has a list of bugs.
Feel free to report any problems and/or provide new ideas.

## Modes

The program has three modes of interaction with user. Whereas one of them (GUI mode) is not ready yet.
Modes list:
1. CLI mode / `--cli-mode`
2. CLI menu mode / `--cli-menu` (default one)
3. GUI mode / `--gui-mode` (not available at the moment)

### CLI mode

Is used to launch the program with a pre-defined list of parameters.
Here is a list of available parameters in this mode:
* `--cli-mode` - defines a program mode
* `--open-file` - gives a path to the source file with words to learn
* `--next-time-min` - minimum time border (used to pick random time step within it)
* `--next-time-max` - maximum time border (used to pick random time step within it)

An example of the program run in this mode:

`$bash> ./main --cli-mode --open-file /tmp/file_name.txt --next-time-min 30 --next-time-max 50`

### CLI menu mode

Is an interaction window, which gives you a possibility to pick out needed options as soon
as you launch the program. Is a default mode, so if you want to run in this mode, you can either
run the program as this:

`$bash> ./main`

or like this:

`$bash> ./main --cli-menu`

### GUI mode

Is under development. An intention is to provide a full support of the graphical interface
and to deprecate interaction via CLI eventually.

## Installation

Pre-requirements:
* SDL2 : `libsdl2-dev libsdl2-2.0 libsdl2-ttf-2.0 libsdl2-ttf-dev`
* ncurses : `libncurses-devi libncurses6`

For now there is no MakeFile for the program, nor cmake. You can simply compile it:
`g++ dataKeeper.cpp dataKeeper.h SDLgraphics.cpp SDLgraphics.h intro.cpp intro.h main.h main.cpp -o ./main -lSDL2 -lncurses`

Remarks:
* you need to link SDL2 and ncurses when compiling
* not necessarily g++ compiler must be used, you can use the one you prefer

## To do list:

* add a program "memory" to remember forgotten words for the next run
* implement GUI interface
* add a possibility to flip only forgotten words and set aside already learned
* add a Makefile file or either implement compilation automatization using cmake
