-- "pass - Karnok's pass, between Sunarin and Pulcannen"

function autoexec()
  refresh();
end
  
function refresh()
  if (get_progress(P_SAVEBREANNE) > 0) then
    place_ent(0,get_ent_tilex(0)-1,get_ent_tiley(0));
    set_ent_facing(0,3);
    place_ent(1,get_ent_tilex(1)+1,get_ent_tiley(1));
    set_ent_facing(1,2);
  end
  if (get_treasure(73) == 1) then
    set_mtile(74,47,39);
  end
  if (get_treasure(74) == 1) then
    set_mtile(75,47,39);
  end
  if (get_progress(P_PASSDOOR2) == 1) then
    set_mtile(78,38,57);
    set_mtile(78,39,33);
    set_obs(78,38,0);
  end
  if (get_progress(P_PASSDOOR3) == 1) then
    set_mtile(106,35,57);
    set_mtile(106,36,33);
    set_obs(106,35,0);
  end
end

function postexec()
  return
end
  
function zone_handler(zn)
--  if (zn == 0) then
--    combat(50);

--  elseif (zn == 1) then
  if (zn == 1) then
    change_map("main",268,103,268,103);
  
  elseif (zn == 2) then
    change_map("main",271,100,271,100);
  
  elseif (zn == 3) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
      return
    else
      sfx(26);
      set_mtile(83,27,57);
      set_mtile(83,28,33);
      drawmap();
      screen_dump();
    end
    change_map("cave5a",0,0,0,0);
  
  elseif (zn == 4) then
    chest(73,I_VITSEED,1);
    refresh();
  
  elseif (zn == 5) then
    chest(74,I_ERUNE,1);
    refresh();
  
  elseif (zn == 6) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
      return
    else
      if (get_progress(P_PASSDOOR2) == 0) then
        set_progress(P_PASSDOOR2,1);
        sfx(26);
        refresh();
      end
    end
    change_map("cave5a",25,12,25,12);
  
  elseif (zn == 7) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
      return
    else
      if (get_progress(P_PASSDOOR3) == 0) then
        set_progress(P_PASSDOOR3,1);
        sfx(26);
        refresh();
      end
    end
    change_map("cave5a",43,13,43,13);
  
  end
end

function entity_handler(en)  
  if (en == 0 or en == 1) then
    if (get_progress(P_SAVEBREANNE) == 0) then
      bubble(en,"This pass is reserved for",
                "use by caravans only.","","");
    else
      bubble(en,"You are free to use the pass.",
                "Just be careful.","","");
    end

  end
end
