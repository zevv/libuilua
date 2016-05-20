#!/usr/bin/lua

ui = require "libuilua"

ui.Init()

win = ui.NewWindow("Hello", 320, 200, false):SetChild(
	ui.NewGroup("Group"):SetMargined(10):SetChild(
		ui.NewVerticalBox():SetPadded(5):Append(
			ui.NewButton("Foo"):OnClicked(function(name)
				print("This is " .. name)
			end, "Foo")
		):Append(
			ui.NewButton("Bar")
		):Append(
			ui.NewSlider(0, 100):SetValue(50)
		):Append(
			ui.NewSpinbox(0, 100):SetValue(50)
		):Append(
			ui.NewHorizontalSeparator()
		):Append(
			ui.NewProgressBar():SetValue(10)
		)
	)
):Show()


ui.Main()

-- vi: ft=lua ts=3 sw=3
