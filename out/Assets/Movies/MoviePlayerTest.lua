MoviePlayer.loadFromFile("Assets/Movies/movie.mp4")
MoviePlayer.setFontPath("Assets/Movies/font.otf")
MoviePlayer.addLoop(10000000,15000000,
    function() print("text1") end,
    "text1", 25000000,
    function() MoviePlayer.stop() end,
    "text2", 0
)
MoviePlayer.play()

