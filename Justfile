all: build run

makeBuild:
	@cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++

build:
	@cmake --build build

run:
	@./bin/server.exe

test:
	@./bin/test.exe

clean:
	@rm -rf build
	@rm -rf bin