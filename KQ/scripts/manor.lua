-- manor - Nostik's manor southwest of Ekla

function autoexec()
  local a;

  if (get_progress(P_TALKBUTLER) == 0) then
    for a=0,7,1 do
      set_ent_id(a,a);
      set_ent_obsmode(a,1);
      set_ent_chrx(a,255);
    end
    set_ent_active(get_pidx(0),0);
    place_ent(HERO1,get_ent_tilex(get_pidx(0)),get_ent_tiley(get_pidx(0)));
    set_ent_facing(HERO1,get_ent_facing(get_pidx(0)));
    calc_viewport(1);
  end
  if (get_progress(P_TALKBUTLER) > 1) then
    for a=0,7,1 do
      set_ent_active(a,0);
    end
    set_ent_active(8,0);
  end
end

function postexec()
  return
end
  
function postexec()
  if (get_progress(P_TALKBUTLER) == 0) then
    rest(200);
    bubble(8,"Welcome all.","","","");
    bubble(8,"My name is Nostik.  The reason I",
             "have gathered all of you here is",
             "that I am in need of adventurers",
             "to partake in a quest for me.");
    bubble(8,"You eight were the",
             "best candidates.","","");
    bubble(8,"You see, for the past 15 years I",
             "was a prisoner of the Ice-Lord",
             "Malkaron. I thwarted him many",
             "years ago and he sought revenge.");
    bubble(8,"He captured me, tortured me and",
             "then imprisoned me. Just",
             "recently, I was released by",
             "rebels of Malkaron's army.");
    bubble(8,"I returned here where I was",
             "hoping to start over. There is",
             "just one problem.","");
    bubble(8,"When I was captured by Malkaron",
             "he took my most prized",
             "possession. The item in question",
             "was half of a magical artifact.");
    bubble(8,"The artifact is called the Staff",
             "of Xenarum. I quested long and",
             "hard to find that one half of",
             "it and then Malkaron stole it.");
    bubble(8,"After escaping, I discovered",
             "that Malkaron did not keep the",
             "Staff. He gave it to someone",
             "else long ago.");
    bubble(8,"Therefore, that means that the",
             "Staff could be anywhere, and",
             "keep in mind that it was only",
             "one of the two pieces.");
    bubble(8,"I also need the other piece.",
             "That's why I need you people to",
             "search for me.","");
    bubble(8,"This will be a difficult quest",
             "and I am getting to old",
             "for adventuring.","");
    bubble(8,"I need you to search until you",
             "find both pieces. Bring them to",
             "me, and fame and fortune will",
             "be yours.");
    bubble(8,"Oh, before you go though, I have",
             "a couple of things I need to",
             "tell you about.","");
    bubble(8,"The first thing is that this",
             "quest is to remain as secret as",
             "possible. Malkaron and his",
             "cronies are all over the place.");
    bubble(8,"That means that you should avoid",
             "telling the citizens of this",
             "land anything about your quest.","");
    bubble(8,"Secondly, I'd appreciate it if",
             "you didn't travel in groups any",
             "bigger than two members.","");
    bubble(8,"Large groups of adventurers",
             "attract attention.","","");
    bubble(8,"Aside from all this, you may do",
             "whatever you have to in order to",
             "complete this quest.","");
    bubble(8,"Oh, and be sure to talk to my",
             "butler before you leave. He has",
             "something you will need.","");
  end
end
  
function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(P_TALKBUTLER) == 1) then
      set_progress(P_TALKBUTLER,2);
    end
    change_map("main",107,40,107,40);
  
  elseif (zn == 2) then
    warp(25,34,8);
  
  elseif (zn == 3) then
    warp(25,5,8);
  
  elseif (zn == 4) then
    bubble(HERO1,"Locked.","","","");
  
  elseif (zn == 5) then
    bubble(HERO1,"Wow! This guy reads",
                 "weird stuff.","","");
  
  elseif (zn == 6) then
    if (get_progress(P_TALKBUTLER) == 0) then
      bubble(9,"Hey! Hold on!","","","");
      set_ent_facing(HERO1,1);
      set_ent_speed(9,5);
      if (get_ent_tilex(HERO1) == 16) then
        set_ent_script(9,"D1R3D2L5D7L1D2");
      else
        set_ent_script(9,"D1R3D2L5D7L2D2");
      end
      wait_for_entity(9,9);
      set_ent_speed(9,3);
      bubble(9,"Maybe you didn't hear Master",
               "Nostik when he said that you",
               "were supposed to see me before",
               "you left.");
      bubble(9,"Anyways, Master Nostik asked",
               "me to give you this.","","");
      LOC_talk_butler();
    end

  end
