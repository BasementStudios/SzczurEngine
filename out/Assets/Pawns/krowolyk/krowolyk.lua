local pawn = THIS

pawn:armature():setSpeed(1.0)
pawn:armature():fadeIn("Idle", 0.1, 0)

pawn:setDamageAnimation("Zabojczy_kas", 0.1, false, 2.0)

pawn:armature():play("Idle", 0) 