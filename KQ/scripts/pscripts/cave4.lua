

-- cave4 - "Cave south of Oracle's tower and west of Denorian village"

--
function autoexec()
  copy_tile_all(39, 92, 21, 46, 5, 8)
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
  if (get_progress(57) > 0) then
    set_ent_active(0, 0)
  end
  if (get_progress(70) == 2) then
    set_obs(37, 66, 0)
    set_zone(37, 66, 2)
  end
  if (not LOC_manor_or_party(2)) then
    --
    set_ent_id(1, 2)
  else
    --
    set_ent_active(1, 0)
  end
  if (get_progress(55) > 2) then
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
    if (get_progress(56) == 0) then
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
      set_progress(56, 1)
    end
  elseif (zn == 3) then
    if (get_progress(56) == 1) then
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
      set_progress(56, 0)
    end
  elseif (zn == 4) then
    if (get_progress(70) == 0) then
      bubble(200, "Locked.")
    elseif (get_progress(70) == 1) then
      bubble(200, "What luck! The key from that crazy Demnas guy unlocks this door!")
      set_progress(70, 2)
      refresh()
    end
  elseif (zn == 5) then
    bubble(200, "Aw... it's full of normal junk.")
  elseif (zn == 6) then
    chest(62, 95, 1)
    refresh()
  elseif (zn == 7) then
    chest(63, 165, 1)
    refresh()
  elseif (zn == 8) then
    chest(64, 106, 1)
    refresh()
  elseif (zn == 9) then
    chest(65, 117, 2)
    refresh()
  elseif (zn == 10) then
    chest(66, 68, 1)
    refresh()
  elseif (zn == 11) then
    --
    copy_tile_all(45, 92, 21, 46, 5, 8)
    view_range(1, 20, 46, 26, 54)
    warpm("ustairs", 8)
    --
  elseif (zn == 12) then
    autoexec()
    view_range(0, 0, 0, 0, 0)
    warpm("dstairs", 8)
  elseif (zn == 13) then
    if (get_progress(55) < 4) then
      bubble(200, "The Denorians were right. There really WAS a troll. Looks like it's already dead, though.")
      msg("The troll suddenly lunges at you... it was only asleep!", 0, 0)
      set_run(0)
      msg("TODO: There needs to be a combat here...", 0, 0)
      set_run(1)
      bubble(200, "If this troll really DID steal the statue from the Denorians, it was probably being directed by that scumbag Demnas.")
      bubble(200, "He used this poor bugger and then left him here to rot.")
      wait(100)
      bubble(200, "Well, I should go back to the village now and report all this.")
      set_progress(55, 4)
    else
      bubble(200, "This cell is now his tomb.")
    end
  elseif (zn == 14) then
    -- Nothing: no random battles here
  elseif (zn == 15) then
    if (get_progress(55) > 2) then
      msg("The statue is no longer here.", 58, 0)
    else
      bubble(200, "This statue of the Oracle appears to be broken in half!")
    end
  end
end
function entity_handler(en)
  if (en == 0) then
    LOC_talk_demnas(en)
  elseif (en == 1) then
    -- You have not spoken to Corin about the troll
    if (get_progress(109) == 0) then
      if (get_progress(55) < 4) then
        -- You have not yet faced the troll down the stairs
        bubble(en, "Oh, $0, it's great to see you!")
        bubble(en, "I traced the missing statue here to Demnas, but before I could confront him, a horde of Malkaron's followers attacked him.")
        bubble(200, "Malkaron's minions have been here?")
        bubble(en, "Yes, and during the battle, the statue was broken in half and Malkaron's men took off with it.")
        bubble(en, "Demnas was so enraged that when he found me, he locked me in this room with this troll.")
        bubble(200, "A troll? So there really IS a troll? How did you defeat it?")
        bubble(en, "I haven't. I hit it with a sleep spell as soon as I was locked in here.")
        set_ent_facing(en, 3)
        bubble(en, "It's down the stairs right there. Why don't you go take care of it now?")
        set_progress(109, 1)
      else
        -- You have defeated the troll
        bubble(en, "$0! I saw you run down the stairs there before I could tell you about the troll!")
        bubble(200, "Yea, the troll was asleep, but it wasn't much of a problem to beat.")
        bubble(en, "My, my. So modest.")
        if (get_numchrs() == 1) then
          bubble(200, "Well, what can I say? I'm just that good!")
        else
          bubble(201, "Well, what can we say? We're just that good!")
        end
        bubble(en, "If that's so, then let me join your party!")
        LOC_join_corin(en)
      end
    -- Corin told you about the troll
    elseif (get_progress(109) == 1) then
      -- You have not fought the troll yet
      if (get_progress(55) < 4) then
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
    set_ent_facing(200, 0)
    LOC_talk_demnas(en)
  end
end
function LOC_join_corin(en)
  local id
  --
  set_all_equip(2, 2, 38, 50, 56, 74, 0)
  id = select_team{2}
  --
  add_to_manor(id)
  if (id[1]) then
    set_ent_id(en, id[1])
    set_ent_speed(en, 4)
    if (id[2]) then
      -- Two heroes were de-selected
      set_ent_id(0, id[2])
      set_ent_active(0, 1)
      set_ent_speed(0, 4)
      if (get_ent_tiley(200) > get_ent_tiley(1)) then
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
      move_entity(0, 37, 73, 1)
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
  set_progress(100, get_progress(100) + 1)
end
function LOC_talk_demnas(en)
  bubble(en, "So you're back trying to steal the other half of the statue from me, are you?")
  bubble(200, "Huh? Have we met before?")
  bubble(en, "Filthy swine! You cannot have this statue!")
  bubble(200, "Does this mean you're the one responsible for the disappearance of the statue from a village near here?")
  bubble(en, "Uh... Are... Are you accusing me of stealing this statue?!")
  bubble(200, "Well, the thief was traced back to this cave.")
  bubble(en, "Oh, so now I'm a thief?!!")
  bubble(200, "That depends... did you take the statue or not?")
  bubble(en, "What if I did?")
  bubble(200, "Well, then you will give it back... or it will be taken by force!")
  bubble(en, "That has been tried before already. You prove to be as stupid as you look.")
  bubble(en, "Now I, Demnas, will destroy you!")
  drawmap()
  screen_dump()
  set_run(0)
  combat(11)
  set_run(1)
  if (get_alldead() == 1) then
    return
  end
  set_progress(57, 1)
  refresh()
  drawmap()
  screen_dump()
  sfx(5)
  msg("Bronze key procured", 255, 0)
  set_progress(70, 1)
  sfx(5)
  msg("Broken Denorian Statue procured", 255, 0)
  set_progress(55, 3)
  refresh()
end
