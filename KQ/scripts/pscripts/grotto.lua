

-- grotto - "Small forest grotto north of Ekla"

--
function autoexec()
  -- You have fallen in the pit at least once; the hole is showing on the map
  if (get_progress(11) > 0) then
    set_btile(16, 14, 153)
  end
  --
  if (get_progress(9) == 0) then
    set_progress(9, 1)
  end
  -- Determine if the fire should be lit or not
  if (get_progress(9) < 2) then
    set_mtile(20, 16, 154)
    set_zone(20, 16, 0)
  end
  -- Allows you to fall down the pit once Derig is back in the Grotto
  if (get_progress(9) == 6) then
    set_progress(11, 3)
  end
  --
  if (get_progress(11) == 2) then
    set_obs(16, 14, 1)
  end
  -- Determine if Derig should be showing or not
  if (get_progress(9) == 3 or
      get_progress(9) == 6) then
    -- Derig WILL be in the Grotto in these two instances
  else
    --
    if (get_progress(108) == 0 or
        get_progress(108) > 2) then
      set_ent_active(0, 0)
    else
      -- Light the fire if Derig is next to it
      set_mtile(20, 16, 154)
      set_zone(20, 16, 0)
    end
  end
  refresh()
end
function refresh()
  -- Treasure on NE corner, in trees
  if (get_treasure(15) == 1) then
    set_obs(24, 16, 0)
    set_zone(24, 16, 0)
  end
  -- Treasure under flowers on E corner
  if (get_treasure(80) == 1) then
    set_obs(26, 10, 0)
    set_zone(26, 10, 0)
  end
end
function postexec()
  return
end
function zone_handler(zn)
  -- Grotto entrance/exit
  if (zn == 1) then
    change_map("main", 129, 19, 129, 19)
  -- Campfire
  elseif (zn == 2) then
    if (get_progress(9) == 2) then
      bubble(200, "That's strange. I wonder who lit this fire?")
    else
      touch_fire(party[0])
    end
  -- Pit
  elseif (zn == 3) then
    if (get_progress(11) == 0) then
      set_btile(16, 14, 153)
      bubble(200, "Uh oh!")
    elseif (get_progress(11) == 2) then
    -- if (get_progress(9) == 4) or (get_progress(9) == 5) then
      bubble(200, "I'd rather not go down there right now.")
      return
    end
    change_map("cave2", 0, 0, 0, 0)
  -- Treasure under flowers on E corner
  elseif (zn == 4) then
    chest(15, 124, 1)
    refresh()
  -- Rune
  elseif (zn == 5) then
    if (get_progress(36) == 0) then
      bubble(200, "Hmm... this seems like it should do something. But what?")
    elseif (get_progress(36) == 1) then
      bubble(200, "Wow. The rune hummed for a second but now it's quiet.")
    elseif (get_progress(36) == 2) then
        change_map("grotto2", 0, 0, 0, 0)
    else
      bubble(200, "I can't go through here anymore since I no longer have the Unadium Coin.")
    end
  -- Treasure on NE corner, in trees
  elseif (zn == 6) then
    chest(80, 1, 1)
    refresh()
  end
end
function entity_handler(en)
  if (en == 0) then
    -- We've never spoken to Tsorin, or we've finished his quest already
    if (get_progress(108) == 0 or
        get_progress(108) > 2) then
      -- TALKDERIG will always == 3 the first time you talk to him
      if (get_progress(9) == 3) then
        if (get_progress(108) > 2) then
          -- You have spoken to Derig because of Tsorin
          bubble(en, "Thank you for your assistance, $0. I have another request for you.")
        else
          -- You have never spoken to him before
          bubble(en, "Hello, I'm Derig. I presume that my granddaughter sent you here?")
        end
        bubble(en, "Let's go back to town.")
        set_progress(11, 2)
        set_progress(9, 4)
        change_mapm("town1", "by_derig")
        -- change_map("town1", 65, 52, 65, 52)
      elseif (get_progress(9) == 6) then
        --
        bubble(en, "Good job with the portal. I have returned the Rod of Cancellation.")
      end
    elseif (get_progress(108) == 1) then
      bubble(en, "That note you're carrying... I recognize the seal on there; let me see it!")
      msg("You show Derig the note.", 255, 0)
      bubble(en, "$0, Tsorin says that a special treasure of the goblins, the Oracle Statue, has been stolen.")
      bubble(en, "Apparently, the goblins are so upset that he's sealed off any entrance to the goblin lands until this is resolved.")
      if (get_numchrs() == 1) then
        bubble(200, "So what am I supposed to do?")
      else
        bubble(200, "So what are we supposed to do?")
      end
      bubble(en, "I agree that you must get through there. Here, deliver this note to Tsorin so he'll let you proceed.")
      msg("Derig hands you a sealed envelope.", 18, 0)
      set_progress(108, 2)
      set_progress(9, 3)
    elseif (get_progress(108) == 2) then
      bubble(en, "You must deliver that note to Tsorin so he'll let you through the fort.")
    else
      bubble(en, "Good luck.")
    end
  end
end
