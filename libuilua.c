
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "ui.h"


/*
 * Boilerplate macros
 */

#define UI_CAST(n, type) ui ## type(((struct wrap *)lua_touserdata(L, n))->control)

#define RETURN_SELF lua_pushvalue(L, 1); return 1;

#define CREATE_META(n) \
	luaL_newmetatable(L, "libui." #n);  \
	luaL_setfuncs(L, meta_ ## n, 0);

#define CREATE_OBJECT(t, c) \
	struct wrap *w = lua_newuserdata(L, sizeof(struct wrap)); \
	w->L = L; \
	w->control = uiControl(c); \
	w->type = TYPE_ ## t; \
	lua_newtable(L); \
	luaL_getmetatable(L, "libui." #t); \
	lua_setfield(L, -2, "__index"); \
	lua_setmetatable(L, -2);



enum control_type {
	TYPE_Box,
	TYPE_Button,
	TYPE_Control,
	TYPE_Group,
	TYPE_Label,
	TYPE_ProgressBar,
	TYPE_Separator,
	TYPE_Slider,
	TYPE_Spinbox,
	TYPE_Window,
};

struct wrap {
	lua_State *L;
	uiControl *control;
	enum control_type type;
};


static struct wrap *create_callback_data(lua_State *L, int n)
{
	struct wrap *w = lua_touserdata(L, n);
	lua_pushlightuserdata(L, w);
	lua_newtable(L);
	lua_pushvalue(L, 2);
	lua_setfield(L, -2, "fn");
	lua_pushvalue(L, 3);
	lua_setfield(L, -2, "data");
	lua_settable(L, LUA_REGISTRYINDEX);

	return w;
}

static void callback(struct wrap *w)
{
	lua_State *L = w->L;

	lua_pushlightuserdata(L, w);
	lua_gettable(L, LUA_REGISTRYINDEX);

	luaL_checktype(L, 1, LUA_TTABLE);

	lua_getfield(L, 1, "fn");
	luaL_checktype(L, -1, LUA_TFUNCTION);
	lua_getfield(L, 1, "data");
	lua_call(L, 1, 0);
}


/*
 * Control
 */

int l_ControlShow(lua_State *L)
{
	uiControlShow(UI_CAST(1, Control));
	RETURN_SELF;
}


int l_ControlDestroy(lua_State *L)
{
	printf("destroy not implemented, garbage collection needs to be implemented\n");
	return 0;
}


/*
 * Button
 */

int l_NewButton(lua_State *L)
{
	CREATE_OBJECT(Button, uiNewButton(
		luaL_checkstring(L, 1)
	));
	return 1;
}

static void on_button_clicked(uiButton *b, void *data)
{
	struct wrap *w = data;
	callback(w);
}

int l_ButtonSetText(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	const char *text = luaL_checkstring(L, 2);
	uiButtonSetText(uiButton(w->control), text);
	RETURN_SELF;
}

int l_ButtonOnClicked(lua_State *L)
{
        struct wrap *w = create_callback_data(L, 1);
        uiButtonOnClicked(uiButton(w->control), on_button_clicked, w);
        RETURN_SELF;
}

static struct luaL_Reg meta_Button[] = {
	{ "SetText",              l_ButtonSetText },
	{ "OnClicked",            l_ButtonOnClicked },
	{ NULL }
};



/*
 * Group
 */

int l_NewGroup(lua_State *L)
{
	CREATE_OBJECT(Group, uiNewGroup(
		luaL_checkstring(L, 1)
	));
	return 1;
}

int l_GroupTitle(lua_State *L)
{
	lua_pushstring(L, uiGroupTitle(UI_CAST(1, Group)));
	return 1;
}

int l_GroupSetTitle(lua_State *L)
{
	const char *title = luaL_checkstring(L, 2);
	uiGroupSetTitle(UI_CAST(1, Group), title);
	RETURN_SELF;
}

int l_GroupSetChild(lua_State *L)
{
	uiGroupSetChild(UI_CAST(1, Group), UI_CAST(2, Control));
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_pushboolean(L, 1);
	lua_settable(L, -3);
	RETURN_SELF;
}

int l_GroupMargined(lua_State *L)
{
	lua_pushnumber(L, uiGroupMargined(UI_CAST(1, Group)));
	return 1;
}

int l_GroupSetMargined(lua_State *L)
{
	uiGroupSetMargined(UI_CAST(1, Group), luaL_checknumber(L, 2));
	RETURN_SELF;
}

static struct luaL_Reg meta_Group[] = {
	{ "Title",                l_GroupTitle },
	{ "SetTitle",             l_GroupSetTitle },
	{ "SetChild",             l_GroupSetChild },
	{ "Margined",             l_GroupMargined },
	{ "SetMargined",          l_GroupSetMargined },
	{ NULL }
};


/*
 * Label
 */

int l_NewLabel(lua_State *L)
{
	CREATE_OBJECT(Label, uiNewLabel(
		luaL_checkstring(L, 1)
	));
	return 1;
}

int l_LabelText(lua_State *L)
{
	lua_pushstring(L, uiLabelText(UI_CAST(1, Label)));
	return 1;
}

int l_LabelSetText(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	const char *text = luaL_checkstring(L, 2);
	uiLabelSetText(uiLabel(w->control), text);
	RETURN_SELF;
}


static struct luaL_Reg meta_Label[] = {
	{ "Text",                 l_LabelText },
	{ "SetText",              l_LabelSetText },
	{ NULL }
};




/*
 * ProgressBar
 */

int l_NewProgressBar(lua_State *L)
{
	CREATE_OBJECT(ProgressBar, uiNewProgressBar());
	return 1;
}

int l_ProgressBarSetValue(lua_State *L)
{
	double value = luaL_checknumber(L, 2);
	uiProgressBarSetValue(UI_CAST(1, ProgressBar), value);
	RETURN_SELF;
}

static struct luaL_Reg meta_ProgressBar[] = {
	{ "SetValue",             l_ProgressBarSetValue },
	{ NULL }
};



/*
 * Separator
 */

int l_NewHorizontalSeparator(lua_State *L)
{
	CREATE_OBJECT(Separator, uiNewHorizontalSeparator());
	return 1;
}

static struct luaL_Reg meta_Separator[] = {
	{ NULL }
};


/*
 * Slider
 */

int l_NewSlider(lua_State *L)
{
	CREATE_OBJECT(Slider, uiNewSlider(
		luaL_checknumber(L, 1),
		luaL_checknumber(L, 2)
	));
	return 1;
}

int l_SliderValue(lua_State *L)
{
	lua_pushnumber(L, uiSliderValue(UI_CAST(1, Slider)));
	return 1;
}

int l_SliderSetValue(lua_State *L)
{
	double value = luaL_checknumber(L, 2);
	uiSliderSetValue(UI_CAST(1, Slider), value);
	RETURN_SELF;
}

static void on_slider_changed(uiSlider *b, void *data)
{
	struct wrap *w = data;
	callback(w);
}

int l_SliderOnChanged(lua_State *L)
{
	struct wrap *w = create_callback_data(L, 1);
	uiSliderOnChanged(uiSlider(w->control), on_slider_changed, w);
	RETURN_SELF;
}

static struct luaL_Reg meta_Slider[] = {
	{ "Value",                l_SliderValue },
	{ "SetValue",             l_SliderSetValue },
	{ "OnChanged",            l_SliderOnChanged },
	{ NULL }
};


/*
 * Spinbox
 */

int l_NewSpinbox(lua_State *L)
{
	CREATE_OBJECT(Spinbox, uiNewSpinbox(
		luaL_checknumber(L, 1),
		luaL_checknumber(L, 2)
	));
	return 1;
}

int l_SpinboxValue(lua_State *L)
{
	lua_pushnumber(L, uiSpinboxValue(UI_CAST(1, Spinbox)));
	return 1;
}

int l_SpinboxSetValue(lua_State *L)
{
	double value = luaL_checknumber(L, 2);
	uiSpinboxSetValue(UI_CAST(1, Spinbox), value);
	RETURN_SELF;
}

static void on_spinbox_changed(uiSpinbox *b, void *data)
{
	struct wrap *w = data;
	callback(w);
}

int l_SpinboxOnChanged(lua_State *L)
{
	struct wrap *w = create_callback_data(L, 1);
	uiSpinboxOnChanged(uiSpinbox(w->control), on_spinbox_changed, w);
	RETURN_SELF;
}

static struct luaL_Reg meta_Spinbox[] = {
	{ "Value",                l_SpinboxValue },
	{ "SetValue",             l_SpinboxSetValue },
	{ "OnChanged",            l_SpinboxOnChanged },
	{ NULL }
};


/* 
 * Window
 */

int l_NewWindow(lua_State *L)
{
	CREATE_OBJECT(Window, uiNewWindow(
		luaL_checkstring(L, 1),
		luaL_checknumber(L, 2),
		luaL_checknumber(L, 2),
		lua_toboolean(L, 3)
	));
	lua_pushvalue(L, -1);
	lua_pushboolean(L, 1);
	lua_settable(L, LUA_REGISTRYINDEX);
	return 1;
}

int l_WindowSetChild(lua_State *L)
{
	uiWindowSetChild(UI_CAST(1, Window), UI_CAST(2, Control));
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_pushboolean(L, 1);
	lua_settable(L, -3);
	RETURN_SELF;
}

static struct luaL_Reg meta_Window[] = {
	{ "SetChild",            l_WindowSetChild },
	{ "Show",                l_ControlShow },
	{ "Destroy",             l_ControlDestroy },
	{ NULL }
};


/*
 * Box
 */

int l_NewVerticalBox(lua_State *L)
{
	CREATE_OBJECT(Box, uiNewVerticalBox());
	return 1;
}

int l_NewHorizontalBox(lua_State *L)
{
	CREATE_OBJECT(Box, uiNewHorizontalBox());
	return 1;
}

int l_BoxAppend(lua_State *L)
{
	uiBoxAppend(UI_CAST(1, Box), UI_CAST(2, Control), lua_toboolean(L, 3));
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	luaL_ref(L, -2);
	RETURN_SELF;
}

int l_BoxPadded(lua_State *L)
{
	lua_pushnumber(L, uiBoxPadded(UI_CAST(1, Box)));
	return 1;
}

int l_BoxSetPadded(lua_State *L)
{
	uiBoxSetPadded(UI_CAST(1, Box), luaL_checknumber(L, 2));
	RETURN_SELF;
}


static struct luaL_Reg meta_Box[] = {
	{ "Append",               l_BoxAppend },
	{ "Padded",               l_BoxPadded },
	{ "SetPadded",            l_BoxSetPadded },
	{ NULL }
};



/*
 * Various top level 
 */

int l_Init(lua_State *L)
{
	uiInitOptions o;
	
	memset(&o, 0, sizeof (uiInitOptions));

	const char *err = uiInit(&o);

	lua_pushstring(L, err);
	return 1;
}

int l_Uninit(lua_State *L)
{
	uiUninit();
	return 0;
}

int l_Main(lua_State *L)
{
	uiMain();
	return 0;
}

int l_Quit(lua_State *L)
{
	uiQuit();
	return 0;
}

static struct luaL_Reg lui_table[] = {

	{ "Init",                   l_Init },
	{ "Uninit",                 l_Uninit },
	{ "Main",                   l_Main },
	{ "Quit",                   l_Quit },

	{ "NewWindow",              l_NewWindow },
	{ "NewButton",              l_NewButton },
	{ "NewVerticalBox",         l_NewVerticalBox },
	{ "NewHorizontalBox",       l_NewHorizontalBox },
	{ "NewHorizontalSeparator", l_NewHorizontalSeparator },
	{ "NewGroup",               l_NewGroup },
	{ "NewLabel",               l_NewLabel },
	{ "NewSlider",              l_NewSlider },
	{ "NewSpinbox",             l_NewSpinbox },
	{ "NewProgressBar",         l_NewProgressBar },


	{ NULL }
};


int luaopen_libuilua(lua_State *L)
{


	CREATE_META(Box)
	CREATE_META(Button)
	CREATE_META(Group)
	CREATE_META(Label)
	CREATE_META(ProgressBar)
	CREATE_META(Separator)
	CREATE_META(Slider)
	CREATE_META(Spinbox)
	CREATE_META(Window)
	luaL_newlib(L, lui_table);
	return 1;
}

/*
 * End
 */
