

-- fort - "Mountain fort south of Andra"

--
function autoexec()
  refresh()
end
function refresh()
  if (get_progress(108) > 3) then
    place_ent(4, 39, 14)
    set_ent_facing(4, 0)
  end
end
function postexec()
end
function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 261, 33, 261, 33)
  elseif (zn == 2) then
    change_map("main", 261, 36, 261, 36)
  elseif (zn == 3) then
    door_in(40, 16, 36, 10, 44, 18)
  elseif (zn == 4) then
    door_out(17, 31)
  elseif (zn == 5) then
    touch_fire(party[0])
  end
end
function entity_handler(en)
  if (en == 0) then
    bubble(en, "We have no outposts in goblin territory. If you get into real trouble, you'll have to go all the way back to Andra.")
  elseif (en == 1) then
    bubble(en, "Make sure to stock up on supplies.")
  elseif (en == 2) then
    if (get_progress(108) < 3) then
      bubble(en, "We cannot let you pass.")
    elseif (get_progress(108) == 3) then
      bubble(en, "The Lieutenant requires Tsorin's authorization before anyone can pass through here.")
    else
      bubble(en, "You are free to come and go as you please.")
    end
  elseif (en == 3) then
    if (get_progress(108) < 3) then
      bubble(en, "No one is allowed through right now.")
    elseif (get_progress(108) == 3) then
      bubble(en, "Show Tsorin's seal to this man next to me. He will inform the Lieutenant that you have it.")
    else
      bubble(en, "Be careful in the goblin lands.")
    end
  elseif (en == 4) then
    if (get_progress(108) < 3) then
      bubble(en, "Only Captain Tsorin can authorize you to come through here.")
    elseif (get_progress(108) == 3) then
      if (get_numchrs() == 1) then
        bubble(200, "Tsorin sends his greetings to you. You are to let me pass through to the Oracle's Tower.")
      else
        bubble(200, "Tsorin sends his greetings to you. You are to let us pass through to the Oracle's Tower.")
      end
      bubble(en, "I see that you are bearing his seal. Very well.")
      bubble(en, "I'll tell the Lieutenant you're here.")
      set_ent_speed(en, 4)
      set_ent_script(en, "D10")
      wait_for_entity(en, en)
      set_progress(108, 4)
      refresh()
    elseif (get_progress(108) == 4) then
      bubble(en, "The Lieutenant says you are free to go into goblin lands. Be cautious.")
    else
      bubble(en, "We have received word that the goblin lands are peaceful now. Thank you for your help.")
    end
  elseif (en == 5) then
    if (get_progress(108) == 4) then
      bubble(en, "Lieutenant:", "South of here is a river you'll need to cross. Head for the tower in the small group of mountains due west from there. You just have to go through some caves to reach it. Then make your way up to the Oracle. Is that understood?")
      bubble(200, "Yeah, I guess so... it all seems clear enough.")
      bubble(en, "Good. Head out whenever you're ready.")
    else
      bubble(en, "Best of luck to you.")
    end
  end
end
