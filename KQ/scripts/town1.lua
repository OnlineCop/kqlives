-- town1 - "Ekla"

-- /*
-- {
-- P_CANCELROD: Possession of Rod of Cancellation
--   0 - Do not have it
--   1 - Got it
--   2 - Returned it to Derig
--
-- P_DARKIMPBOSS: Dark Imp is in the tunnel from Ekla to Randen
--   0 - It is blocking the stairway
--   1 - You defeated it and the pathway is now clear
--
-- P_EKLAWELCOME: Corny welcome message when you talk to the man in Ekla
--   0 - He hasn't yet given you his corny "Yes! This makes 8!" speech
--   1 - Now he likes cheese
--
-- P_PORTALGONE: Whether the portal in the tunnel is still working
--   0 - Still letting monsters through
--   1 - The Portal is sealed shut
--
-- P_START: Monsters will not attack you until you enter and leave Ekla
--   0 - Haven't entered Ekla yet
--   1 - Entered Ekla; monsters will now attack randomly on world map
--
-- P_TALKDERIG: If you've spoken to Derig
--   0 - Never entered grotto
--   1 - Entered grotto; didn't fall down pit
--   2 - Fell down pit in grotto, didn't speak with Derig
--   3 - Fell down pit, spoke with Derig
--   4 - Derig told you about the Rod of Cancellation
--   5 - Returned Unadium coin and Rod of Cancellation to Derig
--
-- P_UCOIN: Unadium Coin from Jen
--   0 - Have not yet spoken with the granddaughter, Jen
--   1 - Spoke to Jen
--   2 - Received coin
--   3 - Returned coin
--
-- P_WARPSTONE: The teleporter from Ajantara <-> Randen
--   0 - Haven't used it yet
--   1 - Stepped on the warp stone and created its counterpart in Randen
-- }
-- */


function autoexec()
  -- Remove Derig from the screen
  if (get_progress(P_TALKDERIG) < 4) or (get_progress(P_TALKDERIG) > 5) then
    set_ent_active(4, 0)
  else
    -- Move granddaughter to the chair next to the table
    set_ent_tilex(2, 64)
    set_ent_tiley(2, 50)
    set_ent_facing(2, FACE_DOWN)
    set_ent_facing(4, FACE_DOWN)
    set_ent_facing(HERO1, FACE_UP)
    if (get_progress(P_TALKDERIG) == 4) then
      set_desc(0)
      view_range(1, 52, 40, 69, 56)
    end
  end

  -- Monsters will not attack on the world map until this == 1
  if (get_progress(P_START) == 0) then
    set_progress(P_START, 1)
  end

  -- Two people are wandering around until you use the transporter from Ajantara to Randen.
  if (get_progress(P_WARPSTONE) == 1) then
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
  if (get_progress(P_TALKDERIG) == 4) then
    bubble(4, "I'm Derig. If I'm here, then you found me in the Grotto.")
    bubble(4, "To stop the monsters in our underground tunnel, you must seal the portal.")
    bubble(4, "In order to do that, you can use a Rod of Cancellation to melt it shut.")
    bubble(4, "I brought you to town to get the Unadium Coin from my granddaughter, which will open the rune back at the Grotto.")
    bubble(4, "That will transport you to the place where the Rod of Cancellation is.")
    set_progress(P_TALKDERIG, 5)
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
    bubble(HERO1, "Locked.")

  elseif (zn == 7) then
    change_map("cave1", 0, 0, 0, 0)

  elseif (zn == 8) then
    chest(0, I_B_SCORCH, 1)
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
    chest(6, I_NLEAF, 1)
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
    bubble(HERO1, "This armor has been highly polished. I can see myself in it!")

  elseif (zn == 36) then
    bubble(HERO1, "Nothing in here but antique junk.")

  elseif (zn == 37) then
    bubble(HERO1, "The drawers are locked.")

  elseif (zn == 38) then
    bubble(HERO1, "This is the pricelist for all of these items.")

  elseif (zn == 39) then
    bubble(HERO1, "None of these potions or herbs are useful.")

  elseif (zn == 40) then
    door_in(78, 49, 73, 40, 83, 51)

  elseif (zn == 41) then
    door_out(20, 33)

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_WARPSTONE) == 0) then
      if (get_progress(P_EKLAWELCOME) == 0) then
        bubble(en, "I welcome people to Ekla.")
        set_progress(P_EKLAWELCOME, 1)
      elseif (get_progress(P_EKLAWELCOME) == 1) then
        bubble(en, "I welcomed you already. You're in Ekla.")
      end
    else
      bubble(en, "Welcome back.")
    end

  elseif (en == 1) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(en, "There is a monster blocking the pass to Randen.")
    else
      bubble(en, "Now the monster is gone.")
    end

  elseif (en == 2) then
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
        if (get_numchrs() > 1) then
          bubble(en, "GO BACK AND TALK TO HIM! HE'S SITTING BY THE FIRE YOU MORON!")
        else
          bubble(en, "GO BACK AND TALK TO HIM! HE'S SITTING BY THE FIRE YOU MORONS!")
        end
      elseif (get_progress(P_TALKDERIG) == 4) then
        -- This should never occur
      elseif (get_progress(P_TALKDERIG) == 5) then
        -- Met Derig
        bubble(en, "Good. Now that you found Derig, here is the Unadium Coin.")
        set_progress(P_UCOIN, 2)
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
      end -- P_TALKDERIG == 5
    elseif (get_progress(P_UCOIN) == 3) then
      -- Returned Rod of Cancellation to Derig
      if (get_treasure(45) == 0) then
        bubble(en, "Thanks for returning the Rod to my grandfather. Here is a Sunstone for you.")
        chest(45, I_SSTONE, 1)
      else
        bubble(en, "Thanks again.")
      end
    end -- P_UCOIN

  elseif (en == 3) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(en, "Stock up on weapons, magic, and experience.")
      bubble(en, "You'll need them against the monster blocking the entrance to Randen.")
    elseif (get_progress(P_DARKIMPBOSS) == 1) then
      if (get_progress(P_PORTALGONE) == 0) then
        bubble(en, "The monster blocking Randen is gone, but there are still monsters underground.")
      elseif (get_progress(P_PORTALGONE) == 1) then
        bubble(en, "All the monsters are gone from the tunnel!")
      end
    end

  elseif (en == 4) then
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
            set_progress(P_CANCELROD, 2)
            set_progress(P_TALKDERIG, 6)
            msg("Derig takes the Rod of Cancellation and Unadium Coin.")
            bubble(en, "I'll take these back to the grotto for safe keeping. Thank you.")
          end
        end
      end
    elseif (get_progress(P_TALKDERIG) == 6) then
      -- Although TALKDERIG==6, Derig will still be on the screen until you leave Ekla and return
      bubble(en, "I will take these back to the grotto. Thanks again.")
    end -- P_TALKDERIG

  elseif (en == 5) then
    bubble(en, "I hid these works of art from Malkaron's forces. When they had come through here, they tried to destroy everything...")

  elseif (en == 6) then
    bubble(en, "Ever since monsters started appearing, my mother doesn't want me to go outdoors alone. It's so boring in here, though! I want to go out and play!")

  elseif (en == 7) then
    bubble(en, "My, my. This is beautiful work. How rare! How exquisite! How affordable!")

  end
end

