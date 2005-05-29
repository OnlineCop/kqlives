-- cave4 - "Cave south of Oracle's tower and west of Denorian village"

-- /*
-- {
-- P_BRONZEKEY: Key into the room with Corin and the troll
--   0 - Don't have the key yet
--   1 - Got the key from Demnas
--   2 - Used the key to open the bronze door
--
-- P_C4DOORSOPEN: See-through ceilings or not
--   0 - Ceilings visible
--   1 - Ceilings invisible
--
-- P_DEMNASDEAD: Whether you defeated Demnas
--   0 - Still kickin'
--   1 - Dead
--
-- P_DENORIAN: Status of Denorian's statue
--   0..2 - Setup in dville.lua
--   3 - You have the Oracle's broken statue, haven't met troll
--   4 - Defeated troll
--
-- P_TALK_CORIN: If you've spoken to Corin
--   0 - Haven't spoken to him yet
--   1 - He tells you about the connection of the troll and Malkaron's armies
--
-- }
-- */

function autoexec()
  refresh()
end


function refresh()
  if (get_treasure(62) == 1) then
    set_btile(45, 51, 256)
  end
  if (get_treasure(63) == 1) then
    set_btile(43, 35, 256)
  end
  if (get_treasure(64) == 1) then
    set_btile(36, 17, 256)
  end
  if (get_treasure(65) == 1) then
    set_btile(16, 35, 256)
  end
  if (get_treasure(66) == 1) then
    set_btile(36, 60, 256)
  end

  if (get_progress(P_DEMNASDEAD) > 0) then
    set_ent_active(0, 0)
  end

  if (get_progress(P_BRONZEKEY) == 2) then
    set_obs(37, 66, 0)
    set_zone(37, 66, 2)
  end

  if (not LOC_manor_or_party(CORIN)) then
    -- // Make the NPC look like Corin if he hasn't been recruited yet
    set_ent_id(1, CORIN)
  else
    -- // Otherwise, remove him from the screen
    set_ent_active(1, 0)
  end

  if (get_progress(P_DENORIAN) > 2) then
    set_ent_active(2, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 0) then
    combat(10)

  elseif (zn == 1) then
    change_map("main", 244, 67, 244, 67)

  elseif (zn == 2) then
    if (get_progress(P_C4DOORSOPEN) == 0) then
      set_foreground(0)
      set_btile(5, 9, 240)
      set_btile(34, 20, 240)
      set_btile(13, 64, 240)
      set_btile(37, 66, 240)
      set_btile(18, 80, 240)
      set_btile(13, 73, 240)
      set_btile(13, 53, 240)
      set_btile(14, 32, 240)
      set_mtile(5, 9, 251)
      set_mtile(34, 20, 251)
      set_mtile(13, 64, 251)
      set_mtile(37, 66, 251)
      set_mtile(18, 80, 251)
      set_mtile(13, 73, 251)
      set_mtile(13, 53, 251)
      set_mtile(14, 32, 251)
      sfx(26)
      set_progress(P_C4DOORSOPEN, 1)
    end

  elseif (zn == 3) then
    if (get_progress(P_C4DOORSOPEN) == 1) then
      set_btile(5, 9, 250)
      set_btile(34, 20, 250)
      set_btile(13, 64, 250)
      set_btile(37, 66, 250)
      set_btile(18, 80, 250)
      set_btile(13, 73, 250)
      set_btile(13, 53, 250)
      set_btile(14, 32, 250)
      set_mtile(5, 9, 0)
      set_mtile(34, 20, 0)
      set_mtile(13, 64, 0)
      set_mtile(37, 66, 0)
      set_mtile(18, 80, 0)
      set_mtile(13, 73, 0)
      set_mtile(13, 53, 0)
      set_mtile(14, 32, 0)
      sfx(26)
      set_foreground(1)
      set_progress(P_C4DOORSOPEN, 0)
    end

  elseif (zn == 4) then
    if (get_progress(P_BRONZEKEY) == 0) then
      bubble(HERO1, "Locked.")
    elseif (get_progress(P_BRONZEKEY) == 1) then
      bubble(HERO1, "What luck! The key from that crazy Demnas guy unlocks this door!")
      set_progress(P_BRONZEKEY, 2)
      refresh()
    end

  elseif (zn == 5) then
    bubble(HERO1, "Aw... it's full of normal junk.")

  elseif (zn == 6) then
    chest(62, I_SHADECLOAK, 1)
    refresh()

  elseif (zn == 7) then
    chest(63, I_B_GLOOM, 1)
    refresh()

  elseif (zn == 8) then
    chest(64, I_STRSEED, 1)
    refresh()

  elseif (zn == 9) then
    chest(65, I_SALVE, 2)
    refresh()

  elseif (zn == 10) then
    chest(66, I_ARMOR4, 1)
    refresh()

  elseif (zn == 11) then
    warp(48, 98, 8)

  elseif (zn == 12) then
    warp(39, 61, 8)

  elseif (zn == 13) then
    if (get_progress(P_DENORIAN) == 3) then
      bubble(HERO1, "The Denorians were right. There really WAS a troll. Looks like it's already dead, though.")
      msg("The troll suddenly lunges at you... it was only asleep!", 0, 0)
      set_run(0)
      msg("There needs to be a combat here...", 0, 0)
      set_run(1)

      bubble(HERO1, "If this troll really DID steal the statue from the Denorians, it was probably being directed by that scumbag Demnas.")
      bubble(HERO1, "He used this poor bugger and then left him here to rot.")
      wait(100)
      bubble(HERO1, "Well, I should go back to the village now and report all this.")
      set_progress(P_DENORIAN, 4)
    else
      bubble(HERO1, "This cell is now his tomb.")
    end

  elseif (zn == 14) then
    -- Nothing: no random battles here

  elseif (zn == 15) then
    if (get_progress(P_DENORIAN) > 2) then
      msg("The statue is no longer here.", 58, 0)
    else
      bubble(HERO1, "This statue of the Oracle appears to be broken in half!")
    end
  end
