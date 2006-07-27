-- town8 - "not yet named"

-- /*
-- {
-- We're going to learn about the Xenars for the first time here.
--
-- P_WALKING: Layer which the player is walking on
--   (0) You're on an elevated cliff: you walk OVER bridges
--   (1) You're on ground level: you walk UNDER bridges
-- }
-- */

function autoexec()
  set_progress(P_WALKING, 0)
  refresh()
end


function refresh()
  if (get_treasure(91) == 1) then
    set_obs("treas_1", 0)
    set_zone("treas_1", 0)
  end

  if (get_treasure(92) == 1) then
    set_obs("treas_2", 0)
    set_zone("treas_1", 0)
  end

  if (get_treasure(93) == 1) then
    set_obs("treas_3", 0)
    set_zone("treas_3", 0)
  end

  if (get_treasure(95) == 1) then
    set_obs("treas_4", 0)
    set_zone("treas_4", 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "town8")

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
    warp("dstairs1", 16)

  elseif (zn == 9) then
    warp("dstairs2", 16)

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
    door_in("inn_1i")

  elseif (zn == 16) then
    door_in("inn_2i")

  elseif (zn == 17) then
    door_out("inn_1o")

  elseif (zn == 18) then
    door_out("inn_2o")

  elseif (zn == 19) then
    door_in("room_1i")

  elseif (zn == 20) then
    door_out("room_1o")

  elseif (zn == 21) then
    door_in("room_2i")

  elseif (zn == 22) then
    door_out("room_2o")

  elseif (zn == 23) then
    door_in("door_3i")

  elseif (zn == 24) then
    door_out("door_3o")

  elseif (zn == 25) then
    door_in("room_4i")

  elseif (zn == 26) then
    door_out("room_4o")

  elseif (zn == 27) then
    door_in("room_5i")

  elseif (zn == 28) then
    door_out("room_5o")

  elseif (zn == 29) then
    door_in("room_6i")

  elseif (zn == 30) then
    door_out("room_6o")

  elseif (zn == 31) then
    warp("dstairs3", 8)

  elseif (zn == 32) then
    warp("ustairs1", 8)

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
