-- town1 - "Ekla"

-- /*
-- {
-- P_EARLYPROGRESS: Used when talking to Derig in the Grotto.
--   0 - Have not yet entered Ekla
--   1 - Entered Ekla
--   2 - Entered Randen
--   3 - Entered Andra
-- P_START: Monsters will not attack you until you enter and leave Ekla
--   0 - Haven't entered Ekla yet
--   1 - Entered Ekla; monsters will now attack randomly on world map
-- P_WARPSTONE: The teleporter from Ajantara <-> Randen
--   0 - Haven't used it yet
--   1 - Stepped on the warp stone and created its counterpart in Randen
-- P_EKLAWELCOME: Corny welcome message when you talk to the man in Ekla
--   0 - He hasn't yet given you his corny "Yes! This makes 8!" speech
--   1 - Now he likes cheese
-- P_DARKIMPBOSS: Dark Imp is in the tunnel from Ekla to Randen
--   0 - It is blocking the stairway
--   1 - You defeated it and the pathway is now clear
-- P_PORTALGONE: Whether the portal in the tunnel is still working
--   0 - Still letting monsters through
--   1 - The Portal is sealed shut
-- P_UCOIN: Unadium Coin from Jen
--   0 - Have not yet spoken with the granddaughter, Jen
--   1 - Spoke to Jen
--   2 - Received coin
--   3 - Returned coin
-- P_CANCELROD: Possession of Rod of Cancellation
--   0 - Do not have it
--   1 - Got it
--   2 - Returned it to Derig
-- P_TALKDERIG: If you've spoken to Derig
--   0 - Never entered grotto
--   1 - Entered grotto; didn't fall down pit
--   2 - Fell down pit in grotto, didn't speak with Derig
--   3 - Fell down pit, spoke with Derig
--   4 - Derig told you about the Rod of Cancellation
--   5 - Returned Unadium coin and Rod of Cancellation to Derig
-- }
-- */


function autoexec()
  -- If we have never been in the town before
  if (get_progress(P_EARLYPROGRESS) == 0) then
    set_progress(P_EARLYPROGRESS, 1)
  end

  -- Remove Derig from the screen
  if (get_progress(P_TALKDERIG) < 3) or (get_progress(P_TALKDERIG) > 4) then
    set_ent_active(4, 0)
  else
    -- Move daughter to the chair next to the table
    set_ent_tilex(2, 64)
    set_ent_tiley(2, 50)
    set_ent_facing(2, FACE_DOWN)
    set_ent_facing(4, FACE_DOWN)
    set_ent_facing(HERO1, 1)
    if (get_progress(P_TALKDERIG) == 3) then
      set_desc(0)
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
  end
end


function postexec()
  if (get_progress(P_TALKDERIG) == 3) then
    bubble(4, "I'm Derig. If I'm here, then you found me in the Grotto.")
    bubble(4, "I am old and I saw how they sealed the portal. It was the Rod of Cancellation. It is in another part of the grotto.")
    bubble(4, "I would go get it but I am old and monsters are there in the forest around the grotto.")
    bubble(4, "My daughter will give you the Unadium Coin, which is the key to the rune on the ground in the clearing.")
    set_progress(P_TALKDERIG, 4)
  end
  return
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

  elseif (zn == 21) then
    book_talk(party[0])

  elseif (zn == 22) then
    door_in(62, 54, 52, 41, 69, 57)

  elseif (zn == 23) then
    door_out(37, 56)

  elseif (zn == 24) then
    touch_fire(party[0])

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
        -- You entered grotto and fell in pit (you had NOT spoken to Jen first, though.  Someone helped you get out.
        bubble(en, "You've already been to the grotto.")
        bubble(HERO1, "Yes, we fell down a hole and someone pulled us out.")
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
      elseif (get_progress(P_TALKDERIG) == 4) then
         -- Met Derig
         bubble(en, "Good. Now that you found Derig, here is the Unadium Coin.")
         set_progress(P_UCOIN, 2)
         msg("Unadium coin procured", 255, 0)
      end -- P_TALKDERIG
    elseif (get_progress(P_UCOIN) == 2) then
      -- You now have the Unadium coin
      if (get_progress(P_TALKDERIG) == 4) then
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
      end -- P_TALKDERIG == 4
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
    if (get_progress(P_TALKDERIG) == 4) then
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
            set_progress(P_TALKDERIG, 5)
            msg("Derig takes the Rod of Cancellation and Unadium Coin.")
            bubble(en, "I'll take these back to the grotto for safe keeping. Thank you.")
          end
        end
      end
    elseif (get_progress(P_TALKDERIG) == 5) then
      -- Although TALKDERIG==5, Derig will still be on the screen until you leave Ekla and return
      bubble(en, "I will take these back to the grotto. Thanks again.")
    end -- P_TALKDERIG

  end
end

