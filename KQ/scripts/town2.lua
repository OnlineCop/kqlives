-- town2 - "Randen"

-- /*
-- {
-- P_BLADE:
--   0 - Woman hasn't mentioned the Phoenix Blade
--   1 - She has now
--
-- P_EARLYPROGRESS: Used when talking to Derig in the Grotto.
--   0 - Have not yet entered Ekla
--   1 - Entered Ekla
--   2 - Entered Randen
--   3 - Entered Andra
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
--   2 - Found mayor & used WARPSTONE to return; he wants your company
--   3 - You have spoken to the mayor
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
  if (get_progress(P_EARLYPROGRESS) < 2) then
    set_progress(P_EARLYPROGRESS, 2)
  end

  if not LOC_manor_or_party(AJATHAR) then
-- // Make one of the ents look like Ajathar if he's not been recruited yet.
    set_ent_id(10, AJATHAR)
  else
    set_ent_active(10, 0)
  end

  -- Move the guard so we can see the mayor
  if (get_progress(P_WARPSTONE) == 1) then
    set_ent_tilex(6, 57)
    if (get_progress(P_FOUNDMAYOR) == 1) then
      set_progress(P_FOUNDMAYOR, 2)
    end
  end

  refresh()
end


function refresh()
  if (get_treasure(3) == 1) then
    set_mtile(110, 19, 265)
  end
  if (get_treasure(4) == 1) then
    set_mtile(111, 19, 265)
  end
  if (get_treasure(5) == 1) then
    set_mtile(112, 19, 265)
  end
  if (get_treasure(7) == 1) then
    set_obs(14, 47, 0)
  end
  if (get_treasure(10) == 1) then
    set_obs(35, 38, 0)
  end
  if (get_treasure(31) == 1) then
    set_mtile(115, 36, 265)
  end
  if (get_treasure(46) == 1) then
    set_mtile(116, 36, 265)
  end
  if (get_progress(P_WARPSTONE) == 1) then
    set_mtile(35, 15, 0)
  end
  if (get_treasure(97) == 1) then
    set_mtile(135, 10, 265)
  end
  
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("cave1", 47, 47, 47, 47)

  elseif (zn == 2) then
    door_in(85, 13, 80, 6, 90, 15)

  elseif (zn == 3) then
    door_in(86, 26, 80, 17, 92, 28)

  elseif (zn == 4) then
    door_in(102, 13, 93, 6, 105, 15)

  elseif (zn == 5) then
    door_in(97, 25, 94, 17, 107, 27)

  elseif (zn == 6) then
    door_in(96, 39, 80, 30, 102, 50)

  elseif (zn == 7) then
    door_in(112, 13, 107, 6, 117, 15)

  elseif (zn == 8) then
    door_in(114, 40, 104, 28, 117, 42)

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
    -- PH added code to check if you do stay over night
    -- This is done indirectly; if your gp goes down it
    -- means you must have spent some here.
    local old_gp = get_gp()
    inn("Wayside Inn", 30, 1)
    -- This means you MUST stay at the inn before the
    -- bridge gets repaired. Correct?
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
    view_range(1, 119, 6, 122, 28)
    warp(121, 9, 8)

  elseif (zn == 21) then
    view_range(1, 109, 17, 117, 24)
    warp(116, 20, 8)

  elseif (zn == 22) then
    view_range(1, 94, 17, 107, 27)
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
    bubble(HERO1, "How to make friends through hypnosis.")

  elseif (zn == 35) then
    bubble(HERO1, "Inns always have boring books.")

  elseif (zn == 36) then
    if (get_progress(P_WARPSTONE) == 1) then
      change_map("town6", 38, 56, 38, 56)
    end

  elseif (zn == 37) then
    touch_fire(party[0])

  elseif (zn == 38) then
    view_range(1, 104, 28, 117, 43)
    warp(116, 32, 8)

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
    if (get_progress(MAYORGUARD2) == 1) then
      bubble(11, "Thank you for rescuing me!")
    else
      bubble(11, "I shouldn't be showing up if you haven't rescued me yet. It hasn't been programmed in to remove my NPC sprite.")
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
    door_in(129, 16, 124, 6, 140, 18)

  elseif (zn == 48) then
    door_out(48, 18)

  elseif (zn == 49) then
    door_in(137, 16, 124, 6, 140, 18)

  elseif (zn == 50) then
    door_out(52, 18)

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
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "Where is that man?")
    else
      bubble(en, "My husband is late again.")
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
    end

  elseif (en == 3) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(en, "Good day.")
    else
      if (get_progress(P_BLADE) == 0)  then
        -- PH: Just my little joke hehe
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
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "Business is good.")
      return
    end
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(en, "Now that the bridge is repaired I'm back in business!")
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
      bubble(en, "An altercation between some travelers and brigands resulted in a volley of magic that left the bridge in cinders.")
      set_ent_script(en, "W25")
      wait_for_entity(en)
      bubble(en, "A new bridge is supposed to be built soon. This town can't survive for long without our major trade route.")
    end

  elseif (en == 6) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "The mayor isn't seeing any visitors at present.")
    else
      if (get_progress(P_FOUNDMAYOR) == 0) then
        if (get_progress(P_FIGHTONBRIDGE) > 4) then
          bubble(en, "The mayor has yet to return from Andra. A few of our men are out looking for him as we speak.")
        else
          bubble(en, "The mayor was going to Andra to talk to the council there. I think that he should have been back by now.")
          bubble(en, "We may have to go and look for him soon. We need to make sure that he is safe.")
        end
      elseif (get_progress(P_FOUNDMAYOR) < 3) then
        bubble(en, "The mayor is back now, thanks to you. However, the mayor is not seeing any visitors for a while.")
        bubble(en, "He is still recovering from his ordeal.")
      else
        bubble(en, "The mayor is back now. He's been through quite a dramatic ordeal and is not seeing any visitors for awhile.")
      end
    end

  elseif (en == 7) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(en, "Are you enjoying your stay?")
    else
      if (get_progress(P_FOUNDMAYOR) ~= 0) then
        bubble(en, "Thank you!")
      else
        bubble(en, "The mayor left for Andra with an adventurer that he had just hired and his usual contingent of guards.")
        bubble(en, "I wonder if they avoided the trouble at the bridge.")
      end
    end

  elseif (en == 8) then
    bubble(en, "I wonder how long it takes to build a bridge?")

  elseif (en == 9) then
    bubble(en, "How long does it take to build a bridge?")

  elseif (en == 10) then
    LOC_join_ajathar(en)

  elseif (en == 11) then
    bubble(en, "Hey, you can't be back here! What are you trying to do, steal from me?")

  elseif (en == 12) then
    bubble(en, "Thanks for rescuing me back there!")

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
    -- Give Ajathar his default equipment
    set_all_equip(AJATHAR, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0)
    id = select_team{AJATHAR}
    -- /* Add the characters that weren't selected to the manor */
    add_to_manor(id)

    if (id[1]) then
      set_ent_id(en, id[1])

      if (id[2]) then
        -- Two heroes were de-selected
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
        -- One hero was de-selected
        bubble(en, "If you need me, I'll be back at the manor.")
        set_ent_script(en, "L1U2L2U1L1U1")
        wait_for_entity(en, en)
      end
    end
    set_ent_active(en, 0)
    set_progress(P_PLAYERS, get_progress(P_PLAYERS) + 1)
  end
end
