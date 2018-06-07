Cinematics.loadFromFile("Assets/Movies/movie3.mp4")


Cinematics.setCallbackFinish(function()

  Cinematics.loadFromFile("Assets/Movies/movie3.mp4")
  Cinematics.play();

end)
Cinematics.play()


