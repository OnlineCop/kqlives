-- tower - "Oracle's tower, southwest of mountain fort"

-- /*
-- {
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
  local a

  if (get_treasure(33) == 1) then
    set_mtile(76, 7, 65)
  end
  if (get_treasure(34) == 1) then
    set_mtile(77, 6, 65)
  end
  if (get_treasure(35) == 1) then
    set_mtile(79, 4, 65)
  end
  if (get_treasure(36) == 1) then
    set_mtile(80, 3, 65)
  end
  if (get_treasure(37) == 1) then
    set_mtile(79, 7, 65)
  end
  if (get_treasure(38) == 1) then
    set_mtile(81, 7, 65)
  end
  if (get_treasure(39) == 1) then
    set_mtile(81, 6, 65)
  end
  if (get_treasure(40) == 1) then
    set_mtile(81, 5, 65)
  end
  if (get_treasure(41) == 1) then
    set_mtile(83, 7, 65)
  end
  if (get_treasure(42) == 1) then
    set_mtile(83, 6, 65)
  end
  if (get_treasure(43) == 1) then
    set_mtile(83, 4, 65)
  end
  if (get_treasure(44) == 1) then
    set_mtile(85, 22, 65)
  end

  -- Defeated dragon (1: Main entrance)
  if (get_progress(P_DRAGONDOWN) == 1) then
    for a = 12, 16, 1 do
      set_ftile(a, 16, 0)
      set_ftile(a, 17, 0)
      set_ftile(a, 18, 0)
      set_btile(a, 19, 160)
      set_obs(a, 19, 0)
      set_zone(a, 19, 0)
    end
  end

  -- Wall hole (3: Treasure room, top left)
  if (get_progress(P_WALL1) == 1) then
    -- White stone
    set_ftile(76, 9, 222)
  elseif (get_progress(P_WALL1) == 2) then
    -- Black stone
    set_ftile(76, 9, 221)
  end

  -- Wall hole (3: Treasure room, top right)
  if (get_progress(P_WALL2) == 1) then
    -- White stone
    set_ftile(80, 9, 222)
  elseif (get_progress(P_WALL2) == 2) then
    -- Black stone
    set_ftile(80, 9, 221)
  end

  -- Wall hole (3: Treasure room, bottom left)
  if (get_progress(P_WALL3) == 1) then
    -- White stone
    set_ftile(90, 13, 222)
  elseif (get_progress(P_WALL3) == 2) then
    -- Black stone
    set_ftile(90, 13, 221)
  end

  -- Wall hole (3: Treasure room, bottom right)
  if (get_progress(P_WALL4) == 1) then
    -- White stone
    set_ftile(94, 13, 222)
  elseif (get_progress(P_WALL4) == 2) then
    -- Black stone
    set_ftile(94, 13, 221)
  end

  -- Floor switch (2: Floor switches, top left)
  if (get_progress(P_FLOOR1) == 1) then
    set_btile(42, 12, 190)
    set_btile(47, 9, 160)
    set_obs(47, 9, 1)
    set_btile(50, 9, 0)
    set_obs(50, 9, 1)
  end

  -- Floor switch (2: Floor switches, bottom left)
  if (get_progress(P_FLOOR2) == 1) then
    set_btile(42, 15, 190)
    set_btile(46, 7, 160)
    set_obs(46, 7, 0)
    set_btile(51, 7, 160)
    set_obs(51, 7, 0)
  end

  -- Floor switch (2: Floor switches, top right)
  if (get_progress(P_FLOOR3) == 1) then
    set_btile(55, 12, 190)
    set_btile(45, 8, 160)
    set_obs(45, 8, 0)
    set_btile(47, 8, 160)
    set_obs(47, 8, 0)
  end

  -- Floor switch (2: Floor switches, bottom right)
  if (get_progress(P_FLOOR4) == 1) then
    set_btile(55, 15, 190)
    set_btile(47, 10, 160)
    set_obs(47, 10, 0)
    set_btile(51, 9, 160)
    set_obs(51, 9, 0)
  end

  -- Door open (4: Single treasure, left floor switch)
  if (get_progress(P_DOOROPEN) == 1) then
    set_btile(115, 20, 190)
    set_ftile(111, 13, 175)
    set_btile(111, 14, 176)
    set_obs(111, 13, 0)
    set_zone(111, 13, 0)
  -- Door closed (4: Single treasure, right floor switch)
  else
    set_btile(115, 20, 189)
    set_ftile(111, 13, 169)
    set_btile(111, 14, 170)
    set_obs(111, 13, 1)
    set_zone(111, 13, 10)
  end

  -- Door open (3: Treasure room, bottom)
  if (get_progress(P_DOOROPEN2) == 1) then
    set_ftile(83, 19, 175)
    set_btile(83, 20, 176)
    set_obs(83, 19, 0)
    set_zone(83, 19, 0)
  end

  -- Door open (3: Treasure room, top)
  if (get_progress(P_TREASUREROOM) == 1) then
    set_ftile(78, 9, 175)
    set_btile(78, 10, 176)
    set_obs(78, 9, 0)
    set_zone(78, 9, 0)
  end

  -- Picked up White stone (3: Treasure room, top right)
  if (get_progress(P_STONE1) == 1) then
    set_ftile(89, 4, 0)
    set_obs(89, 4, 0)
    set_zone(89, 4, 0)
  end

  -- Picked up White stone (2: Floor switches, bottom left)
  if (get_progress(P_STONE2) == 1) then
    set_ftile(41, 20, 0)
    set_obs(41, 20, 0)
    set_zone(41, 20, 0)
  end

  -- Picked up Black stone (1: Main entrance, right)
  if (get_progress(P_STONE3) == 1) then
    set_ftile(16, 13, 0)
    set_obs(16, 13, 0)
    set_zone(16, 13, 0)
  end

  -- Picked up Black stone (4: Single treasure, treasure chest)
  if (get_progress(P_STONE4) == 1) then
    set_mtile(111, 8, 65)
  end
end


function postexec()
  return
end


function zone_handler(zn)

  -- Combat can be anywhere that other zones do not populate
  if (zn == 0) then
    combat(54)

  -- Front doors (1: Main entrance)
  elseif (zn == 1) then
    if (get_progress(P_ORACLE) > 0) then
      set_progress(P_TOWEROPEN, 0)
    end

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

    change_map("main", 263, 53, 263, 53)

  -- Stairs up (1: Main entrance -> 2: Floor switches)
  elseif (zn == 2) then
    warp(49, 3, 8)

  -- Stairs down (2: Floor switches -> 1: Main Entrance)
  elseif (zn == 3) then
    warp(14, 3, 8)

  -- Stairs up (2: Floor switches -> 3: Treasure room)
  elseif (zn == 4) then
    warp(85, 6, 8)

  -- Stairs down (3: Treasure room -> 2: Floor switches)
  elseif (zn == 5) then
    warp(49, 7, 8)

  -- Stairs up (3: Treasure room -> 4: Single treasure)
  elseif (zn == 6) then
    warp(110, 15, 8)

  -- Stairs down (4: Single treasure -> 3: Treasure room)
  elseif (zn == 7) then
    warp(76, 17, 8)

  -- Stairs up (4: Single treasure -> 5: Oracle room)
  elseif (zn == 8) then
    warp(152, 3, 8)

  -- Stairs down (5: Oracle room -> 4: Single treasure)
  elseif (zn == 9) then
    warp(120, 4, 8)

  -- Locked doors (3: Treasure room & 4: Single treasure)
  elseif (zn == 10) then
    bubble(HERO1, "Locked.")

  -- Oracle bookshelves (5: Oracle room)
  elseif (zn == 11) then
    bubble(HERO1, "Wow! There are a lot of strange books here.")

  -- Floor switch (2: Floor switches, top left)
  elseif (zn == 12) then
    if (get_progress(P_FLOOR1) == 0) then
      if (get_progress(P_FTOTAL) < 2) then
        set_progress(P_FLOOR1, 1)
        set_progress(P_FTOTAL, get_progress(P_FTOTAL) + 1)
        set_btile(42, 12, 190)
        set_btile(47, 9, 160)
        set_obs(47, 9, 1)
        set_btile(50, 9, 160)
        set_obs(50, 9, 0)
        sfx(26)
      end
    else
      set_progress(P_FLOOR1, 0)
      set_progress(P_FTOTAL, get_progress(P_FTOTAL) - 1)
      set_btile(42, 12, 189)
      set_btile(47, 9, 0)
      set_obs(47, 9, 1)
      set_btile(50, 9, 0)
      set_obs(50, 9, 1)
      sfx(26)
    end

  -- Floor switch (2: Floor switches, bottom left)
  elseif (zn == 13) then
    if (get_progress(P_FLOOR2) == 0) then
      if (get_progress(P_FTOTAL) < 2) then
        set_progress(P_FLOOR2, 1)
        set_progress(P_FTOTAL, get_progress(P_FTOTAL) + 1)
        set_btile(42, 15, 190)
        set_btile(46, 7, 160)
        set_obs(46, 7, 0)
        set_btile(51, 7, 160)
        set_obs(51, 7, 0)
        sfx(26)
      end
    else
      set_progress(P_FLOOR2, 0)
      set_progress(P_FTOTAL, get_progress(P_FTOTAL) - 1)
      set_btile(42, 15, 189)
      set_btile(46, 7, 0)
      set_obs(56, 7, 1)
      set_btile(51, 7, 0)
      set_obs(51, 7, 1)
      sfx(26)
    end

  -- Floor switch (2: Floor switches, top right)
  elseif (zn == 14) then
    if (get_progress(P_FLOOR3) == 0) then
      if (get_progress(P_FTOTAL) < 2) then
        set_progress(P_FLOOR3, 1)
        set_progress(P_FTOTAL, get_progress(P_FTOTAL) + 1)
        set_btile(55, 12, 190)
        set_btile(45, 8, 160)
        set_obs(45, 8, 0)
        set_btile(47, 8, 160)
        set_obs(47, 8, 0)
        sfx(26)
      end
    else
      set_progress(P_FLOOR3, 0)
      set_progress(P_FTOTAL, get_progress(P_FTOTAL) - 1)
      set_btile(55, 12, 189)
      set_btile(45, 8, 0)
      set_obs(45, 8, 1)
      set_btile(47, 8, 0)
      set_obs(47, 8, 1)
      sfx(26)
    end

  -- Floor switch (2: Floor switches, bottom right)
  elseif (zn == 15) then
    if (get_progress(P_FLOOR4) == 0) then
      if (get_progress(P_FTOTAL) < 2) then
        set_progress(P_FLOOR4, 1)
        set_progress(P_FTOTAL, get_progress(P_FTOTAL) + 1)
        set_btile(55, 15, 190)
        set_btile(47, 10, 160)
        set_obs(47, 10, 0)
        set_btile(51, 9, 160)
        set_obs(51, 9, 0)
        sfx(26)
      end
    else
      set_progress(P_FLOOR4, 0)
      set_progress(P_FTOTAL, get_progress(P_FTOTAL) - 1)
      set_btile(55, 15, 189)
      set_btile(47, 10, 0)
      set_obs(47, 10, 1)
      set_btile(51, 9, 0)
      set_obs(51, 9, 1)
      sfx(26)
    end

  -- Wall switch (3: Treasure room, top left)
  elseif (zn == 16) then
    LOC_stoner(P_WALL1, 76, 9)

  -- Wall switch (3: Treasure room, top right)
  elseif (zn == 17) then
    LOC_stoner(P_WALL2, 80, 9)

  -- Wall switch (3: Treasure room, bottom left)
  elseif (zn == 18) then
    LOC_stoner(P_WALL3, 90, 13)

  -- Wall switch (3: Treasure room, bottom right)
  elseif (zn == 19) then
    LOC_stoner(P_WALL4, 94, 13)

  -- White stone (3: Treasure room)
  elseif (zn == 20) then
    if (get_progress(P_STONE1) == 0) then
      set_progress(P_STONE1, 1)
      set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
      refresh()
      sfx(5)
      msg("White Stone procured!", 15, 0)
    end

  -- White stone (2: Floor switches)
  elseif (zn == 21) then
    if (get_progress(P_STONE2) == 0) then
      set_progress(P_STONE2, 1)
      set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
      refresh()
      sfx(5)
      msg("White Stone procured!", 15, 0)
    end

  -- Black stone (1: Main entrance)
  elseif (zn == 22) then
    if (get_progress(P_STONE3) == 0) then
      set_progress(P_STONE3, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
      refresh()
      sfx(5)
      msg("Black Stone procured!", 15, 0)
    end

  -- Treasure chest (4: Single treasure)
  elseif (zn == 23) then
    if (get_progress(P_STONE4) == 0) then
      set_progress(P_STONE4, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
      refresh()
      sfx(5)
      msg("Black Stone procured!", 15, 0)
    end

  -- Floor switch (4: Single treasure, left)
  elseif (zn == 24) then
    if (get_progress(P_DOOROPEN) == 0) then
      sfx(26)
      set_progress(P_DOOROPEN, 1)
      refresh()
    end

  -- Floor switch (4: Single treasure, right)
  elseif (zn == 25) then
    if (get_progress(P_DOOROPEN) == 1) then
      sfx(26)
      set_progress(P_DOOROPEN, 0)
      refresh()
    end

  -- Floor switch (5: Oracle room, left)
  elseif (zn == 26) then
    if (get_progress(P_DOOROPEN2) == 0) then
      sfx(26)
      bubble(HERO1, "Oh! I think I stepped on a switch!")
      set_ftile(83, 19, 175)
      set_btile(83, 20, 176)
      set_obs(83, 19, 0)
      set_zone(83, 19, 0)
      set_progress(P_DOOROPEN2, 1)
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
      set_ftile(78, 9, 175)
      set_btile(78, 10, 176)
      set_obs(78, 9, 0)
      set_zone(78, 9, 0)
      refresh()
      sfx(26)
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
        for a=12, 16, 1 do
          set_ftile(a, 16, 0)
          set_ftile(a, 17, 0)
          set_ftile(a, 18, 0)
          set_btile(a, 19, 160)
          set_obs(a, 19, 0)
          set_zone(a, 19, 0)
        end
        set_progress(P_DRAGONDOWN, 1)
      else
        return
      end
    end

  -- Save spot (5: Oracle room)
  elseif (zn == 43) then
    sfx(7)
    set_save(1)
    set_sstone(1)

  -- Off of Save spot (5: Oracle room)
  elseif (zn == 44) then
    set_save(0)
    set_sstone(0)

  end
end


function entity_handler(en)
  local player_response, done_talking

  -- Oracle
  if (en == 0) then
    if (get_progress(P_ORACLE) == 0) then
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
      bubble(en, "The passage will take you to the the tiny kingdom of Esteria. There you will find an evil warlord named Binderak.")
      bubble(en, "He is the one who acquired the Staff from Malkaron all those years ago. I believe he still has that half of the Staff.")
      bubble(en, "However, I may be wrong. And I don't know where the other half is. Unfortunately, that is all that I can tell you.")
      bubble(HERO1, "That's plenty. Thanks.")
      bubble(en, "Now you must go, for the Stone Dragon will soon piece himself back together and be even stronger.")
      bubble(HERO1, "Okay... thanks again.")
      bubble(en, "Oh, just one more thing.")
      bubble(en, "In the caves south of here there was a locked door. That door leads to a teleporter and will now be unlocked.")
      bubble(HERO1, "Thanks!")
      set_progress(P_ORACLE, 1)
      set_autoparty(1)
-- PH added 'D1' so you don't get stuck if you're talking 
-- to the oracle from the right hand side
      set_ent_script(HERO1, "D1X150Y10F1")
      if (get_numchrs() == 1) then
        wait_for_entity(HERO1, HERO1)
        bubble(HERO1, "I hope that this Binderak guy hasn't gotten a hold of the others.")
        bubble(HERO1, "I don't even want to think about that!")
        set_ent_facing(HERO1, FACE_DOWN)
        drawmap()
        screen_dump()
        bubble(HERO1, "Heh... I should probably stop talking to myself too.")
      else
        set_ent_script(HERO2, "D1X150Y11F1")
        wait_for_entity(HERO1, HERO2)
        set_ent_facing(HERO1, FACE_DOWN)
        bubble(HERO2, "Hey $0.")
        bubble(HERO1, "Yeah?")
        bubble(HERO2, "Do you think that this Binderak could have done something to the others?")
        bubble(HERO1, "You mean capture them?")
        bubble(HERO2, "Or worse. What if he knows about our quest and is slowly knocking us off?")
        bubble(HERO1, "I never thought of that. Let's hope that he doesn't know about it. We have no choice but to go on anyways.")
        bubble(HERO2, "Yeah, I guess you're right. Let's go.")
        set_autoparty(0)
        orient_heroes()
      end
    else
      bubble(en, "There is nothing else that I can tell you.")
    end

  end
end


function LOC_stoner(a, b, c)
  if (get_progress(a) == 0) then

    -- No stones picked up
    if (get_progress(P_BSTONES) == 0 and get_progress(P_WSTONES) == 0) then
      return
    end

    -- Black stones picked up but no White stones
    if (get_progress(P_WSTONES) == 0 and get_progress(P_BSTONES) > 0) then
      set_progress(a, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) - 1)
      sfx(5)
      set_ftile(b, c, 222)
      return
    end

    -- White stones picked up but no Black stones
    if (get_progress(P_BSTONES) == 0 and get_progress(P_WSTONES) > 0) then
      set_progress(a, 2)
      set_progress(P_WSTONES, get_progress(P_WSTONES) - 1)
      sfx(5)
      set_ftile(b, c, 221)
      return
    end

    -- At least one of each color stone picked up
    if (prompt(255, 2, 0, "What stone will you place?",
                          "  black",
                          "  white") == 0) then
      set_progress(a, 1)
      set_progress(P_BSTONES, get_progress(P_BSTONES) - 1)
      sfx(5)
      set_ftile(b, c, 222)
      return
    else
      set_progress(a, 2)
      set_progress(P_WSTONES, get_progress(P_WSTONES) - 1)
      sfx(5)
      set_ftile(b, c, 221)
      return
    end

  else
    -- Actually, this is already implicitely true
    if (get_progress(a) ~= 0) then

      -- Remove a White stone
      if (get_progress(a) == 1 and get_progress(P_TREASUREROOM) == 0) then
        set_progress(a, 0)
        set_progress(P_BSTONES, get_progress(P_BSTONES) + 1)
        sfx(4)
        set_ftile(b, c, 220)
        return
      end

      -- Remove a Black stone
      if (get_progress(a) == 2 and get_progress(P_TREASUREROOM) == 0) then
        set_progress(a, 0)
        set_progress(P_WSTONES, get_progress(P_WSTONES) + 1)
        sfx(4)
        set_ftile(b, c, 220)
        return
      end
    end
  end
end
