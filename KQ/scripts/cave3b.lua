-- cave3b - "Second part of cave on way to Oracle's tower"

function autoexec()
  refresh()
end


function refresh()
  if (get_treasure(20) == 1) then
    set_mtile(11, 36, 95)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
--    combat(9)

  elseif (zn == 1) then
    change_map("cave3a", "uldoor1")

  elseif (zn == 2) then
    chest(20, I_B_LIFE, 1)
    refresh()

  elseif (zn == 3) then
    change_map("cave3a", "drdoor1")

  -- zn == 4 is a no-combat tile

  end
end


function entity_handler(en)
  return
end
