

-- tower - "Oracle's tower, southwest of mountain fort"

--
function autoexec()
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
  LOC_set_floor("floor1", 20, 55, 18, 58, 19)
  LOC_set_floor("floor1", 21, 54, 16, 59, 16)
  LOC_set_floor("floor1", 22, 53, 17, 55, 17)
  LOC_set_floor("floor1", 23, 55, 19, 58, 18)
  -- Wall holes (3: Treasure room, top left)
  LOC_set_wall("wall1", 26)
  LOC_set_wall("wall2", 27)
  LOC_set_wall("wall3", 28)
  LOC_set_wall("wall4", 29)
  -- Door (3: Treasure room, top)
  LOC_set_door("door3_1", 34, 40)
  -- Door (3: Treasure room, bottom)
  LOC_set_door("door3_2", 31, 10)
  -- Door (4: Single treasure, left floor switch)
  if (get_progress(30) == 1) then
    set_btile(124, 31, 190)
  else
    set_btile(124, 31, 189)
  end
  LOC_set_door("door4", 30, 10)
  -- Black stone (1: Main entrance, right)
-- LOC_get_stone("stone1", 51, 20, 222)
  -- White stone (2: Floor switches, bottom left)
-- LOC_get_stone("stone2", 52, 21, 221)
  -- White stone (3: Treasure room, top right)
-- LOC_get_stone("stone3", 53, 22, 221)
  -- Black stone (4: Single treasure, treasure chest)
  if (get_progress(54) == 1) then
    set_mtile(120, 19, 65)
  end
end
function postexec()
  return
end
function zone_handler(zn)
  -- Combat can be anywhere that other zones do not populate
  if (zn == 0) then
