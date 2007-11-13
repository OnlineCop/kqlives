-- town1 - "Ekla"

-- /*
-- {
-- P_CANCELROD: Possession of Rod of Cancellation
--   (0) Do not have it
--   (1) Got it
--   (2) Returned it to Derig
--
-- P_DARKIMPBOSS: Dark Imp is in the tunnel from Ekla to Randen
--   (0) It is blocking the stairway to Randen
--   (1) You defeated it and the pathway is now clear
--
-- P_EKLAWELCOME: Corny welcome message when you talk to the man in Ekla
--   (0) He hasn't yet given you his corny "Yes! This makes 8!" speech
--   (1) Now he likes cheese
--
-- P_PORTALGONE: Whether the portal in the tunnel is still working
--   (0) Still letting monsters through
--   (1) The Portal is sealed shut
--
-- P_START: Monsters will not attack you until you enter Ekla
--   (0) (not used)
--   (1) Entered Ekla; monsters will now attack randomly on world map
--
-- P_TALKDERIG: If you've spoken to Derig
--   (0) Never entered grotto
--   (1) Entered grotto; didn't fall down pit
--   (2) Fell down pit in grotto, didn't speak with Derig
--   (3) Fell down pit, spoke with Derig
--   (4) Derig told you about the Rod of Cancellation
--   (5) Returned Unadium coin and Rod of Cancellation to Derig
--   (6) Derig has left Ekla and is back in the Grotto
--
-- P_TALK_TSORIN: If you've spoken to Tsorin in Andra (and got his seal)
--   (0) You haven't spoken to him yet
--   (1) Tsorin gave you a note to give to Derig
--   (2) Derig gave you a note to return to Tsorin
--   (3) Tsorin gave you his seal to get through the fort
--   (4) You've shown the seal to the guards at the fort
--   (5) You are free pass through the fort anytime (no contention in goblin lands) (not done)
--
-- P_UCOIN: Unadium Coin from Jen
--   (0) Have not yet spoken with the granddaughter, Jen
--   (1) Spoke to Jen
--   (2) Received coin
--   (3) Returned coin
--
-- P_WARPSTONE: The teleporter from Ajantara <-> Randen
--   (0) Haven't used it yet
--   (1) Stepped on the warp stone and created its counterpart in Randen
-- }
-- */


function autoexec()
  -- Remove Derig from the screen
  if (get_progress(P_TALKDERIG) < 4) or (get_progress(P_TALKDERIG) > 5) then
    set_ent_active(4, 0)
  else
    -- Move granddaughter to the chair next to the table
    local x, y = marker("by_derig")
    place_ent(2, x - 1, y - 2)

    set_ent_facing(2, FACE_DOWN)
    set_ent_facing(4, FACE_DOWN)
    if (get_progress(P_TALKDERIG) == 4) then
      set_desc(0)
      set_ent_facing(HERO1, FACE_UP)
      set_ent_facing(HERO2, FACE_UP)
      drawmap()
      screen_dump()
    end
  end

  -- Two people are wandering around until you use the transporter from Ajantara to Randen.
  if (get_progress(P_WARPSTONE) == 1) then
    set_ent_active(1, 0)
    set_ent_active(3, 0)
  end

  refresh()
end


function entity_handler(en)
  if (en == 0) then
    LOC_ekla_welcome(en)

  elseif (en == 1) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(en, "There is a monster blocking the pass to Randen.")
    else
      bubble(en, "Now the monster is gone.")
    end -- P_DARKIMPBOSS

  elseif (en == 2) then
    LOC_talk_jen(en)

  elseif (en == 3) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(en, "Stock up on weapons, magic, and experience.")
      bubble(en, "You'll need them against the monster blocking the entrance to Randen.")
    elseif (get_progress(P_DARKIMPBOSS) == 1) then
      if (get_progress(P_PORTALGONE) == 0) then
        bubble(en, "The monster blocking Randen is gone, but there are still monsters underground.")
      elseif (get_progress(P_PORTALGONE) == 1) then
        bubble(en, "All the monsters are gone from the tunnel!")
      end -- P_PORTALGONE
    end -- P_DARKIMPBOSS

  elseif (en == 4) then
    LOC_talk_derig(en)

  elseif (en == 5) then
    bubble(en, "I hid these works of art from Malkaron's forces. When they had come through here, they tried to destroy everything...")

  elseif (en == 6) then
    bubble(en, "Ever since monsters started appearing, my mother doesn't want me to go outdoors alone. It's so boring in here, though! I want to go out and play!")

  elseif (en == 7) then
    bubble(en, "My, my. This is beautiful work. How rare! How exquisite! How affordable!")

  end
