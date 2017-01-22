str.a: string.o str_utf8.o
	@echo AR $(ARFLAGS) $@ $<
	@$(AR) $(ARFLAGS) $@ $< > /dev/null
	@printf "\e[A\e[2K"
