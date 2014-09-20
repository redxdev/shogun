@echo off

sholan %1.sl output/%1.shasm
shasm -i output/%1.shasm -o output/%1.sx
shogun output/%1.sx