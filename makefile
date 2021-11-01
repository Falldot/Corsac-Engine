.DEFAULT_GOAL := play

RUST-LIB-PATH = ./targer/debug


##########################################
## Testing

go-test:
	go test ./test -test.v

c-test-ecs:
	cd build/src/ecs/test && ctest -C Debug --extra-verbose

c-test:
	make c-test-ecs

ecs-test:
	make c-test-ecs
##########################################

.PHONY: play
play:
	go run ./cmd/ce2d/main.go

engine-build:
	cd build && cmake .. && cmake --build .

engine-start:
	cd build && cmake --build . && cd src/engine/ && corsac_engine.exe
	make ecs-test

.PHONY: build
build:
	go build -o ./build/ -ldflags '-s -w' ./cmd/ce2d/main.go

.PHONY: benchmark
benchmark:
	go test ./benchmark -bench=. -benchmem