end


function postexec()
  -- Monsters will not attack on the world map until P_START == 1
  if (get_progress(P_START) == 0) then
    msg("The strong-scented oils wore off.", 255, 0)
    set_progress(P_START, 1)
  end

  if (get_progress(P_TALKDERIG) == 4) then
    local en = 4
    bubble(en, "I'm Derig. If I'm here, then you found me in the Grotto.")
    bubble(en, "To stop the monsters in our underground tunnel, you must seal the portal.")
    bubble(en, "In order to do that, you can use a Rod of Cancellation to melt it shut.")
    bubble(en, "I brought you to town to get the Unadium Coin from my granddaughter, which will open the rune back at the Grotto.")
    bubble(en, "That will transport you to the place where the Rod of Cancellation is.")
    set_progress(P_TALKDERIG, 5)
  end
end


function refresh()
  -- Chest in magic shop
  if (get_treasure(0) == 1) then
    set_mtile("treasure1", 265)
    set_zone("treasure1", 0)
  end

  -- Cauldron next to item shop
  if (get_treasure(6) == 1) then
    set_zone("treasure2", 0)
  end

  -- Patch of flowers behind houses
  if (get_treasure(98) == 1) then
    set_zone("treasure3", 0)
    set_obs("treasure3", 0)
  end
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "town1")

  elseif (zn == 2) then
    inn("The Blue Boar Inn", 25, 1)

  elseif (zn == 3) then
    shop(0)

  elseif (zn == 4) then
    shop(1)

  elseif (zn == 5) then
    shop(2)

  elseif (zn == 6) then
    bubble(HERO1, "Locked.")

  elseif (zn == 7) then
    change_map("cave1", "entrance")

  elseif (zn == 8) then
    chest(0, I_B_SCORCH, 1)
    refresh()

  elseif (zn == 9) then
    bubble(255, "I don't have much.")
    shop(3)

  elseif (zn == 10) then
    door_in("inn_i")

  elseif (zn == 11) then
    door_out("inn_o")

  elseif (zn == 12) then
    door_in("shop_4i")

  elseif (zn == 13) then
    door_out("shop_4o")

  elseif (zn == 14) then
    door_in("shop_2i")

  elseif (zn == 15) then
    door_out("shop_2o")

  elseif (zn == 16) then
    door_in("shop_1i")

  elseif (zn == 17) then
    door_out("shop_1o")

  elseif (zn == 18) then
    door_in("shop_3i")

  elseif (zn == 19) then
    door_out("shop_3o")

  elseif (zn == 20) then
    chest(6, I_NLEAF, 1)
    refresh()

  elseif (zn == 21) then
    book_talk(party[0])

  elseif (zn == 22) then
    door_in("derig_i")

  elseif (zn == 23) then
    door_out("derig_o")

  elseif (zn == 24) then
    touch_fire(party[0])

  elseif (zn == 25) then
    chest(98, 0, 155)
    refresh()

  elseif (zn == 26) then
    thought(HERO1, "`Erupting Volcano'")

  elseif (zn == 27) then
    thought(HERO1, "`Mountain Nightscape'")

  elseif (zn == 28) then
    thought(HERO1, "`My Barn Chimney'")

  elseif (zn == 29) then
    thought(HERO1, "`The Beach Front'")

  elseif (zn == 30) then
    thought(HERO1, "`The Eye of Sardine'", "Hmm, sounds strangely familiar.")

  elseif (zn == 31) then
    thought(HERO1, "`The Battle Is Over'")

  elseif (zn == 32) then
    thought(HERO1, "`Forgotten Barn'")

  elseif (zn == 33) then
    thought(HERO1, "`Fire Sunset'")

  elseif (zn == 34) then
    bubble(5, "This is my art collection. Any piece of work starting at only 300000 GP!")

  elseif (zn == 35) then
    bubble(HERO1, "This armour has been highly polished. I can see myself in it!")

  elseif (zn == 36) then
    bubble(HERO1, "Nothing in here but antique junk.")

  elseif (zn == 37) then
    bubble(HERO1, "The drawers are locked.")

  elseif (zn == 38) then
    bubble(HERO1, "This is the price list for all of these items.")

  elseif (zn == 39) then
    bubble(HERO1, "None of these potions or herbs are useful.")

  elseif (zn == 40) then
    door_in("shop_5i")

  elseif (zn == 41) then
    door_out("shop_5o")

  end
