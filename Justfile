all: build serve

set dotenv-load := true

makemigration name:
	@atlas migrate diff {{name}} --env local

migrate:
	@atlas migrate apply --env local

makeBuild:
	@cmake -S . -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ 

build:
	@cmake --build build

build-seeder:
	@cmake --build build --target seeder

serve:
	@./bin/server.exe

test:
	@./bin/test.exe

erd:
	@atlas schema inspect --url "sqlite://app.db" -w 

replicate:
	@litestream replicate -config litestream.yml

restore:
	@litestream restore -config litestream.yml "$DB_FILENAME"

cli *args:
    -./bin/cli.exe {{args}}

clean:
	@rm -rf build
	@rm -rf bin

cleanmigration:
	@rm -rf src/database/migrations/*