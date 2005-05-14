-- town2 - "Randen"

-- /*
-- {
-- P_BLADE: Whether woman mentioned the Phoenix Blade
--   0 - You have not spoken to her
--   1 - She has now
--
-- P_FIGHTONBRIDGE:
--   0 - Have not visited the bridge, not fought
--   1..3 - [Not calculated]: see bridge[2].lua for more
--   4 - Defeated monster on bridge, have not slept at Inn
--   5 - Slept at inn; bridge is passable
--
-- P_FOUNDMAYOR:
--   0 - Have not found the mayor
--   1 - Found the mayor, he does not want company
--   2 - Found mayor & used WARPSTONE to return; he awaits your company
--   3 - You have spoken to the mayor
--
-- P_MAYORGUARD1:
--   0 - The first of the mayor's guards is still being held in the orc camp
--   1 - The first guard was released: gives you money
--   2 - The first guard already gave you money for releasing him
--
-- P_MAYORGUARD2:
--   0 - The second of the mayor's guards is still being held in the orc camp
--   1 - The second guard was released
--
-- P_PORTALGONE: Whether the portal in the tunnel is still working
--   0 - Still letting monsters through
--   1 - The Portal is sealed shut
--
-- P_SHOWBRIDGE:
--   0 - Bridge is incomplete
--   1 - Monsters on bridge defeated; slept at Inn: bridge is passable
--   2 - Bridge is totally complete, bridge(2).(lua|map) no longer used at all
--
-- P_TALK_AJATHAR:
--   0 - Have not spoken to Ajathar when you entered the map
--   1 - You have spoken to him at least once already
--
-- P_WARPSTONE: The teleporter from Ajantara <-> Randen
--   0 - Have not used it yet
--   1 - Stepped on the warp stone and created its counterpart in Randen
-- }
-- */


function autoexec()
  if (not LOC_manor_or_party(AJATHAR)) then
    -- // Make the NPC look like Ajathar if he hasn't been recruited yet
    set_ent_id(10, AJATHAR)
  else
    -- // Otherwise, remove him from screen
    set_ent_active(10, 0)
  end

  -- // WARPSTONE is found late in the game, so some things are now available
  -- // that weren't available earlier
  if (get_progress(P_WARPSTONE) == 1) then
    -- // Move the guard guarding the houses in the north-east section of town
    -- // over one tile so we can get in
    set_ent_tilex(6, 57)

    -- // The bridge repairs will now be completed
    if (get_progress(P_SHOWBRIDGE) == 1) then
      set_progress(P_SHOWBRIDGE, 2)
    end

    -- // If the Mayor had been found, he will now have recovered from his
    -- // ordeal and will be willing to speak to you
    if (get_progress(P_FOUNDMAYOR) == 1) then
      set_progress(P_FOUNDMAYOR, 2)
    end
  end

  if (get_progress(P_FOUNDMAYOR) > 0 and
      not LOC_manor_or_party(CASANDRA)) then
    -- // Casandra should be available to join your party
    set_ent_id(13, CASANDRA)
  else
    set_ent_active(13, 0)
  end

  -- // This NPC will only appear if you spoke with him in the camp
  if (get_progress(P_MAYORGUARD1) == 0) then
    set_ent_active(12, 0)
  end

  -- // Ditto, plus make sure you can't speak to a "ghost" over the counter
  if (get_progress(P_MAYORGUARD2) == 0) then
    set_ent_active(11, 0)
    set_zone(134, 11, 0)
  end

  refresh()
end


function refresh()
  if (get_treasure(3) == 1) then
    set_mtile(110, 19, 265)
    set_zone(110, 19, 0)
  end
  if (get_treasure(4) == 1) then
    set_mtile(111, 19, 265)
    set_zone(111, 19, 0)
  end
  if (get_treasure(5) == 1) then
    set_mtile(112, 19, 265)
    set_zone(112, 19, 0)
  end
  if (get_treasure(7) == 1) then
    set_obs(14, 47, 0)
    set_zone(14, 47, 0)
  end
  if (get_treasure(10) == 1) then
    set_obs(35, 38, 0)
    set_zone(35, 38, 0)
  end
  if (get_treasure(31) == 1) then
    set_mtile(115, 38, 265)
    set_zone(115, 38, 0)
  end
  if (get_treasure(46) == 1) then
    set_mtile(116, 38, 265)
    set_zone(116, 38, 0)
  end
  if (get_treasure(97) == 1) then
    set_mtile(135, 10, 265)
    set_zone(135, 10, 0)
  end

  if (get_progress(P_WARPSTONE) == 1) then
    set_mtile(35, 15, 0)
  else
    set_zone(35, 15, 0)
  end

