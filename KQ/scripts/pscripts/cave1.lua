

-- cave1 - "Underground tunnel from Ekla to Randen"

--
function autoexec()
  if (get_treasure(2) == 0) then
    set_progress(1, 0)
  end
  -- PH added refresh here to get rid of DarkImp
  refresh()
end
function refresh()
  -- Pot in SW corner
  if (get_treasure(1) == 1) then
    set_zone(29, 45, 0)
  end
  -- Treasure chest on W
  if (get_treasure(2) == 1) then
    set_zone(2, 18, 41)
    set_mtile(2, 18, 41)
  end
  -- Dark Imp boss in SE corner
  if (get_progress(2) == 1) then
    set_ftile(41, 42, 0)
    set_obs(41, 42, 0)
    set_zone(41, 42, 7)
  end
  -- Dying man in NE corner
  if (get_progress(3) == 1 or get_progress(2) == 1) then
    set_btile(45, 14, 25)
    set_obs(45, 14, 0)
    set_zone(45, 14, 0)
  end
  -- Portal in SW corner
  if (get_progress(38) == 1) then
    set_ftile(7, 43, 217)
  end
end
function postexec()
  return
end
function zone_handler(zn)
  if (zn == 0) then
    if (get_progress(38) == 0) then
      combat(6)
    end
  elseif (zn == 1) then
    change_mapm("town1", "dstairs1")
    -- change_map("town1", 27, 57, 27, 57)
  elseif (zn == 2) then
    change_mapm("town2", "dstairs1")
    -- change_map("town2", 11, 11, 11, 11)
  elseif (zn == 4) then
    chest(1, 103, 1)
    refresh()
  elseif (zn == 5) then
    chest(2, 126, 1)
    refresh()
  elseif (zn == 6) then
    bubble(200, "These stairs are blocked!")
  elseif (zn == 7) then
    -- This is simply a monster-free zone
  elseif (zn == 8) then
    if (get_progress(1) == 0) then
      bubble(200, "Hmm... this wall looks odd.")
      set_progress(1, 1)
    end
  elseif (zn == 9) then
    warp(4, 24, 8)
  elseif (zn == 10) then
    warp(23, 37, 8)
  elseif (zn == 11) then
    if (get_progress(3) == 0) then
      bubble(255, "... don't go any further. A strange creature has... blocked the path. It just appeared there out of nowhere.")
      set_progress(3, 1)
    else
      bubble(200, "He's dead.")
    end
  elseif (zn == 12) then
    if (get_progress(2) == 0) then
      set_run(0)
      combat(7)
      set_run(1)
      set_progress(2, 1)
      refresh()
    end
  elseif (zn == 13) then
    if (get_progress(38) == 0) then
      if (get_progress(37) == 1) then
        bubble(200, "Hmmm... I guess if I just touch it with the rod...")
        do_fadeout(4)
        set_progress(38, 1)
        refresh()
        drawmap()
        screen_dump()
        do_fadein(4)
        bubble(200, "Whoa! The rod melted the portal shut!")
      else
        bubble(200, "Hmmm... this is very old and has several deep cracks in it.")
      end
    else
      bubble(200, "I wonder where this came from?")
    end
  end
end
function entity_handler(en)
  return
end
