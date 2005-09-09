-- town7 - "Pulcannen"

function autoexec()

  if ((get_progress(P_TALKOLDMAN) == 0) and
      (get_progress(P_OPALHELMET) > 0) and
      (get_progress(P_OPALSHIELD) > 0) and
      (get_progress(P_OPALBAND) > 0) and
      (get_progress(P_OPALARMOUR) > 0)) then
    set_progress(P_TALKOLDMAN, 1)
  end
  refresh()
end


function refresh()
  if (get_treasure(72) == 1) then
    set_obsm("treas_1", 0)
    set_zonem("treas_1", 0)
  end

  if (get_progress(P_TALKOLDMAN) == 1) then
    set_obsm("door1", 0)
  elseif (get_progress(P_TALKOLDMAN) > 1) then
    place_ent(6, 103, 10)
    set_zone(104, 23, 0)
    set_zone(104, 26, 0)
    if (get_progress(P_TALKOLDMAN) == 5) then
      -- // Unlock the doors leading to the old man's counters
      set_obsm("hdoor1", 0)
      set_obsm("hdoor2", 0)
    end
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 293, 82, 293, 82)

  elseif (zn == 2) then
    door_in(74, 36, 69, 22, 97, 39)

  elseif (zn == 3) then
    door_in(83, 36, 69, 22, 97, 39)

  elseif (zn == 4) then
    door_in(92, 36, 69, 22, 97, 39)

  elseif (zn == 5) then
    door_in(75, 50, 69, 40, 81, 53)

  elseif (zn == 6) then
    door_in(89, 50, 83, 40, 95, 53)

  elseif (zn == 7) then
    door_out(32, 33)

  elseif (zn == 8) then
    door_out(37, 33)

  elseif (zn == 9) then
    door_out(42, 33)

  elseif (zn == 10) then
    door_out(31, 21)

  elseif (zn == 11) then
    door_out(49, 43)

  elseif (zn == 12) then
    if (get_progress(P_TALKOLDMAN) > 0) then
      door_in(104, 33, 99, 4, 111, 36)
      -- HERE
    else
      bubble(HERO1, "Locked.")
    end

  elseif (zn == 13) then
    shop(19)

  elseif (zn == 14) then
    shop(20)

  elseif (zn == 15) then
    shop(21)

  elseif (zn == 16) then
    inn("Pulcannen Inn", 200, 1)

  elseif (zn == 17) then
    view_range(1, 69, 4, 97, 20)
    warpm("dstairs1", 8)

  elseif (zn == 18) then
    view_range(1, 69, 4, 97, 20)
    warpm("dstairs2", 8)

  elseif (zn == 19) then
    view_range(1, 69, 22, 97, 39)
    warpm("ustairs1", 8)

  elseif (zn == 20) then
    view_range(1, 69, 22, 97, 39)
    warpm("ustairs2", 8)

  elseif (zn == 21) then
    chest(71, I_SALVE, 1)

  elseif (zn == 22) then
    chest(72, I_MPUP, 1)
    refresh()

  elseif (zn == 23) then
    book_talk(party[0])

  elseif (zn == 24) then
    touch_fire(party[0])

  elseif (zn == 25) then
    door_out(52, 26)

  elseif (zn == 26) then
    entity_handler(6)

  elseif (zn == 27) then
    bubble(HERO1, "This door appears locked.")

  elseif (zn == 28) then
    bubble(HERO1, "This door appears locked.")

  elseif (zn == 29) then
    bubble(HERO1, "Locked.")

  elseif (zn == 30) then
    bubble(HERO1, "This barrel is filled with rainwater.")

  elseif (zn == 31 or zn == 32) then
    bubble(HERO1, "The chest is locked.")

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "You'll find great things here.")

  elseif (en == 1) then
    bubble(en, "Things here are rather pricey.")

  elseif (en == 2) then
    bubble(en, "It's quite peaceful here.", "I like it.")

  elseif (en == 3) then
    bubble(en, "I love the sea.")

  elseif (en == 4) then
    bubble(en, "Nothing exciting ever happens here.")

  elseif (en == 5) then
    bubble(en, "This town does not get a lot of visitors.")

  elseif (en == 6) then
    if (get_progress(P_TALKOLDMAN) == 0) then
      bubble(en, "Hey, how did you get in here? Get out!")
    elseif (get_progress(P_TALKOLDMAN) == 1) then
      set_ent_speed(en, 5)
      set_ent_movemode(en, 2)
      -- Move the old man to the counter closest to you
      if (get_ent_tiley(HERO1) == 22) then
        -- Move old man to top (northern) counter
        move_entity(en, 104, 24, 0)
        wait_for_entity(en, en)
        set_ent_facing(en, FACE_UP)
      else
        -- Move old man to bottom (southern) counter
        move_entity(en, 104, 25, 0)
        wait_for_entity(en, en)
        set_ent_facing(en, FACE_DOWN)
      end
      bubble(en, "Wow! You have all the Opal items! Quick, come up here to my study and let me see them!")
      -- Unlock doors that lead behind his counters
      set_obsm("hdoor1", 0)
      set_obsm("hdoor2", 0)
      set_ent_speed(en, 5)

      move_entity(en, 103, 10, 0)
      wait_for_entity(en, en)
      set_ent_facing(en, FACE_DOWN)
      set_ent_movemode(en, 0)

      set_progress(P_TALKOLDMAN, 2)
      refresh()
    elseif (get_progress(P_TALKOLDMAN) == 2) then
      bubble(en, "I see you have everything! Here, take this key.")
      sfx(5)
      msg("Rusty key procured", 63, 0)
      bubble(en, "This key will open the temple found in the Grotto around Ekla.")
      if (get_numchrs() == 1) then
        bubble(HERO1, "The Grotto by Ekla? What will I find there?")
      else
        bubble(HERO1, "The Grotto by Ekla? What will we find there?")
      end
      bubble(en, "It's an underwater tunnel that leads from the grotto to the island Malkaron is staying.")
      bubble(HERO1, "Oh. Thanks!")
      set_progress(P_TALKOLDMAN, 3)
    elseif (get_progress(P_TALKOLDMAN) == 3) then
      bubble(en, "I hope the key still fits in the temple down in the Grotto by Ekla. It's a bit rusty.")
    elseif (get_progress(P_TALKOLDMAN) == 4) then
      bubble(en, "I see the key worked. Excellent.")
      set_progress(P_TALKOLDMAN, 5)
    else
      bubble(en, "Good luck on your journey.")
    end
  end
end
