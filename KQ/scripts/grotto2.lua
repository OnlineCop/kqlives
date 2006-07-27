-- grotto2 - "Second, larger part of grotto north of Ekla"

function autoexec()
  refresh()
end


function refresh()
  if (get_progress(P_CANCELROD) > 0) then
    set_mtile("cancel", 265)
    set_zone("cancel", 0)
  end

  if (get_treasure(48) == 1) then
    set_mtile("treasure1", 265)
    set_zone("treasure1", 0)
  end

  if (get_treasure(49) == 1) then
    set_mtile("treasure2", 265)
    set_zone("treasure2", 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    combat(24)

  elseif (zn == 1) then
    change_map("grotto", "portal")

  elseif (zn == 2) then
    if (get_progress(P_TALKOLDMAN) == 3) then
      msg("$0 fits the rusty key into the lock.", 255, 0)
      set_progress(P_TALKOLDMAN, 4)
    elseif (get_progress(P_TALKOLDMAN) == 4) then
      bubble(HERO1, "Something's supposed to happen here... it hasn't been programmed yet.")
    else
      bubble(HERO1, "This door is stuck tight.")
    end

  elseif (zn == 3) then
    if (get_progress(P_CANCELROD) == 0) then
      set_progress(P_CANCELROD, 1)
      sfx(5)
      msg("Rod of Cancellation procured", 255, 0)
      refresh()
    end

  elseif (zn == 4) then
    chest(48, I_MHERB, 1)
    refresh()

  elseif (zn == 5) then
    chest(49, I_HELM1, 1)
    refresh()

  end
end


function entity_handler(en)
  return
end
