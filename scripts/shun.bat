@echo off

echo = sholan =
sholan %1.sl output/%1.shasm
echo = shasm =
shasm -i output/%1.shasm -o output/%1.sx %2
echo = execution =
shogun output/%1.sx %3