-- combat(54)
  -- Front doors (1: Main entrance)
  elseif (zn == 1) then
    if (get_progress(18) > 0) then
      set_progress(32, 2)
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
    bubble(200, "Locked.")
  -- Oracle bookshelves (5: Oracle room)
  elseif (zn == 11) then
    bubble(200, "Wow! There are a lot of strange books here.")
  -- Floor switch (2: Floor switches, top left)
  elseif (zn == 12) then
    LOC_floor_switch(20)
  -- Floor switch (2: Floor switches, bottom left)
  elseif (zn == 13) then
    LOC_floor_switch(21)
  -- Floor switch (2: Floor switches, top right)
  elseif (zn == 14) then
    LOC_floor_switch(22)
  -- Floor switch (2: Floor switches, bottom right)
  elseif (zn == 15) then
    LOC_floor_switch(23)
  -- Wall switch (3: Treasure room, top left)
  elseif (zn == 16) then
    LOC_stoner("wall1", 26)
  -- Wall switch (3: Treasure room, top right)
  elseif (zn == 17) then
    LOC_stoner("wall2", 27)
  -- Wall switch (3: Treasure room, bottom left)
  elseif (zn == 18) then
    LOC_stoner("wall3", 28)
  -- Wall switch (3: Treasure room, bottom right)
  elseif (zn == 19) then
    LOC_stoner("wall4", 29)
  -- Black stone (1: Main entrance, right)
  elseif (zn == 20) then
    if (get_progress(51) == 0) then
      set_progress(51, 1)
      set_progress(25, get_progress(25) + 1)
      sfx(5)
      msg("Black Stone procured!", 15, 0)
      LOC_get_stone("stone1", 51, 20, 222)
    end
  -- White stone (2: Floor switches, bottom left)
  elseif (zn == 21) then
    if (get_progress(52) == 0) then
      set_progress(52, 1)
      set_progress(24, get_progress(24) + 1)
      sfx(5)
      msg("White Stone procured!", 15, 0)
      LOC_get_stone("stone2", 52, 21, 221)
    end
  -- White stone (3: Treasure room)
  elseif (zn == 22) then
    if (get_progress(53) == 0) then
      set_progress(53, 1)
      set_progress(24, get_progress(24) + 1)
      sfx(5)
      msg("White Stone procured!", 15, 0)
      LOC_get_stone("stone3", 53, 22, 221)
    end
  -- Treasure chest (4: Single treasure)
  elseif (zn == 23) then
    if (get_progress(54) == 0) then
      set_progress(54, 1)
      set_progress(25, get_progress(25) + 1)
      sfx(5)
      msg("Black Stone procured!", 15, 0)
      if (get_progress(54) == 1) then
        set_mtile(120, 19, 65)
      end
    end
  -- Floor switch (4: Single treasure, left)
  elseif (zn == 24) then
    if (get_progress(30) == 0) then
      set_progress(30, 1)
      sfx(26)
      LOC_set_door("door4", 30, 10)
    end
  -- Floor switch (4: Single treasure, right)
  elseif (zn == 25) then
    if (get_progress(30) == 1) then
      set_progress(30, 0)
      sfx(26)
      refresh()
    end
  -- Floor switch (5: Oracle room, left)
  elseif (zn == 26) then
    if (get_progress(31) == 0) then
      bubble(200, "Oh! I think I stepped on a switch!")
      set_progress(31, 1)
      sfx(26)
      LOC_set_door("door3_2", 31, 10)
    end
  -- Treasure chest (3: Treasure room)
  elseif (zn == 27) then
    chest(33, 116, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 28) then
    chest(34, 114, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 29) then
    chest(35, 83, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 30) then
    chest(36, 11, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 31) then
    chest(37, 57, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 32) then
    chest(38, 0, 400)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 33) then
    chest(39, 142, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 34) then
    chest(40, 112, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 35) then
    chest(41, 121, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 36) then
    chest(42, 89, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 37) then
    chest(43, 34, 1)
    refresh()
  -- Treasure chest (3: Treasure room)
  elseif (zn == 38) then
    chest(44, 81, 1)
    refresh()
  -- Fire (5: Oracle room)
  elseif (zn == 39) then
    touch_fire(party[0])
  -- Locked door (3: Treasure room)
  elseif (zn == 40) then
    if (get_progress(26) == 1 and get_progress(27) == 2 and get_progress(28) == 2 and get_progress(29) == 1) then
      set_progress(34, 1)
      sfx(26)
      LOC_set_door("door3_1", 34, 40)
    end
  -- zn == 41: no enemies attack (3: Treasure room, 5: Oracle room)
  -- Guardian Dragon (1: Main entrance)
  elseif (zn == 42) then
    if (get_progress(33) == 0) then
      bubble(255, "The Oracle is not to be disturbed!")
      drawmap()
      screen_dump()
      set_run(0)
      combat(55)
      set_run(1)
      if (get_alldead() == 0) then
        set_progress(33, 1)
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
    --
    if (get_progress(111) == 0) then
      -- 0:
      if (get_progress(18) == 0) then
        bubble(en, "$0, thank you for coming.")
        if (get_numchrs() == 1) then
          bubble(200, "Hello, um... Oracle. How did you know I was coming?")
        else
          bubble(200, "Hello, um... Oracle. How did you know we were coming?")
        end
        bubble(en, "Tsorin sent a messenger here, of course.")
        bubble(en, "I'm surprised that you came up to see me, seeing as how the TravelPoint is back in the caves from whence you came.")
        if (get_progress(55) == 0) then
          -- 0:
          --
          bubble(en, "I am glad you stopped by, though. There is civil unrest in the goblin lands to the south.")
          bubble(en, "The Statue, which I gave to the Denorians as a gift, has been stolen. Because of that, there is much unrest.")
          if (get_numchrs() == 1) then
            bubble(200, "I am a bit hurried in my quest to find... well, something of importance.")
          else
            bubble(200, "We are a bit hurried in our quest to find... well...")
            bubble(201, "To find, um... something of importance.")
          end
          bubble(en, "Yes, but would you consider helping my people find their missing Statue?")
          if (prompt(en, 2, 1, "Anything you can do would",
                     "help.",
                     "  yes",
                     "  no") == 0) then
            bubble(en, "Thank you so much. Please talk to the head of the Denorian Council to find out more information.")
          else
            bubble(en, "That is too bad. But if you would like to help in the future, please talk to the head of the Denorian Council.")
          end
          bubble(en, "The Denorian village is south of here, by the mountain stream.")
        elseif (get_progress(55) == 1) then
          -- 1:
          bubble(en, "I am glad you stopped by, though. There is civil unrest in the goblin lands to the south.")
          if (get_numchrs() == 1) then
            bubble(200, "Yes, I know. The Denorian Council told me about the missing statue.")
          else
            bubble(200, "Yes, we know. The Denorian Council told us about the missing statue.")
          end
          bubble(en, "Oh! So you've agreed to help?")
          bubble(200, "Well ...mumble... no...")
          if (get_numchrs() == 1) then
            bubble(200, "I am a bit hurried in my quest to find... well, something of importance.")
          else
            bubble(201, "Not exactly...")
            bubble(200, "We are a bit hurried in our quest to find... well, something of importance.")
          end
          bubble(en, "Oh, I see. Your quest must be very important, then. Very well, I shall ask someone else.")
        elseif (get_progress(55) == 2) then
          -- 2:
          --
          bubble(en, "I am glad you stopped by, though. There is civil unrest in the goblin lands to the south.")
          if (get_numchrs() == 1) then
            bubble(200, "Yes, I know. The Denorian Council told me about the missing statue but I haven't found anything yet.")
          else
            bubble(200, "Yes, we know. The Denorian Council told us about the missing statue.")
            bubble(201, "We haven't found anything yet, however.")
          end
          bubble(en, "Don't worry; I'm sure you'll find something. Good luck to you!")
        elseif (get_progress(55) == 3) then
          -- 3:
          --
          --
          bubble(en, "I am glad you stopped by, though. I hear some of the civil unrest in the goblin lands has let up somewhat.")
          bubble(200, "Yes, a Narakian named Demnas supposedly stole the missing statue.")
          bubble(en, "And did you get the statue back?")
          bubble(200, "Well, only half of it. It seems it was broken in half before we ever got to it.")
          if (get_numchrs() == 1) then
            bubble(en, "Which means someone may still have it.")
          else
            bubble(201, "Which means someone probably still has it.")
          end
          bubble(en, "Could you do me a favor?")
          if (prompt(en, 2, 0, "Could you find out who has",
                     "the other half of the statue?",
                     "  yes",
                     "  no") == 0) then
            bubble(en, "Thank you so very much.")
          else
            bubble(en, "Well, that's alright. You've done enough already. Thank you so very much.")
          end
        elseif (get_progress(55) == 4) then
          -- 4: You found the troll, too, but you did not return the broken
          -- statue to the Denorians. You still have it with you.
          bubble(en, "I am glad you stopped by, though. I hear...")
          bubble(en, "...wait!", "What is that statue you have there?")
          if (get_numchrs() == 1) then
            bubble(200, "This is the Denorian Statue. I rescued it from a guy named Demnas and his troll.")
          else
            bubble(200, "This is the Denorian Statue. We rescued it from a guy named Demnas and his troll.")
          end
          bubble(en, "So you did... What happened to it? Part of it is missing.")
          if (get_progress(109) == 0) then
            --
            --
            --
            if (get_numchrs() == 1) then
              bubble(200, "I'm not too sure. He accused me of trying to steal the second half of it from him.")
            else
              bubble(201, "We're not too sure. He accused us of trying to steal the second half of it from him.")
            end
            bubble(en, "Second half? It sounds as if someone else may have gotten to him first.")
            if (get_numchrs() == 1) then
              bubble(200, "That's what I think as well.")
            else
              bubble(201, "That's what we think as well.")
            end
            bubble(en, "Well, either way, if you could return that half back to the Denorian village, it will help with the unrest.")
          else
            --
            --
            bubble(200, "It was Malkaron's armies. They had a spat with Demnas and when the statue broke, they stole half of it.")
            if (get_pidx(0) == 2) then
              bubble(200, "Demnas thought I was with them and locked me in with a troll.")
            else
              bubble(200, "Demnas thought CORIN was with them and locked him in with a troll.")
            end
            bubble(en, "Oh, how awful!")
            bubble(200, "It's alright. Everything worked out. But now we have to consider Malkaron's involvement in this.")
            bubble(en, "Yes, I certainly hope you get to the bottom of this.")
            bubble(en, "Would you return the statue to the Denorian village? It is a very important treasure.")
          end
        elseif (get_progress(55) == 5) then
          -- 5: The broken statue was returned to the village
          bubble(en, "I am glad you stopped by. The Denorians sent word that you returned part of their statue.")
          bubble(200, "Yea, well, someone had to do something about that whole mess...")
          if (get_progress(109) == 0) then
            bubble(en, "An important question is, who has the other half?")
            if (get_numchrs() == 1) then
              bubble(200, "That's a good question. I'm searching for the same person... or persons, for that matter.")
            else
              bubble(200, "That's a good question. We're searching for the same person.")
              bubble(201, "Or persons, for that matter.")
            end
            bubble(en, "Be careful. Whoever is out there may do rash and unpredictable things if he thinks you are following him.")
          else
            bubble(en, "An important question is, what will we do about Malkaron's followers?")
            if (get_numchrs() == 1) then
              bubble(200, "That's actually what I'm trying to find out.")
            else
              bubble(200, "That's actually what we're trying to find out.")
            end
            bubble(en, "If Malkaron is responsible for all this upheaval, you must be careful.")
            bubble(en, "One as strong as he may do rash and unpredictable things if he thinks you are following him.")
          end
          if (get_numchrs() == 1) then
            bubble(200, "Yes, I was warned to keep my party small.")
          else
            bubble(200, "Yes, we were warned to keep our numbers small. That's why we're traveling in a party of only two.")
          end
          bubble(en, "Well, I wish you luck. Thank you for all you've done already.")
        elseif (get_progress(55) == 6) then
          -- 6:
          --
          --
        end
        set_autoparty(1)
        move_entitym(200, "oracle", 0)
        if (get_numchrs() == 1) then
          wait_for_entity(200, 200)
          bubble(200, "This whole thing sure seems fishy. I hope I can figure out what's going on.")
          bubble(200, "I wonder if this statue has anything to do with this missing staff Nostik wants everyone to find?")
          set_ent_facing(200, 0)
          drawmap()
          screen_dump()
          bubble(200, "...I should probably stop talking to myself too.")
        elseif (get_numchrs() == 2) then
          move_entity(201, get_marker_tilex("oracle"), get_marker_tiley("oracle") + 1, 0)
          wait_for_entity(200, 201)
          orient_heroes()
          bubble(201, "Hey $0.")
          set_ent_facing(200, 0)
          bubble(200, "Yeah?")
          bubble(201, "Do you think that this statue could have something to with this missing staff Nostik wants us to find?")
          bubble(200, "Whoa, $1. Keep your voice down!")
          bubble(201, "Oh, right. Sorry.")
          bubble(201, "So... do you?")
          bubble(200, "I never thought of that. Let's hope that it doesn't. We have no choice but to go on anyways.")
          bubble(201, "Yeah, I guess you're right. Let's go.")
        end
        bubble(en, "Oh, one last thing. I have a request of you before you go.")
        if (get_numchrs() == 2) then
          set_ent_facing(201, 0)
        end
        bubble(en, "If you can, please get rid of the monsters in the caves below. You can also use my TravelPoint in the caves as well.")
        bubble(en, "Good luck. I hope to meet with you again.")
        set_autoparty(0)
        set_progress(18, 1)
      elseif (get_progress(18) == 1) then
        bubble(en, "The monsters in the caves below appeared out of nowhere. Please get rid of them so travelers who wish to use my TravelPoint may do so.")
      else
        -- We will never get 18 > 1 here since 111 will
        -- modify that separately.
      end
    elseif (get_progress(111) == 1) then
      -- 1:
      --
      if (get_progress(55) == 0) then
        --
        if (get_numchrs() == 1) then
          bubble(200, "Oracle! I saw Malkaron's monsters take some statue that looked like you through the portal below!")
          bubble(200, "At least, it looked like part of a statue, anyway.")
        else
          bubble(201, "Oracle! We saw Malkaron's monsters take some statue that looked like you through the portal below!")
          bubble(200, "At least, it looked like part of a statue, anyway.")
        end
        bubble(en, "That must have been the Denorian's statue; that has been stolen from the village to the south.")
      else
        --
        --
        if (get_numchrs() == 1) then
          bubble(200, "Oracle! I saw Malkaron's monsters take the stolen statue through the portal below!")
        else
          bubble(200, "Oracle! We saw Malkaron's monsters take your statue through the portal below!")
        end
      end
      bubble(en, "You must go after it! That statue is very important to my people.")
      set_progress(111, 2)
    elseif (get_progress(111) == 2) then
      -- 2:
      bubble(en, "Please hurry and get that statue back!")
    elseif (get_progress(111) == 3) then
      -- 3:
      bubble(en, "Welcome back.")
      bubble(200, "Oracle! The monsters went through the portal below with your statue!")
      if (get_numchrs() == 1) then
        bubble(200, "I went through the portal, but was sent to Maldea somehow.")
      else
        bubble(200, "We went through the portal, but were was sent to Maldea somehow.")
      end
      bubble(en, "Yes, the statue probably disrupted the portal somehow. I'm glad to see you made it back safely.")
      if (get_progress(18) == 0) then
        bubble(en, "I ask you to find the missing statue, if you possibly can.")
      else
        bubble(en, "Please continue your search for the statue. It is very important to my people.")
      end
      bubble(en, "And would you be able to take care of the monsters in the caves below?")
      set_progress(111, 4)
    elseif (get_progress(111) == 4) then
      -- 4:
      bubble(en, "I thank you for looking into the missing statue situation.")
      bubble(en, "There are still monsters lurking around in the caves below. Please get rid of them.")
    elseif (get_progress(111) == 5) then
      -- 5:
      bubble(en, "The monsters are gone! Thank you so much!")
    end
  end
