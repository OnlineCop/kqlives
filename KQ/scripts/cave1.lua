-- cave1 - underground tunnel from Ekla to Randen

function autoexec()
--PH added refresh here to get rid of DarkImp
  refresh();
end

function refresh()
  if (get_treasure(2) == 1) then
    set_ftile(2,18,41);
  end
  if (get_progress(P_DARKIMPBOSS) == 1) then
    set_ftile(41,42,0);
    set_obs(41,42,0);
    set_zone(41,42,7);
  end
  if (get_progress(P_DYINGDUDE) == 1 or get_progress(P_DARKIMPBOSS) == 1) then
    set_btile(45,14,25);
    set_obs(45,14,0);
    set_zone(45,14,7);
  end
  if (get_progress(P_PORTALGONE) == 1) then
    set_ftile(7,43,217);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 0) then
    if (get_progress(P_PORTALGONE) == 0) then
      combat(6);
    end
  
  elseif (zn == 1) then
    change_map("town1",27,57,27,57);
  
  elseif (zn == 2) then
    change_map("town2",11,11,11,11);
  
  elseif (zn == 4) then
    chest(1,I_MHERB,1);
    refresh();
  
  elseif (zn == 5) then
    chest(2,I_IRUNE,1);
    refresh();
  
  elseif (zn == 6) then
    bubble(HERO1,"These stairs are blocked!","","","");
  
  elseif (zn == 8) then
    if (get_progress(P_ODDWALL) == 0) then
      bubble(HERO1,"That wall looks odd.","","","");
      set_progress(P_ODDWALL,1);
    end
  
  elseif (zn == 9) then
    warp(4,24,8);
  
  elseif (zn == 10) then
    warp(23,37,8);
  
  elseif (zn == 11) then
    if (get_progress(P_DYINGDUDE) == 0) then
      bubble(255,"... don't go any further.  A",
                 "strange creature has... blocked",
                 "the path. It just appeared there",
                 "out of nowhere.");
    else
      bubble(HERO1,"He's dead.","","","");
    end
    set_progress(P_DYINGDUDE,1);
  
  elseif (zn == 12) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      set_run(0);
      combat(7);
      set_run(1);
      set_progress(P_DARKIMPBOSS,1);
      refresh();
    end
  
  elseif (zn == 13) then
    if (get_progress(P_PORTALGONE) == 0) then
      if (get_progress(P_CANCELROD) == 1) then
        bubble(HERO1,"Hmmm... I guess if I just",
                     "touch it with the rod...","","");
        do_fadeout(4);
        set_progress(P_PORTALGONE,1);
        set_progress(P_CANCELROD,2);
        refresh();
        drawmap();
        screen_dump();
        do_fadein(4);
        bubble(HERO1,"Whoa! The rod is gone too!","","","");
      else
        bubble(HERO1,"Hmmm... exit only I guess.","","","");
      end
    else
      bubble(HERO1,"I wonder where this came from.","","","");
    end

  end
end

function entity_handler(en)
  return
end
