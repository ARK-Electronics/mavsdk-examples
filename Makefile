all:
	@cmake -Bbuild -H.
	cmake --build build -j$(nproc)

install:
	@cmake -Bbuild -H.
	cmake --build build -j$(nproc)
	@sudo cmake --install build

clean:
	@rm -rf build logs
	@echo "All build artifacts removed"

.PHONY: all clean
