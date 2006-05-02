-- cult - "Cult of the Shadow Walker base on island west of Sunarin"

function autoexec()
  local a

  view_range(1, 6, 1, 30, 30)
  if (get_progress(P_AVATARDEAD) < 2) then
    set_obs("front_door", 1)
    set_zone("front_door", 32)
  else
    set_zone("avatar", 31)
    set_obs("front_door", 0)
    for a = 0, 19, 1 do
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
    set_mtile(39, 7, 65)
  end
  if (get_treasure(76) == 1) then
    set_mtile(47, 7, 65)
  end
  if (get_treasure(77) == 1) then
    set_mtile(39, 24, 65)
  end
  if (get_treasure(78) == 1) then
    set_mtile(28, 37, 65)
  end
  if (get_treasure(79) == 1) then
    set_mtile(30, 38, 65)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    if (get_progress(P_AVATARDEAD) < 2) then
      combat(19)
    end

  elseif (zn == 1) then
    change_map("main", "cult")

  elseif (zn == 2) then
    view_range(1, 31, 1, 55, 29)
    warp("dstairs2", 8)

  elseif (zn == 3) then
    view_range(1, 6, 1, 30, 30)
    warp("ustairs1", 8)

  elseif (zn == 4) then
    view_range(1, 56, 1, 79, 30)
    warp("dstairs3", 8)

  elseif (zn == 5) then
    view_range(1, 31, 1, 55, 29)
    warp("ustairs2", 8)

  elseif (zn == 6) then
    view_range(1, 6, 31, 32, 50)
    warp("ustairs4", 8)

  elseif (zn == 7) then
    view_range(1, 6, 1, 30, 30)
    warp("dstairs1", 8)

  elseif (zn == 8) then
    view_range(1, 32, 34, 66, 50)
    warp("ustairs5", 8)

  elseif (zn == 9) then
    view_range(1, 6, 31, 32, 50)
    warp("dstairs4", 8)

  elseif (zn == 10) then
    view_range(1, 66, 34, 80, 53)
    warp("ustairs6", 8)

  elseif (zn == 11) then
    if (get_progress(P_AVATARDEAD) == 2) then
      set_ent_active(20, 1)
      set_ent_active(21, 1)
      set_ent_active(22, 1)
      place_ent(23, 56, 45)
      set_ent_facing(23, FACE_RIGHT)
      set_ftile(56, 43, 0)
      set_zone(56, 43, 0)
      set_obs(56, 43, 0)
      set_btile(56, 43, 160)
    end
    view_range(1, 32, 34, 66, 50)
    warp("dstairs5", 8)
    if (get_progress(P_AVATARDEAD) == 2) then
      set_autoparty(1)
      if (get_numchrs() == 2) then
        set_ent_script(HERO1, "L1")
        wait_for_entity(HERO1, HERO1)
        set_ent_script(HERO1, "L6")
        set_ent_script(HERO2, "L6")
        wait_for_entity(HERO1, HERO2)
      else
        set_ent_script(HERO1, "L7")
        wait_for_entity(HERO1, HERO1)
      end
      set_autoparty(0)
      orient_heroes()
      bubble(HERO1, "Hey, you're free!")
      bubble(23, "Yes, some of my father's men were waiting outside. Once the doors became unsealed they figured things were safe.")
      bubble(23, "It seems that you've taken care of the Cult of the Shadow.")
      bubble(HERO1, "Yeah, that avatar wasn't very strong. It was probably weak due to just having been summoned.")
      bubble(23, "Don't be modest, you did a great job.")
      bubble(23, "By the way, these men here mentioned that you came here because you wanted my Opal Band.")
      bubble(HERO1, "Well, that was partly the reason we came, but once we found about this Cult and the Avatar, we had to act.")
      bubble(23, "That's okay... I believe you. I want you to have the Band... you've earned it.")
      bubble(HERO1, "Are you sure?")
      bubble(23, "Yes.")
      drawmap()
      screen_dump()
      sfx(5)
      msg("Opal Band procured", 255, 0)
      set_progress(P_OPALBAND, 1)
      drawmap()
      screen_dump()
      bubble(HERO1, "Thank you very much!")
      bubble(23, "No problem, and good luck!")
      bubble(HERO1, "See ya!")
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
      set_progress(P_AVATARDEAD, 3)
      set_progress(P_SAVEBREANNE, 3)
    end

  elseif (zn == 12) then
    LOC_open_door(15, 20)

  elseif (zn == 13) then
    LOC_open_door(21, 20)

  elseif (zn == 14) then
    LOC_open_door(18, 13)

  elseif (zn == 15) then
    LOC_open_door(43, 13)

  elseif (zn == 16) then
    LOC_open_door(43, 20)

  elseif (zn == 17) then
    LOC_open_door(13, 41)

  elseif (zn == 18) then
    if (get_progress(P_IRONKEY) == 0) then
      bubble(HERO1, "Locked tight.")
    else
      if (get_progress(P_IRONKEY) == 1) then
        bubble(HERO1, "Unlocked.")
        set_progress(P_IRONKEY, 2)
      end
      LOC_open_door(19, 41)
    end

  elseif (zn == 19) then
    LOC_open_door(25, 41)

  elseif (zn == 20) then
    chest(75, I_ARMOR6, 1)
    refresh()

  elseif (zn == 21) then
    chest(76, 0, 900)
    refresh()

  elseif (zn == 22) then
    chest(77, I_ROBE4, 1)
    refresh()

  elseif (zn == 23) then
    chest(78, I_SHIELD5, 1)
    refresh()

  elseif (zn == 24) then
    chest(79, I_AGRAN, 1)
    refresh()

  elseif (zn == 25) then
    bubble(HERO1, "Egad, these books are horrible!")

  elseif (zn == 26) then
    bubble(HERO1, "These are just sick!")

  elseif (zn == 27) then
    bubble(HERO1, "These books are just plain bad.")

  elseif (zn == 28) then
    bubble(HERO1, "There is nothing here but common supplies.")

  elseif (zn == 29) then
    if (get_progress(P_AVATARDEAD) == 0) then

      bubble(HERO1, "Breanne... are you in here?")
      wait(50)
      bubble(23, "Huh?")
      bubble(HERO1, "Breanne... are you in here?")
      wait(50)
      set_ent_script(23, "R2D4W50")
      wait_for_entity(23, 23)
      bubble(23, "Who are you?")
      bubble(HERO1, "Your parents sent us to rescue you.")
      bubble(23, "You'll have to get me out later. Those creeps took some of my blood and they are going to do something evil with it.")
      bubble(23, "You've got to stop them!")
      bubble(HERO1, "Well... I guess we should hurry then. We'll be back shortly to get you out.")
      bubble(23, "Thanks! Now hurry!")
      set_progress(P_AVATARDEAD, 1)
      return
    elseif (get_progress(P_AVATARDEAD) == 1) then
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
    set_zone(70, 48, 31)
    set_zone(70, 49, 31)
    set_ent_active(17, 0)
    set_ent_active(18, 0)
    set_ent_active(19, 0)
    set_mtile(74, 43, 250)
    set_autoparty(1)
    set_ent_script(HERO1, "R4U1")
    if (get_numchrs() == 2) then
      set_ent_script(HERO2, "R5")
      wait_for_entity(HERO1, HERO2)
    else
      wait_for_entity(HERO1, HERO1)
    end
    set_autoparty(0)
    wait(50)
    bubble(HERO1, "Uh oh!")
    LOC_avatar_abounds()
    drawmap()
    screen_dump()
    wait(50)
    bubble(HERO1, "The time to attack is now!")
    drawmap()
    screen_dump()
    set_autoparty(1)
    set_ent_script(HERO1, "U2")
    if (get_numchrs() == 2) then
      set_ent_script(HERO2, "U2")
      wait_for_entity(HERO1, HERO2)
    else
      wait_for_entity(HERO1, HERO1)
    end
    set_autoparty(0)
    orient_heroes()
    set_run(0)
    combat(21)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_mtile(74, 42, 0)
    set_mtile(73, 43, 0)
    set_mtile(74, 43, 0)
    set_mtile(75, 43, 0)
    set_mtile(73, 44, 0)
    set_mtile(74, 44, 0)
    set_mtile(75, 44, 0)
    set_progress(P_AVATARDEAD, 2)
    set_obs("front_door", 0)
    set_zone("front_door", 1)

  elseif (zn == 32) then
    bubble(HERO1, "Hey! The door is sealed!")

  elseif (zn == 33) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_IRONKEY) == 0) then
      bubble(0, "What th...")
      bubble(HERO1, "Where's the girl?")
      bubble(0, "Do you really think that I'm going to tell you?")
      bubble(HERO1, "That depends... do you want to live?")
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
      set_progress(P_IRONKEY, 1)
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
  set_btile(a, b + 1, 176)
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
  move_camera(1032, 576, 1)

  for a = 1, 23, 1 do
    for b = 0, 3, 3 do
      drawmap()
      for dx = 0, 6, 1 do
        for dy = 1, 5, 1 do
          maskblit(1, b * 16, 0, dx * 16 + 104, dy * 16 + 64, 16, 16)
        end
      end
      for dx = 1, 5, 1 do
        maskblit(1, b * 16, 0, dx * 16 + 104, 64, 16, 16)
        maskblit(1, b * 16, 0, dx * 16 + 104, 160, 16, 16)
      end
      maskblit(0, 0, 0, 148, 128 - a, 24, a)
      screen_dump()
      rest(40)
    end
  end
  destroy_bmp(0)
  destroy_bmp(1)

  for a = 1, 19, 1 do
    set_ent_active(a, 0)
  end
  set_mtile(74, 42, 270)
  set_mtile(73, 43, 271)
  set_mtile(74, 43, 272)
  set_mtile(75, 43, 273)
  set_mtile(73, 44, 274)
  set_mtile(74, 44, 275)
  set_mtile(75, 44, 276)

  move_camera(ax, ay, 1)
end
