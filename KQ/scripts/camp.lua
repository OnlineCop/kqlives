-- camp - "Orc forest encampment south of Randen"

-- /*
-- {
-- P_FOUNDMAYOR:
--   (0) Mayor is still behind bars
--   (1) Mayor has been freed and is now home
-- P_MAYORGUARD1:
--   (0) The first of the mayor's guards is still being held
--   (1) The first guard was released (and spoken to)
-- P_MAYORGUARD2:
--   (0) The second of the mayor's guards is still being held
--   (1) The second guard was released (and spoken to)
-- }
-- */


function autoexec()
  local a

  if (get_progress(P_FOUNDMAYOR) == 0) then
    set_ent_id(37, CASANDRA)
  else
    set_ent_active(37, 0)
    for a = 0, 4, 1 do
      set_ent_active(a, 0)
    end
    for a = 25, 28, 1 do
      set_ent_active(a, 0)
    end
  end

  if (get_progress(P_MAYORGUARD1) > 0) then
    set_ent_active(5, 0)
  end

  if (get_progress(P_MAYORGUARD2) > 0) then
    set_ent_active(6, 0)
  end

  refresh()
end


function refresh()
  if (get_treasure(55) == 1) then
    set_mtile(66, 45, 265)
    set_zone(66, 45, 0)
  end

  if (get_treasure(56) == 1) then
    set_mtile(73, 43, 265)
    set_zone(73, 43, 0)
  end

  if (get_treasure(57) == 1) then
    set_mtile(81, 24, 265)
    set_zone(81, 24, 0)
  end

  if (get_treasure(58) == 1) then
    set_mtile(83, 11, 265)
    set_zone(83, 11, 0)
  end

  if (get_treasure(59) == 1) then
    set_mtile(84, 11, 265)
    set_zone(84, 11, 0)
  end

  if (get_treasure(60) == 1) then
    set_mtile(13, 57, 265)
    set_zone(13, 57, 0)
  end

  if (get_treasure(61) == 1) then
    set_mtile(14, 57, 265)
    set_zone(14, 57, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "camp_n")

  elseif (zn == 2) then
    change_map("main", "camp_s")

  elseif (zn == 3) then
    set_ent_script(7, "L1U2")
    set_ent_script(8, "L1U3")
    wait_for_entity(7, 8)
    bubble(7, "You're not welcome here!")
    drawmap()
    screen_dump()
    set_run(0)
    combat(2)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(7, 0)
    set_ent_active(8, 0)
    set_zone(23, 24, 0)
    set_zone(24, 24, 0)

  elseif (zn == 4) then
    set_ent_script(12, "U1L2U1")
    set_ent_script(13, "L2U3")
    set_ent_script(14, "U2L2U2")
    set_ent_script(15, "R1U6L2F1")
    wait_for_entity(12, 15)
    set_run(0)
    combat(4)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(12, 0)
    set_ent_active(13, 0)
    set_ent_active(14, 0)
    set_ent_active(15, 0)
    set_zone(20, 43, 0)
    set_zone(21, 43, 0)

  elseif (zn == 5) then
    set_ent_script(16, "R1D4L3D3")
    set_ent_script(17, "L8U3")
    if (get_numchrs() == 2 and get_ent_tilex(HERO2) == 32 and get_ent_tiley(HERO2) == 49) then
      set_ent_script(18, "U1L8U2")
    else
      set_ent_script(18, "U1L8U3")
    end
    wait_for_entity(16, 18)

    set_run(0)
    combat(3)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(16, 0)
    set_ent_active(17, 0)
    set_ent_active(18, 0)
    set_zone(32, 48, 0)
    set_zone(34, 52, 0)
    set_zone(34, 53, 0)

  elseif (zn == 6) then
    set_ent_script(19, "R1D3")
    set_ent_script(20, "D1R1D1")
    set_ent_script(21, "D6R2F0")
    set_ent_script(22, "D4R1D2")
    wait_for_entity(19, 22)
    set_run(0)
    combat(4)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(19, 0)
    set_ent_active(20, 0)
    set_ent_active(21, 0)
    set_ent_active(22, 0)
    set_zone(36, 28, 0)
    set_zone(37, 28, 0)

  elseif (zn == 7) then
    set_ent_speed(16, 6)
    set_ent_script(16, "R1D4L3D7R1F0")
    set_ent_script(17, "L2D1L2")
    set_ent_script(18, "U1L5")
    wait_for_entity(16, 18)

    set_run(0)
    combat(3)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(16, 0)
    set_ent_active(17, 0)
    set_ent_active(18, 0)
    set_zone(32, 48, 0)
    set_zone(34, 52, 0)
    set_zone(34, 53, 0)

  elseif (zn == 8) then
    chest(55, I_NLEAF, 2)
    refresh()

  elseif (zn == 9) then
    chest(56, 0, 250)
    refresh()

  elseif (zn == 10) then
    chest(57, I_RRUNE, 1)
    refresh()

  elseif (zn == 11) then
    chest(58, I_SALVE, 2)
    refresh()

  elseif (zn == 12) then
    chest(59, I_EDROPS, 1)
    refresh()

  elseif (zn == 13) then
    chest(60, I_SPEAR1, 1)
    refresh()

  elseif (zn == 14) then
    chest(61, I_ARMOR2, 1)
    refresh()

  elseif (zn == 15) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  if (en >= 0) and (en <= 4) then
    bubble(en, "It sure it clammy in here...")

  elseif (en == 5) then
    if (get_progress(P_MAYORGUARD1) == 0) then
      bubble(en, "Wow, thanks for helping me! When we get back to my place, feel free to stop by sometime!")
      set_progress(P_MAYORGUARD1, 1)
    else
      bubble(en, "My place is right next to the mayor's.")
    end

  elseif (en == 6) then
    if (get_progress(P_MAYORGUARD2) == 0) then
      bubble(en, "Thanks for your help! I should be able to find my way out.")
      set_progress(P_MAYORGUARD2, 1)
    else
      bubble(en, "Don't worry about me. I'm still looking for the exit...")
    end

  elseif (en == 9) then
    if (get_numchrs() > 1) then
      bubble(en, "Intruders!")
    else
      bubble(en, "Intruder!")
    end

    set_ent_script(10, "D1R6U1")
    set_ent_script(11, "D1R3U2")
    wait_for_entity(10, 11)
    drawmap()
    set_run(0)
    combat(3)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(9, 0)
    set_ent_active(10, 0)
    set_ent_active(11, 0)

  elseif (en == 23 or en == 24 or (en >= 29 and en <= 36) or en == 38) then
    if (get_numchrs() > 1) then
      bubble(en, "Intruders!")
    else
      bubble(en, "Intruder!")
    end
    drawmap()

    set_run(0)
    combat(1)
    set_run(1)

    if (get_alldead() == 1) then
      return
    end

    set_ent_active(en, 0)

  elseif (en == 28) then
    bubble(en, "You have to get through us to get to them.")
    drawmap()
    set_run(0)
    combat(5)
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(28, 0)
    set_ent_active(25, 0)
    set_ent_active(26, 0)
    set_ent_active(27, 0)

  elseif (en == 37) then
    local a

    bubble(en, "Hey, $0! Am I ever glad to see you! We're in a bit of a jam here.")
    bubble(en, "I took a small job to help guard the mayor on his trip to Andra.")
    bubble(en, "We were on the Brayden bridge when we were ambushed by these orcs. During the fight, one of them used a fire spell.")
    bubble(en, "The bridge was engulfed in flames and we were trapped.")
    bubble(HERO1, "But how did you end up here?")
    bubble(en, "Well, we had no choice but to go back the way we came, and we ran into the whole tribe.")
    bubble(en, "We were horribly out-numbered and forced to surrender. They brought us here and then most of them left for somewhere else.")
    bubble(HERO1, "Hmm... this almost sounds as though the whole thing was planned.")
    bubble(en, "Hmm...")
    bubble(HERO1, "Excuse me, Mr. Mayor.")
    bubble(2, "Yes?")
    bubble(HERO1, "Do you think that this could have possibly been a planned attack with the purpose of trying to kidnap you?")
    bubble(2, "I don't see why they would want to kidnap me, but uh... could we possibly get out of here now?")
    set_ent_facing(en, FACE_UP)
    bubble(en, "Yes sir!")
    set_ent_facing(en, FACE_DOWN)
    bubble(en, "I'll go back to Randen with the mayor. Why don't you meet me there after?")
    bubble(HERO1, "Sure.")

    set_ftile(41, 14, 0)
    set_obs(41, 14, 0)
    set_autoparty(1)

    set_ent_script(HERO1, "D2R1F2")
    if (get_numchrs() == 2) then
      if (get_ent_tilex(HERO2) == get_ent_tilex(HERO1) + 1) then
        set_ent_script(HERO2, "L1D3R1F2")
        wait_for_entity(HERO1, HERO2)
      elseif (get_ent_tilex(HERO2) == get_ent_tilex(HERO1) - 1) then
        set_ent_script(HERO2, "R1D3R1F2")
        wait_for_entity(HERO1, HERO2)
      else
        set_ent_script(HERO2, "D2R1F2")
        wait_for_entity(HERO1, HERO2)
      end
    else
      wait_for_entity(HERO1, HERO1)
    end

    set_autoparty(0)
    orient_heroes()

    -- TT: This keeps the dead brigands from trying to move too
    for a = 5, 36, 1 do
      set_ent_movemode(a, 0)
    end

    set_ent_script(37, "D5")
    set_ent_script(4, "L1D4")
    set_ent_script(3, "D1L1D3")
    set_ent_script(2, "R1D1L1D2")
    set_ent_script(1, "R2D1L1D1")
    set_ent_script(0, "R3D1L1F0")

    wait_for_entity(0, 37)

    set_ent_script(37, "L4D10K")
    set_ent_script(4, "D1L4D10K")
    set_ent_script(3, "D2L4D10K")
    set_ent_script(2, "D3L4D10K")
    set_ent_script(1, "D4L4D10K")
    set_ent_script(0, "D5L4D10K")

    wait_for_entity(0, 37)

    set_ent_active(en, 0)

    for a = 0, 4, 1 do
      set_ent_active(a, 0)
    end

    set_progress(P_FOUNDMAYOR, 1)
    set_progress(P_PARTNER1, 1 + get_pidx(0))
    if (get_numchrs() > 1) then
      set_progress(P_PARTNER2, 1 + get_pidx(1))
    end

  end
end
