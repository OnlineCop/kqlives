-- starting - "Various pieces of heroes' homes back in Antoria"

function autoexec()
  if (get_progress(P_SKIPINTRO) == 0) then
    add_chr(SENSAR);
    set_vfollow(1);
    set_ent_facing(HERO1,1);
    view_range(1,0,0,14,19);
    set_holdfade(1);
    warp(7,18,16);
  end
end

function postexec()
  local a;

  if (get_progress(P_SKIPINTRO) == 1) then
    LOC_choose_hero();
    return
  end
  set_holdfade(0);
  drawmap();
  screen_dump();
  do_fadein(4);
  drawmap();
  screen_dump();
  set_autoparty(1);
  set_ent_script(HERO1,"W50U4");
  wait_for_entity(HERO1,HERO1);
  bubble(HERO1,"Hey, what's that?","","","");
  set_ent_script(HERO1,"L2U5");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"Some guy named Nostik is",
                "looking for adventurers to",
                "go on a quest to find a",
                "missing magical staff.");
  bubble(HERO1,"Fame and fortune!",
               "Cool!","","");
  bubble(HERO1,"I'm there man!","","","");
  set_ent_script(HERO1,"F0W50");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"Who am I talking to?","","","");
  thought(HERO1,"Oh well... I'm off.","","","");
  set_ent_script(HERO1,"D4R2D4");
  wait_for_entity(HERO1,HERO1);
  sfx(25);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(SARINA);
  view_range(0,0,0,0,0);
  warp(27,43,4);
  set_ent_script(HERO1,"L17");
  wait_for_entity(HERO1,HERO1);
  bubble(HERO1,"What?","","","");
  set_ent_script(HERO1,"F0W10");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"That wasn't there this morning.","","","");
  set_ent_script(HERO1,"R2U1");
  wait_for_entity(HERO1,HERO1);
  bubble(HERO1,"Fortune!","","","");
  thought(HERO1,"Perhaps this is what I need in",
                "order to help Mom and Dad.","","");
  thought(HERO1,"I'll have to talk to",
                "them right away.","","");
  set_ent_speed(HERO1,5);
  set_ent_script(HERO1,"D1R15");
  wait_for_entity(HERO1,HERO1);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(CORIN);
  view_range(0,0,0,0,0);
  warp(37,27,4);
  set_ent_speed(HERO1,3);
  set_ent_script(HERO1,"U7");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"Hmm... a note from master",
                "Largon. He wants me to go",
                "and investigate some matter",
                "regarding a magical staff.");
  set_ftile(37,19,0);
  set_ent_script(HERO1,"F0W10");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"I don't like the sound of this",
                "one bit. I wonder who this",
                "Nostik guy is?","");
  thought(HERO1,"Well, I'd better get packed.",
                "It's a long way to Reisha",
                "Mountain.","");
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(AJATHAR);
  view_range(1,50,0,64,16);
  warp(57,15,4);
  set_ent_speed(HERO1,3);
  set_ent_script(HERO1,"U7");
  wait_for_entity(HERO1,HERO1);
  bubble(HERO1,"You wanted to see me sir?","","","");
  bubble(24,"Yes Ajathar. It is time for you",
            "to leave the temple and discover",
            "the world. I have found the",
            "perfect way for you to do that.");
  bubble(HERO1,"Excellent sir.  What am I to do?","","","");
  bubble(24,"One of the others brought in a",
            "notice that speaks of a man",
            "named Nostik.","");
  bubble(24,"This man is seeking the aid of",
            "adventurers to find a lost",
            "magical staff for him.","");
  bubble(24,"Aside from what adventure can",
            "teach you, it would be a good",
            "opportunity for you to",
            "investigate this whole quest.");
  bubble(24,"It seems a little suspicious.","","","");
  bubble(HERO1,"I would be honored sir.",
               "Where do I go first?","","");
  bubble(24,"The note says to go to Reisha",
             "Mountain where you will receive",
             "further instruction.","");
  bubble(HERO1,"I'm on my way.","","","");
  bubble(24,"Fare thee well Ajathar.","","","");
  set_ent_script(HERO1,"D5");
  wait_for_entity(HERO1,HERO1);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(CASANDRA);
  view_range(0,0,0,0,0);
  warp(55,49,4);
  set_ent_speed(HERO1,4);
  set_ent_script(HERO1,"U10");
  wait_for_entity(HERO1,HERO1);
  thought(HERO1,"Nella was right. This is an",
                "extremely stupid place to put",
                "a public notice.","");
  bubble(HERO1,"Wait a minute!","","","");
  bubble(HERO1,"Magical staff!","","","");
  thought(HERO1,"I could definitely use that.","","","");
  thought(HERO1,"Looks like I'm going to",
                "Reisha Mountain.","","");
  set_ent_speed(0,5);
  set_ent_script(HERO1,"D9");
  wait_for_entity(HERO1,HERO1);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(TEMMIN);
  view_range(0,0,0,0,0);
  warp(88,29,4);
  set_ent_speed(HERO1,4);
  set_ent_script(HERO1,"L9");
  wait_for_entity(HERO1,HERO1);
  bubble(HERO1,"I was told you had a new",
               "mission for me sir.","","");
  bubble(25,"Yes Temmin, you are going to",
            "Reisha Mountain. You will likely",
            "find others there who are",
            "preparing for a quest.");
  bubble(25,"This quest is to find some man's",
            "lost magical staff.","","");
  bubble(25,"You are to go on this quest",
            "and if possible return with",
            "the staff.","");
  bubble(25,"If the staff poses no threat to",
            "anyone, then we will return it",
            "to this Nostik person.","");
  bubble(25,"Understood?","","","");
  bubble(HERO1,"Yes sir. I'll leave",
               "immediately sir.","","");
  set_ent_script(HERO1,"R9");
  wait_for_entity(HERO1,HERO1);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(AYLA);
  view_range(1,91,0,103,11);
  warp(97,10,4);
  set_ent_speed(HERO1,4);
  set_ent_script(HERO1,"U6R2F3W50");
  wait_for_entity(HERO1,HERO1);
  set_ftile(100,4,312);
  wait(50);
  thought(HERO1,"This is so perfect.","","","");
  thought(HERO1,"Just as my luck starts running",
                "out, along comes an opportunity",
                "to get out of town and make some",
                "serious coin at the same time.");
  bubble(HERO1,"Life is good.","","","");
  set_ent_facing(HERO1,HERO1);
  wait(50);
  thought(HERO1,"Hmm... where am I going to",
                "fence a magical staff?","","");
  wait(100);
  remove_chr(get_pidx(0));

  set_vfollow(1);
  add_chr(NOSLOM);
  set_ent_speed(HERO1,4);
  set_ent_facing(HERO1,1);
  view_range(1,101,31,111,49);
  warp(106,35,4);
  wait(50);
  thought(HERO1,"This does not look good. There",
                "is a really good chance that it",
                "could be the same staff.","");
  set_ent_facing(HERO1,HERO1);
  wait(50);
  thought(HERO1,"I have no choice but",
                "to check it out.","","");
  wait(50);
  bubble(HERO1,"Well... I'd best be on my way.","","","");
  set_ent_script(HERO1,"D12");
  wait_for_entity(HERO1,HERO1);
  set_autoparty(0);
  remove_chr(get_pidx(0));

  view_range(0,0,0,0,0);
  set_ent_active(HERO1,0);
  set_vfollow(1);
  place_ent(28,129,26);
  place_ent(29,135,27);
  place_ent(30,132,27);
  place_ent(31,126,19);
  place_ent(32,133,19);
  place_ent(33,133,21);
  place_ent(34,125,25);
  place_ent(35,129,24);
  for a=0,7,1 do
    set_ent_id(28+a,a);
    set_ent_active(28+a,1);
    set_ent_movemode(28+a,0);
  end
  set_noe(get_noe()+8);
  warp(130,22,4);
  msg("Reisha Mountain",255,1500);
  wait(100);
  bubble(255,"I have decided...","","","");
  wait(100);
  do_fadeout(4);
  for a=28,35,1 do
    set_ent_active(a,0);
  end
  drawmap();
  screen_dump();
  do_fadein(4);
  wait(100);
  LOC_choose_hero();
