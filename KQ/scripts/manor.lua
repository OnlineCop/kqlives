-- manor - "Nostik's manor southwest of Ekla"

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
  local player_response, done_talking;

  if (get_progress(P_TALKBUTLER) == 0) then
    rest(200);
    bubble(8,"Welcome all.","","","");
    bubble(8,"I am the legendary Nostik. I am a wizard. A very old wizard in fact. I know the secret of the Great Magic, and you have come to learn.");
    bubble(8,"Be heretofore warned however: my words are not to be meddled with, for you are crunchy and taste good with...");
    bubble(8,"Well, never mind. I shall get right to the point.");
    bubble(8,"I have said that I am in need of adventurers to partake in a quest for me. You eight have shown great courage to stay. Courage alone isn't enough, however. You must prove your worthiness of this Great Magic. Does anyone wish to leave?");
    set_ent_script(8,"W25F1W35F3W35F0W35F3W50");
    wait_for_entity(8,8);
    if (prompt(8,2,0,"Very well then. Do you need a",
                     "background?","  yes","  no") == 0) then
      -- yes
      while(not done_talking) do
        player_response = prompt(8,3,0,"What do you want to learn about?",
                                       "  Staff of Xenarum",
                                       "  Malkaron",
                                       "  Done");
        if (player_response == 0) then
          bubble(8,"A very long time ago, a great priest appeared at the Xenarum shrine. There he chose three Disciples to follow and learn of a better life. Some say that this priest was the very Adrial the Messiah.");
          bubble(8,"Taking a non-violent approach to everything, Adrial chose Three Disciples to follow him and spread peace and knowledge around the land. Adrial taught of becoming the best that you can be in the followings of `The God'. The legend suggests that his Three Disciples were jealous of his connection with `The God' and betrayed him.");
          bubble(8,"This is where you come in. Supposedly one Adrial's Disciples, Chaiman, kept a journal of his voyage. Inside the journal speaks of what actually happened with the miracles and everything. No one has ever seen this journal but word has it that it is kept somewhere in Xenarum. That land is inaccessible except with the Staff of Xenarum, which is the key to the Shifting Mounts.");
          bubble(8,"Your journey is to find this Staff. Bring it to me; I must see what is in that journal! I am offering to teach any of you the Great Magic in exchange for that Staff.");
          bubble(8,"You see now why I have chosen only the eight of you; this journey must be as inconspicuous as possible, as the Three Disciples may have minions everywhere. You can travel with anyone you wish, but please stay in groups of no more than two. More than that will draw attention to you, and may put all of us in jeopardy.");
        elseif (player_response == 1) then
          -- explain the whole Maklaron bit
          thought(8,"Oh great. How will I explain",
                    "THIS one?","","");
        elseif (player_response == 2) then
          done_talking=1;
        end
      end
    end
    bubble(8,"I thank you for your help. Talk to my butler before you leave and he will help you get started on your journey.");
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
               "me to give you this.");
      LOC_talk_butler();
    end

  elseif (zn == 7) then
    touch_fire(get_pidx(0));

  end
end

function entity_handler(en)
  if (en >= 0 and en <= 7) then
    LOC_chit_chat(en);

  elseif (en == 8) then
    if (get_progress(P_TALKBUTLER) == 0) then
      bubble(8,"Talk to my butler before you leave and he will help you get started on your journey.");
    else
      bubble(8,"Feel free to look through my books and scrolls. I have made a study about this Disciple's journal and you may learn much.");
    end

  elseif (en == 9) then
    if (get_progress(P_TALKBUTLER) == 0) then
      bubble(9,"Ah yes, Master Nostik asked",
               "me to give you this.","","");
      LOC_talk_butler();
    elseif (get_progress(P_TALKBUTLER) == 1) then
      bubble(9,"Good luck.","","","");
    else
      bubble(9,"I'm afraid Master Nostik has already gone in search of the Staff of Xenarum.");
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