end


function entity_handler(en)
  if (en == 0) then
    LOC_talk_demnas(en)

  elseif (en == 1) then
    -- You have not spoken to Corin about the troll
    if (get_progress(P_TALK_CORIN) == 0) then
      if (get_progress(P_DENORIAN) < 4) then
        -- You have not yet faced the troll down the stairs
        bubble(en, "Oh, $0, it's great to see you!")
        bubble(en, "I traced the missing statue here to Demnas, but before I could confront him, a horde of Malkaron's followers attacked him.")
        bubble(HERO1, "Malkaron's minions have been here?")
        bubble(en, "Yes, and during the battle, the statue was broken in half and Malkaron's men took off with it.")
        bubble(en, "Demnas was so enraged that when he found me, he locked me in this room with this troll.")
        bubble(HERO1, "A troll? So there really IS a troll? How did you defeat it?")
        bubble(en, "I haven't. I hit it with a sleep spell as soon as I was locked in here.")
        set_ent_facing(en, FACE_RIGHT)
        bubble(en, "It's down the stairs right there. Why don't you go take care of it now?")
        set_progress(P_TALK_CORIN, 1)
      else
        -- You have defeated the troll
        bubble(en, "$0! I saw you run down the stairs there before I could tell you about the troll!")
        bubble(HERO1, "Yea, the troll was asleep, but it wasn't much of a problem to beat.")
        bubble(en, "My, my. So modest.")
        bubble(HERO1, "Well, what can I say? I'm just that good!")
        bubble(en, "If that's so, then let me join your party!")
        LOC_join_corin(en)
      end
    -- Corin told you about the troll
    elseif (get_progress(P_TALK_CORIN) == 1) then
      -- You have not fought the troll yet
      if (get_progress(P_DENORIAN) < 4) then
        bubble(en, "I'll wait up here. I'm not ready to take on a troll just yet.")
      -- You fought the troll
      else
        bubble(en, "Good work! Let me join your team!")
        LOC_join_corin(en)
      end
    else
      bubble(en, "Hi, $0. Let me join your team!")
      LOC_join_corin(en)
    end
    refresh()

  elseif (en == 2) then
    -- Statue will not talk, but Demnas attacks if you try to grab it
    en = 0
    bubble(en, "Not so fast!")
    set_ent_facing(HERO1, FACE_DOWN)
    LOC_talk_demnas(en)

  end
end


function LOC_join_corin(en)
  local id

  -- // Give Corin him default equipment
  set_all_equip(CORIN, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0)
  id = select_team{CORIN}
  -- // Add the characters that weren't selected to the manor
  add_to_manor(id)

  if (id[1]) then
    set_ent_id(en, id[1])
    set_ent_speed(en, 4)

    if (id[2]) then
      -- Two heroes were de-selected
      set_ent_id(0, id[2])
      set_ent_active(0, 1)
      set_ent_speed(0, 4)
      if (get_ent_tiley(HERO1) > get_ent_tiley(1)) then
        -- Hero is below Corin
        set_ent_tilex(0, get_ent_tilex(en) + 1)
        set_ent_tiley(0, get_ent_tiley(en))
      else
        -- Hero is anywhere else
        set_ent_tilex(0, get_ent_tilex(en))
        set_ent_tiley(0, get_ent_tiley(en) + 1)
      end

      bubble(en, "If you need us, we'll be back at the manor.")

      set_ent_movemode(0, 2)
      set_ent_movemode(en, 2)
      move_entity(0,  37, 73, 1)
      move_entity(en, 37, 73, 1)
      wait_for_entity(0, en)
    else
      -- One hero was de-selected
      bubble(en, "If you need me, I'll be back at the manor.")
      set_ent_movemode(en, 2)
      move_entity(en, 37, 73, 1)
      wait_for_entity(en, en)
    end
  end
  set_progress(P_PLAYERS, get_progress(P_PLAYERS) + 1)

end


function LOC_talk_demnas(en)
  bubble(en, "So you're back trying to steal the other half of the statue from me, are you?")
  bubble(HERO1, "Huh? Have we met before?")
  bubble(en, "Filthy swine! You cannot have this statue!")
  bubble(HERO1, "Does this mean you're the one responsible for the disappearance of the statue from a village near here?")
  bubble(en, "Are you accusing me of stealing this statue?")
  bubble(HERO1, "Well, the thief was traced back to this cave.")
  bubble(en, "Oh, so now I'm a thief?!!")
  bubble(HERO1, "That depends... did you take the statue or not?")
  bubble(en, "What if I did?")
  bubble(HERO1, "Well, then you will give it back... or it will be taken by force!")
  bubble(en, "That has been tried before already. You proven to be as stupid as you look.")
  bubble(en, "Now I, Demnas, will destroy you!")
  drawmap()
  screen_dump()
  set_run(0)
  combat(11)
  set_run(1)
  if (get_alldead() == 1) then
    return
  end
  set_progress(P_DEMNASDEAD, 1)
  refresh()
  drawmap()
  screen_dump()

  sfx(5)
  msg("Bronze key procured", 255, 0)
  set_progress(P_BRONZEKEY, 1)

  sfx(5)
  msg("Broken Denorian Statue procured", 255, 0)
  set_progress(P_DENORIAN, 3)
  refresh()
end
