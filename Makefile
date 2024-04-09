all:
	@cmake -Bbuild -H.; cmake --build build -j $(nproc)
	@size build/ftp_client

clean:
	@rm -rf build logs
	@echo "All build artifacts removed"

.PHONY: all clean
