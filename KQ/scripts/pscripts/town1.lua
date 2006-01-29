

-- town1 - "Ekla"

--
function autoexec()
  -- Remove Derig from the screen
  if (get_progress(9) < 4) or (get_progress(9) > 5) then
    set_ent_active(4, 0)
  else
    -- Move granddaughter to the chair next to the table
    set_ent_tilex(2, 64)
    set_ent_tiley(2, 50)
    set_ent_facing(2, 0)
    set_ent_facing(4, 0)
    if (get_progress(9) == 4) then
      set_desc(0)
      view_range(1, 52, 40, 69, 57)
      set_ent_facing(200, 1)
      if (get_numchrs() == 2) then
        set_ent_facing(201, 1)
      end
      drawmap()
      screen_dump()
    end
  end
  -- Two people are wandering around until you use the transporter from Ajantara to Randen.
  if (get_progress(73) == 1) then
    set_ent_active(1, 0)
    set_ent_active(3, 0)
  end
  refresh()
end
function refresh()
  -- Chest in magic shop
  if (get_treasure(0) == 1) then
    set_mtile(63, 27, 265)
    set_zone(63, 27, 0)
  end
  -- Cauldron next to item shop
  if (get_treasure(6) == 1) then
    set_zone(30, 36, 0)
  end
  -- Patch of flowers behind houses
  if (get_treasure(98) == 1) then
    set_obs(34, 11, 0)
    set_zone(34, 11, 0)
  end
end
function postexec()
  -- Monsters will not attack on the world map until 0 == 1
  if (get_progress(0) == 0) then
    msg("The strong-scented oils wore off.", 255, 0)
    set_progress(0, 1)
  end
  if (get_progress(9) == 4) then
    local en = 4
    bubble(en, "I'm Derig. If I'm here, then you found me in the Grotto.")
    bubble(en, "To stop the monsters in our underground tunnel, you must seal the portal.")
    bubble(en, "In order to do that, you can use a Rod of Cancellation to melt it shut.")
    bubble(en, "I brought you to town to get the Unadium Coin from my granddaughter, which will open the rune back at the Grotto.")
    bubble(en, "That will transport you to the place where the Rod of Cancellation is.")
    set_progress(9, 5)
  end
end
function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 132, 30, 132, 30)
  elseif (zn == 2) then
    inn("The Blue Boar Inn", 25, 1)
  elseif (zn == 3) then
    shop(0)
  elseif (zn == 4) then
    shop(1)
  elseif (zn == 5) then
    shop(2)
  elseif (zn == 6) then
    bubble(200, "Locked.")
  elseif (zn == 7) then
    change_map("cave1", 0, 0, 0, 0)
  elseif (zn == 8) then
    chest(0, 161, 1)
    refresh()
  elseif (zn == 9) then
    bubble(255, "I don't have much.")
    shop(3)
  elseif (zn == 10) then
    door_in(60, 20, 52, 11, 64, 22)
  elseif (zn == 11) then
    door_out(20, 19)
  elseif (zn == 12) then
    door_in(81, 33, 76, 24, 88, 35)
  elseif (zn == 13) then
    door_out(18, 40)
  elseif (zn == 14) then
    door_in(56, 33, 52, 24, 60, 35)
  elseif (zn == 15) then
    door_out(14, 45)
  elseif (zn == 16) then
    door_in(73, 19, 66, 11, 79, 22)
  elseif (zn == 17) then
    door_out(34, 37)
  elseif (zn == 18) then
    door_in(68, 31, 62, 24, 74, 33)
  elseif (zn == 19) then
    door_out(31, 27)
  elseif (zn == 20) then
    chest(6, 104, 1)
    refresh()
  elseif (zn == 21) then
    book_talk(party[0])
  elseif (zn == 22) then
    door_in(62, 54, 52, 41, 69, 57)
  elseif (zn == 23) then
    door_out(37, 56)
  elseif (zn == 24) then
    touch_fire(party[0])
  elseif (zn == 25) then
    chest(98, 0, 155)
    refresh()
  elseif (zn == 26) then
    thought(200, "`Erupting Volcano'")
  elseif (zn == 27) then
    thought(200, "`Mountain Nightscape'")
  elseif (zn == 28) then
    thought(200, "`My Barn Chimney'")
  elseif (zn == 29) then
    thought(200, "`The Beach Front'")
  elseif (zn == 30) then
    thought(200, "`The Eye of Sardine'", "Hmm, sounds strangely familiar.")
  elseif (zn == 31) then
    thought(200, "`The Battle Is Over'")
  elseif (zn == 32) then
    thought(200, "`Forgotten Barn'")
  elseif (zn == 33) then
    thought(200, "`Fire Sunset'")
  elseif (zn == 34) then
    bubble(5, "This is my art collection. Any piece of work starting at only 300000 GP!")
  elseif (zn == 35) then
    bubble(200, "This armour has been highly polished. I can see myself in it!")
  elseif (zn == 36) then
    bubble(200, "Nothing in here but antique junk.")
  elseif (zn == 37) then
    bubble(200, "The drawers are locked.")
  elseif (zn == 38) then
    bubble(200, "This is the price list for all of these items.")
  elseif (zn == 39) then
    bubble(200, "None of these potions or herbs are useful.")
  elseif (zn == 40) then
    door_in(78, 49, 73, 40, 83, 51)
  elseif (zn == 41) then
    door_out(20, 33)
  end
