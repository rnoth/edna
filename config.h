/* config.h - configuration for edna(1) */

#define PROMPT	":"
#define ERROR	"?\n"

const Command commands[] = {
	{ "",	nop },
	{ "",	gotol },
	{ "a",	append },
	{ "c",	change },
	{ "d",	delete },
	{ "h",	help },
	{ "i",	insert },
	{ "p",	print },
	{ "q",	quit },
};
