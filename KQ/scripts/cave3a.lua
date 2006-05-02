-- cave3a - "First part of cave on way to Oracle's tower"

-- // P_TRAVELPOINT: Whether we've just come through the TravelPoint

function autoexec()
  if (get_ent_tilex(HERO1) == get_marker_tilex("exit") and
      get_ent_tiley(HERO1) == get_marker_tiley("exit")) then
    set_warp(1, 264, 57)
  elseif (get_ent_tilex(HERO1) == get_marker_tilex("uldoor1") and
          get_ent_tiley(HERO1) == get_marker_tiley("uldoor1")) then
    set_warp(1, 264, 60)
  end

  if (get_progress(P_ORACLEMONSTERS) > 0) then
    local a
    for a = 0, 4, 1 do
      set_ent_active(a, 0)
    end
  end

  if (get_progress(P_TRAVELPOINT) == 1) then
    view_range(1, 22, 23, 43, 39)
    set_progress(P_TRAVELPOINT, 0)
  else
    set_desc(0)
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
  if (get_progress(P_WARPEDTOT4) > 0) then
    set_ftile(get_marker_tilex("dldoor1"), get_marker_tiley("dldoor1") - 1, 119)    -- set_ftile(13, 49, 119)
    set_obs(get_marker_tilex("dldoor1"), get_marker_tiley("dldoor1") - 1, 0)    -- set_obs(13, 49, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    combat(8)

  elseif (zn == 1) then
    change_map("main", "cave3a_s")

  elseif (zn == 2) then
    view_range(1, 44, 8, 57, 58)
    warp("halldoor", 8)

  elseif (zn == 3) then
    view_range(1, 8, 47, 20, 57)
    warp("dldoor2", 8)

  elseif (zn == 4) then
    change_map("cave3b", "entrance")

  elseif (zn == 5) then
    view_range(1, 9, 8, 22, 21)
    warp("uldoor3", 8)

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
    warp("uldoor2", 8)

  elseif (zn == 10) then
    if (get_progress(P_WARPEDTOT4) == 0) then
      msg("The door creaks open noisily.", 255, 0)
      set_progress(P_WARPEDTOT4, 1)
      refresh()
    else
      view_range(1, 22, 23, 43, 39)
      warp("mrdoor1", 8)
      if (get_progress(P_ORACLEMONSTERS) == 0) then
        LOC_monsters_statue()
      end
    end

  -- zn == 11 is a no-combat zone

  elseif (zn == 12) then
    view_range(1, 8, 47, 20, 57)
    warp("dldoor1", 8)

  elseif (zn == 13) then
    if (get_progress(P_WARPEDTOT4) < 3) then
      set_progress(P_ORACLEMONSTERS, 3)
      change_map("town4", "bad_portal")
    else
      set_progress(P_TRAVELPOINT, 1)
      change_map("town7", "travelpoint")
    end

  elseif (zn == 14) then
    change_map("cave1", "dstairs1")

  elseif (zn == 15) then
    change_map("cave3b", "exit")

  elseif (zn == 16) then
    view_range(1, 29, 9, 38, 19)
    warp("urstairs1", 8)

  elseif (zn == 17) then
    view_range(1, 32, 48, 39, 58)
    warp("drstairs1", 8)

  elseif (zn == 18) then
    change_map("main", "cave3a_n")

  elseif (zn == 19) then
    if (get_progress(P_ORACLEMONSTERS) == 1) then
      if (get_numchrs() == 1) then
        bubble(HERO1, "No, really. I need to tell the Oracle about this before I go through this TravelPoint!")
      else
        bubble(HERO1, "No, really. We need to tell the Oracle about this before we go through this TravelPoint!")
      end
    end

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
end


function LOC_monsters_statue()
  local a
  set_progress(P_ORACLEMONSTERS, 1)

  set_ent_script(0, "R5K")
  set_ent_script(1, "R6K")
  set_ent_script(2, "R7K")
  set_ent_script(3, "R8K")
  set_ent_script(4, "U1R8K")

  bubble(HERO1, "Hey, who's that?!")

  if (get_numchrs() == 1) then
    set_ent_script(HERO1, "U2")
    wait_for_entity(HERO1, HERO1)
  else
    set_ent_script(HERO1, "U2")
    set_ent_script(HERO2, "U1")
    wait_for_entity(HERO1, HERO2)
    orient_heroes()
  end

  wait_for_entity(0, 4)
  for a = 0, 4, 1 do
    set_ent_movemode(a, 2)
  end

  if (get_progress(P_DENORIAN) == 0) then
    bubble(HERO1, "That looked like the missing statue Tsorin was talking about!")
    bubble(HERO1, "And those were Malkaron's men!")
  else
    bubble(HERO1, "That looked like Malkaron's men with the Denorian's statue!")
  end

  if (get_numchrs() == 1) then
    bubble(HERO1, "I've got to report this to the Oracle!")
  else
    bubble(HERO1, "We've got to report this to the Oracle!")
  end

end
