/* config.h - configuration for edna(1) */

const struct Command commands[] = {
	{ "",	nop },
	{ "a",	append },
	{ "d",	delete },
	{ "h",	help },
	{ "p",	print },
	{ "q",	quit },
	{ "-",	back },
	{ "+",	forward },
};
