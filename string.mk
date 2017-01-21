str.a: string.o str_utf8.o
	@echo AR $@
	@$(AR) $(ARFLAGS) $@ $< > /dev/null
	@printf "\e[A\e[2K" # ar has no --silent option
