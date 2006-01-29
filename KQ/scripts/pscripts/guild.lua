

-- guild - "Home of the Embers in Sunarin"
--





function autoexec()
  refresh()
end


function refresh()
  if (get_progress(48) == 1) then
    set_mtile(12, 25, 0)
    set_mtile(12, 26, 0)
    set_obs(12, 26, 0)
    set_zone(12, 26, 0)
    set_ftile(13, 25, 242)
    set_ftile(13, 26, 241)
  elseif (get_progress(48) == 2) then
    set_mtile(12, 25, 242)
    set_mtile(12, 26, 241)
    set_ftile(13, 25, 0)
    set_ftile(13, 26, 0)
    set_obs(12, 26, 1)
  end

  if (get_progress(47) == 1) then
    set_ent_active(0, 0)
    set_ent_active(1, 0)
  end

  if (get_progress(43) == 1) then
    set_mtile(58, 24, 265)
  end

  -- Should Ayla appear or not?
  if (LOC_manor_or_party(6) or get_progress(47) < 1) then
    set_ent_active(2, 0)
  else
    set_ent_active(2, 1)
    set_ent_id(2, 6)
  end
end


function postexec()
  return
end


-- Updated 20020929 PH
-- Added test for 43 so
-- that you can re-enter the guild
-- if you do not have the helmet yet
function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(43) == 1) then
      set_progress(47, 2)
    end
    change_mapm("town5", "guild_door")
    -- change_map("town5", 18, 37, 18, 37)

  elseif (zn == 2) then
    warp(10, 5, 8)

  elseif (zn == 3) then
    if (get_progress(48) == 0) then
      bubble(200, "Well I'll be...")
      bubble(200, "The book really IS called 'How to Enter the Ember's Secret Hideout'. Heh... brilliant in its simplicity, I suppose.")
      set_progress(48, 1)
      sfx(26)
      refresh()
    end

  elseif (zn == 4) then
    if (party[0] == 6) then
      bubble(200, "I'm a thief... but even I wouldn't read this garbage!")
    else
      bubble(200, "Thieves read?!")
    end

  elseif (zn == 5) then
    chest(47, 0, 500)
    refresh()

  elseif (zn == 6) then
    warp(37, 7, 8)

  elseif (zn == 7) then
    warp(23, 19, 8)

  elseif (zn == 8) then
    chest(50, 12, 1)
    refresh()

  elseif (zn == 9) then
    bubble(200, "These look interesting.")

  elseif (zn == 10) then
    bubble(200, "Locked.")

  elseif (zn == 11) then
    local a, b

    if (get_progress(47) == 0) then
      a = get_vx()
      b = get_vy()
      move_camera(816, 288, 2)
      bubble(200, "Ooh... a chest! But who are these clowns?")
      wait(50)
      bubble(0, "I'll follow the others to the Coliseum and you wait here for the Guildmaster to return from his visit.")
      bubble(1, "Yes sir! I'll...")
      bubble(0, "Who is that?")
      set_ent_facing(1, 3)
      drawmap()
      screen_dump()
      move_camera(a, b, 1)
      drawmap()
      screen_dump()
      bubble(0, "Guards!")
      bubble(200, "Oh oh!")
      wait(50)
      set_ftile(65, 19, 369)
      set_ftile(64, 20, 372)
      set_ftile(66, 20, 371)
      set_ftile(65, 21, 370)
      wait(50)
      bubble(200, "What the...")
      drawmap()
      screen_dump()
      set_run(0)
      combat(26)
      set_run(1)
      if (get_alldead() == 1) then
        return
      end
      set_progress(47, 1)
      set_ftile(65, 19, 0)
      set_ftile(64, 20, 0)
      set_ftile(66, 20, 0)
      set_ftile(65, 21, 0)
      refresh()
      drawmap()
      screen_dump()
      wait(50)
      bubble(200, "Those guys are gone!")
      bubble(200, "But they left the chest behind.")
    end

  elseif (zn == 12) then
    if (get_progress(43) == 0) then
      sfx(5)
      msg("Opal Helmet procured", 255, 0)
      set_progress(43, 1)
      refresh()
      drawmap()
      screen_dump()
      wait(50)
      bubble(200, "I don't like this.",
                   "It was just too easy.")
    end

  elseif (zn == 13) then
    warp(10, 27, 8)

  elseif (zn == 14) then
    touch_fire(party[0])

  elseif(zn == 15) then
    -- Close the secret door
    sfx(26)
    set_progress(48, 2)
    refresh()

  elseif (zn == 16) then
    -- Open the secret door (a second time)
    sfx(26)
    set_progress(48, 1)
    refresh()
  end
end


function entity_handler(en)
   if (en==2) then --
      bubble(en, "Wha...? Oh, it's you!")
      bubble(200, "Hello... I recognise you from Nostik's manor, don't I?")
      bubble(en, "Yes, I broke into the house, but I couldn't find the secret passage.", "Can I join you now?")
      set_ent_active(en,0)
      set_all_equip(6, 12, 40, 47, 62, 75, 0)
      id = select_team{6}
      -- Add the characters that were deselected to the manor
      add_to_manor(id)
   end
end
