
Lua binding for libui: https://github.com/andlabs/libui


- All top level functions are in the table returned by `require`

- New objects are created with ui.NewType()

- All uiTypeMethod() functions are accessible as object:Method() calls

- All Set() methods return the object itself, which allow for chaining like 
  `s = ui.NewSlider(0, 10):SetValue(5):OnChanged(fn)`

````
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

````
