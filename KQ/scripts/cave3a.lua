-- cave3a - "First part of cave on way to Oracle's tower"

function autoexec()
  if (get_ent_tilex(HERO1) == 24 and get_ent_tiley(HERO1) == 3) then
    set_warp(1, 264, 57)
  elseif (get_ent_tilex(HERO1) == 8 and get_ent_tiley(HERO1) == 10) then
    set_warp(1, 264, 60)
  end
  refresh()
end


function refresh()
  if (get_treasure(17) == 1) then
    set_btile(51, 13, 95)
  end
  if (get_treasure(18) == 1) then
    set_btile(52, 14, 95)
  end
  if (get_treasure(19) == 1) then
    set_btile(53, 13, 95)
  end
  if (get_progress(P_ORACLE) > 0) then
    set_ftile(get_marker_tilex("dldoor1"), get_marker_tiley("dldoor1") - 1, 119)    -- set_ftile(5, 41, 119)
    set_obs(get_marker_tilex("dldoor1"), get_marker_tiley("dldoor1") - 1, 0)    -- set_obs(5, 41, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    combat(8)

  elseif (zn == 1) then
    change_map("main", 264, 60, 264, 60)

  elseif (zn == 2) then
    view_range(1, 44, 8, 57, 58)
    warpm("halldoor", 8)

  elseif (zn == 3) then
    view_range(1, 8, 47, 20, 57)
    warpm("dldoor2", 8)
    -- warp(8, 46, 8)

  elseif (zn == 4) then
    change_map("cave3b", 0, 0, 0, 0)

  elseif (zn == 5) then
    view_range(1, 9, 8, 22, 21)
    warpm("uldoor3", 8)
    -- warp(9, 3, 8)

  elseif (zn == 6) then
    chest(18, I_NLEAF, 1)
    refresh()

  elseif (zn == 7) then
    chest(17, I_PCURING, 1)
    refresh()

  elseif (zn == 8) then
    chest(19, I_WOODCLOAK, 1)
    refresh()

  elseif (zn == 9) then
    view_range(1, 9, 8, 22, 21)
    warpm("uldoor2", 8)
    -- warp(7, 3, 8)

  elseif (zn == 10) then
    if (get_progress(P_ORACLE) == 0) then
      bubble(HERO1, "The door is locked.")
    else
      view_range(1, 22, 23, 43, 39)
      warpm("mrdoor1", 8)
    -- warp(27, 28, 8)
      set_progress(P_ORACLEMONSTERS, 1)
    end

  -- zn == 11 is a no-combat zone

  elseif (zn == 12) then
    view_range(1, 8, 47, 20, 57)
    warpm("dldoor1", 8)
    -- warp(5, 42, 8)

  elseif (zn == 13) then
    change_mapm("town4", "bad_portal")
    -- change_map("town4", 32, 21, 32, 21)

  elseif (zn == 14) then
    change_mapm("cave1", "dstairs1")
    -- change_map("cave1", 2, 17, 2, 17)

  elseif (zn == 15) then
    change_mapm("cave3b", "door_1")
    -- change_map("cave3b", 13, 13, 13, 13)

  elseif (zn == 16) then
    view_range(1, 29, 9, 38, 19)
    warpm("urstairs1", 8)
    -- warp(27, 8, 8)

  elseif (zn == 17) then
    view_range(1, 32, 48, 39, 58)
    warpm("drstairs1", 8)
    -- warp(28, 44, 8)

  elseif (zn == 18) then
    change_map("main", 264, 57, 264, 57)

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "You cannot stop us! Begone!")
  elseif (en == 1) then
    bubble(en, "This statue is ours! You cannot have it!")
  elseif (en == 2) then
    bubble(HERO1, "This is the other broken half of the stolen Oracle Statue!")
  elseif (en == 3) then
    bubble(en, "We will die before we give you the statue!")
  elseif (en == 4) then
    bubble(en, "A curse be upon you... a curse upon you all!")
  end
  return
end