end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("cave1", 47, 47, 47, 47)

  elseif (zn == 2) then
    door_in(85, 13, 80, 6, 90, 16)

  elseif (zn == 3) then
    door_in(86, 26, 80, 17, 92, 29)

  elseif (zn == 4) then
    door_in(102, 13, 93, 6, 105, 16)

  elseif (zn == 5) then
    door_in(97, 25, 94, 17, 107, 28)

  elseif (zn == 6) then
    door_in(96, 39, 80, 30, 102, 51)

  elseif (zn == 7) then
    door_in(112, 13, 107, 6, 117, 16)

  elseif (zn == 8) then
    door_in(114, 43, 104, 30, 117, 45)

  elseif (zn == 9) then
    bubble(HERO1, "Locked.")

  elseif (zn == 10) then
    door_out(20, 19)

  elseif (zn == 11) then
    door_out(28, 20)

  elseif (zn == 12) then
    door_out(24, 31)

  elseif (zn == 13) then
    door_out(63, 29)

  elseif (zn == 14) then
    door_out(18, 42)

  elseif (zn == 15) then
    door_out(49, 37)

  elseif (zn == 16) then
    door_out(66, 39)

  elseif (zn == 17) then
    -- /* PH added code to check if you do stay over night.
    --    This is done indirectly; if your GP goes down it
    --    means you must have slept here. */
    local old_gp = get_gp()
    inn("Wayside Inn", 30, 1)
    -- // This means you MUST stay at the inn before the bridge gets repaired.
    if (get_gp() < old_gp) then
      if (get_progress(P_FIGHTONBRIDGE) == 4) then
        set_progress(P_FIGHTONBRIDGE, 5)
        set_progress(P_SHOWBRIDGE, 1)
      end
    end

  elseif (zn == 18) then
    shop(4)

  elseif (zn == 19) then
    shop(5)

  elseif (zn == 20) then
    view_range(1, 119, 6, 122, 29)
    warp(121, 9, 8)

  elseif (zn == 21) then
    view_range(1, 109, 17, 117, 25)
    warp(116, 20, 8)

  elseif (zn == 22) then
    view_range(1, 94, 17, 107, 28)
    warp(106, 21, 8)

  elseif (zn == 23) then
    view_range(1, 119, 6, 122, 29)
    warp(121, 27, 8)

  elseif (zn == 24) then
    chest(3, I_KNIFE2, 1)
    refresh()

  elseif (zn == 25) then
    chest(4, 0, 250)
    refresh()

  elseif (zn == 26) then
    chest(5, I_SALVE, 1)
    refresh()

  elseif (zn == 27) then
    chest(7, I_PURITYGEM, 1)
    refresh()

  elseif (zn == 28) then
    change_map("main", 222, 19, 222, 19)

  elseif (zn == 29) then
    chest(10, I_MHERB, 1)
    refresh()

  elseif (zn == 30) then
    book_talk(party[0])

  elseif (zn == 31) then
    chest(11, I_LTONIC, 1)
    refresh()

  elseif (zn == 32) then
    book_talk(party[0])

  elseif (zn == 33) then
    bubble(HERO1, "Hmmm... books about herbs.")

  elseif (zn == 34) then
    if (get_progress(P_FOUNDMAYOR) > 1) then
      door_in(129, 30, 124, 20, 140, 33)
    else
      bubble(HERO1, "Locked.")
    end

  elseif (zn == 35) then
    bubble(HERO1, "Inns always have boring books.")

  elseif (zn == 36) then
    change_map("town6", 38, 56, 38, 56)

  elseif (zn == 37) then
    touch_fire(party[0])

  elseif (zn == 38) then
    view_range(1, 104, 30, 117, 45)
    warp(116, 34, 8)

  elseif (zn == 39) then
    view_range(1, 109, 17, 117, 25)
    warp(113, 23, 8)

  elseif (zn == 40) then
    chest(31, 0, 150)
    refresh()

  elseif (zn == 41) then
    chest(46, I_STRSEED, 2)
    refresh()

  elseif (zn == 42) then
    chest(96, 0, 250)
    refresh()

  elseif (zn == 43) then
    if (get_progress(MAYORGUARD2) > 0) then
      bubble(11, "Thank you for rescuing me!")
    end

  elseif (zn == 44) then
    set_btile(139, 8, 0)
    set_ftile(139, 8, 518)
    set_mtile(139, 9, 519)
    set_obs(139, 8, 0)
    set_zone(139, 8, 0)
    sfx(25)

  elseif (zn == 45) then
    set_btile(134, 8, 0)
    set_ftile(134, 8, 518)
    set_mtile(134, 9, 519)
    set_obs(134, 8, 0)
    set_zone(134, 8, 0)
    sfx(25)

  elseif (zn == 46) then
    chest(97, 0, 1500)
    refresh()

  elseif (zn == 47) then
    door_in(129, 16, 124, 6, 140, 19)

  elseif (zn == 48) then
    door_out(48, 18)

  elseif (zn == 49) then
    door_in(137, 16, 124, 6, 140, 19)

  elseif (zn == 50) then
    door_out(52, 18)

  elseif (zn == 51) then
    door_out(61, 18)

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "Back again, are you?")
    else
      bubble(en, "You must be an adventurer. There has been an unusual number of you people around lately.")
    end
    if (get_progress(P_FOUNDMAYOR) > 0) then
      bubble(en, "Oh, and thank you for finding the mayor.")
    end

  elseif (en == 1) then
    if (get_progress(P_FOUNDMAYOR) == 0) then
      bubble(en, "My husband is late again.")
    elseif (get_progress(P_FOUNDMAYOR) == 1) then
      bubble(en, "I'm getting so worried...")
      bubble(en, "You don't think Malkaron has something to do with it?")
    elseif (get_progress(P_FOUNDMAYOR) == 2) then
      bubble(en, "Where is that man?")
    else
      bubble(en, "Oh... that bad man, making me worry like this, I'm gonna kill him!")
    end

  elseif (en == 2) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      if (get_progress(P_SHOWBRIDGE) == 2) then
        bubble(en, "Thank you so much for restoring the trade route to the citizens of Randen!")
      else
        bubble(en, "Welcome to Randen.")
      end
    else
      bubble(en, "This is the town of Randen. We're mostly a coastal trade town, but with the bridge out, trade is extremely slow.")
      bubble(en, "Do you think it has something to do with Malkaron? We've heard a lot of rumors about him as of late...")
    end

  elseif (en == 3) then
    if (get_progress(P_FOUNDMAYOR) > 1 and get_progress(P_MAYORGUARD1) > 0 and get_progress(P_MAYORGUARD2) > 0) then
      bubble(en, "My husband gets so forgetful. Sometimes he doesn't lock up after himself.")
      set_obs(113, 23, 0)
      set_zone(113, 23, 0)
    elseif (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(en, "Good day.")
    else
      if (get_progress(P_BLADE) == 0)  then
        -- // PH: Just my little joke hehe
        bubble(en, "I'm just preparing some vegetables.")
        bubble(HERO1, "That's a strange knife you've got there.")
        bubble(en, "What? Oh, this. Yes, it's a Phoenix Blade.")
        bubble(en, "I found it lying about under a pile of leaves in the forest.")
        bubble(HERO1, "Isn't there supposed to be someone out looking for that?")
        bubble(en, "I heard that, too, but it's been ages and no-one has turned up.")
        bubble(en, "I would give it back, but I need it for slicing carrots!")
        bubble(HERO1, "Your secret's safe with me.")
        set_progress(P_BLADE, 1)
      else
        bubble(en, "Oh, goodness, I didn't make enough for company... sorry.")
      end
    end

  elseif (en == 4) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      if (get_progress(P_SHOWBRIDGE) == 2) then
        bubble(en, "Wow! Mom says you're the ones who beat up the bad guys! You're so cool!")
      else
        bubble(en, "Hi!")
      end
    else
      bubble(en, "I'm hungry, I hope mom is done making lunch soon.")
    end

  elseif (en == 5) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      if (get_progress(P_WARPSTONE) == 1) then
        bubble(en, "Business is good.")
      else
        bubble(en, "Now that the bridge is repaired I'm back in business!")
      end

      if (get_treasure(9) == 0) then
        bubble(en, "I hear you had something to do with speeding along the bridge's construction. I'd like you to have this.")
        chest(9, I_B_SHOCK, 1)
      end

      if (get_progress(P_SHOWBRIDGE) < 2) then
        bubble(en, "You should look around there to see if the workers left anything behind.")
      else
        if (get_treasure(8) == 0) then
          bubble(en, "I found this Olginar Seed on the bridge. You can have it.")
          chest(8, I_OSEED, 1)
        end
        if (get_treasure(90) == 0) then
          bubble(en, "Oh, and I found this curious artifact there as well...")
          chest(90, I_REGENERATOR, 1)
        end
      end
    else
      bubble(en, "If you haven't already noticed, the bridge across Brayden river is gone.")
      bubble(en, "It appears that some travelers were stopped by Malkaron's brigands on the bridge, resulting in a volley of magic that left it in cinders.")
      set_ent_script(en, "W25")
      wait_for_entity(en)
      bubble(en, "A new bridge is supposed to be built soon. This town can't survive for long without our major trade route.")
    end

  elseif (en == 6) then
    if (get_progress(P_WARPSTONE) == 1) then
      if (get_progress(P_FOUNDMAYOR) < 2) then
        bubble(en, "The mayor isn't seeing any visitors at present.")
      elseif (get_progress(P_FOUNDMAYOR) == 2) then
        bubble(en, "$0! The mayor is back now, thank you! He wants to see you right away!")
      else
        bubble(en, "Thank you again for everything!")
      end
    else
      if (get_progress(P_FOUNDMAYOR) == 0) then
        if (get_progress(P_FIGHTONBRIDGE) > 4) then
          bubble(en, "The mayor has yet to return from Andra. A few of our men are out looking for him as we speak.")
        else
          bubble(en, "The mayor was going to Andra to talk to the Council there. I think that he should have been back by now.")
          bubble(en, "We may have to go and look for him soon. We need to make sure that he is safe.")
        end
      elseif (get_progress(P_FOUNDMAYOR) < 2) then
        bubble(en, "The mayor is back now, thanks to you. However, the mayor is not seeing any visitors for a while.")
        bubble(en, "He is still recovering from his ordeal.")
      else
        bubble(en, "I have a scripting error.")
      end
    end

  elseif (en == 7) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "Are you enjoying your stay?")
    else
      if (get_progress(P_FOUNDMAYOR) == 0) then
        bubble(en, "The mayor left for Andra with an adventurer that he had just hired and his usual contingent of guards.")
        bubble(en, "I wonder if they avoided the trouble at the bridge?")
      else
        bubble(en, "Thank you!")
      end
    end

  elseif (en == 8) then
    bubble(en, "I wonder why those adventurers were stopped at the bridge? I'm not sure I want to try crossing it just yet.")

  elseif (en == 9) then
    bubble(en, "How long does it take to build a bridge?")

  elseif (en == 10) then
    LOC_join_ajathar(en)

  elseif (en == 11) then
    bubble(en, "Hey, you can't be back here! What are you trying to do, steal from me?")

  elseif (en == 12) then
    if (get_progress(P_MAYORGUARD1) == 1) then
      bubble(en, "Thanks for rescuing me back there! Here, have this:")
      set_gp(get_gp() + 1000)
      msg("You acquired 1000 gold pieces!")
      set_progress(P_MAYORGUARD1, 2)
    else
      bubble(en, "Those forces of Malkaron's sure are tough!")
    end

  elseif (en == 13) then
    LOC_join_casandra(en)

  elseif (en == 14) then
    if (get_progress(P_FOUNDMAYOR) < 2) then
      bubble(en, "How did you get in here past my locked door?")
    elseif (get_progress(P_FOUNDMAYOR) == 2) then
      bubble(en, "Oh, $0, thank you for rescuing me!")
      set_progress(P_FOUNDMAYOR, 3)
    else
      bubble(en, "I had a spa installed in my house! What do you think?")
    end

  end