end


function LOC_ekla_welcome(en)
  if (get_progress(P_WARPSTONE) == 0) then
    if (get_progress(P_EKLAWELCOME) == 0) then
      bubble(en, "Welcome to the town of Ekla.")
      bubble(en, "Yes! That makes eight. If I welcome enough newcomers to this town, I will get promoted.")
      bubble(en, "I might get a job sitting in a house all day saying the same thing over and over to anyone who talks to me.")
      bubble(en, "I should start practicing.")
      set_progress(P_EKLAWELCOME, 1)
    else
      bubble(en, "I like cheese.")
    end
  else -- P_WARPSTONE
    if (get_progress(P_EKLAWELCOME) == 0) then
      bubble(en, "I welcome people to Ekla.")
      bubble(en, "Welcome back.")
    else
      bubble(en, "I welcomed you already. You're in Ekla.")
    end
  end -- P_WARPSTONE
end


--// Jen helps Town's needs in this order:
--//    1: Monsters (Monsters are still attacking their town)
--//    2: Tunnel (DarkImp is blocking the travel to Randen)
--//    3: Trade (Trade route stopped working between Randen and the rest of the world)
--//    4: No extra assistance needed
function LOC_help_jen_portal(en)
  if (get_progress(P_CANCELROD) == 0) then
    LOC_help_town_rod(en)
  elseif (get_progress(P_DARKIMPBOSS) == 0) then
    LOC_help_town_boss(en)
  elseif (get_progress(P_WARPSTONE) == 0) then
    LOC_help_town_warpstone(en)
  else
    LOC_help_town_none(en)
  end
end


--// Jen helps Tsorin's needs in this order:
--//    1: Derig (She will help you find him to give him Tsorin's note)
--//    2: Tsorin (She will tell you to give Derig's note to Tsorin)
function LOC_help_jen_tsorin(en)
  if (get_progress(P_TALKDERIG) == 0) then
    LOC_help_tsorin_derig(en)
  else
    LOC_help_tsorin_tsorin(en)
  end
end


--// Town helps Derig's needs in this order:
--//    1: Coin (Jen gives it to you)
--//    2: Rod (Derig tells you how to find it)

function LOC_help_town_derig(en)
  if (get_progress(P_TALKDERIG) < 3) then
    LOC_help_derig_coin(en)
  elseif (get_progress(P_CANCELROD) == 0) then
    LOC_help_derig_rod(en)
  else
    LOC_help_derig_portal(en)
  end
end


--// Town's needs in this order:
--//    1: Coin (Need coin to get rid of the monsters)
function LOC_help_town_rod(en)
  if (get_progress(P_UCOIN) < 2) then
    LOC_help_town_derig(en)
  end
end


