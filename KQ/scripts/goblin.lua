-- goblin - "Goblin village somewhere on the map"

function autoexec()
  return
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 186, 108, 186, 108)

  elseif (zn == 2) then
    touch_fire(party[0])

  elseif (zn == 3) then
    door_in(67, 12, 63, 5, 71, 15)

  elseif (zn == 4) then
    door_out(30, 7)

  elseif (zn == 5) then
    door_in(77, 12, 73, 5, 81, 15)

  elseif (zn == 6) then
    door_out(42, 15)

  elseif (zn == 7) then
    door_in(67, 24, 63, 17, 71, 27)

  elseif (zn == 8) then
    door_out(7, 20)

  elseif (zn == 9) then
    door_in(77, 24, 73, 17, 81, 27)

  elseif (zn == 10) then
    door_out(41, 32)

  elseif (zn == 11) then
    door_in(67, 36, 63, 29, 71, 39)

  elseif (zn == 12) then
    door_out(11, 36)

  elseif (zn == 13) then
    door_in(77, 36, 73, 29, 81, 39)

  elseif (zn == 14) then
    door_out(39, 38)

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
