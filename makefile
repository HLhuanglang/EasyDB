build:
	go build -gcflags "-N -l" -o bin/edb cmd/main.go

clean:
	rm -rf bin/edb

clean_all:
	rm -rf bin

.PHONY: build clean clean_all