function LOC_talk_derig(en)
  if (get_progress(P_TALK_TSORIN) == 0 or
      get_progress(P_TALK_TSORIN) > 2) then
    if (get_progress(P_TALKDERIG) == 5) then
      if (get_progress(P_UCOIN) == 1) then
        bubble(en, "Talk to Jen to get the Unadium coin.")
      elseif (get_progress(P_UCOIN) == 2) then
        if (get_progress(P_CANCELROD) == 0) then
          bubble(en, "Go get the Rod of Cancellation out of the grotto. Use the rune.")
        elseif (get_progress(P_CANCELROD) == 1) then
          if (get_progress(P_PORTALGONE) == 0) then
            bubble(en, "Now that you have the rod, go down and seal the portal.")
          elseif (get_progress(P_PORTALGONE) == 1) then
            bubble(en, "You've done it! The portal is gone, and you have returned the Unadium coin and Rod of Cancellation.")
            set_progress(P_UCOIN, 3)
            remove_special_item(SI_UCOIN)
            set_progress(P_CANCELROD, 2)
            remove_special_item(SI_CANCELROD)
            set_progress(P_TALKDERIG, 6)
            msg("Derig takes the Rod of Cancellation and Unadium Coin.", 255, 0)
            bubble(en, "I'll take these back to the grotto for safe keeping. Thank you.")
          end -- P_PORTALGONE
        end -- P_CANCELROD
      end -- P_UCOIN
    elseif (get_progress(P_TALKDERIG) == 6) then
      -- Although TALKDERIG==6, Derig will still be on the screen until you leave Ekla and return
      bubble(en, "I will take these back to the grotto. Thanks again.")
    end -- P_TALKDERIG
  elseif (get_progress(P_TALK_TSORIN) == 1) then
    bubble(en, "Ah yes, $0, I... wait a minute! What is that note you are carrying?")
    msg("You show Derig the note.", 18, 0)
    bubble(en, "Oh, this is very important. Yes, of utmost importance.")
    bubble(en, "Forgive me, $0, but Tsorin writes that the Oracle's Statue has been stolen. This causes great reason for concern.")
    bubble(HERO1, "What's the big emergency?")
    bubble(en, "No time for that now. Please, take this note to Tsorin. I authorize you to pass into the goblin lands.")
    msg("Derig continues to mumble worredly.", 255, 0)
    thought(HERO1, "Boy, this is just too weird all of a sudden.")
    msg("Derig hands you a sealed envelope.", 18, 0)
    set_progress(P_TALK_TSORIN, 2)
    remove_special_item(SI_NOTE_TSORIN)
    add_special_item(SI_NOTE_DERIG)
  elseif (get_progress(P_TALK_TSORIN) == 2) then
    bubble(en, "Please hurry. Take this note to Tsorin immediately!")
  else
    -- // Nothing here, since Derig will only focus on Tsorin's quest if P_TALK_TSORIN < 3
  end -- P_TALK_TSORIN
end


