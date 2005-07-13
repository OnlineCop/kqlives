-- tower - "Oracle's tower, southwest of mountain fort"

-- /*
-- {
--
-- P_ORACLE: Spoke to Oracle in the Tower
--   0 - Haven't spoken to her yet
--   1 - She told you your quest, requested help from monsters
--   2 - You helped with monsters
--   3 - You used her TravelPoint but haven't spoken to her yet
--   4 - You used her TravelPoint and spoke with her
--
-- P_FTOTAL: Total number of floor switches activated
-- P_FLOOR1..P_FLOOR4: Status of this floor switch
--
-- P_BSTONES: # of Black stones in your inventory
-- P_WSTONES: # of White stones in your inventory
-- P_STONE1..P_STONE4: Whether this stone is in your inventory
-- P_WALL1..P_WALL4: Which stone (if any) is in this wall slot
--
-- P_DOOROPEN: (4: Single treasure, left floor switch)
-- P_DOOROPEN2: (3: Treasure room, bottom)
-- P_TREASUREROOM: (3: Treasure room, top)
--
-- P_TOWEROPEN: Whether you can return and talk to the Oracle
--   0 - Never spoke to Oracle, cannot enter Tower
--   1 - Never spoke to Oracle, can enter Tower
--   2 - Spoke to Oracle, cannot enter Tower
--   3 - Spoke to Oracle, can enter Tower
--
-- P_DRAGONDOWN: Whether the stone dragon is alive or not
--
-- P_DENORIAN: Status of the Denorian statue
--   0 - You have not entered dville or have not spoken to the town council
--   1 - If you refused to help the Denorians
--   2 - You've agreed to help, but haven't found Demnas
--   3 - You found Demnas (but not the troll)
--   4 - You found the troll, too
--   5 - The broken statue was returned to the village
--   6 - The 2nd half of the statue had been returned
--
-- Pretty much done as it is; no real changes need to be made.
-- }
-- */


function autoexec()

-- set_progress(P_ORACLE, 0)
-- TT: This was changed so they only reset if you enter or exit
-- the front doors, not just enter the map (like load a game)

  refresh()
end


function refresh()
  if (get_treasure(33) == 1) then
    set_mtile(83, 17, 65)
  end
  if (get_treasure(34) == 1) then
    set_mtile(84, 16, 65)
  end
  if (get_treasure(35) == 1) then
    set_mtile(86, 14, 65)
  end
  if (get_treasure(36) == 1) then
    set_mtile(87, 13, 65)
  end
  if (get_treasure(37) == 1) then
    set_mtile(86, 17, 65)
  end
  if (get_treasure(38) == 1) then
    set_mtile(88, 17, 65)
  end
  if (get_treasure(39) == 1) then
    set_mtile(88, 16, 65)
  end
  if (get_treasure(40) == 1) then
    set_mtile(88, 15, 65)
  end
  if (get_treasure(41) == 1) then
    set_mtile(90, 17, 65)
  end
  if (get_treasure(42) == 1) then
    set_mtile(90, 16, 65)
  end
  if (get_treasure(43) == 1) then
    set_mtile(90, 14, 65)
  end
  if (get_treasure(44) == 1) then
    set_mtile(92, 32, 65)
  end

  -- Dragon guard (1: Main entrance)
  LOC_draw_dragon()

  -- Floor switches (2: Floor switches)
  LOC_set_floor("floor1", P_FLOOR1, 55, 18, 58, 19)
  LOC_set_floor("floor1", P_FLOOR2, 54, 16, 59, 16)
  LOC_set_floor("floor1", P_FLOOR3, 53, 17, 55, 17)
  LOC_set_floor("floor1", P_FLOOR4, 55, 19, 58, 18)

  -- Wall holes (3: Treasure room, top left)
  LOC_set_wall("wall1", P_WALL1)
  LOC_set_wall("wall2", P_WALL2)
  LOC_set_wall("wall3", P_WALL3)
  LOC_set_wall("wall4", P_WALL4)

  -- Door (3: Treasure room, top)
  LOC_set_door("door3_1", P_TREASUREROOM, 40)

  -- Door (3: Treasure room, bottom)
  LOC_set_door("door3_2", P_DOOROPEN2, 10)

  -- Door (4: Single treasure, left floor switch)
  if (get_progress(P_DOOROPEN) == 1) then
    set_btile(124, 31, 190)
  else
    set_btile(124, 31, 189)
  end
  LOC_set_door("door4", P_DOOROPEN, 10)

  -- Black stone (1: Main entrance, right)
