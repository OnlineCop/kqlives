

-- temple1 - "First part of temple north of Andra"

--
function autoexec()
  local b
  if (not LOC_manor_or_party(5) and get_progress(45) == 3) then
    --
    set_ent_id(41, 5)
  else
    --
    set_ent_active(41, 0)
  end
  if (get_progress(45) > 0) then
    set_ent_tilex(38, 23)
  end
  set_ent_active(42, 0)
  if (get_progress(45) == 4) then
    for b = 23, 29, 1 do
      set_ent_active(b, 0)
    end
    for b = 38, 40, 1 do
      set_ent_active(b, 0)
    end
  end
  refresh()
end
function refresh()
  if (get_progress(15) == 1) then
    set_ftile(54, 14, 0)
    set_btile(54, 15, 29)
    set_obs(54, 15, 0)
    set_ftile(53, 14, 118)
    set_btile(53, 15, 119)
    set_obs(53, 15, 1)
    set_zone(54, 15, 9)
  end
  if (get_progress(35) > 0) then
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
    if (get_progress(15) == 0) then
      bubble(200, "A switch!")
      sfx(26)
      set_progress(15, 1)
      refresh()
    end
  elseif (zn == 9) then
    warp(56, 44, 8)
  elseif (zn == 10) then
    change_map("temple2", 0, 0, 0, 0)
  elseif (zn == 11) then
    bubble(200, "Locked.")
  elseif (zn == 12) then
    warp(58, 19, 8)
  elseif (zn == 13) then
    set_ftile(24, 12, 154)
    set_btile(24, 13, 156)
    set_obs(24, 12, 0)
    set_zone(24, 12, 12)
    warp(24, 13, 8)
  elseif (zn == 14) then
    warp(54, 15, 8)
  elseif (zn == 15) then
    if (get_progress(17) == 0) then
      combat(51)
    end
  end
end
function entity_handler(en)
  if (en >= 0 and en <= 3) then
    bubble(en, "We are the Special Forces team.")
  elseif (en >= 4 and en <= 7) then
    bubble(en, "We were relieved of duty while the monks guard the alter room.")
  elseif (en >= 8 and en <= 11) then
    if (get_progress(17) == 0) then
      bubble(en, "We are guards from Andra, helping defend the temple.")
    else
      bubble(en, "I'll guess we'll be going back to Andra soon.")
    end
  elseif (en >= 12 and en <= 15) then
    if (get_progress(17) == 0) then
      bubble(en, "I'm famished. We just finished beating back a bunch of monsters.")
    else
      bubble(en, "Things are finally get back to normal around here.")
    end
  elseif (en == 16) then
    bubble(en, "I have much to do. Please excuse me.")
  elseif (en == 17) then
    bubble(en, "These soldiers are quite hungry. I am getting them food.")
  elseif (en == 18) then
    bubble(en, "I must rush. I haven't enough time to talk with you now.")
  elseif (en == 19) then
    bubble(en, "We have to listen to his lessons. I can't talk to you now.")
  elseif (en == 20) then
    if (get_progress(17) == 0) then
      bubble(en, "I wish I could go beat the bad guys! This school is boring.")
    else
      bubble(en, "This guy just goes on and on.")
    end
  elseif (en == 21) then
    bubble(en, "Those monks lock themselves away until there's a problem. I haven't heard them say a word all week!")
  elseif (en == 22) then
    if (get_progress(17) == 0) then
      bubble(en, "I was asked to teach these children, so as to distract them from the monster threat. Pardon me.")
    else
      bubble(en, "These children simply won't pay attention.")
    end
  elseif ((en >= 23 and en <= 29) or en == 39 or en == 40) then
    bubble(en, "...")
    thought(200, "They appear to be in deep meditation.")
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
    if (get_progress(17) == 0) then
      bubble(en, "...Guarding the... ...zzz... Gotta defend the... zzz...")
    else
      bubble(en, "...zzz... must sleep...")
    end
  elseif (en == 37) then
    bubble(en, "This door is drafty. I can't get to sleep.")
  elseif (en == 38) then
    if (get_progress(45) == 0) then
      bubble(en, "You should not enter this section of the Temple. It is dangerous.")
      if (get_numchrs() > 1) then
        bubble(200, "We're here to help.")
      else
        bubble(200, "I'm here to help.")
      end
      bubble(en, "A young man has already gone in with the Goblin Jewel. He believes it has something to do with the problem.")
      bubble(200, "Who was this young man and what exactly is the problem?")
      bubble(en, "The young man's name was Temmin and the problem is that hordes of undead spirits have risen from beneath the Temple.")
      if (get_numchrs() > 1) then
        bubble(200, "We know this Temmin fellow, and we can help him.")
      else
        bubble(200, "I know this Temmin fellow, and I can help him.")
      end
      bubble(en, "Well, I see no real point in arguing. Go ahead.")
      bubble(200, "Thank you.")
      set_progress(15, 1)
      refresh()
      set_ent_script(38, "L1F3")
      wait_for_entity(38, 38)
      set_progress(45, 1)
    elseif (get_progress(45) < 3) then
      bubble(en, "The spirits are restless.")
    else
      bubble(en, "The spirits are at peace.")
    end
  elseif (en == 41) then
    LOC_join_temmin(en)
  end
end
function LOC_join_temmin(en)
  local id, a
  bubble(en, "Welcome back, you have done a great thing. Your bravery outshines even my cowardice.")
  bubble(200, "Don't sell yourself short. After all, you're the one who figured out that the Goblin Jewel was the source of the spirits' unrest.")
  bubble(en, "That was the easy part. When courage was truly needed, I ran.")
  bubble(200, "Well, I guess that's for you to come to grips with then.")
  bubble(en, "Indeed. For now, I am at your service.")
  --
  set_all_equip(5, 10, 39, 51, 67, 80, 0)
  a = get_party_xp(get_pidx(0))
  a = a * 80 / 100
  if (a > 3331) then
    give_xp(5, 3331 + krnd(20), 1)
  else
    give_xp(5, a + krnd(20), 1)
  end
  drawmap()
  id = select_team{5}
  --
  add_to_manor(id)
  if (id[1]) then
    set_ent_id(en, id[1])
    set_ent_obsmode(en, 0)
    if (id[2]) then
        --
      set_ent_obsmode(42, 0)
      set_ent_id(42, id[2])
      set_ent_active(42, 1)
      set_ent_tilex(42, get_ent_tilex(en))
      set_ent_tiley(42, get_ent_tiley(en) + 1)
      bubble(en, "If you need us, we'll be back at the manor.")
      set_ent_script(en, "D9K")
      set_ent_script(42, "D9K")
      wait_for_entity(en, 42)
    else
      --
      bubble(en, "If you need me, I'll be back at the manor.")
      set_ent_script(en, "D9K")
      wait_for_entity(en, en)
    end
  end
  set_ent_active(en, 0)
  set_progress(100, get_progress(100) + 1)
  set_progress(45, 4)
end
