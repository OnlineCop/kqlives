-- cave3a - "First part of cave on way to Oracle's tower"

function autoexec()
  refresh();
end

function refresh()
  if (get_treasure(17) > 0) then
    set_btile(43,5,95);
  end
  if (get_treasure(18) > 0) then
    set_btile(44,6,95);
  end
  if (get_treasure(19) > 0) then
    set_btile(45,5,95);
  end
  if (get_progress(P_ORACLE) > 0) then
    set_ftile(5,41,119);
    set_obs(5,41,0);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",264,60,264,60);

  elseif (zn == 2) then
    warp(44,47,8);

  elseif (zn == 3) then
    warp(8,46,8);

  elseif (zn == 4) then
    change_map("cave3b",0,0,0,0);

  elseif (zn == 5) then
    warp(9,3,8);

  elseif (zn == 6) then
    chest(18,I_NLEAF,1);
    refresh();

  elseif (zn == 7) then
    chest(17,I_PCURING,1);
    refresh();

  elseif (zn == 8) then
    chest(19,I_WOODCLOAK,1);
    refresh();

  elseif (zn == 9) then
    warp(7,3,8);

  elseif (zn == 10) then
    if (get_progress(P_ORACLE) == 0) then
      bubble(HERO1,"The door is locked.","","","");
    else
      warp(27,28,8);
    end

  elseif (zn == 11) then
    combat(8);

  elseif (zn == 12) then
    warp(5,42,8);

  elseif (zn == 13) then
    change_map("town4",32,21,32,21);

  elseif (zn == 14) then
    change_map("cave1",2,17,2,17);

  elseif (zn == 15) then
    change_map("cave3b",13,13,13,13);

  elseif (zn == 16) then
    warp(27,8,8);

  elseif (zn == 17) then
    warp(28,44,8);

  elseif (zn == 18) then
    change_map("main",264,57,264,57);

  end
end

function entity_handler(en)
  return
end
