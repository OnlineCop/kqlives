-- temple2 - second, underground part of temple north of Andra

function autoexec()
  refresh();
end

function refresh()
  if (get_treasure(21) == 1) then
    set_ftile(90,16,41);
  end
  if (get_treasure(22) == 1) then
    set_ftile(91,17,41);
  end
  if (get_treasure(23) == 1) then
    set_ftile(92,18,41);
  end
  if (get_treasure(24) == 1) then
    set_ftile(93,17,41);
  end
  if (get_treasure(25) == 1) then
    set_ftile(94,16,41);
  end
  if (get_treasure(26) == 1) then
    set_ftile(69,17,41);
  end
  if (get_treasure(27) == 1) then
    set_ftile(30,67,41);
  end
  if (get_treasure(28) == 1) then
    set_ftile(34,71,41);
  end
  if (get_treasure(29) == 1) then
    set_ftile(68,87,41);
  end
  if (get_treasure(30) == 1) then
    set_ftile(69,87,41);
  end
  if (get_progress(35) == 2) then
    set_btile(69,69,237);
  end
end

function postexec()
  return
end

function zone_handler(zn)  
  if (zn == 0) then
    if (get_progress(17) == 0) then
      combat(52);
    end
  
  elseif (zn == 1) then
    change_map("temple1",46,26,46,26);
  
  elseif (zn == 2) then
    warp(2,45,8);
  
  elseif (zn == 3) then
    warp(14,25,8);
  
  elseif (zn == 4) then
    warp(25,6,8);
  
  elseif (zn == 5) then
    warp(92,86,8);
  
  elseif (zn == 6) then
    warp(87,21,8);
  
  elseif (zn == 7) then
    if (get_progress(16) == 0) then
      set_ent_script(200,"X91Y66F0");
      if (get_numchrs() == 1) then
        wait_for_entity(200,200);
      else
        set_ent_script(201,"X91Y65F0");
        wait_for_entity(200,201);
      end
      set_ftile(91,67,137);
      wait(75);
  
  -- change this bubble to be spoken by an entity
      bubble(255,"Foolish humans... you will",
                 "soon join the others!","","");
  
      drawmap();
      screen_dump();
      set_run(0);
      combat(53);
      set_run(1);
      set_progress(16,1);
      set_ftile(91,67,0);
      return
    end
    warp(75,71,8);
  
  elseif (zn == 8) then
    warp(60,49,8);
  
  elseif (zn == 9) then
    chest(21,86,1);
    refresh();
  
  elseif (zn == 10) then
    chest(22,0,500);
    refresh();
  
  elseif (zn == 11) then
    chest(23,112,1);
    refresh();
  
  elseif (zn == 12) then
    chest(24,126,1);
    refresh();
  
  elseif (zn == 13) then
    chest(25,61,1);
    refresh();
  
  elseif (zn == 14) then
    chest(26,80,1);
    refresh();
  
  elseif (zn == 15) then
    chest(27,118,1);
    refresh();
  
  elseif (zn == 16) then
    chest(28,160,1);
    refresh();
  
  elseif (zn == 17) then
    chest(29,122,2);
    refresh();
  
  elseif (zn == 18) then
    chest(30,105,1);
    refresh();
  
  elseif (zn == 19) then
    warp(91,67,8);
  
  elseif (zn == 20) then
    if (get_progress(17) == 1) then
      bubble(200,"Ooohh... shiny.","","","");
      return
    end
    if (get_progress(35) == 1 and get_progress(17) == 0) then
      bubble(200,"Hey, that gem would fit here.","","","");
      sfx(5);
      set_btile(69,69,237);
      rest(500);
      bubble(255,"Thank you.","","","");
      set_ent_facing(0,0);
      if (get_numchrs() == 2) then
        set_ent_facing(1,0);
      end
      drawmap();
      screen_dump();
      bubble(200,"Who are you?","","","");
      bubble(255,"My name is Kaleg... I am the",
                 "Goblin king. This tomb is my",
                 "home.","");
      bubble(200,"I guess that would",
                    "make you dead then?","","");
      bubble(255,"That is correct.","","","");
      bubble(200,"No problem. I'll",
                   "just be going now.","","");
      bubble(255,"Wait! I know who you are",
                 "and why you came here.","","");
      bubble(200,"How does everybody know who",
                   "I am and what I'm doing?","","");
      bubble(255,"Death gives one surprising",
                 "insight. In any case, I want",
                 "you to have this.","");
      set_progress(17,1);
      sfx(5);
      msg("Jade pendant procured.",255,0);
      refresh();
      bubble(255,"This will help you",
                 "in your quest.","","");
      bubble(255,"I must go now. Fare thee well.","","","");
      bubble(200,"Wait! Can you tell me anything",
                   "about the Oracle?","","");
      wait(50);
      bubble(200,"Hello?","","","");
      bubble(200,"Damn!","","","");
      set_progress(35,2);
      return
    else
      bubble(200,"It looks like there should be",
                   "something here. Perhaps this is",
                   "the cause of the goblin's",
                   "unrest.");
    end
  
  elseif (zn == 21) then
    sfx(7);
    set_save(1);
    set_sstone(1);
  
  elseif (zn == 22) then
    set_save(0);
    set_sstone(0);

  end
end

function entity_handler(en)
  return
end
