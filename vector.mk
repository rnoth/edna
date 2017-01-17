tests/vector_test: tests/vector_test.c
	@echo -e "Compiling\e[35m" $< "\e[0m"
	@echo CC $(CFLAGS) -o $@ $<
	@$(CC) $(CFLAGS) -o $@ $<
	@echo -e "Running\e[36m" $@ "\e[0m"
	@$@
	@echo -e "Test successful:\e[36m" $@ "\e[0m"
	@echo
