#if 0
#define generate_ctor(type) 			\
	Object *				\
	type ## _ctor(void)			\
	{					\
		Object *ret;			\
						\
		ret = make_object();		\
		ret->type = type ## _type;	\
		ret->meth = tpye ## _meth;	\
						\
		return ret;			\
	}

#define line_type "l"
#define expr_type "x"
#define number_type "n"
#define string_type "s"

Method line_meth = {
	{ "tostring",	tostring }
	{ "tonumber",	tonumber }
};

generate_ctor(line)
generate_ctor(expr)
generate_ctor(number)
generate_ctor(string)
#endif
