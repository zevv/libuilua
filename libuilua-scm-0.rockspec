package = "libuilua"
version = "scm-0"

description = {
	summary = "Lua binding for libui";
	detailed = [[Lua binding for libui: https://github.com/andlabs/libui]];
	homepage = "https://github.com/zevv/libuilua";
}

source = {
	url = "git+https://github.com/zevv/libuilua.git";
}

dependencies = {
	"lua >= 5.1, < 5.4"
}

external_dependencies = {
	UI = {
		header = "ui.h";
		library = "ui";
	};
}

build = {
	type = "builtin",
	modules = {
		libuilua = {
			sources = "libuilua.c";
			libraries = "ui";
			incdirs = {
				"$(UI_INCDIR)";
			};
			libdirs = {
				"$(UI_LIBDIR)";
			};
		};
	};
}
