## OOML case generator

This is a fork of https://bitbucket.org/leemes/ooml-case-factory /
http://www.thingiverse.com/thing:70838.  Write a C header file that describes
your case and the program will generate [OpenSCAD](http://www.openscad.org/)
files that you can print.

Requires [OOML](https://github.com/avalero/OOML) to be installed.

There are two example case files included: `bb-atxra.h` and `cubieboard.h`.
Use them as follows:

```sh
./make-case.sh bb-atxra
# or
./make-case.sh bb-atxra.h
```

```sh
./make-case.sh cubieboard
```
