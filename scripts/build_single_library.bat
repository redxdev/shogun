@echo off

echo --- BUILDING %1 ---
echo = sholan =
sholan lib\%1.build.sl lib\%1.build.shasm
echo = shasm =
shasm -i lib\%1.build.shasm -o lib\%1.sxl
echo --- FINISHED %1 ---