-- temple1 - "First part of temple north of Andra"

-- /*
-- {
-- P_ALTARSWITCH: Whether the switch on the alter has been flipped
--   0 - Not yet
--   1 - Switched; stairs showing
--
-- P_GOBLINITEM:
--
-- P_PLAYERS:
--
-- P_TALK_TEMMIN: Whether you have spoken to Temmin when you entered the map
--   0 - Haven't spoken to him
--   1 - You spoken to him at least once already
--
-- P_UNDEADJEWEL: Whether you collected the Jewel needed to seal the portal
--   0 - Don't have it
--   1 - Have it
-- }
-- */




function autoexec()
  if not LOC_manor_or_party(TEMMIN) then
    -- // Make the NPC look like Temmin if he hasn't been recruited yet
    set_ent_id(38, TEMMIN)
  else
    -- // Otherwise, remove him from screen
    set_ent_active(38, 0)
  end

  refresh()
end


function refresh()
  if (get_progress(P_ALTARSWITCH) == 1) then
    set_ftile(54, 14, 0)
    set_btile(54, 15, 29)
    set_obs(54, 15, 0)
    set_ftile(53, 14, 118)
    set_btile(53, 15, 119)
    set_obs(53, 15, 1)
    set_zone(54, 15, 9)
    set_ftile(24, 12, 154)
    set_btile(24, 13, 156)
    set_obs(24, 12, 0)
    set_zone(24, 12, 12)
  end
  if (get_progress(P_UNDEADJEWEL) > 0) then
    set_mtile(30, 42, 65)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 268, 12, 268, 12)

  elseif (zn == 2) then
    sfx(26)
    set_zone(18, 34, 0)
    set_ftile(18, 34, 154)
    set_btile(18, 35, 156)
    set_obs(18, 34, 0)

  elseif (zn == 3) then
    sfx(26)
    set_zone(18, 29, 0)
    set_ftile(18, 29, 154)
    set_btile(18, 30, 156)
    set_obs(18, 29, 0)

  elseif (zn == 4) then
    sfx(26)
    set_zone(36, 41, 0)
    set_ftile(36, 41, 154)
    set_btile(36, 42, 156)
    set_obs(36, 41, 0)

  elseif (zn == 5) then
    sfx(26)
    set_zone(36, 37, 0)
     set_ftile(36, 37, 154)
    set_btile(36, 38, 156)
    set_obs(36, 37, 0)

  elseif (zn == 6) then
    sfx(26)
    set_zone(31, 20, 0)
    set_ftile(31, 20, 154)
    set_btile(31, 21, 156)
    set_obs(31, 20, 0)

  elseif (zn == 7) then
    sfx(26)
    set_ftile(24, 12, 154)
    set_btile(24, 13, 156)
    set_obs(24, 12, 0)
    set_zone(24, 12, 12)

  elseif (zn == 8) then
    if (get_progress(P_ALTARSWITCH) == 0) then
      bubble(HERO1, "A switch!")
      sfx(26)
      set_progress(P_ALTARSWITCH, 1)
      refresh()
    end

  elseif (zn == 9) then
    warp(56, 44, 8)

  elseif (zn == 10) then
    change_map("temple2", 0, 0, 0, 0)

  elseif (zn == 11) then
    bubble(HERO1, "Locked.")

  elseif (zn == 12) then
    warp(58, 19, 8)

  elseif (zn == 13) then
    warp(24, 13, 8)

  elseif (zn == 14) then
    warp(54, 15, 8)

  elseif (zn == 15) then
    if (get_progress(P_GOBLINITEM) == 0) then
      combat(51)
    end

  elseif (zn == 16) then
    if (get_progress(P_UNDEADJEWEL) == 0) then
      set_progress(P_UNDEADJEWEL, 1)
      sfx(5)
      msg("Goblin jewel procured", 255, 0)
      refresh()
    end

  end
end


