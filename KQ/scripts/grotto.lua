-- grotto - "Small forest grotto north of Ekla"

-- /*
-- P_EARLYPROGRESS: Used when talking to Derig in the Grotto.
--   0 - Have not yet entered Ekla
--   1 - Entered Ekla
--   2 - Entered Randen
--   3 - Entered Andra
-- P_FELLINPIT: Set when you fall down the pit
--   0 - Haven't fallen down the pit
--   1 - Fell down the pit, haven't spoken to Derig
--   2 - Fell down the pit, Derig helped you out
--   3 - You may fall down the pit again
-- P_TALKDERIG: Set when you make contact with Derig
--   0 - If you've never entered the grotto.  This will be set to 1 immediately when you enter.
--   1 - Set as soon as you've been to the grotto at least once
--   2 - Fell down the pit, haven't met Derig
--   3 - Fell down the pit, met Derig.
--   4 - Derig is in Ekla with Jen, the granddaugther
--   5 - Derig is back, you have sealed the portal
-- P_UCOIN: Spoke to Jen, the granddaughter in Ekla
--   0 - Have not yet spoken with Jen
--   1 - Spoke to Jen
--   2 - Jen gave you Unadium coin
--   3 - Returned coin to Derig
--
--   OLD:
--   01 - Declined her offer to help get rid of Tunnel Portal
--   02 - Accepted her offer to help
--   03 - Received Coin (unnecessary, since it immediately follows 2)
--   04 - Returned Coin; received SunStone
-- */


function autoexec()
  -- // Treasure on NE corner, in trees
  if (get_treasure(15) == 1) then
    set_obs(24, 16, 0)
  end

  -- // Treasure under flowers on E corner
  if (get_treasure(80) == 1) then
    set_obs(26, 10, 0)
  end

  -- // You have fallen in the pit at least once; the hole is showing on the map
  if (get_progress(P_FELLINPIT) > 0) then
    set_btile(16, 14, 153)
  end

  -- // You've entered the grotto at least once
  if (get_progress(P_TALKDERIG) < 2) then
    set_progress(P_TALKDERIG, 1)
    -- // Remove Derig from the screen: The fire is out
    set_ent_active(0, 0)
    set_ftile(20, 16, 154)
    set_zone(20, 16, 0)
  elseif (get_progress(P_TALKDERIG) == 2) or (get_progress(P_TALKDERIG) == 4) then
    -- // Remove Derig from the screen: The fire will be lit, though
    set_ent_active(0, 0)
  end

end


function postexec()
  return
end


function zone_handler(zn)
  -- // Grotto entrance/exit
  if (zn == 1) then
    change_map("main", 129, 19, 129, 19)

  elseif (zn == 2) then
    if (get_progress(P_TALKDERIG) == 2) then
      bubble(HERO1, "That's strange. I wonder who lit this fire?")
    else
      touch_fire(party[0])
    end

  elseif (zn == 3) then
    if (get_progress(P_TALKDERIG) == 4) then
      bubble(HERO1, "I'd rather not go down there right now.")
      return
    end

    if (get_progress(P_FELLINPIT) == 0) then
      set_btile(16, 14, 153)
      bubble(HERO1, "Uh oh!")
    end
    change_map("cave2", 0, 0, 0, 0)

  elseif (zn == 4) then
    chest(15, I_ERUNE, 1)
    set_obs(24, 16, 0)

  elseif (zn == 5) then
    if (get_progress(P_UCOIN) == 0) then
      bubble(HERO1, "Hmm... this seems like it should do something. But what?")
    elseif (get_progress(P_UCOIN) < 2) then
      bubble(HERO1, "Wow. The rune hummed for a second but now it's quiet.")
    elseif (get_progress(P_UCOIN) == 2) then
        change_map("grotto2", 0, 0, 0, 0)
    else
      bubble(HERO1, "Looks like the rune is sealed again. And I no longer have the Unadium coin.")
    end

  elseif (zn == 6) then
    chest(80, I_MACE1, 1)
    set_obs(26, 10, 0)

  end
end


function entity_handler(en)
  if (en == 0) then
    -- // TALKDERIG will always == 3 the first time you talk to him
    if (get_progress(P_TALKDERIG) == 3) then
      -- // You have never spoken to him before
      bubble(en, "Hello, I'm Derig. Let's go back to the town and get you the Unadium coin.")
      change_map("town1", 65, 52, 65, 52)
    elseif (get_progress(P_TALKDERIG) == 5) then
      -- // Derig would only be here after you've finished with the Rod of Cancellation
      bubble(en, "Good job with the portal. I have returned the Rod of Cancellation.")
    end

  end
end
