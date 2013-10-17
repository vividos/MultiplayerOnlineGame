.PHONY: clean All

All:
	@echo ----------Building project:[ XInt - Release, Header Only ]----------
	@"$(MAKE)" -f "XInt.mk"
clean:
	@echo ----------Cleaning project:[ XInt - Release, Header Only ]----------
	@"$(MAKE)" -f "XInt.mk" clean
