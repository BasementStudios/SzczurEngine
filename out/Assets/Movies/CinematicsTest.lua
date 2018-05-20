Cinematics.loadFromFile("Assets/Movies/movie.mp4")
Cinematics.setFontPath("Assets/Movies/font.otf")
Cinematics.addLoop(10000000,15000000,
    function() print("text1") end,
    "text1", 25000000,
    function() Cinematics.stop() end,
    "text2", 0
)
Cinematics.play()

