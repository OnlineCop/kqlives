

-- cave2 - "Pit in grotto north of Ekla"

--
function autoexec()
  refresh()
end
function postexec()
  if (get_progress(11) == 0) then
    bubble(200, "Ouch!")
    bubble(200, "This is no ordinary gopher hole!")
  end
end
function refresh()
  -- Show the shiny, glowing rescue spot
  if (get_progress(11) > 0) then
    set_btile(15, 18, 30)
    set_zone(15, 18, 3)
  end
end
function zone_handler(zn)
  if (zn == 1) then
    --
    if (get_progress(11) == 0) then
      bubble(200, "There's some kind of barrier here. I can't get past it.")
      set_progress(11, 1)
      refresh()
      bubble(200, "Hey... What's that thing down there?")
    --
    else
      bubble(200, "This barrier is still here. I can't get out.")
    end
  elseif (zn == 2) then
    chest(16, 103, 1)
    refresh()
  elseif (zn == 3) then
    if (get_progress(36) == 0) then
      if (get_progress(9) == 1) then
        --
        bubble(200, "This doesn't do anything; I can't get out of here. I'll give up and just go to sleep.")
        set_progress(9, 2)
        inn("You decide to sleep.", 0, 0)
      elseif (get_progress(9) == 2) then
        --
        bubble(200, "Oops, I'm stuck down here again. I'll try sleeping again to get back out.")
        inn("You decide to sleep.", 0, 0)
      end
      set_ent_facing(200, 3)
      change_mapm("grotto", "by_fire")
      -- change_map("grotto", 19, 16, 19, 16)
    elseif (get_progress(36) == 1) then
      bubble(200, "Hmm... this doesn't look like it does anything.")
      msg("Hello? Who's there? Is that somebody down there?", 255, 0)
      bubble(200, "Yeah... I fell down here and the door's blocked.")
      msg("Hang on, let me get you out of there.", 255, 0)
      set_progress(9, 3)
      change_mapm("grotto", "by_pit")
      -- change_map("grotto", 17, 14, 17, 14)
    else
      --
      bubble(200, "Derig, can you help me out of here?")
      msg("Sure thing. Here you go.", 255, 0)
      set_ent_facing(200, 3)
      change_mapm("grotto", "by_fire")
      -- change_map("grotto", 19, 16, 19, 16)
    end
  end
end
function entity_handler(en)
  return
end
