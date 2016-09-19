#!/usr/bin/lua

local ui = require "libuilua"

ui.Init()
					

local spinbox, slider, progressbar

local function update(control)
	local v = control:Value()
	spinbox:SetValue(v)
	slider:SetValue(v)
	progressbar:SetValue(v)
end

spinbox = ui.NewSpinbox(0, 100):OnChanged(update) 
slider = ui.NewSlider(0, 100):OnChanged(update)
progressbar = ui.NewProgressBar(0, 100)


local win = ui.NewWindow("Hello", 320, 200, false):SetMargined(1):SetChild(
	ui.NewVerticalBox():Append(
		ui.NewHorizontalBox():Append(
			ui.NewGroup("Basic Controls"):SetMargined(1):SetChild(
				ui.NewVerticalBox():SetPadded(1):Append(
					ui.NewButton("Button"),
					ui.NewCheckbox("Checkbox"),
					ui.NewLabel("Label"),
					ui.NewHorizontalSeparator(),
					ui.NewDatePicker(),
					ui.NewDateTimePicker(),
					ui.NewTimePicker()
				)
			), 
			ui.NewVerticalBox():Append(
				ui.NewGroup("Numbers"):SetMargined(1):SetChild(
					ui.NewVerticalBox():SetPadded(1):Append(spinbox, slider, progressbar)
				)
			):Append(
				ui.NewGroup("Lists"):SetMargined(1):SetChild(
					ui.NewVerticalBox():SetPadded(1):Append(
						ui.NewCombobox():Append(
							"Combobox Item 1",
							"Combobox Item 2",
							"Combobox Item 3"
						),
						ui.NewEditableCombobox():Append(
							"Editable Item 1",
							"Editable Item 2",
							"Editable Item 3"
						),
						ui.NewRadioButtons():Append(
							"Radio Button 1",
							"Radio Button 2",
							"Radio Button 3"
						)
					):Append(
						ui.NewTab():Append(
							"Page 1", ui.NewHorizontalBox(),
							"Page 2", ui.NewHorizontalBox(),
							"Page 3", ui.NewHorizontalBox()
						), true
					)
				), true
			), true
		), true
	)
)


win:Show()

while true do
	ui.MainStep(true)
end


-- vi: ft=lua ts=3 sw=3
