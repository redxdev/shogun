@echo off

echo --- BUILDING standard.sxl ---
sholan lib\standard.build.sl lib\standard.build.shasm
shasm -i lib\standard.build.shasm -o lib\standard.sxl
echo --- FINISHED standard.sxl ---

echo --- BUILDING test.sxl ---
sholan lib\test.build.sl lib\test.build.shasm
shasm -i lib\test.build.shasm -o lib\test.sxl
echo --- FINISHED test.sxl ---