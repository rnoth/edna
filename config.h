/* config.h - configuration for edna(1) */

#define PROMPT ":"

const Command commands[] = {
	{ "",	nop },
	{ "a",	append },
	{ "c",	change },
	{ "d",	delete },
	{ "h",	help },
	{ "i",	insert },
	{ "p",	print },
	{ "q",	quit },
	{ "-",	back },
	{ "+",	forward },
};
