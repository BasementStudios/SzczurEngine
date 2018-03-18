function THIS:_onUpdate()
    self.pos.x = 500 + math.sin(World.getTime()*4)*200
    self.pos.y = 500 + math.cos(World.getTime()*2)*200
    end