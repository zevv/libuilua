
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
	lua_pushcfunction(L, l_gc); \
	lua_setfield(L, -2, "__gc"); \
	lua_setmetatable(L, -2);



enum control_type {
	TYPE_Box,
	TYPE_Button,
	TYPE_Checkbox,
	TYPE_Combobox,
	TYPE_Control,
	TYPE_Group,
	TYPE_Label,
	TYPE_ProgressBar,
	TYPE_RadioButtons,
	TYPE_Separator,
	TYPE_Slider,
	TYPE_Spinbox,
	TYPE_Tab,
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


int l_gc(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	uint32_t s = w->control->TypeSignature;
	printf("gc %p %c%c%c%c\n", w->control, s >> 24, s >> 16, s >> 8, s >> 0);
	return 0;

	uiControl *control = UI_CAST(1, Control);

	uiControl *parent = uiControlParent(control);
	
	if(parent) {
		if(parent->TypeSignature == 0x57696E64) {
			uiWindowSetChild(uiWindow(parent), NULL);
		}
		if(parent->TypeSignature == 0x426F784C) {
			uiWindowSetChild(uiWindow(parent), NULL);
		}
		printf("  Unparent %p %p\n", control, parent);
		uiControlSetParent(control, NULL);
	}
	uiControlDestroy(control);

	return 0;
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
	int n = lua_gettop(L);
	int stretchy = 0;

	if(lua_isnumber(L, n) || lua_isboolean(L, n)) {
		stretchy = lua_toboolean(L, n);
	}

	int i;

	for(i=2; i<=n; i++) {
		if(lua_isuserdata(L, i)) {
			uiBoxAppend(UI_CAST(1, Box), UI_CAST(i, Control), stretchy);
			lua_getmetatable(L, 1);
			lua_pushvalue(L, i);
			luaL_ref(L, -2);
		}
	}
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
 * Checkbox
 */

int l_NewCheckbox(lua_State *L)
{
	CREATE_OBJECT(Checkbox, uiNewCheckbox(
		luaL_checkstring(L, 1)
	));
	return 1;
}

static void on_checkbox_toggled(uiCheckbox *c, void *data)
{
	struct wrap *w = data;
	callback(w);
}

int l_CheckboxSetText(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	const char *text = luaL_checkstring(L, 2);
	uiCheckboxSetText(uiCheckbox(w->control), text);
	RETURN_SELF;
}

int l_CheckboxOnToggled(lua_State *L)
{
        struct wrap *w = create_callback_data(L, 1);
        uiCheckboxOnToggled(uiCheckbox(w->control), on_checkbox_toggled, w);
        RETURN_SELF;
}

static struct luaL_Reg meta_Checkbox[] = {
	{ "SetText",              l_CheckboxSetText },
	{ "OnToggled",            l_CheckboxOnToggled },
	{ NULL }
};


/*
 * Combobox
 */

int l_NewCombobox(lua_State *L)
{
	CREATE_OBJECT(Combobox, uiNewCombobox());
	return 1;
}

int l_NewEditableCombobox(lua_State *L)
{
	CREATE_OBJECT(Combobox, uiNewEditableCombobox());
	return 1;
}

static void on_combobox_selected(uiCombobox *c, void *data)
{
	struct wrap *w = data;
	callback(w);
}

int l_ComboboxAppend(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	int n = lua_gettop(L);
	int i;
	for(i=2; i<=n; i++) {
		const char *text = luaL_checkstring(L, n);
		uiComboboxAppend(uiCombobox(w->control), text);
	}
	RETURN_SELF;
}

int l_ComboboxOnToggled(lua_State *L)
{
        struct wrap *w = create_callback_data(L, 1);
        uiComboboxOnSelected(uiCombobox(w->control), on_combobox_selected, w);
        RETURN_SELF;
}

static struct luaL_Reg meta_Combobox[] = {
	{ "Append",               l_ComboboxAppend },
	{ "OnToggled",            l_ComboboxOnToggled },
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
 * RadioButtons
 */

int l_NewRadioButtons(lua_State *L)
{
	CREATE_OBJECT(RadioButtons, uiNewRadioButtons());
	return 1;
}


int l_RadioButtonsAppend(lua_State *L)
{
	struct wrap *w = lua_touserdata(L, 1);
	int n = lua_gettop(L);
	int i;
	for(i=2; i<=n; i++) {
		const char *text = luaL_checkstring(L, i);
		uiRadioButtonsAppend(uiRadioButtons(w->control), text);
	}
	RETURN_SELF;
}


static struct luaL_Reg meta_RadioButtons[] = {
	{ "Append",               l_RadioButtonsAppend },
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
 * Tab
 */

int l_NewTab(lua_State *L)
{
	CREATE_OBJECT(Tab, uiNewTab());
	return 1;
}

int l_TabAppend(lua_State *L)
{
	int n = lua_gettop(L);
	int i;
	for(i=2; i<=n; i+=2) {
		uiTabAppend(UI_CAST(1, Tab), luaL_checkstring(L, i+0), UI_CAST(i+1, Control));
		lua_getmetatable(L, 1);
		lua_pushvalue(L, i+1);
		luaL_ref(L, -2);
	}
	RETURN_SELF;
}

static struct luaL_Reg meta_Tab[] = {
	{ "Append",               l_TabAppend },
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
	//lua_pushvalue(L, -1);
	//lua_pushboolean(L, 1);
	//lua_settable(L, LUA_REGISTRYINDEX);
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

int l_WindowSetMargined(lua_State *L)
{
	uiWindowSetMargined(UI_CAST(1, Window), luaL_checknumber(L, 2));
	RETURN_SELF;
}

static struct luaL_Reg meta_Window[] = {
	{ "SetChild",            l_WindowSetChild },
	{ "SetMargined",         l_WindowSetMargined },
	{ "Show",                l_ControlShow },
	{ "Destroy",             l_ControlDestroy },
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

	{ "NewButton",              l_NewButton },
	{ "NewCheckbox",            l_NewCheckbox },
	{ "NewCombobox",            l_NewCombobox },
	{ "NewEditableCombobox",    l_NewEditableCombobox },
	{ "NewGroup",               l_NewGroup },
	{ "NewHorizontalBox",       l_NewHorizontalBox },
	{ "NewHorizontalSeparator", l_NewHorizontalSeparator },
	{ "NewLabel",               l_NewLabel },
	{ "NewProgressBar",         l_NewProgressBar },
	{ "NewRadioButtons",        l_NewRadioButtons },
	{ "NewSlider",              l_NewSlider },
	{ "NewSpinbox",             l_NewSpinbox },
	{ "NewTab",                 l_NewTab },
	{ "NewVerticalBox",         l_NewVerticalBox },
	{ "NewWindow",              l_NewWindow },


	{ NULL }
};


int luaopen_libuilua(lua_State *L)
{


	CREATE_META(Box)
	CREATE_META(Button)
	CREATE_META(Checkbox)
	CREATE_META(Combobox)
	CREATE_META(Group)
	CREATE_META(Label)
	CREATE_META(ProgressBar)
	CREATE_META(Separator)
	CREATE_META(RadioButtons)
	CREATE_META(Slider)
	CREATE_META(Spinbox)
	CREATE_META(Tab)
	CREATE_META(Window)
	luaL_newlib(L, lui_table);
	return 1;
}

/*
 * End
 */
