-- estate - "Home of Dungar and Emma east of Maldea"

function autoexec()
  if (get_progress(P_OPALHELMET) == 0 and get_progress(P_TALKGELIK) < 3) then
    set_ent_active(2,0);
  end
  if (get_progress(P_TALKGELIK) > 3) then
    set_obs(22,22,1);
    set_zone(22,22,6);
    set_ent_active(0,0);
  end
  if (get_progress(P_EMBERSKEY) == 1) then
    view_range(1,53,5,69,21);
    set_ent_facing(0,1);
    set_desc(0);
  end
end

function postexec()
  if (get_progress(P_EMBERSKEY) == 1) then
    wait(50);
    bubble(2,"Well, you can get into the Ember's",
             "guild through the house in the",
             "south-west part of town. The door",
             "is locked, so you'll need this.");
    set_progress(P_EMBERSKEY,2);
    sfx(5);
    msg("Ember's key procured",255,0);
    bubble(2,"In the house are some stairs",
             "leading to a small basement.","","");
    bubble(2,"On the center bookshelf is a book",
             "titled 'How to Enter the Embers'",
             "Secret Hideout'.","");
    bubble(2,"Just pull on the book and the",
             "bookshelf will slide over revealing",
             "an entrance to the guild.","");
    bubble(2,"The rest is up to you after that.","","","");
    bubble(HERO1,"How do you know all this?","","","");
    bubble(2,"Well, I... umm... that is to say...","","","");
    bubble(1,"Dungar is a retired thief...","","","");
    bubble(2,"Emma!","","","");
    bubble(1,"Well it's true! In fact Dungar was",
             "only one step away from becoming",
             "guildmaster!","");
    bubble(2,"Yes, but I got out! The guild was",
             "getting too big, and too violent. I",
             "just had to leave.","");
    bubble(2,"I may have been a thief, but I",
             "refused to kill innocent people!","","");
    bubble(HERO1,"So why are the Embers trying to",
                 "get this Helmet back? Did you",
                 "take it from them?","");
    bubble(2,"Well, more or less... but it's",
             "mine. The guild wanted it, of",
             "course, but it's mine and I do not",
             "wish to part with it.");
    bubble(2,"It has sentimental value to me.","","","");
    bubble(HERO1,"Well, that's good enough for me.",
                 "I will be off then.","","");
    bubble(2,"Great! Oh, could you please bring",
             "the Helmet back here before you",
             "continue on your quest?","");
    bubble(2,"I just want to make sure that",
             "they haven't damaged it.","","");
    bubble(HERO1,"No problem. I'll be back in a bit.","","","");
  end
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",222,41,222,41);

  elseif (zn == 2) then
    door_in(61,17,53,5,69,20);

  elseif (zn == 3) then
    door_in(69,31,66,23,74,34);

  elseif (zn == 4) then
    chest(54,I_EDROPS,1);

  elseif (zn == 5) then
    door_out(22,23);

  elseif (zn == 6) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 7) then
    view_range(1,71,5,87,19);
    warp(86,17,8);

  elseif (zn == 8) then
    if (get_progress(P_TALKGELIK) < 4) then
      bubble(HERO1,"That scream came from up here",
                   "somewhere! I can't leave!","","");
    else
      view_range(1,53,5,69,20);
      warp(68,17,8);
    end

  elseif (zn == 9) then
    bubble(HERO1,"These look rare.","","","");

  elseif (zn == 10) then
    door_out(36,23);

  elseif (zn == 11) then
    if (get_progress(P_TALKGELIK) < 4) then
      set_ent_script(HERO1,"U1F0");
      wait_for_entity(HERO1,HERO1);
    end

  elseif (zn == 12) then
    if (get_progress(P_TALKGELIK) < 4) then
      set_progress(P_TALKGELIK,4);
      set_ent_facing(1,0);
      bubble(1,"Oh $0!","","","");
      bubble(1,"I woke up because I thought",
               "I heard fighting in your room.","","");
      bubble(1,"When I turned to wake Dungar,",
               "I realized that he was dead.","","");
      bubble(1,"Someone stabbed him",
               "in his sleep!","","");
      bubble(HERO1,"Oh no! You were right though,",
                   "I was fighting an assassin who",
                   "was sent here to kill me. He",
                   "obviously was not alone.");
      bubble(HERO1,"This is all my fault!","","","");
      bubble(1,"No it's not... this is all",
               "because of that damn guild!","","");
      bubble(HERO1,"Yes, but when I was there to get",
                   "the Helmet back, it all seemed",
                   "as though it was too easy.","");
      bubble(HERO1,"They set me up. They wanted",
                   "to follow me so that they",
                   "could kill Dungar!","");
      bubble(1,"That may be true $0,",
               "but you were doing the right",
               "thing. You didn't know.","");
      bubble(HERO1,"I'm so sorry Emma!","","","");
      bubble(1,"It's okay... I guess I've been",
               "expecting something like this to",
               "happen for a long time.","");
      bubble(1,"You can't marry an ex-thief and",
               "expect to have a normal life.",
               "I'll be alright.","");
      bubble(HERO1,"Are you sure... is there",
                   "anything I can do?","","");
      bubble(HERO1,"I know... I'll take care of that",
                   "guild once and for all!","","");
      bubble(1,"No! Don't do that. You'll",
               "just get yourself killed! You",
               "have a quest to continue.","");
      bubble(1,"What you can do for me is take",
               "that damn Helmet far away from",
               "here... for good.","");
      bubble(HERO1,"Are you sure?","","","");
      bubble(1,"I'm very sure. As long as I",
               "have that thing I too will",
               "be in danger. You keep it.","");
      bubble(1,"And don't worry about the guild.",
               "They won't be able to find",
               "you if you keep moving around.","");
      bubble(HERO1,"Okay... but what about you?",
                   "It's not safe here. You have",
                   "to leave!","");
      bubble(1,"I will. I'll gather my things",
               "and leave right after I give my",
               "husband a proper funeral.","");
      bubble(1,"You should leave right away.",
               "They know you're here.","","");
      bubble(HERO1,"Okay... you be careful alright?","","","");
      bubble(1,"Don't worry about me.",
               "I'll be fine. Now go!","","");
      bubble(HERO1,"Goodbye.","","","");
    end

  elseif (zn == 13) then
    bubble(HERO1,"Goodbye Dungar. I'm sorry.","","","");

  elseif (zn == 14) then
    touch_fire(get_pidx(0));

  end
