#!/usr/bin/lua

ui = require "libuilua"

ui.Init()


win = ui.NewWindow("Hello", 320, 200, false):SetChild(
	ui.NewGroup("Group"):SetMargined(10):SetChild(
		ui.NewHorizontalBox():SetPadded(5):Append(
			ui.NewVerticalBox():SetPadded(5):Append(
				ui.NewButton("Foo"):OnClicked(function(name)
					print("This is " .. name)
				end, "Foo")
			):Append(
				ui.NewSlider(0, 100):SetValue(50)
			):Append(
				ui.NewSpinbox(0, 100):SetValue(50)
			), true
		):Append(
			ui.NewVerticalBox():SetPadded(5):Append(
				ui.NewHorizontalSeparator()
			):Append(
				ui.NewLabel("Label one")
			):Append(
				ui.NewProgressBar():SetValue(10)
			):Append(
				ui.NewButton("Quit"):OnClicked(ui.Quit)
			), true
		)
	)
):Show()

win = nil
collectgarbage("collect")
collectgarbage("collect")
collectgarbage("collect")

ui.Main()


-- vi: ft=lua ts=3 sw=3
