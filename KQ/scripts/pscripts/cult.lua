

-- cult - "Cult of the Shadow Walker base on island west of Sunarin"

function autoexec()
  local a

  view_range(1, 0, 0, 22, 25)
  if (get_progress(67) < 2) then
    set_obs(11, 24, 1)
    set_zone(11, 24, 32)
  else
    set_zone(63, 44, 31)
    set_zone(63, 45, 31)
    set_obs(11, 24, 0)
    for a=0, 19, 1 do
      set_ent_active(a, 0)
    end
    set_ent_active(23, 0)
  end
  set_ent_active(20, 0)
  set_ent_active(21, 0)
  set_ent_active(22, 0)
  refresh()
end


function refresh()
  if (get_treasure(75) == 1) then
    set_mtile(31, 4, 65)
  end
  if (get_treasure(76) == 1) then
    set_mtile(39, 4, 65)
  end
  if (get_treasure(77) == 1) then
    set_mtile(31, 20, 65)
  end
  if (get_treasure(78) == 1) then
    set_mtile(21, 33, 65)
  end
  if (get_treasure(79) == 1) then
    set_mtile(23, 34, 65)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    if (get_progress(67) < 2) then
      combat(19)
    end

  elseif (zn == 1) then
    change_map("main", 225, 104, 225, 104)

  elseif (zn == 2) then
    view_range(1, 24, 0, 46, 25)
    warp(25, 13, 8)

  elseif (zn == 3) then
    view_range(1, 0, 0, 22, 25)
    warp(21, 13, 8)

  elseif (zn == 4) then
    view_range(1, 48, 0, 69, 24)
    warp(49, 12, 8)

  elseif (zn == 5) then
    view_range(1, 24, 0, 46, 25)
    warp(45, 13, 8)

  elseif (zn == 6) then
    view_range(1, 0, 28, 24, 45)
    warp(12, 32, 8)

  elseif (zn == 7) then
    view_range(1, 0, 0, 22, 25)
    warp(1, 13, 8)

  elseif (zn == 8) then
    view_range(1, 26, 31, 58, 45)
    warp(27, 41, 8)

  elseif (zn == 9) then
    view_range(1, 0, 28, 24, 45)
    warp(1, 40, 8)

  elseif (zn == 10) then
    view_range(1, 60, 31, 72, 48)
    warp(61, 35, 8)

  elseif (zn == 11) then
    if (get_progress(67) == 2) then
      set_ent_active(20, 1)
      set_ent_active(21, 1)
      set_ent_active(22, 1)
      place_ent(23, 49, 41)
      set_ent_facing(23, 3)
      set_ftile(49, 39, 0)
      set_zone(49, 39, 0)
      set_obs(49, 39, 0)
      set_btile(49, 40, 160)
    end
    view_range(1, 26, 31, 58, 45)
    warp(57, 41, 8)
    if (get_progress(67) == 2) then
      set_autoparty(1)
      if (get_numchrs() == 2) then
        set_ent_script(200, "L1")
        wait_for_entity(200, 200)
        set_ent_script(200, "L6")
        set_ent_script(201, "L6")
        wait_for_entity(200, 201)
      else
        set_ent_script(200, "L7")
        wait_for_entity(200, 200)
      end
      set_autoparty(0)
      orient_heroes()
      bubble(200, "Hey, you're free!")
      bubble(23, "Yes, some of my father's men were waiting outside. Once the doors became unsealed they figured things were safe.")
      bubble(23, "It seems that you've taken care of the Cult of the Shadow.")
      bubble(200, "Yeah, that avatar wasn't very strong. It was probably weak due to just having been summoned.")
      bubble(23, "Don't be modest, you did a great job.")
      bubble(23, "By the way, these men here mentioned that you came here because you wanted my Opal Band.")
      bubble(200, "Well, that was partly the reason we came, but once we found about this Cult and the Avatar, we had to act.")
      bubble(23, "That's okay... I believe you. I want you to have the Band... you've earned it.")
      bubble(200, "Are you sure?")
      bubble(23, "Yes.")
      drawmap()
      screen_dump()
      sfx(5)
      msg("Opal Band procured", 255, 0)
      set_progress(69, 1)
      drawmap()
      screen_dump()
      bubble(200, "Thank you very much!")
      bubble(23, "No problem, and good luck!")
      bubble(200, "See ya!")
      set_ent_script(20, "L1U1L3")
      wait_for_entity(20, 20)
      set_ent_script(21, "L3")
      wait_for_entity(21, 21)
      set_ent_script(23, "L3")
      wait_for_entity(23, 23)
      set_ent_script(22, "D1L1")
      wait_for_entity(22, 22)
      set_ent_script(20, "L8")
      set_ent_script(21, "L8")
      set_ent_script(23, "L8")
      set_ent_script(22, "L8")
      wait_for_entity(20, 23)
      set_ent_active(20, 0)
      set_ent_active(21, 0)
      set_ent_active(23, 0)
      set_ent_active(22, 0)
      set_progress(67, 3)
      set_progress(64, 3)
    end

  elseif (zn == 12) then
    LOC_open_door(8, 15)

  elseif (zn == 13) then
    LOC_open_door(14, 15)

  elseif (zn == 14) then
    LOC_open_door(11, 11)

  elseif (zn == 15) then
    LOC_open_door(35, 10)

  elseif (zn == 16) then
    LOC_open_door(35, 16)

  elseif (zn == 17) then
    LOC_open_door(6, 37)

  elseif (zn == 18) then
    if (get_progress(66) == 0) then
      bubble(200, "Locked tight.")
    else
      if (get_progress(66) == 1) then
        bubble(200, "Unlocked.")
        set_progress(66, 2)
      end
      LOC_open_door(12, 37)
    end

  elseif (zn == 19) then
    LOC_open_door(18, 37)

  elseif (zn == 20) then
    chest(75, 70, 1)
    refresh()

  elseif (zn == 21) then
    chest(76, 0, 900)
    refresh()

  elseif (zn == 22) then
    chest(77, 58, 1)
    refresh()

  elseif (zn == 23) then
    chest(78, 42, 1)
    refresh()

  elseif (zn == 24) then
    chest(79, 85, 1)
    refresh()

  elseif (zn == 25) then
    bubble(200, "Egad, these books are horrible!")

  elseif (zn == 26) then
    bubble(200, "These are just sick!")

  elseif (zn == 27) then
    bubble(200, "These books are just plain bad.")

  elseif (zn == 28) then
    bubble(200, "There is nothing here but common supplies.")

  elseif (zn == 29) then
    if (get_progress(67) == 0) then

      bubble(200, "Breanne... are you in here?")
      wait(50)
      bubble(23, "Huh?")
      bubble(200, "Breanne... are you in here?")
      wait(50)
      set_ent_script(23, "R2D4W50")
      wait_for_entity(23, 23)
      bubble(23, "Who are you?")
      bubble(200, "Your parents sent us to rescue you.")
      bubble(23, "You'll have to get me out later. Those creeps took some of my blood and they are going to do something evil with it.")
      bubble(23, "You've got to stop them!")
      bubble(200, "Well... I guess we should hurry then. We'll be back shortly to get you out.")
      bubble(23, "Thanks! Now hurry!")
      set_progress(67, 1)
      return
    elseif (get_progress(67) == 1) then
      bubble(23, "What are you waiting for?")
    end

  elseif (zn == 30) then
    bubble(5, "Hey! Who is that?")
    bubble(5, "You three take care of them!")
    wait(50)
    set_ent_script(17, "L1D1")
    set_ent_script(18, "D1L1")
    set_ent_script(19, "D1L2")
    wait_for_entity(17, 19)
    wait(50)
    set_run(0)
    combat(20)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_zone(63, 44, 31)
    set_zone(63, 45, 31)
    set_ent_active(17, 0)
    set_ent_active(18, 0)
    set_ent_active(19, 0)
    set_mtile(67, 39, 250)
    set_autoparty(1)
    if (get_ent_tiley(200) == 44) then
      set_ent_script(200, "R4U1")
    else
      set_ent_script(200, "R4U2")
    end
    if (get_numchrs() == 2) then
      if (get_ent_tiley(201) == 44) then
        set_ent_script(201, "R5")
      else
        set_ent_script(201, "R5U1")
      end
      wait_for_entity(200, 201)
    else
      wait_for_entity(200, 200)
    end
    set_autoparty(0)
    wait(50)
    bubble(200, "Uh oh!")
    LOC_avatar_abounds()
    drawmap()
    screen_dump()
    wait(50)
    bubble(200, "The time to attack is now!")
    drawmap()
    screen_dump()
    set_autoparty(1)
    set_ent_script(200, "U2")
    if (get_numchrs() == 2) then
      set_ent_script(201, "U2")
      wait_for_entity(200, 201)
    else
      wait_for_entity(200, 200)
    end
    set_autoparty(0)
    orient_heroes()
    set_run(0)
    combat(21)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_mtile(67, 38, 0)
    set_mtile(66, 39, 0)
    set_mtile(67, 39, 0)
    set_mtile(68, 39, 0)
    set_mtile(66, 40, 0)
    set_mtile(67, 40, 0)
    set_mtile(68, 40, 0)
    set_progress(67, 2)
    set_obs(11, 24, 0)
    set_zone(11, 24, 1)

  elseif (zn == 32) then
    bubble(200, "Hey! The door is sealed!")

  elseif (zn == 33) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(66) == 0) then
      bubble(0, "What th...")
      bubble(200, "Where's the girl?")
      bubble(0, "Do you really think that I'm going to tell you?")
      bubble(200, "That depends... do you want to live?")
      bubble(0, "Bah! Don't be deceived by the robes. I'm all muscle under here.")
      drawmap()
      screen_dump()
      set_run(0)
      combat(22)
      set_run(1)
      if (get_alldead() == 1) then
        return
      end
      drawmap()
      screen_dump()
      set_ent_active(0, 0)
      set_progress(66, 1)
      sfx(5)
      msg("Iron key procured", 255, 0)
    end

  end
