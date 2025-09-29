rm -rf build/
rm -rf bin/
mkdir bin
mkdir build
cd build
cmake ..
make
cd ..
./bin/app
