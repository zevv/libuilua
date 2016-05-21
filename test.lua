#!/usr/bin/lua

ui = require "libuilua"

ui.Init()

win = ui.NewWindow("Hello", 320, 200, false):SetMargined(1):SetChild(
	ui.NewVerticalBox():Append(
		ui.NewHorizontalBox():Append(
			ui.NewGroup("Basic Controls"):SetMargined(1):SetChild(
				ui.NewVerticalBox():SetPadded(1):Append(
					ui.NewButton("Button")
				):Append(
					ui.NewCheckbox("Checkbox")
				):Append(
					ui.NewLabel("Label")
				):Append(
					ui.NewHorizontalSeparator()
				)
			), 1
		):Append(
			ui.NewVerticalBox():Append(
				ui.NewGroup("Numbers"):SetMargined(1):SetChild(
					ui.NewVerticalBox():SetPadded(1):Append(
						ui.NewSpinbox(0, 100)
					):Append(
						ui.NewSlider(0, 100)
					):Append(
						ui.NewProgressBar(0, 100)
					)
				)
			):Append(
				ui.NewGroup("Lists"):SetMargined(1):SetChild(
					ui.NewVerticalBox():SetPadded(1):Append(
						ui.NewCombobox()
							:Append("Combobox Item 1")
							:Append("Combobox Item 2")
							:Append("Combobox Item 3")
					):Append(
						ui.NewEditableCombobox()
							:Append("Editable Item 1")
							:Append("Editable Item 2")
							:Append("Editable Item 3")
					):Append(
						ui.NewRadioButtons()
							:Append("Radio Button 1")
							:Append("Radio Button 2")
							:Append("Radio Button 3")
					):Append(
						ui.NewTab()
							:Append("Page 1", ui.NewHorizontalBox())
							:Append("Page 2", ui.NewHorizontalBox())
							:Append("Page 3", ui.NewHorizontalBox())
						, 1
					)
				), 1
			), 1
		), 1
	)
)

win:Show();



--win = nil
--collectgarbage("collect")
--collectgarbage("collect")
--collectgarbage("collect")

ui.Main()


-- vi: ft=lua ts=3 sw=3