end

function zone_handler(zn)
  return
end

function entity_handler(en)
  return
end

function LOC_choose_hero()
  local stop = 0;
  local rd = 1;
  local a, p;
  local ptr = 0;

  clear_buffer();
  screen_dump();
  do_fadein(16);
  while (stop == 0) do
    if (rd == 1) then
      clear_buffer();
      for a=0,7,1 do
        if (ptr == a) then
          dark_mbox(a*32+32,16,2,2);
        else
          mbox(a*32+32,16,2,2);
        end
        drawframe(a,0,a*32+40,24);
      end
      mbox(80,64,18,5);
      draw_pstat(ptr,88,72);
      mbox(80,120,18,8);
      ptext(88,128,"Strength",5);
      ptext(88,136,"Agility",5);
      ptext(88,144,"Vitality",5);
      ptext(88,152,"Intellect",5);
      ptext(88,160,"Sagacity",5);
      ptext(88,168,"Speed",5);
      ptext(88,176,"Aura",5);
      ptext(88,184,"Spirit",5);
      for p=0,7,1 do
        ptext(184,p*8+128,":",5);
        pnum(208,p*8+128,get_party_stats(ptr,p)/100,0);
      end
      screen_dump();
    end
    read_controls(0,0,1,1,1,0,0,0);
    rd = 0;
    if (check_key(2) == 1) then
      ptr = ptr - 1;
      if (ptr < 0) then
        ptr = 7;
      end
      sfx(0);
      rd = 1;
    elseif (check_key(3) == 1) then
      ptr = ptr + 1;
      if (ptr > 7) then
        ptr = 0;
      end
      sfx(0);
      rd = 1;
    elseif (check_key(4) == 1) then
      stop = 1;
    end
  end
  add_chr(ptr);
  set_all_equip(get_pidx(0),I_KNIFE1,0,0,I_SUIT1,0,0);
  change_map("manor",37,7,37,7);
end
