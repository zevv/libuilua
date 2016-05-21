#!/usr/bin/lua

ui = require "libuilua"

ui.Init()

win = ui.NewWindow("Hello", 320, 200, false):SetMargined(1):SetChild(
	ui.NewVerticalBox():Append(
		ui.NewHorizontalBox():Append(
			ui.NewGroup("Basic Controls"):SetMargined(1):SetChild(
				ui.NewVerticalBox():SetPadded(1):Append(
					ui.NewButton("Button"),
					ui.NewCheckbox("Checkbox"),
					ui.NewLabel("Label"),
					ui.NewHorizontalSeparator()
				)
			), 
			ui.NewVerticalBox():Append(
				ui.NewGroup("Numbers"):SetMargined(1):SetChild(
					ui.NewVerticalBox():SetPadded(1):Append(
						ui.NewSpinbox(0, 100),
						ui.NewSlider(0, 100),
						ui.NewProgressBar(0, 100)
					)
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
						), 1
					)
				), 1
			), 1
		), 1
	)
)

win:Show();


win = nil
collectgarbage("collect")
collectgarbage("collect")
collectgarbage("collect")

ui.Main()


-- vi: ft=lua ts=3 sw=3
