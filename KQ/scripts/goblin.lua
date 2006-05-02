-- goblin - "Goblin village somewhere on the map"

function autoexec()
  return
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "goblin")

  elseif (zn == 2) then
    touch_fire(party[0])

  elseif (zn == 3) then
    door_in(67+7, 12+8, 63+7, 5+8, 71+7, 15+8)

  elseif (zn == 4) then
    door_out(30+7, 7+8)

  elseif (zn == 5) then
    door_in(77+7, 12+8, 73+7, 5+8, 81+7, 15+8)

  elseif (zn == 6) then
    door_out(42+7, 15+8)

  elseif (zn == 7) then
    door_in(67+7, 24+8, 63+7, 17+8, 71+7, 27+8)

  elseif (zn == 8) then
    door_out(7+7, 20+8)

  elseif (zn == 9) then
    door_in(77+7, 24+8, 73+7, 17+8, 81+7, 27+8)

  elseif (zn == 10) then
    door_out(41+7, 32+8)

  elseif (zn == 11) then
    door_in(67+7, 36+8, 63+7, 29+8, 71+7, 39+8)

  elseif (zn == 12) then
    door_out(11+7, 36+8)

  elseif (zn == 13) then
    door_in(77+7, 36+8, 73+7, 29+8, 81+7, 39+8)

  elseif (zn == 14) then
    door_out(39+7, 38+8)

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "I'm a goblin. Fear me.")

  elseif (en == 1) then
    bubble(en, "This village is often overlooked by Malkaron's armies, so we are relatively safe.")

  else
    bubble(en, "Booga-booga! I'm a goblin! Rowr!")
  end
end