--  LOC_get_stone("stone1", P_STONE1, 20, 222)

  -- White stone (2: Floor switches, bottom left)
--  LOC_get_stone("stone2", P_STONE2, 21, 221)

  -- White stone (3: Treasure room, top right)
--  LOC_get_stone("stone3", P_STONE3, 22, 221)

  -- Black stone (4: Single treasure, treasure chest)
  if (get_progress(P_STONE4) == 1) then
    set_mtile(120, 19, 65)
  end
end


function postexec()
  return
end


function zone_handler(zn)


  -- Combat can be anywhere that other zones do not populate
  if (zn == 0) then
--    combat(54)

  -- Front doors (1: Main entrance)
  elseif (zn == 1) then
    if (get_progress(P_ORACLE) > 0) then
      set_progress(P_TOWEROPEN, 2)
    end
    LOC_reset_progress()
    change_map("main", 263, 53, 263, 53)

  -- Stairs up (1: Main entrance -> 2: Floor switches)
  elseif (zn == 2) then
    take_stairs("dstairs2", 44, 9, 69, 36)

  -- Stairs down (2: Floor switches -> 1: Main Entrance)
  elseif (zn == 3) then
    take_stairs("ustairs1", 8, 8, 35, 37)

  -- Stairs up (2: Floor switches -> 3: Treasure room)
  elseif (zn == 4) then
    take_stairs("dstairs3", 80, 10, 103, 35)

  -- Stairs down (3: Treasure room -> 2: Floor switches)
  elseif (zn == 5) then
    take_stairs("ustairs2", 44, 9, 69, 36)

  -- Stairs up (3: Treasure room -> 4: Single treasure)
  elseif (zn == 6) then
    take_stairs("dstairs4", 116, 11, 137, 34)

  -- Stairs down (4: Single treasure -> 3: Treasure room)
  elseif (zn == 7) then
    take_stairs("ustairs3", 80, 10, 103, 35)

  -- Stairs up (4: Single treasure -> 5: Oracle room)
  elseif (zn == 8) then
    take_stairs("dstairs5", 152, 12, 171, 33)

  -- Stairs down (5: Oracle room -> 4: Single treasure)
  elseif (zn == 9) then
    take_stairs("ustairs4", 116, 11, 137, 34)

  -- Locked doors (3: Treasure room) or (4: Single treasure)
  elseif (zn == 10) then
    bubble(HERO1, "Locked.")

  -- Oracle bookshelves (5: Oracle room)
  elseif (zn == 11) then
    bubble(HERO1, "Wow! There are a lot of strange books here.")

  -- Floor switch (2: Floor switches, top left)
  elseif (zn == 12) then
    LOC_floor_switch(P_FLOOR1)

  -- Floor switch (2: Floor switches, bottom left)
  elseif (zn == 13) then
    LOC_floor_switch(P_FLOOR2)

  -- Floor switch (2: Floor switches, top right)
  elseif (zn == 14) then
    LOC_floor_switch(P_FLOOR3)

  -- Floor switch (2: Floor switches, bottom right)
  elseif (zn == 15) then
    LOC_floor_switch(P_FLOOR4)

  -- Wall switch (3: Treasure room, top left)
  elseif (zn == 16) then
    LOC_stoner("wall1", P_WALL1)

  -- Wall switch (3: Treasure room, top right)
  elseif (zn == 17) then
    LOC_stoner("wall2", P_WALL2)

  -- Wall switch (3: Treasure room, bottom left)
  elseif (zn == 18) then
    LOC_stoner("wall3", P_WALL3)

  -- Wall switch (3: Treasure room, bottom right)
  elseif (zn == 19) then
    LOC_stoner("wall4", P_WALL4)

  -- Black stone (1: Main entrance, right)
  elseif (zn == 20) then
    if (get_progress(P_STONE1) == 0) then
      set_progress(P_STONE1, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
      sfx(5)
      msg("Black Stone procured!", 15, 0)
      LOC_get_stone("stone1", P_STONE1, 20, 222)
    end

  -- White stone (2: Floor switches, bottom left)
  elseif (zn == 21) then
    if (get_progress(P_STONE2) == 0) then
      set_progress(P_STONE2, 1)
      set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
      sfx(5)
      msg("White Stone procured!", 15, 0)
      LOC_get_stone("stone2", P_STONE2, 21, 221)
    end

  -- White stone (3: Treasure room)
  elseif (zn == 22) then
    if (get_progress(P_STONE3) == 0) then
      set_progress(P_STONE3, 1)
      set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
      sfx(5)
      msg("White Stone procured!", 15, 0)
      LOC_get_stone("stone3", P_STONE3, 22, 221)
    end

  -- Treasure chest (4: Single treasure)
  elseif (zn == 23) then
    if (get_progress(P_STONE4) == 0) then
      set_progress(P_STONE4, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
      sfx(5)
      msg("Black Stone procured!", 15, 0)
      if (get_progress(P_STONE4) == 1) then
        set_mtile(120, 19, 65)
      end
    end

  -- Floor switch (4: Single treasure, left)
  elseif (zn == 24) then
    if (get_progress(P_DOOROPEN) == 0) then
      set_progress(P_DOOROPEN, 1)
      sfx(26)
      LOC_set_door("door4", P_DOOROPEN, 10)
    end

  -- Floor switch (4: Single treasure, right)
  elseif (zn == 25) then
    if (get_progress(P_DOOROPEN) == 1) then
      set_progress(P_DOOROPEN, 0)
      sfx(26)
      refresh()
    end

  -- Floor switch (5: Oracle room, left)
  elseif (zn == 26) then
    if (get_progress(P_DOOROPEN2) == 0) then
      bubble(HERO1, "Oh! I think I stepped on a switch!")
      set_progress(P_DOOROPEN2, 1)
      sfx(26)
      LOC_set_door("door3_2", P_DOOROPEN2, 10)
    end

  -- Treasure chest (3: Treasure room)
  elseif (zn == 27) then
    chest(33, I_PCURING, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 28) then
    chest(34, I_LTONIC, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 29) then
    chest(35, I_SPEEDBOOTS, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 30) then
    chest(36, I_SWORD3, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 31) then
    chest(37, I_ROBE3, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 32) then
    chest(38, 0, 400)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 33) then
    chest(39, I_B_WHIRLWIND, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 34) then
    chest(40, I_EDAENRA, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 35) then
    chest(41, I_SSTONE, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 36) then
    chest(42, I_MESRA, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 37) then
    chest(43, I_STAFF2, 1)
    refresh()

  -- Treasure chest (3: Treasure room)
  elseif (zn == 38) then
    chest(44, I_GAUNTLET2, 1)
    refresh()

  -- Fire (5: Oracle room)
  elseif (zn == 39) then
    touch_fire(party[0])

  -- Locked door (3: Treasure room)
  elseif (zn == 40) then
    if (get_progress(P_WALL1) == 1 and get_progress(P_WALL2) == 2 and get_progress(P_WALL3) == 2 and get_progress(P_WALL4) == 1) then
      set_progress(P_TREASUREROOM, 1)
      sfx(26)
      LOC_set_door("door3_1", P_TREASUREROOM, 40)
    end

  -- zn == 41: no enemies attack (3: Treasure room, 5: Oracle room)

  -- Guardian Dragon (1: Main entrance)
  elseif (zn == 42) then
    if (get_progress(P_DRAGONDOWN) == 0) then
      bubble(255, "The Oracle is not to be disturbed!")
      drawmap()
      screen_dump()
      set_run(0)
      combat(55)
      set_run(1)
      if (get_alldead() == 0) then
        set_progress(P_DRAGONDOWN, 1)
        LOC_draw_dragon()
      else
        return
      end
    end

  -- Save spot (5: Oracle room)
  elseif (zn == 43) then
    set_save(1)
    set_sstone(1)
    sfx(7)

  -- Off of Save spot (5: Oracle room)
  elseif (zn == 44) then
    set_save(0)
    set_sstone(0)

  end
end


function entity_handler(en)

  -- Oracle
  if (en == 0) then
    if (get_progress(P_ORACLE) == 0) then
      bubble(en, "$0, thank you for coming. Please get rid of the monsters in the caves below.")
      bubble(en, "You can also use my TravelPoint in the caves as well.")
      set_progress(P_ORACLE, 1)

      set_autoparty(1)
      move_entity(HERO1, get_marker_tilex("oracle"), get_marker_tiley("oracle"), 0)

      if (get_numchrs() == 1) then
        wait_for_entity(HERO1, HERO1)

        bubble(HERO1, "I hope that this Binderak guy hasn't gotten a hold of the others.")
        bubble(HERO1, "I don't even want to think about that!")
        set_ent_facing(HERO1, FACE_DOWN)
        drawmap()
        screen_dump()
        bubble(HERO1, "Heh... I should probably stop talking to myself too.")
      elseif (get_numchrs() == 2) then
        move_entity(HERO2, get_marker_tilex("oracle"), get_marker_tiley("oracle") + 1, 0)
        wait_for_entity(HERO1, HERO2)
        orient_heroes()

        bubble(HERO2, "Hey $0.")
        set_ent_facing(HERO1, FACE_DOWN)
        bubble(HERO1, "Yeah?")
        bubble(HERO2, "Do you think that this Binderak could have done something to the others?")
        bubble(HERO1, "You mean capture them?")
        bubble(HERO2, "Or worse. What if he knows about our quest and is slowly knocking us off?")
        bubble(HERO1, "I never thought of that. Let's hope that he doesn't know about it. We have no choice but to go on anyways.")
        bubble(HERO2, "Yeah, I guess you're right. Let's go.")
      end

      set_autoparty(0)

    elseif (get_progress(P_ORACLE) == 1) then
      bubble(en, "You still need to get rid of the monsters in the caves below.")
    elseif (get_progress(P_ORACLE) == 2) then
      bubble(en, "You got rid of the monsters! Thank you.")
    elseif (get_progress(P_ORACLE) == 3) then
      bubble(en, "Welcome, $0. You didn't come in and speak with me the first time you came through here.")
    elseif (get_progress(P_ORACLE) == 4) then
      bubble(en, "Welcome back. You've used my TravelPoint, and have spoken to me before.")
    end

  end
end


-- /*
-- {
  local player_response, done_talking

    if (get_progress(P_ORACLE) == 0) then
      bubble(HERO1, "I will try to introduce myself.")
      bubble(en, "And I, the Oracle, will already know who you are, which surprises you.")
      bubble(HERO1, "We talk about the stolen statue.")
-- Should I add P_TALK_CORIN here?

      if (get_progress(P_DENORIAN) == 0) then
        bubble(HERO1, "I haven't been to the Denorian village, so I know nothing about where to look.")
      elseif (get_progress(P_DENORIAN) < 3) then
        bubble(HERO1, "I know the Denorian statue is missing, but haven't beaten Demnas for it.")
      elseif (get_progress(P_DENORIAN) == 3) then
        bubble(HERO1, "I fought Demnas but didn't report it to the Denorians. I have the broken statue with me.")
      elseif (get_progress(P_DENORIAN) == 4) then
        if (get_progress(P_TALK_CORIN) == 0) then
          bubble(HERO1, "I also fought the troll but didn't talk to Corin, so still don't know about Malkaron. Which was stupid, since he was standing RIGHT THERE.")
        elseif (get_progress(P_TALK_CORIN) == 1) then
          bubble(HERO1, "I also fought the troll and Corin told me about Malkaron's army stealing the statue.")
        end
        bubble(HERO1, "However, I didn't return the statue to the Denorians. I have the broken statue with me.")
      elseif (get_progress(P_DENORIAN) == 5)then
        bubble(HERO1, "I returned the broken statue to the Denorians and there was much rejoicing.")
      end

      bubble(en, "I respond appropriately, then inform you of my dilema.")
      bubble(HERO1, "You have a dilema?")
      bubble(en, "Yes, now shut up while I tell you about it.")
      if (get_progress(P_DENORIAN) < 3) then
        bubble(en, "The civil war is because that statue is missing and you need to help find it.")
      elseif (get_progress(P_DENORIAN) == 3) then
        bubble(en, "I see you have the statue. You're holding the reason of the civil war in your hands. Give it back to the Denorians.")
      elseif (get_progress(P_DENORIAN) == 4) then
        if (get_progress(P_TALK_CORIN) == 0) then
          bubble(en, "I see you have the statue. You're holding the reason of the civil war in your hands. Give it back to the Denorians.")
        elseif (get_progress(P_TALK_CORIN) == 1) then
          bubble(en, "I deduce that Malkaron's army stole the statue you're carrying and still has it!")
        end
      elseif (get_progress(P_DENORIAN) == 5) then
        bubble(en, "The civil war is over for the goblins in the south since you returned the statue to the Denorians.")
        if (get_progress(P_TALK_CORIN) == 0) then
          bubble(en, "I wonder if it was Malkaron's minions who stole the other half of the statue.")
        elseif (get_progress(P_TALK_CORIN) == 1) then
          bubble(en, "I deduce that Malkaron's army has to be who stole the other half of the statue.")
        end
      end

      bubble(en, "Now I ask you to rid the caves below of monsters as they prevent people from using her TravelPoint.")
      bubble(HERO1, "We as heros blindly agree to this side-quest because, hey, why not?")
      set_progress(P_ORACLE, 1)
    elseif (get_progress(P_ORACLE) == 1) then
      -- You've spoken to her once: she's told you about her dilema with the monsters downstairs.

      bubble(HERO1, "I guess you knew I was coming, huh?")
      bubble(en, "Quite right. I have known for some time now that you would be paying me a visit. I have been waiting.")
      bubble(HERO1, "Have any of the others been here?")
      bubble(en, "I haven't seen any of them. They wouldn't be able to get into the tower anyways.")
      bubble(HERO1, "I don't understand. Where are the others? I hardly seen any of them, so I assumed that they were way ahead of me.")
      bubble(en, "I am not certain. They are probably lost or confused. They did not speak to Derig as you did.")
      bubble(en, "They probably have no idea where to go.")
      bubble(HERO1, "Why wouldn't they have spoken to Derig? It seemed as though Derig was there to help us.")
      bubble(en, "Actually, Derig chose you above the others. He is a bit of a prophet himself, and he knew exactly who to help.")
      bubble(en, "If you are concerned about the others you may want to seek them out.")
      bubble(en, "However, you should wait until later, for there are more important things to take care of first.")
      bubble(HERO1, "Yes, I suppose there are. Are you going to tell me how to find the staff?")
      bubble(en, "Well, not exactly, but I do know where you have to go and how you will get there.")
      bubble(en, "You have to gather several items, all of which are made of Opal, in order to proceed.")
      bubble(en, "Once you find these things, you will then use them to travel the underwater passage on the western shore.")
      bubble(en, "The passage will take you to the tiny kingdom of Esteria. There you will find an evil warlord named Binderak.")
      bubble(en, "He is the one who acquired the Staff from Malkaron all those years ago. I believe he still has that half of the Staff.")
      bubble(en, "However, I may be wrong. And I don't know where the other half is. Unfortunately, that is all that I can tell you.")
      bubble(HERO1, "That's plenty. Thanks.")
      bubble(en, "Now you must go, for the Stone Dragon will soon piece himself back together and be even stronger.")
      bubble(HERO1, "Okay... thanks again.")
      bubble(en, "Oh, just one more thing.")
      bubble(en, "In the caves south of here there was a locked door. That door leads to a teleporter and will now be unlocked.")
      bubble(HERO1, "Thanks!")
      set_progress(P_ORACLE, 1)
    else
      bubble(en, "There is nothing else that I can tell you.")
    end
-- }
-- */



function LOC_draw_dragon()
  local a

  if (get_progress(P_DRAGONDOWN) == 0) then
    -- Dragon icon tile (223..228) in the for..loop below:
    local b = 223
    for a = get_marker_tilex("dragon"), get_marker_tilex("dragon") + 2, 1 do
      set_ftile(a, get_marker_tiley("dragon") - 2, b)
      set_ftile(a, get_marker_tiley("dragon") - 1, b + 3)
      b = b + 1
    end
    -- Dragon icon tile (229..230) in the for..loop below:
    b = 229
    for a = get_marker_tilex("dragon"), get_marker_tilex("dragon") + 1, 1 do
      set_btile(a, get_marker_tiley("dragon"), b)
      set_obs(a, get_marker_tiley("dragon"), 1)
      set_zone(a, get_marker_tiley("dragon"), 42)
      b = b + 1
    end
  elseif (get_progress(P_DRAGONDOWN) == 1) then
    for a = get_marker_tilex("dragon"), get_marker_tilex("dragon") + 2, 1 do
      set_ftile(a, get_marker_tiley("dragon") - 2, 0)
      set_ftile(a, get_marker_tiley("dragon") - 1, 0)
    end
    for a = get_marker_tilex("dragon"), get_marker_tilex("dragon") + 1, 1 do
      set_btile(a, get_marker_tiley("dragon"), 160)
      set_obs(a, get_marker_tiley("dragon"), 0)
      set_zone(a, get_marker_tiley("dragon"), 0)
    end
  end
end


function LOC_floor_switch(p_floor)
  if (get_progress(p_floor) == 0) then
    if (get_progress(P_FTOTAL) < 2) then
      set_progress(p_floor, 1)
      set_progress(P_FTOTAL, get_progress(P_FTOTAL) + 1)
    end
  else
    set_progress(p_floor, 0)
    set_progress(P_FTOTAL, get_progress(P_FTOTAL) - 1)
  end
  refresh()
end


function LOC_get_stone(stone, p_stone, zone, color)
  if (get_progress(p_stone) == 0) then
    set_ftile(get_marker_tilex(stone), get_marker_tiley(stone), color)
    set_obs(get_marker_tilex(stone), get_marker_tiley(stone), 1)
    set_zone(get_marker_tilex(stone), get_marker_tiley(stone), zone)
    return
  elseif (get_progress(p_stone) == 1) then
    set_ftile(get_marker_tilex(stone), get_marker_tiley(stone), 0)
    set_obs(get_marker_tilex(stone), get_marker_tiley(stone), 0)
    set_zone(get_marker_tilex(stone), get_marker_tiley(stone), 0)
    return
  end
end


function LOC_reset_progress()
  -- Reset the Black and White stones on the ground
  set_progress(P_STONE1, 0)
  set_progress(P_STONE2, 0)
  set_progress(P_STONE3, 0)
  set_progress(P_STONE4, 0)
  set_progress(P_BSTONES, 0)
  set_progress(P_WSTONES, 0)

  -- Reset the stones located in the walls
  set_progress(P_WALL1, 0)
  set_progress(P_WALL2, 0)
  set_progress(P_WALL3, 0)
  set_progress(P_WALL4, 0)

  -- Close the open-by-switch doors
  set_progress(P_DOOROPEN, 0)
  set_progress(P_DOOROPEN2, 0)
  set_progress(P_TREASUREROOM, 0)

  -- Reset the holes on the 2nd floor
  set_progress(P_FTOTAL, 0)
  set_progress(P_FLOOR1, 0)
  set_progress(P_FLOOR2, 0)
  set_progress(P_FLOOR3, 0)
  set_progress(P_FLOOR4, 0)
end


function LOC_set_door(door, p_door, zone)
  if (get_progress(p_door) == 0) then
    set_ftile(get_marker_tilex(door), get_marker_tiley(door) - 1, 169)
    set_btile(get_marker_tilex(door), get_marker_tiley(door) - 1, 178)
    set_btile(get_marker_tilex(door), get_marker_tiley(door), 170)
    set_obs(get_marker_tilex(door), get_marker_tiley(door) - 1, 1)
    set_zone(get_marker_tilex(door), get_marker_tiley(door) - 1, zone)
  else
    set_ftile(get_marker_tilex(door), get_marker_tiley(door) - 1, 175)
    set_btile(get_marker_tilex(door), get_marker_tiley(door), 176)
    set_obs(get_marker_tilex(door), get_marker_tiley(door) - 1, 0)
    set_zone(get_marker_tilex(door), get_marker_tiley(door) - 1, 0)
  end
  sfx(26)
end


function LOC_set_floor(floor, p_floor, x1, y1, x2, y2)
  if (get_progress(p_floor) == 0) then
    set_btile(get_marker_tilex(floor), get_marker_tiley(floor), 189)
    set_btile(x1, y1, 0)
    set_obs(x1, y1, 1)
    set_btile(x2, y2, 0)
    set_obs(x2, y2, 1)
  else
    set_btile(get_marker_tilex(floor), get_marker_tiley(floor), 190)
    set_btile(x1, y1, 160)
    set_obs(x1, y1, 0)
    set_btile(x2, y2, 160)
    set_obs(x2, y2, 0)
  end
  sfx(26)
end


function LOC_set_wall(wall, p_wall)
  if (get_progress(p_wall) == 0) then
    -- No stone
    set_ftile(get_marker_tilex(wall), get_marker_tiley(wall), 220)
  elseif (get_progress(p_wall) == 1) then
    -- White stone
    set_ftile(get_marker_tilex(wall), get_marker_tiley(wall), 222)
  elseif (get_progress(p_wall) == 2) then
    -- Black stone
    set_ftile(get_marker_tilex(wall), get_marker_tiley(wall), 221)
  end
end


function LOC_stoner(wall, p_wall)
  if (get_progress(p_wall) == 0) then

    -- No stones picked up
    if (get_progress(P_BSTONES) == 0 and get_progress(P_WSTONES) == 0) then
      return
    end

    -- Black stones picked up but no White stones
    if (get_progress(P_BSTONES) > 0 and get_progress(P_WSTONES) == 0) then
      set_progress(p_wall, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end

    -- White stones picked up but no Black stones
    if (get_progress(P_BSTONES) == 0 and get_progress(P_WSTONES) > 0) then
      set_progress(p_wall, 2)
      set_progress(P_WSTONES, get_progress(P_WSTONES) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end

    -- At least one of each color stone picked up
    if (prompt(255, 2, 0, "What stone will you place?",
                          "  black",
                          "  white") == 0) then
      set_progress(p_wall, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    else
      set_progress(p_wall, 2)
      set_progress(P_WSTONES, get_progress(P_WSTONES) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end

  else
    -- Actually, this is already implicitly true
    if ((get_progress(p_wall) ~= 0) and (get_progress(P_TREASUREROOM) == 0)) then
      if (get_progress(p_wall) == 1) then
        -- Remove a White stone
        set_progress(p_wall, 0)
        set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
        sfx(4)
        LOC_set_wall(wall, p_wall)
      elseif (get_progress(p_wall) == 2) then
      -- Remove a Black stone
        set_progress(p_wall, 0)
        set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
        LOC_set_wall(wall, p_wall)
        return
      end
    end
  end
end