function entity_handler(en)
  if (en >= 0 and en <= 3) then
    bubble(en, "We are the Special Forces team.")

  elseif (en >= 4 and en <= 7) then
    bubble(en, "We were relieved of duty while the monks guard the alter room.")

  elseif (en >= 8 and en <= 11) then
    bubble(en, "We are guards from Andra, helping defend the temple.")

  elseif (en >= 12 and en <= 15) then
    bubble(en, "I'm famished. We just finished beating back a bunch of monsters.")

  elseif (en == 16) then
    bubble(en, "I have much to do. Please excuse me.")

  elseif (en == 17) then
    bubble(en, "These soldiers are quite hungry. I am getting them food.")

  elseif (en == 18) then
    bubble(en, "I must rush. I haven't enough time to talk with you now.")

  elseif (en == 19) then
    bubble(en, "We have to listen to his lessons. I can't talk to you now.")

  elseif (en == 20) then
    bubble(en, "I wish I could go beat the bad guys! This school is boring.")

  elseif (en == 21) then
    bubble(en, "Those monks lock themselves away until there's a problem. I haven't heard them say a word all week!")

  elseif (en == 22) then
    bubble(en, "I was asked to teach these children, so as to distract them from the monster threat. Pardon me.")

  elseif (en >= 23 and en <= 29) then
    bubble(en, "...")
    thought(HERO1, "They appear to be in deep meditation.")

  elseif (en == 30) then
    bubble(en, "Zzz...")

  elseif (en == 31) then
    bubble(en, "...snore...")

  elseif (en == 32) then
    bubble(en, "Yawn... I'm quite tired. Go away.")

  elseif (en == 33) then
    bubble(en, "Grrxx... zzz...")

  elseif (en == 34) then
    bubble(en, "Not now. I'm resting.")

  elseif (en == 35) then
    bubble(en, "...zzz...")

  elseif (en == 36) then
    bubble(en, "...Guarding the... ...zzz... Gotta defend the... zzz...")

  elseif (en == 37) then
    bubble(en, "This door is drafty. I can't get to sleep.")

  elseif (en == 38) then
    LOC_join_temmin(en)

  end
end


function LOC_join_temmin(en)
  local id
  if (get_progress(P_PORTAL2GONE) == 0) then
    if (get_progress(P_TALK_TEMMIN) == 0) then
      bubble(HERO1, "Hello! What are you doing here?")
      bubble(en, "I am helping the monks defend the stairway. Monsters have been coming up from the caverns below.")
      bubble(HERO1, "These monks don't look like they would be very useful in a fight.")
      bubble(en, "They don't fight; I do. I protect them, and fight the monsters, and they in turn heal me when I need it.")
      bubble(HERO1, "Hmm... sounds like a plan.")
      set_progress(P_TALK_TEMMIN, 1)
    elseif (get_progress(P_TALK_TEMMIN) == 1) then
      bubble(en, "I am very protective of my friends.")
    end
  else
    if (get_progress(P_TALK_TEMMIN) == 0) then
      bubble(en, "Ah, $0. I was worried for a while. The monsters were getting very strong.")
      if (get_numchrs() == 1) then
        bubble(HERO1, "I sealed the monster's portal. I don't think there should be any more of a threat.")
      else
        bubble(HERO1, "We sealed the monster's portal. We don't think there should be any more of a threat.")
      end
      set_progress(P_TALK_TEMMIN, 1)
    else
      bubble(HERO1, "Finished. The monster's portal is sealed now.")
    end
    bubble(en, "Great! Can I offer my services?")

    -- // Give Temmin his default equipment
    set_all_equip(TEMMIN, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0)
    id = select_team{TEMMIN}
    -- // Add the characters that weren't selected to the manor
    add_to_manor(id)

    if (id[1]) then
      set_ent_id(en, id[1])
    end
    set_ent_active(en, 0)
    set_progress(P_PLAYERS, get_progress(P_PLAYERS) + 1)
  end
end