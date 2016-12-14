# RaXaR

A simple ray-tracer built for a computer graphics assignment. Originally built in VCS on Windows, so some whitespace errors are still persent in port.

## Usage

Toggle AA by uncommenting #define SUPER_SAMPLE and #define JITTER_AA flags in RaXaR.cpp

Modify scene by pushing lights and scene objects into respective lists inside main() method of RaXaR class

`make`

Execute `raxar` and check output.tga in directory

## Authors

- Lewis Christie
- Max Brosnahan for work on GeomX.cpp / GeomX.h
- Richard Lobb for original implementation of Colour.cpp / Colour.h in Python

Textures were taken from royalty free tutorials / sources when this was originally written - but I have not been able to track them down again.