end

function entity_handler(en)  
  if (en >= 0 and en <= 7) then
    LOC_chit_chat(en);

  elseif (en == 8) then
    bubble(8,"Good luck.","","","");
  
  elseif (en == 9) then
    if (get_progress(P_TALKBUTLER) == 0) then
      bubble(9,"Ah yes, Master Nostik asked",
               "me to give you this.","","");
      LOC_talk_butler();
    elseif (get_progress(P_TALKBUTLER) == 1) then
      bubble(9,"Good luck.","","","");
    else
      bubble(9,"I'm afraid Master Nostik is",
               "away at present.","","");
    end

  end
end
  
function LOC_talk_butler()  
  drawmap();
  screen_dump();
  sfx(6);
  msg("You've acquired 200 gp!",255,0);
  set_gp(get_gp()+200);
  drawmap();
  screen_dump();
  set_progress(P_TALKBUTLER,1);
  bubble(9,"Oh and there is one thing",
           "that I better warn you about.","","");
  bubble(9,"It's nothing major really, but",
           "this world just happens to be",
           "full of monsters that will",
           "constantly attack you.");
  bubble(HERO1,"Well, that's not so",
               "weird actually.","","");
  bubble(9,"These creatures are not your",
           "ordinary violent miscreants.","","");
  bubble(HERO1,"Are they ever?","","","");
  bubble(9,"These creatures are the",
           "manifestations of evil spirits.","","");
  bubble(9,"These creatures exist in a realm",
           "between the world of the living",
           "and the world of the dead.","");
  bubble(9,"They have no qualms about",
           "attacking anyone or anything.","","");
  bubble(HERO1,"That kind of thing happens",
               "everywhere. I didn't think",
               "it would be any different here.","");
  bubble(9,"Yes, but in your world, these",
           "wandering monsters are real and",
           "visible... even avoidable. That",
           "is not the case here.");
  bubble(HERO1,"Why not?","","","");
  bubble(9,"Well, like I said, the monsters",
           "here are spirits. That means",
           "that you can't see them.","");
  bubble(9,"You will see them once they",
           "attack you, but then it's",
           "already too late.","");
  bubble(HERO1,"So, do you mean to say that I",
               "could be attacked at any time",
               "without any warning whatsoever?","");
  bubble(9,"Well, not necessarily. For some",
           "unknown reason, these spirits",
           "do not enter the towns and",
           "villages of the land.");
  bubble(9,"In most other places though,",
           "these encounters are highly",
           "probable.","");
  if (get_pidx(0) == AYLA) then
    bubble(HERO1,"This reward your boss is",
                 "offering better be a lot",
                 "more than just handsome.","");
  else
    bubble(HERO1,"Oh well, I didn't think this",
                 "would be a walk in the park.","","");
  end
  bubble(9,"Yes, well, you had better",
           "be leaving now.","","");
  bubble(HERO1,"Certainly, thanks.","","","");
end
  
function LOC_chit_chat(a)
  local b;
  local c;

  b = get_ent_id(a);
  if (b == SENSAR) then
    bubble(a,"Good luck to you friend.","","","");
  elseif (b == SARINA) then
    bubble(a,"I really need that reward.","","","");
  elseif (b == CORIN) then
    bubble(a,"Hmmm... this all seems very odd.","","","");
  elseif (b == AJATHAR) then
    bubble(a,"I hope to learn much on this",
             "journey.","","");
  elseif (b == CASANDRA) then
    bubble(a,"That Staff is as good as mine.","","","");
  elseif (b == TEMMIN) then
    bubble(a,"This will certainly not be easy.","","","");
  elseif (b == AYLA) then
    bubble(a,"I am confident we will",
             "all do well.","","");
  elseif (b == NOSLOM) then
    bubble(a,"I am fortunate to",
             "have been selected.","","");
  end
end