end


function LOC_open_door(a, b)
  sfx(26)
  set_ftile(a, b, 175)
  set_obs(a, b, 0)
  set_zone(a, b, 0)
  set_btile(a, b+1, 176)
end


function LOC_avatar_abounds()
  local a, b, dx, dy
  local ax, ay, z

  create_df("data/mpcx.dat", "LFLAME_PCX")
  create_bmp(0, 24, 24)
  df2bmp(0, 0, 16, 0, 0, 24, 24)
  create_bmp(1, 64, 16)
  df2bmp(1, 0, 0, 0, 0, 64, 16)
  destroy_df()

  ax = get_vx()
  ay = get_vy()
  move_camera(920, 512, 1)

  for a=1, 23, 1 do
    for b=0, 3, 3 do
      drawmap()
      for dx=0, 6, 1 do
        for dy=1, 5, 1 do
          maskblit(1, b*16, 0, dx*16+104, dy*16+64, 16, 16)
        end
      end
      for dx=1, 5, 1 do
        maskblit(1, b*16, 0, dx*16+104, 64, 16, 16)
        maskblit(1, b*16, 0, dx*16+104, 160, 16, 16)
      end
      maskblit(0, 0, 0, 148, 128-a, 24, a)
      screen_dump()
      rest(40)
    end
  end
  destroy_bmp(0)
  destroy_bmp(1)

  for a=1, 19, 1 do
    set_ent_active(a, 0)
  end
  set_mtile(67, 38, 270)
  set_mtile(66, 39, 271)
  set_mtile(67, 39, 272)
  set_mtile(68, 39, 273)
  set_mtile(66, 40, 274)
  set_mtile(67, 40, 275)
  set_mtile(68, 40, 276)

  move_camera(ax, ay, 1)
end