end


function LOC_join_ajathar(en)
  local id
  if (get_progress(P_PORTALGONE) == 0) then
    if (get_progress(P_TALK_AJATHAR) == 0) then
      bubble(HERO1, "Hello! You haven't ventured very far!")
      bubble(en, "I have been maintaining a constant prayer vigil at this point. It should discourage the monsters from emerging into the town.")
      bubble(HERO1, "And what if they DO come out?")
      bubble(en, "My training also included swordsmanship.")
      set_progress(P_TALK_AJATHAR, 1)
    elseif (get_progress(P_TALK_AJATHAR) == 1) then
      bubble(en, "I hope I am doing the right thing here.")
    end
  else
    if (get_progress(P_TALK_AJATHAR) == 0) then
      bubble(en, "Be careful! That tunnel is infested with monsters.")
      bubble(HERO1, "Fortunately, I have been successful in closing the Portal. No more monsters will trouble us now.")
      set_progress(P_TALK_AJATHAR, 1)
    else
      bubble(HERO1, "You can rest easy now. I have closed the Portal that let the monsters through.")
    end
    bubble(en, "Great! Can I offer my services?")
    -- // Give Ajathar his default equipment
    set_all_equip(AJATHAR, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0)
    id = select_team{AJATHAR}
    -- /* Add the characters that weren't selected to the manor */
    add_to_manor(id)

    if (id[1]) then
      set_ent_id(en, id[1])

      if (id[2]) then
        -- // Two heroes were de-selected
        set_ent_id(9, id[2])
        set_ent_active(9, 1)
        set_ent_tilex(9, get_ent_tilex(en))
        set_ent_tiley(9, get_ent_tiley(en) + 1)
        bubble(en, "If you need us, we'll be back at the manor.")
        set_ent_script(en, "L1U1L1U2L2U1K")
        set_ent_script(9,  "L1U2L1U2L2U1K")
        wait_for_entity(9, en)
        set_ent_active(9, 0)
      else
        -- // One hero was de-selected
        bubble(en, "If you need me, I'll be back at the manor.")
        set_ent_script(en, "L1U1L1U2L2U1K")
        wait_for_entity(en, en)
      end
    end
    set_progress(P_PLAYERS, get_progress(P_PLAYERS) + 1)
  end
