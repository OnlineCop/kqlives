-- town8 - "not yet named"

-- /*
-- {
-- P_WALKING: Layer which the player is walking on
--   0 - You're on an elevated cliff: you walk OVER bridges
--   1 - You're on ground level: you walk UNDER bridges
-- }
-- */

function autoexec()
  set_progress(P_WALKING, 0)
  refresh()
end


function refresh()
  if (get_treasure(91) == 1) then
    set_obs(1, 48, 0)
    set_zone(1, 48, 0)
  end

  if (get_treasure(92) == 1) then
    set_obs(7, 48, 0)
    set_zone(7, 48, 0)
  end

  if (get_treasure(93) == 1) then
    set_obs(13, 48, 0)
    set_zone(13, 48, 0)
  end

  if (get_treasure(95) == 1) then
    set_obs(50, 7, 0)
    set_zone(50, 7, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 230, 162, 230, 162)

  elseif (zn == 2) then
    set_progress(P_WALKING, 0)
    TOC_switch_layers()

  elseif (zn == 3) then
    set_progress(P_WALKING, 1)
    TOC_switch_layers()

  elseif (zn == 4) then
    chest(91, 0, 1200)
    refresh()

  elseif (zn == 5) then
    chest(92, 0, 150)
    refresh()

  elseif (zn == 6) then
    chest(93, I_SALVE, 1)
    refresh()

  elseif (zn == 7) then
    bubble(HERO1, "This appears to be a hologram of the Oracle!")

  elseif (zn == 8) then
    warp(7, 43, 16)

  elseif (zn == 9) then
    warp(35, 42, 16)

  elseif (zn == 10) then
    chest(94, I_ARMOR8, 1)
    refresh()

  elseif (zn == 11) then
    bubble(HERO1, "This barrel is filled with rainwater.")

  elseif (zn == 12) then
    touch_fire(party[0])

  elseif (zn == 13) then
    bubble(HERO1, "Locked.")

  elseif (zn == 14) then
    chest(95, I_MACE3, 1)
    refresh()

  elseif (zn == 15) then
    door_in(100, 40, 92, 32, 110, 43)

  elseif (zn == 16) then
    door_in(102, 40, 92, 32, 110, 43)

  elseif (zn == 17) then
    door_out(37, 23)

  elseif (zn == 18) then
    door_out(38, 23)

  elseif (zn == 19) then
    door_in(86, 16, 81, 8, 91, 19)

  elseif (zn == 20) then
    door_out(15, 11)

  elseif (zn == 21) then
    door_in(96, 16, 91, 8, 101, 19)

  elseif (zn == 22) then
    door_out(8, 21)

  elseif (zn == 23) then
    door_in(106, 16, 101, 8, 111, 19)

  elseif (zn == 24) then
    door_out(38, 9)

  elseif (zn == 25) then
    door_in(86, 28, 81, 20, 91, 31)

  elseif (zn == 26) then
    door_out(16, 34)

  elseif (zn == 27) then
    door_in(96, 28, 91, 20, 101, 31)

  elseif (zn == 28) then
    door_out(50, 13)

  elseif (zn == 29) then
    door_in(106, 28, 101, 20, 111, 31)

  elseif (zn == 30) then
    door_out(64, 20)

  elseif (zn == 31) then
    view_range(1, 81, 31, 91, 42)
    warp(89, 36, 8)

  elseif (zn == 32) then
    view_range(1, 101, 19, 111, 31)
    warp(109, 24, 8)

  elseif (zn == 33) then
    thought(HERO1, "This spa is relaxing.")
    
  elseif (zn == 34) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "This is a warm fire.")

  elseif (en == 1) then
    bubble(en, "I am an NPC. Do you know what that means? It means I have no will of my own, and must obey my programmer.")

  elseif (en == 2) then
    bubble(en, "I get teased a lot because I'm the only one with blonde hair. Neither of my parents have blonde hair...")

  elseif (en == 3) then
    bubble(en, "For a long time, we lived in peace. Now that monsters have attacked, we can't even leave the safety of our village.")

  elseif (en == 4) then
    bubble(en, "I wish I could find some opal armour. I hear there's some sort of magic that can make it very strong.")

  elseif (en == 5) then
    bubble(en, "I am a silhouette of the Oracle. I can't figure out how you're talking to me, as I'm floating above the water like this. Hmm... must be a bug in this game.")

  elseif (en == 6) then
    shop(24)

  elseif (en == 7) then
    inn("Long Journey Inn", 65, 1)
  end
end


function TOC_switch_layers()
  if (get_progress(P_WALKING) == 0) then
    set_obs(17, 45, 3)
    set_obs(18, 43, 0)
    set_obs(18, 47, 0)
    set_obs(19, 44, 5)
    set_obs(19, 45, 5)
    set_obs(19, 46, 5)

    set_mtile(18, 44, 33)
    set_ftile(18, 44, 0)
    set_mtile(18, 45, 33)
    set_ftile(18, 45, 0)
    set_mtile(18, 46, 33)
    set_ftile(18, 46, 0)

    set_obs(57, 5, 0)
    set_obs(56, 6, 3)
    set_obs(58, 6, 5)
    set_obs(57, 7, 0)

    set_mtile(57, 6, 33)
    set_ftile(57, 6, 0)
  else
    set_obs(17, 45, 0)
    set_obs(18, 43, 1)
    set_obs(18, 47, 1)
    set_obs(19, 44, 0)
    set_obs(19, 45, 0)
    set_obs(19, 46, 0)

    set_mtile(18, 44, 0)
    set_ftile(18, 44, 33)
    set_mtile(18, 45, 0)
    set_ftile(18, 45, 33)
    set_mtile(18, 46, 0)
    set_ftile(18, 46, 33)

    set_obs(57, 5, 1)
    set_obs(56, 6, 0)
    set_obs(58, 6, 0)
    set_obs(57, 7, 2)

    set_mtile(57, 6, 0)
    set_ftile(57, 6, 33)
  end
end
