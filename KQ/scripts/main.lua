-- main - "World map of Anistal"

function autoexec()

  if (get_progress(P_SHOWBRIDGE) == 0) then
    set_mtile(241, 29, 82)
  end
  if (get_progress(P_SHOWBRIDGE) < 2) then
    set_btile(240, 29, 80)
    set_btile(242, 29, 80)
    set_obs(241, 29, 1)
    set_zone(240, 29, 7)
    set_zone(242, 29, 71)
  end

  if (get_progress(P_TOWEROPEN) > 0) then
    set_obs(263, 52, 0)
  end

  if (get_progress(P_FOUNDMAYOR) == 0) then
    set_zone(235, 37, 0)
  end

  if ((get_progress(P_OPALHELMET) == 1) and
      (get_progress(P_OPALSHIELD) == 1) and
      (get_progress(P_OPALBAND) == 1) and
      (get_progress(P_OPALARMOUR) == 1)) then
    set_zone(264, 164, 73)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("manor", 0, 0, 0, 0)

  elseif (zn == 2) then
    if (get_progress(P_START) == 1) then
      if (in_forest(HERO1)) then
        combat(27)
      else
        combat(28)
      end
    end

  elseif (zn == 3) then
    change_map("town1", 0, 0, 0, 0)

  elseif (zn == 4) then
    change_map("town2", 0, 0, 0, 0)

  elseif (zn == 5) then
    if (in_forest(HERO1)) then
      combat(29)
    else
      combat(30)
    end

  elseif (zn == 6) then
    if (in_forest(HERO1)) then
      combat(31)
    else
      combat(32)
    end

  elseif (zn == 7) then
    if (get_progress(P_FIGHTONBRIDGE) == 5) then
      set_ent_facing(HERO1, 2)
      change_map("bridge2", 0, 0, 0, 0)
    else
      set_ent_facing(HERO1, 2)
      change_map("bridge", 0, 0, 0, 0)
    end

  elseif (zn == 8) then
    change_map("town3", 0, 0, 0, 0)

  elseif (zn == 9) then
    change_map("grotto", 0, 0, 0, 0)

  elseif (zn == 10) then
    change_map("fort", 15, 8, 10, 1)

  elseif (zn == 11) then
    change_map("fort", 15, 42, 10, 35)

  elseif (zn == 12) then
    change_map("cave3a", 0, 0, 0, 0)

  elseif (zn == 13) then
    change_map("cave3a", 24, 4, 24, 4)

  elseif (zn == 14) then
    if (get_progress(P_GOBLINITEM) == 1 and get_progress(P_TOWEROPEN) == 0) then
      set_progress(P_TOWEROPEN, 1)
      set_progress(P_GOBLINITEM, 2)
      bubble(HERO1, "Hey! The pendant is glowing!")
      bubble(255, "The doors fly open and the pendant disappears in a puff of smoke.")
      change_map("tower", 0, 0, 0, 0)
      return
    end
    if (get_progress(P_GOBLINITEM) > 1 and get_progress(P_TOWEROPEN) == 0) then
      bubble(HERO1, "The tower is sealed.")
    end
    if (get_progress(P_TOWEROPEN) == 1) then
      change_map("tower", 0, 0, 0, 0)
    end

  elseif (zn == 15) then
    if (in_forest(HERO1)) then
      combat(33)
    else
      combat(34)
    end

  elseif (zn == 16) then
    if (in_forest(HERO1)) then
      combat(35)
    else
      combat(36)
    end

  elseif (zn == 17) then
    change_map("temple1", 0, 0, 0, 0)

  elseif (zn == 18) then
    change_map("town4", 0, 0, 0, 0)

  elseif (zn == 19) then
    change_map("camp", 0, 0, 0, 0)

  elseif (zn == 20) then
    change_map("estate", 0, 0, 0, 0)

  elseif (zn == 21) then
    change_map("town5", 0, 0, 0, 0)

  elseif (zn == 22) then
    if (in_forest(HERO1)) then
      combat(37)
    else
      combat(38)
    end

  elseif (zn == 23) then
    if (in_forest(HERO1)) then
      combat(39)
    else
      combat(40)
    end

  elseif (zn == 24) then
    if (in_forest(HERO1)) then
      combat(41)
    else
      combat(42)
    end

  elseif (zn == 25) then
    if (in_forest(HERO1)) then
      combat(43)
    else
      combat(44)
    end

  elseif (zn == 26) then
    if (in_forest(HERO1)) then
      combat(45)
    else
      combat(46)
    end

  elseif (zn == 27) then
    if (in_forest(HERO1)) then
      combat(47)
    else
      combat(48)
    end

  elseif (zn == 28) then
    if (get_progress(P_SEECOLISEUM) < 2) then
      bubble(HERO1, "The Coliseum is closed.")
      if (get_progress(P_SEECOLISEUM) == 0) then
        set_progress(P_SEECOLISEUM, 1)
      end
    else
      change_map("coliseum", 0, 0, 0, 0)
    end

  elseif (zn == 29) then
    if (get_progress(P_DENORIAN) == 1) then
      bubble(255, "You are not allowed in the village.")
    else
      change_map("dville", 0, 0, 0, 0)
    end

  elseif (zn == 30) then
    change_map("cave4", 0, 0, 0, 0)

  elseif (zn == 31) then
    if (get_progress(P_DENORIAN) < 2) then
      bubble(HERO1, "Hmm... there's a huge iron door blocking the entrance to this cave.")
    else
      bubble(HERO1, "Stones 1, 4 then 3...")
      set_btile(244, 66, 54)
      set_zone(244, 66, 30)
      set_obs(244, 66, 0)
      sfx(26)
      bubble(HERO1, "Bingo.")
    end

  elseif (zn == 32) then
    change_map("town6", 0, 0, 0, 0)

  elseif (zn == 33) then
    warp(233, 128, 16)

  elseif (zn == 34) then
    warp(247, 128, 16)

  elseif (zn == 35) then
    change_map("pass", 0, 0, 0, 0)

  elseif (zn == 36) then
    change_map("pass", 124, 35, 124, 35)

  elseif (zn == 37) then
    change_map("town7", 0, 0, 0, 0)

  elseif (zn == 38) then
    if (get_progress(P_GIANTDEAD) == 0) then
      combat(49)
      if (get_alldead() == 1) then
        return
      else
        set_progress(P_GIANTDEAD, 1)
      end
    else
      warp(211, 108, 16)
    end

  elseif (zn == 39) then
    warp(295, 108, 16)

  elseif (zn == 40) then
    change_map("cult", 0, 0, 0, 0)

  elseif (zn == 41) then
    change_map("unfinished", 0, 0, 0, 0)

  elseif (zn == 42) then
    change_map("unfinished", 0, 0, 0, 0)

  elseif (zn == 43) then
    change_map("town8", 0, 0, 0, 0)

  elseif (zn == 44) then
    warp(255, 73, 16)

  elseif (zn == 45) then
    warp(61, 58, 16)

  elseif (zn == 46) then
    warp(29, 33, 16)

  elseif (zn == 47) then
    warp(26, 53, 16)

  elseif (zn == 48) then
    warp(69, 56, 16)

  elseif (zn == 49) then
    warp(37, 81, 16)

  elseif (zn == 50) then
    warp(20, 47, 16)

  elseif (zn == 51) then
    warp(37, 52, 16)

  elseif (zn == 52) then
    warp(44, 71, 16)

  elseif (zn == 53) then
    warp(70, 40, 16)

  elseif (zn == 54) then
    warp(45, 45, 16)

  elseif (zn == 55) then
    warp(20, 53, 16)

  elseif (zn == 56) then
    warp(66, 45, 16)

  elseif (zn == 57) then
    warp(12, 85, 16)

  elseif (zn == 58) then
    warp(26, 47, 16)

  elseif (zn == 59) then
    warp(31, 77, 16)

  elseif (zn == 60) then
    warp(15, 94, 16)

  elseif (zn == 61) then
    warp(33, 119, 16)

  elseif (zn == 62) then
    warp(44, 84, 16)

  elseif (zn == 63) then
    warp(108, 101, 16)

  elseif (zn == 64) then
    warp(28, 118, 16)

  elseif (zn == 65) then
    warp(85, 129, 16)

  elseif (zn == 66) then
    warp(86, 137, 16)

  elseif (zn == 67) then
    warp(18, 135, 16)

  elseif (zn == 68) then
    warp(79, 130, 16)

  elseif (zn == 69) then
    warp(41, 54, 16)

  elseif (zn == 70) then
    warp(12, 24, 16)

  elseif (zn == 71) then
    set_ent_facing(HERO1, 2)
    change_map("bridge2", 63, 18, 63, 18)

  elseif (zn == 72) then
    if ((get_progress(P_OPALHELMET) == 1) and
        (get_progress(P_OPALSHIELD) == 1) and
        (get_progress(P_OPALBAND) == 1) and
        (get_progress(P_OPALARMOUR) == 1)) then
      set_btile(264, 164, 54)
      set_zone(264, 164, 73)
      set_obs(264, 164, 0)
      sfx(26)
      bubble(HERO1, "Ah, there we go.")
    else
      bubble(HERO1, "I think this entrance will open once I have all the opal stuff.")
    end

  elseif (zn == 73) then
    change_map("cave6a", 0, 0, 0, 0)

  elseif (zn == 74) then
    change_map("cave6b", 5, 7, 5, 7)

  elseif (zn == 75) then
    bubble(HERO1, "The underwater tunnel should go here.")
    warp(203, 182, 16)
  
  elseif (zn == 76) then
    bubble(HERO1, "The second part of the underwater tunnel should go here.")
    warp(262, 159, 16)

  elseif (zn == 77) then
    bubble(HERO1, "This is where the castle town of Xenar goes.")
    bubble(HERO1, "It's not finished yet.")

  elseif (zn == 78) then
    bubble(HERO1, "This is the cave behind the Xenar Castle. Sorry, you can't go in there yet.")

  elseif (zn == 79) then
    bubble(HERO1, "This is as far as the dock goes.")

  end
end


function entity_handler(en)
  return
end
