@echo off
mkdir "Build"
cd "Build"
qmake "../Wordle.pro"
mingw32-make
"release/Wordle"