-- cave4 - cave south of Oracle tower and west of Denorian village

function autoexec()
  refresh();
end

function refresh()
  if (get_treasure(62) == 1) then
    set_btile(45,51,256);
  end
  if (get_treasure(63) == 1) then
    set_btile(43,35,256);
  end
  if (get_treasure(64) == 1) then
    set_btile(36,17,256);
  end
  if (get_treasure(65) == 1) then
    set_btile(16,35,256);
  end
  if (get_treasure(66) == 1) then
    set_btile(36,60,256);
  end
  if (get_progress(57) > 0) then
    set_ent_active(E_DEMNAS,0);
  end
  if (get_progress(70) == 2) then
    set_obs(37,66,0);
    set_zone(37,66,2);
  end
end
  
function postexec()
  return
end

function zone_handler(zn)
  if (zn == 0) then
    combat(10);
  
  elseif (zn == 1) then
    change_map("main",244,67,244,67);
  
  elseif (zn == 2) then
    if (get_progress(56) == 0) then
      set_foreground(0);
      set_btile(5,9,240);
      set_btile(34,20,240);
      set_btile(13,64,240);
      set_btile(37,66,240);
      set_btile(18,80,240);
      set_btile(13,73,240);
      set_btile(13,53,240);
      set_btile(14,32,240);
      set_mtile(5,9,251);
      set_mtile(34,20,251);
      set_mtile(13,64,251);
      set_mtile(37,66,251);
      set_mtile(18,80,251);
      set_mtile(13,73,251);
      set_mtile(13,53,251);
      set_mtile(14,32,251);
      sfx(26);
      set_progress(56,1);
    end
  
  elseif (zn == 3) then
    if (get_progress(56) == 1) then
      set_btile(5,9,250);
      set_btile(34,20,250);
      set_btile(13,64,250);
      set_btile(37,66,250);
      set_btile(18,80,250);
      set_btile(13,73,250);
      set_btile(13,53,250);
      set_btile(14,32,250);
      set_mtile(5,9,0);
      set_mtile(34,20,0);
      set_mtile(13,64,0);
      set_mtile(37,66,0);
      set_mtile(18,80,0);
      set_mtile(13,73,0);
      set_mtile(13,53,0);
      set_mtile(14,32,0);
      sfx(26);
      set_foreground(1);
      set_progress(56,0);
    end
  
  elseif (zn == 4) then
    if (get_progress(70) == 0) then
      bubble(200,"Locked.","","","");
    elseif (get_progress(70) == 1) then
      bubble(200,"What luck! The key from",
                   "that crazy Demnas guy",
                   "unlocks this door!","");
      set_progress(70,2);
      refresh();
    end
  
  elseif (zn == 5) then
    bubble(200,"Aw... it's full of normal junk.","","","");
  
  elseif (zn == 6) then
    chest(62,95,1);
    refresh();
  
  elseif (zn == 7) then
    chest(63,165,1);
    refresh();
  
  elseif (zn == 8) then
    chest(64,107,1);
    refresh();
  
  elseif (zn == 9) then
    chest(65,117,2);
    refresh();
  
  elseif (zn == 10) then
    chest(66,68,1);
    refresh();
  
  elseif (zn == 11) then
    warp(48,98,8);
  
  elseif (zn == 12) then
    warp(39,61,8);
  
  elseif (zn == 13) then
    if (get_progress(55) == 3) then
      bubble(200,"The Denorians were right.  This",
                    "troll was being directed by",
                    "someone... that scumbag Demnas.","");
      bubble(200,"He used this poor bugger and",
                    "then left him here to rot.","","");
      wait(100);
      bubble(200,"Well, I should go back to the",
                    "village now and report all this.","","");
      set_progress(55,4);
    else
      bubble(200,"This cell became his tomb.","","","");
    end

  end
end

function entity_handler(en)
  if (en == 0) then
    bubble(200,"Are you the one responsible for",
                 "the disappearance of a statue",
                 "from a village near here?","");
    bubble(0,"Are you accusing me of stealing",
             "this statue?","","");
    bubble(200,"Well, the thief was traced",
                       "back to this cave.","","");
    bubble(0,"Oh, so now I'm a thief?!!","","","");
    bubble(200,"That depends... did you take",
                 "the statue or not?","","");
    bubble(0,"What if I did?","","","");
    bubble(200,"Well, then you will give",
                 "it back... or it will be",
                 "taken by force!","");
    bubble(0,"Ha ha ha... you've proven to",
             "be as stupid as you look.","","");
    bubble(0,"Now I, Demnas, will destroy you!","","","");
    drawmap();
    screen_dump();
    set_run(0);
    combat(11);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_progress(57,1);
    set_progress(55,3);
    refresh();
    drawmap();
    screen_dump();
    sfx(5);
    msg("Bronze key procured",255,0);
    set_progress(70,1);
    sfx(5);
    msg("Denorian Statue procured",255,0);

  end
end
