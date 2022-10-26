project=dotl_clang

up:
	cd deploy && docker-compose up -d --build

dev:
	docker run --rm -v "$(PWD):/build" -it $(project) /build/tests/run.sh

down:
	cd deploy & &docker-compose down