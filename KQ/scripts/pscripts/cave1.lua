-- cave1 - underground tunnel from Ekla to Randen

function autoexec()
  return
end

function refresh()
  if (get_treasure(2) == 1) then
    set_ftile(2,18,41);
  end
  if (get_progress(2) == 1) then
    set_ftile(41,42,0);
    set_obs(41,42,0);
    set_zone(41,42,7);
  end
  if (get_progress(3) == 1 or get_progress(2) == 1) then
    set_btile(45,14,25);
    set_obs(45,14,0);
    set_zone(45,14,7);
  end
  if (get_progress(38) == 1) then
    set_ftile(7,43,217);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 0) then
    if (get_progress(38) == 0) then
      combat(6);
    end
  
  elseif (zn == 1) then
    change_map("town1",27,57,27,57);
  
  elseif (zn == 2) then
    change_map("town2",11,11,11,11);
  
  elseif (zn == 4) then
    chest(1,103,1);
    refresh();
  
  elseif (zn == 5) then
    chest(2,126,1);
    refresh();
  
  elseif (zn == 6) then
    bubble(200,"These stairs are blocked!","","","");
  
  elseif (zn == 8) then
    if (get_progress(1) == 0) then
      bubble(200,"That wall looks odd.","","","");
      set_progress(1,1);
    end
  
  elseif (zn == 9) then
    warp(4,24,8);
  
  elseif (zn == 10) then
    warp(23,37,8);
  
  elseif (zn == 11) then
    if (get_progress(3) == 0) then
      bubble(255,"... don't go any further.  A",
                 "strange creature has... blocked",
                 "the path. It just appeared there",
                 "out of nowhere.");
    else
      bubble(200,"He's dead.","","","");
    end
    set_progress(3,1);
  
  elseif (zn == 12) then
    if (get_progress(2) == 0) then
      set_run(0);
      combat(7);
      set_run(1);
      set_progress(2,1);
      refresh();
    end
  
  elseif (zn == 13) then
    if (get_progress(38) == 0) then
      if (get_progress(37) == 1) then
        bubble(200,"Hmmm... I guess if I just",
                     "touch it with the rod...","","");
        do_fadeout(4);
        set_progress(38,1);
        set_progress(37,2);
        refresh();
        drawmap();
        screen_dump();
        do_fadein(4);
        bubble(200,"Whoa! The rod is gone too!","","","");
      else
        bubble(200,"Hmmm... exit only I guess.","","","");
      end
    else
      bubble(200,"I wonder where this came from.","","","");
    end

  end
end

function entity_handler(en)
  return
end
