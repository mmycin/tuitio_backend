all: build run

makeBuild:
	@cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++

build:
	@cmake --build build

run:
	@./bin/server.exe

test:
	@./bin/test.exe

makemigration name="$(date +%s)":
	@atlas migrate diff {{name}} --env local

migrate:
    atlas migrate diff --env local

erd:
	@atlas schema inspect --url "sqlite://app.db" -w 

clean:
	@rm -rf build
	@rm -rf bin

cleanmigration:
	@rm -rf src/database/migrations/*