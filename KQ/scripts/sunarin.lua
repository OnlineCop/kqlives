-- sunarin - "Sunarin Tower"

function autoexec()
  return
end


function entity_handler(en)
  if (en == 0) then
    local x, y = get_ent_tile(HERO1)
    move_entity(HERO2, x, y + 1)
    wait_for_entity(HERO2)

    bubble(en, "I am the king of thieves!")
    if (get_progress(P_AYLA_QUEST) == 7 and get_ent_chrx(HERO1) ~= 0) then
      bubble(HERO1, "Yea? Well I'm Ayla, "..pick("Daughter of Malkaron",
        "Princess Ayla", "heir to Dungar's estate".."!")
      bubble(en, "Oh really. Is that a fact?")
      thought(HERO1, "Uh, oh. Wrong thing to say...")
      bubble(en, "Guards!!")
      move_entity(1, x - 1, y)
      move_entity(2, x + 1, y)
      move_entity(3, x - 1, y + 1)
      move_entity(4, x + 1, y + 1)
      wait_for_entity(1, 4)
      set_run(0)
      combat(61)
      set_run(1)
      if (get_alldead() == 1) then
        bubble(en, "And now, your remains will be food for the fishes!")
        return
      end

      set_ent_active(1, 0)
      set_ent_active(2, 0)
      set_ent_active(3, 0)
      set_ent_active(4, 0)

      bubble(en, "You shall rue the day you came against me!")
      wait(50)
      set_ent_active(0, 0)
      wait(50)
      bubble(HERO1, "Whoa! He just disappeared! Oh well, that treasure is still down there.")
    else
      bubble(en, "Honor me!"
    end
  elseif (en == 1) then
    bubble(en, "WE GUARD THE SACRED TREASURE!")
  elseif (en == 2) then
    bubble(en, "None shall inherit the treasure except who joins the guild!")
  elseif (en == 3) then
    bubble(en, "Who are you? Are you trying to steal from us?"
  elseif (en == 4) then
    bubble(255, "The man glowers at you, but says nothing.")
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "town5", 2, -1)
  elseif (zn == 2) then
    LOC_door("door1")
  end

end


function LOC_door(which_marker)
  sfx(25)

  local x, y = marker(which_marker)
  set_mtile(x, y, 0)
  set_mtile(x, y + 1, 0)
  set_obs(x, y, 0)
  set_shadow(x, y, 8)
  set_zone(x, y, 0)
end