-- Jen will provide assistance in this order:
--    1: Tsorin (war is top priority)
--    2: Town (monsters in tunnel)
--    3: Self (she wants her coin back)
--    4: No extra assistance needed
function LOC_talk_jen (en)
  -- Jen will only help you if you are not on Official Business for Tsorin
  if (get_progress(P_TALK_TSORIN) == 0 or
      get_progress(P_TALK_TSORIN) > 2) then
    -- You have never spoken to Jen before
    if (get_progress(P_UCOIN) == 0) then
      if (get_progress(P_TALKDERIG) == 0) then
        -- You have never gone to the grotto.
        bubble(en, "Jen:", "I'm Derig's granddaughter. Go find Derig in the grotto north of here.")
      elseif (get_progress(P_TALKDERIG) == 1) then
        -- You entered the grotto, but did not fall in the pit.
        bubble(en, "Jen:", "I'm Derig's granddaughter. Go find Derig in the grotto north of here. You've been there once before.")
      elseif (get_progress(P_TALKDERIG) == 2) then
        -- You entered the grotto and fell in pit (you had NOT spoken to Jen first, though).  Someone helped you get out.
        bubble(en, "You've already been to the grotto.")
        if (get_numchrs() > 1) then
          bubble(HERO1, "Yes, we fell down a hole and someone pulled us out.")
        else
          bubble(HERO1, "Yes, I fell down a hole and someone pulled me out.")
        end
        bubble(en, "That is Derig, my grandfather. Go back and look for him.")
      else
        -- This should never occur
        bubble(en, "Scripting Error:", "P_UCOIN == 0, but P_TALKDERIG != 0..2")
      end
      -- Now you have spoken to Jen
      set_progress(P_UCOIN, 1)
    -- You have spoken to Jen at least once
    elseif (get_progress(P_UCOIN) == 1) then
      if (get_progress(P_TALKDERIG) == 0) then
        bubble(en, "I said to go talk to Derig.")
      elseif (get_progress(P_TALKDERIG) == 1) then
        -- You entered the grotto, but did not fall in the pit.
        bubble(en, "You must find Derig in the grotto. He's there somewhere.")
      elseif (get_progress(P_TALKDERIG) == 2) then
        bubble(en, "Well? Go find Derig in the grotto!")
      elseif (get_progress(P_TALKDERIG) == 3) then
        bubble(en, "Okay, that was stupid. You saw him by the fire and you didn't talk to him.")
        if (get_numchrs() == 1) then
          bubble(en, "GO BACK AND TALK TO HIM! He's sitting by the fire you moron!")
        else
          bubble(en, "GO BACK AND TALK TO HIM! He's sitting by the fire you morons!")
        end
      elseif (get_progress(P_TALKDERIG) == 4) then
        -- This should never occur
        bubble(en, "Scripting Error:", "P_UCOIN == 1, but P_TALKDERIG == 4")
      elseif (get_progress(P_TALKDERIG) == 5) then
        -- Met Derig
        bubble(en, "Good. Now that you found Derig, here is the Unadium Coin.")
        set_progress(P_UCOIN, 2)
        add_special_item(SI_UCOIN)
        msg("Unadium coin procured", 255, 0)
      end -- P_TALKDERIG
    elseif (get_progress(P_UCOIN) == 2) then
      -- You now have the Unadium coin
      if (get_progress(P_TALKDERIG) == 5) then
        if (get_progress(P_CANCELROD) == 0) then
          -- You do not have the rod
          bubble(en, "Go get the Rod of Cancellation.")
        elseif (get_progress(P_CANCELROD) == 1) then
          -- You have the rod
          if (get_progress(P_PORTALGONE) == 0) then
            -- The portal is still there
            bubble(en, "Get rid of the portal now.")
          elseif (get_progress(P_PORTALGONE) == 1) then
            -- The portal is gone
            bubble(en, "You got rid of the portal! Give my father the coin and rod back and I'll give you a SunStone.")
          end -- P_PORTALGONE
        end -- P_CANCELROD
      else
        -- This should never occur
        bubble(en, "Scripting Error:", "P_UCOIN == 2, but P_TALKDERIG != 5")
      end -- P_TALKDERIG == 5
    elseif (get_progress(P_UCOIN) == 3) then
      -- Returned Rod of Cancellation to Derig
      if (get_treasure(45) == 0) then
        bubble(en, "Thanks for returning the Rod to my grandfather. Here is a SunStone for you.")
        chest(45, I_SSTONE, 1)
      else
        bubble(en, "Thanks again.")
      end
    end -- P_UCOIN
  elseif (get_progress(P_TALK_TSORIN) == 1) then
    bubble(en, "Jen:", "You must be looking for Derig, my grandfather. Find him in the grotto north of here.")
  elseif (get_progress(P_TALK_TSORIN) == 2) then
    bubble(en, "You should finish Derig's assignment before I give you another.")
  end -- P_TALK_TSORIN

-- /*
--  if (get_progress(P_TALK_TSORIN) == 1 or get_progress(P_TALK_TSORIN) == 2) then
--    LOC_help_jen_tsorin(en)
--  elseif (get_progress(P_PORTALGONE) == 0) then
--    LOC_help_jen_portal(en)
--  elseif (get_progress(P_UCOIN) == 3) then
--    LOC_help_jen_coin(en)
--  else
--    LOC_help_jen_none(en)
--  end
-- */
end
