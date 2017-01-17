str.o: string.c
	@echo CC $(CLFAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o $@ $<

str.a: string.o
	@echo AR $(ARFLAGS) $@ $<
	@$(AR) $(ARFLAGS) $@ $< > /dev/null
	@printf "\e[A\e[2K"
