all:
	@cmake -Bbuild -H.
	cmake --build build -j$(nproc)

install:
	@cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=$(HOME)/.local
	cmake --build build -j$(nproc)
	@cmake --install build

clean:
	@rm -rf build logs
	@echo "All build artifacts removed"

.PHONY: all clean