end
function LOC_draw_dragon()
  local a
  if (get_progress(33) == 0) then
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
  elseif (get_progress(33) == 1) then
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
    if (get_progress(19) < 2) then
      set_progress(p_floor, 1)
      set_progress(19, get_progress(19) + 1)
    end
  else
    set_progress(p_floor, 0)
    set_progress(19, get_progress(19) - 1)
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
  set_progress(51, 0)
  set_progress(52, 0)
  set_progress(53, 0)
  set_progress(54, 0)
  set_progress(25, 0)
  set_progress(24, 0)
  -- Reset the stones located in the walls
  set_progress(26, 0)
  set_progress(27, 0)
  set_progress(28, 0)
  set_progress(29, 0)
  -- Close the open-by-switch doors
  set_progress(30, 0)
  set_progress(31, 0)
  set_progress(34, 0)
  -- Reset the holes on the 2nd floor
  set_progress(19, 0)
  set_progress(20, 0)
  set_progress(21, 0)
  set_progress(22, 0)
  set_progress(23, 0)
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
    if (get_progress(25) == 0 and get_progress(24) == 0) then
      return
    end
    -- Black stones picked up but no White stones
    if (get_progress(25) > 0 and get_progress(24) == 0) then
      set_progress(p_wall, 1)
      set_progress(25, get_progress(25) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end
    -- White stones picked up but no Black stones
    if (get_progress(25) == 0 and get_progress(24) > 0) then
      set_progress(p_wall, 2)
      set_progress(24, get_progress(24) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end
    -- At least one of each color stone picked up
    if (prompt(255, 2, 0, "What stone will you place?",
                          "  black",
                          "  white") == 0) then
      set_progress(p_wall, 1)
      set_progress(25, get_progress(25) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    else
      set_progress(p_wall, 2)
      set_progress(24, get_progress(24) - 1)
      sfx(5)
      LOC_set_wall(wall, p_wall)
      return
    end
  else
    -- Actually, this is already implicitly true
    if ((get_progress(p_wall) ~= 0) and (get_progress(34) == 0)) then
      if (get_progress(p_wall) == 1) then
        -- Remove a White stone
        set_progress(p_wall, 0)
        set_progress(25, get_progress(25) + 1)
        sfx(4)
        LOC_set_wall(wall, p_wall)
      elseif (get_progress(p_wall) == 2) then
      -- Remove a Black stone
        set_progress(p_wall, 0)
        set_progress(24, get_progress(24) + 1)
        LOC_set_wall(wall, p_wall)
        return
      end
    end
  end
end