end

function entity_handler(en)
  if (en == 0) then
    bubble(0,"I'm the groundskeeper here.","","","");

  elseif (en == 1) then
    local a;

    if (get_progress(P_OPALHELMET) == 0) then
      a = get_progress(P_TALKGELIK);
      if (a == 0) then
        bubble(1,"Hello, are you a friend",
                 "of my husband's?","","");
      elseif (a == 1) then
        bubble(1,"You need the Opal helmet?  I'm",
                 "afraid you are too late... It",
                 "was stolen!","");
        bubble(1,"My husband left for Sunarin to",
                 "try and get it back. If you go",
                 "there you will probably find",
                 "him.");
        set_progress(P_TALKGELIK,2);
      elseif (a == 2) then
        bubble(1,"Did you find my husband?","","","");
      elseif (a == 3) then
        bubble(1,"Good luck!","","","");
      end
    else
      if (get_progress(P_TALKGELIK) == 3) then
        bubble(1,"Fabulous!","","","");
      else
        bubble(1,"...","","","");
      end
    end

  elseif (en == 2) then
    if (get_progress(P_OPALHELMET) == 0) then
      bubble(2,"Just return here with the Helmet",
               "once you find it. I'd like to",
               "see it before you borrow it.","");
    else
      bubble(2,"Excellent work! The Helmet",
               "appears to be undamaged... this",
               "is wonderful.","");
      bubble(1,"Dungar, why don't you invite our",
               "young friend to stay the night?",
               "It's getting awfully late.","");
      bubble(HERO1,"Well, I'd love to, but I",
                   "really should be going.","","");
      bubble(2,"Nonsense! I insist you spend",
               "the night with us and celebrate.",
               "","");
      bubble(HERO1,"Well, alright... but I'll",
                   "have to leave first thing",
                   "in the morning.","");
      bubble(2,"Stupendous! Now let's",
               "start the celebration.","","");
      do_fadeout(4);
      wait(100);
      set_holdfade(1);
      set_autoparty(1);
      warp(81,11,16);
      set_ftile(61,13,0);
      set_zone(61,13,0);
      set_obs(61,13,0);
      set_btile(61,14,201);
      set_ent_active(3,0);
      view_range(1,80,5,87,12);
      set_holdfade(0);
      set_ent_facing(HERO1,1);
      drawmap();
      screen_dump();
      do_fadein(4);
      msg("Later...",255,1000);
      bubble(HERO1,"Ugh... what a party!","","","");
      set_ent_script(HERO1,"R3U1L1W50F1");
      wait_for_entity(HERO1,HERO1);
      bubble(HERO1,"Ah... sleep.","","","");
      drawmap();
      screen_dump();
      stop_song();
      sfx(36);
      do_fadeout(2);
      rest(3000);
      set_ent_active(3,1);
      set_ent_active(1,1);
      place_ent(1,76,11);
      set_ent_facing(1,1);
      set_ent_active(2,1);
      set_ent_facing(2,0);
      drawmap();
      screen_dump();
      do_fadein(16);
      bubble(HERO1,"Huh... what was that?","","","");
      set_ent_facing(HERO1,3);
      drawmap();
      screen_dump();
      bubble(3,"Oops!","","","");
      bubble(HERO1,"Hey!","","","");
      set_ent_script(HERO1,"R1D1R1");
      wait_for_entity(HERO1,HERO1);
      bubble(HERO1,"Who are you?","","","");
      bubble(3,"I'm an assassin, you moron.","","","");
      bubble(HERO1,"So you're here to kill me?","","","");
      bubble(3,"No, I'm selling tickets to the",
               "assassin's jamboree. Of course",
               "I'm here to kill you!","");
      bubble(HERO1,"Well there's no need to",
                   "be so rude about it.","","");
      bubble(3,"Shut up!","","","");
      drawmap();
      screen_dump();
      set_run(0);
      combat(23);
      set_run(1);
      if (get_alldead() == 1) then
        return
      end
      view_range(1,71,5,87,20);
      set_ent_active(3,0);
      set_ent_active(2,0);
      set_mtile(73,11,288);
      place_ent(1,76,11);
      set_ent_facing(1,2);
      drawmap();
      screen_dump();
      wait(50);
      bubble(HERO1,"Gee... good thing I wore",
                   "all my equipment to bed.","","");
      wait(50);
      bubble(1,"Aaaahhhh!","","","");
      set_ent_facing(HERO1,2);
      drawmap();
      screen_dump();
      bubble(HERO1,"Oh oh, that sounded",
                   "like Dungar's wife!","","");
      set_vfollow(1);
      set_autoparty(0);
    end

  end
end