end
function entity_handler(en)
  if (en == 0) then
    if (get_progress(73) == 0) then
      if (get_progress(12) == 0) then
        bubble(en, "Welcome to the town of Ekla.")
        bubble(en, "Yes! That makes eight. If I welcome enough newcomers to this town, I will get promoted.")
        bubble(en, "I might get a job sitting in a house all day saying the same thing over and over to anyone who talks to me.")
        bubble(en, "I should start practicing.")
        set_progress(12, 1)
      elseif (get_progress(12) == 1) then
        bubble(en, "I like cheese.")
      end
    else
      if (get_progress(12) == 0) then
        bubble(en, "I welcome people to Ekla.")
        bubble(en, "Welcome back.")
      elseif (get_progress(12) == 1) then
        bubble(en, "I welcomed you already. You're in Ekla.")
      end
    end
  elseif (en == 1) then
    if (get_progress(2) == 0) then
      bubble(en, "There is a monster blocking the pass to Randen.")
    else
      bubble(en, "Now the monster is gone.")
    end
  elseif (en == 2) then
    -- Jen will only help you if you are not on Official Business for Tsorin
    if (get_progress(108) == 0 or
        get_progress(108) > 2) then
      -- You have never spoken to Jen before
      if (get_progress(36) == 0) then
        if (get_progress(9) == 0) then
          -- You have never gone to the grotto.
          bubble(en, "Jen:", "I'm Derig's granddaughter. Go find Derig in the grotto north of here.")
        elseif (get_progress(9) == 1) then
          -- You entered the grotto, but did not fall in the pit.
          bubble(en, "Jen:", "I'm Derig's granddaughter. Go find Derig in the grotto north of here. You've been there once before.")
        elseif (get_progress(9) == 2) then
          -- You entered the grotto and fell in pit (you had NOT spoken to Jen first, though). Someone helped you get out.
          bubble(en, "You've already been to the grotto.")
          if (get_numchrs() > 1) then
            bubble(200, "Yes, we fell down a hole and someone pulled us out.")
          else
            bubble(200, "Yes, I fell down a hole and someone pulled me out.")
          end
          bubble(en, "That is Derig, my grandfather. Go back and look for him.")
        else
          -- This should never occur
          bubble(en, "Scripting Error:", "P_UCOIN == 0, but P_TALKDERIG != 0..2")
        end
        -- Now you have spoken to Jen
        set_progress(36, 1)
      -- You have spoken to Jen at least once
      elseif (get_progress(36) == 1) then
        if (get_progress(9) == 0) then
          bubble(en, "I said to go talk to Derig.")
        elseif (get_progress(9) == 1) then
          -- You entered the grotto, but did not fall in the pit.
          bubble(en, "You must find Derig in the grotto. He's there somewhere.")
        elseif (get_progress(9) == 2) then
          bubble(en, "Well? Go find Derig in the grotto!")
        elseif (get_progress(9) == 3) then
          bubble(en, "Okay, that was stupid. You saw him by the fire and you didn't talk to him.")
          if (get_numchrs() == 1) then
            bubble(en, "GO BACK AND TALK TO HIM! He's sitting by the fire you moron!")
          else
            bubble(en, "GO BACK AND TALK TO HIM! He's sitting by the fire you morons!")
          end
        elseif (get_progress(9) == 4) then
          -- This should never occur
          bubble(en, "Scripting Error:", "P_UCOIN == 1, but P_TALKDERIG == 4")
        elseif (get_progress(9) == 5) then
          -- Met Derig
          bubble(en, "Good. Now that you found Derig, here is the Unadium Coin.")
          set_progress(36, 2)
          msg("Unadium coin procured", 255, 0)
        end -- 9
      elseif (get_progress(36) == 2) then
        -- You now have the Unadium coin
        if (get_progress(9) == 5) then
          if (get_progress(37) == 0) then
            -- You do not have the rod
            bubble(en, "Go get the Rod of Cancellation.")
          elseif (get_progress(37) == 1) then
            -- You have the rod
            if (get_progress(38) == 0) then
              -- The portal is still there
              bubble(en, "Get rid of the portal now.")
            elseif (get_progress(38) == 1) then
              -- The portal is gone
              bubble(en, "You got rid of the portal! Give my father the coin and rod back and I'll give you a SunStone.")
            end -- 38
          end -- 37
        else
          -- This should never occur
          bubble(en, "Scripting Error:", "P_UCOIN == 2, but P_TALKDERIG != 5")
        end -- 9 == 5
      elseif (get_progress(36) == 3) then
        -- Returned Rod of Cancellation to Derig
        if (get_treasure(45) == 0) then
          bubble(en, "Thanks for returning the Rod to my grandfather. Here is a SunStone for you.")
          chest(45, 121, 1)
        else
          bubble(en, "Thanks again.")
        end
      end -- 36
    elseif (get_progress(108) == 1) then
      bubble(en, "Jen:", "You must be looking for Derig, my grandfather. Find him in the grotto north of here.")
    elseif (get_progress(108) == 2) then
      bubble(en, "You should finish Derig's assignment before I give you another.")
    end -- 108
  elseif (en == 3) then
    if (get_progress(2) == 0) then
      bubble(en, "Stock up on weapons, magic, and experience.")
      bubble(en, "You'll need them against the monster blocking the entrance to Randen.")
    elseif (get_progress(2) == 1) then
      if (get_progress(38) == 0) then
        bubble(en, "The monster blocking Randen is gone, but there are still monsters underground.")
      elseif (get_progress(38) == 1) then
        bubble(en, "All the monsters are gone from the tunnel!")
      end
    end -- 2
  elseif (en == 4) then
    -- 108 {
    if (get_progress(108) == 0 or
        get_progress(108) > 2) then
      if (get_progress(9) == 5) then
        if (get_progress(36) == 1) then
          bubble(en, "Talk to Jen to get the Unadium coin.")
        elseif (get_progress(36) == 2) then
          if (get_progress(37) == 0) then
            bubble(en, "Go get the Rod of Cancellation out of the grotto. Use the rune.")
          elseif (get_progress(37) == 1) then
            if (get_progress(38) == 0) then
              bubble(en, "Now that you have the rod, go down and seal the portal.")
            elseif (get_progress(38) == 1) then
              bubble(en, "You've done it! The portal is gone, and you have returned the Unadium coin and Rod of Cancellation.")
              set_progress(36, 3)
              set_progress(37, 2)
              set_progress(9, 6)
              msg("Derig takes the Rod of Cancellation and Unadium Coin.", 255, 0)
              bubble(en, "I'll take these back to the grotto for safe keeping. Thank you.")
            end -- 38
          end -- 37
        end -- 36
      elseif (get_progress(9) == 6) then
        -- Although TALKDERIG==6, Derig will still be on the screen until you leave Ekla and return
        bubble(en, "I will take these back to the grotto. Thanks again.")
      end -- 9
    elseif (get_progress(108) == 1) then
      bubble(en, "Ah yes, $0, I... wait a minute! What is that note you are carrying?")
      msg("You show Derig the note.", 18, 0)
      bubble(en, "Oh, this is very important. Yes, of utmost importance.")
      bubble(en, "Forgive me, $0, but Tsorin writes that the Oracle's Statue has been stolen. This causes great reason for concern.")
      bubble(200, "What's the big emergency?")
      bubble(en, "No time for that now. Please, take this note to Tsorin. I authorize you to pass into the goblin lands.")
      msg("Derig continues to mumble worredly.", 255, 0)
      thought(200, "Boy, this is just too weird all of a sudden.")
      msg("Derig hands you a sealed envelope.", 18, 0)
      set_progress(108, 2)
    elseif (get_progress(108) == 2) then
      bubble(en, "Please hurry. Take this note to Tsorin immediately!")
    else
      --
    end -- 108 }
  elseif (en == 5) then
    bubble(en, "I hid these works of art from Malkaron's forces. When they had come through here, they tried to destroy everything...")
  elseif (en == 6) then
    bubble(en, "Ever since monsters started appearing, my mother doesn't want me to go outdoors alone. It's so boring in here, though! I want to go out and play!")
  elseif (en == 7) then
    bubble(en, "My, my. This is beautiful work. How rare! How exquisite! How affordable!")
  end
end