end


function LOC_join_casandra(en)
  local id

  bubble(en, "$0, thank you for helping us escape from the orc's camp!")
  bubble(HERO1, "Don't mention it. How did you get caught up in that mess anyway?")
  bubble(en, "Well, like I mentioned earlier, the mayor needed a bodyguard while he delivered an urgent message to some guy named Tsorin in Andra.")
  bubble(en, "That's when we were caught by the orcs. They Mayor ate the note when we were under attack. I have no idea what it was about.")
  bubble(HERO1, "Hmm, still sounds a bit fishy.")
  bubble(en, "I'd gladly join you to find out what this is all about!")

  -- // Give Casandra her default equipment
  set_all_equip(CASANDRA, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0)
  id = select_team{CASANDRA}
  -- /* Add the characters that weren't selected to the manor */
  add_to_manor(id)

  if (id[1]) then
    set_ent_id(en, id[1])

    if (id[2]) then
      -- // Two heroes were de-selected
      set_ent_id(9, id[2])
      set_ent_active(9, 1)
      set_ent_tilex(9, get_ent_tilex(en))
      set_ent_tiley(9, get_ent_tiley(en) - 1)
      bubble(en, "If you need us, we'll be back at the manor.")
      set_ent_script(en, "U8K")
      set_ent_script(9,  "U9K")
      wait_for_entity(9, en)
      set_ent_active(9, 0)
    else
      -- // One hero was de-selected
      bubble(en, "If you need me, I'll be back at the manor.")
      set_ent_script(en, "U8K")
      wait_for_entity(en, en)
    end
  end
  set_ent_active(en, 0)
  set_progress(P_PLAYERS, get_progress(P_PLAYERS) + 1)
end
