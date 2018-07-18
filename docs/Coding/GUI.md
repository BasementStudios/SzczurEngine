System GUI autorstwa Tomasz (Knayder) opiera się na polimorfiźmie i klasie bazowej `Widget`. Wszystkie Widgety dziedziczą z tej klasy.
Wyróżniamy na razie:
--ImageWidget
--TextWidget
--TextAreaWidget
--ScrollWidget
--Widget
Każdy Widget, ma w sobie kontener Widgetów. Można to troszeczkę porównać do HTML'a. Jak wsadzimy jeden Widget w drugi, to przesuwając ten wyżej, przesuwamy także ten, który siedzi w nim. Pozycje Widgetów zawsze podajemy relatywnie do Widgeta matki:

```lua
local interface = GUI.addInterface() --interface to na dobrą sprawę po prostu Widget

local wid1 = interface:addWidget({
    position = {200,200};
    visible = true; --Nie trzeba tego pisać, z automatu jest true
    activate = true; --Nie trzeba tego pisać, z automatu jest true
})

local wid2 = wid1:addWidget({
    position = {50,50};
}) --Pozycja widgetu wid2 na ekranie, to {250,250};
```

Trochę inne działanie przejawia ScrollArea, gdyż Widgety które wsadzimy do niego, będą się wyświetlać na scrolowanym fragmencie.

Do każdej klasy dziedziczącej z Widget, wchodzą w sklad jakieś atrybuty oraz atrybuty klasy Widget:

Widget: -- interface:addWidget({...})
    position = {200,300};
    visible = true;
    active = true;

ImageWidget: -- interface:addImage({...})
    texture = GUI.getTexture("Path/To/Image.png");

TextWidget: -- interface:addText({...})
    font = GUI.getFont("Path/To/Font.ttf");
    color = {255,0,255};
    fontSize = 28;
    text = "Caption on Button";

TextAreaWidget: -- interface:addTextArea({...})
    font = GUI.getFont("Path/To/Font.ttf");
    color = {255,0,255};
    fontSize = 28;
    text = "Caption on Button";
    size = {28,4}; Ilość znaków mieszczących się w TextArea

ScrollAreaWidget: -- interface:addArea({...})
    size = {500,600};
    scrollSpeed = 10;

