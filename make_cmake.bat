rmdir /s /q buildw64
mkdir buildw64
cd buildw64
cmake .. -DCMAKE_GENERATOR_PLATFORM=x64 -DVCPKG_TARGET_TRIPLET=x64-wxwidget-static-stl -DCMAKE_TOOLCHAIN_FILE=c:/home/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
