-- temple1 - "First part of temple north of Andra"

function autoexec()
  refresh();
end

function refresh()
  if (get_progress(P_ALTARSWITCH) == 1) then
    set_ftile(44,4,0);
    set_btile(44,5,29);
    set_obs(44,5,0);
    set_ftile(43,4,118);
    set_btile(43,5,119);
    set_obs(43,5,1);
    set_zone(44,5,9);
    set_ftile(14,2,154);
    set_btile(14,3,156);
    set_obs(14,2,0);
    set_zone(14,2,12);
  end
  if (get_progress(P_UNDEADJEWEL) > 0) then
    set_mtile(20,32,65);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",268,12,268,12);

  elseif (zn == 2) then
    sfx(26);
    set_zone(8,24,0);
    set_ftile(8,24,154);
    set_btile(8,25,156);
    set_obs(8,24,0);

  elseif (zn == 3) then
    sfx(26);
    set_zone(8,19,0);
    set_ftile(8,19,154);
    set_btile(8,20,156);
    set_obs(8,19,0);

  elseif (zn == 4) then
    sfx(26);
    set_zone(26,31,0);
    set_ftile(26,31,154);
    set_btile(26,32,156);
    set_obs(26,31,0);

  elseif (zn == 5) then
    sfx(26);
    set_zone(26,27,0);
     set_ftile(26,27,154);
    set_btile(26,28,156);
    set_obs(26,27,0);

  elseif (zn == 6) then
    sfx(26);
    set_zone(21,10,0);
    set_ftile(21,10,154);
    set_btile(21,11,156);
    set_obs(21,10,0);

  elseif (zn == 7) then
    sfx(26);
    set_ftile(14,2,154);
    set_btile(14,3,156);
    set_obs(14,2,0);
    set_zone(14,2,12);

  elseif (zn == 8) then
    if (get_progress(P_ALTARSWITCH) == 0 and get_progress(P_TALKDERIG) == 2) then
      bubble(HERO1,"A switch!","","","");
      sfx(26);
      set_progress(P_ALTARSWITCH,1);
      refresh();
    end

  elseif (zn == 9) then
    warp(46,34,8);

  elseif (zn == 10) then
    change_map("temple2",0,0,0,0);

  elseif (zn == 11) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 12) then
    warp(48,9,8);

  elseif (zn == 13) then
    warp(14,3,8);

  elseif (zn == 14) then
    warp(44,5,8);

  elseif (zn == 15) then
    if (get_progress(P_GOBLINITEM) == 0) then
      combat(51);
    end

  elseif (zn == 16) then
    if (get_progress(P_UNDEADJEWEL) == 0) then
      set_progress(P_UNDEADJEWEL,1);
      sfx(5);
      msg("Goblin jewel procured",255,0);
      refresh();
    end

  end
end

function entity_handler(en)
  return
end
