-- town4 - "Maldea"

function autoexec()
  if (get_progress(P_WARPEDTOT4) == 0) then
    if (get_numchrs() > 1) then
      set_progress(P_OLDPARTNER,get_pidx(1)+1);
      remove_chr(get_pidx(1));
      set_ent_active(HERO2,0);
    else
      set_progress(P_OLDPARTNER,0);
    end
    set_progress(P_WARPEDTOT4,1);
    set_desc(0);
  end
  if (get_progress(P_BOUGHTHOUSE) == 1) then
    set_ent_active(6,0);
  end
-- TT edit:
--  if (get_ent_tilex(HERO1) > 93 and get_ent_tilex(HERO1) < 107 and get_ent_tiley(HERO1) > 0 and get_ent_tiley(HERO1) < 12) then
  if (get_ent_tilex(HERO1) > 93 and get_ent_tilex(HERO1) < 107 and
      get_ent_tiley(HERO1) > 0 and get_ent_tiley(HERO1) < 12) then
    set_sstone(0);
    view_range(1,93,0,107,12);
    set_desc(0);
  end
  refresh();
end

function refresh()
  if (get_treasure(81) > 0) then
    set_ftile(94,7,237);
  end
end

function postexec()
  local a,b;

  if (get_progress(P_WARPEDTOT4) == 1) then
    if (get_progress(P_OLDPARTNER) > 0) then
-- TT edit:
--      bubble(HERO1,"What?","","","","");
      bubble(HERO1,"What?");
      set_autoparty(1);
      set_ent_script(HERO1,"F0W25F2W25F1W25F3W25F0");
      wait_for_entity(HERO1,HERO1);
      set_autoparty(0);
      a = get_progress(P_OLDPARTNER)-1;
      b = "Where's "..get_party_name(a).."?";
-- TT edit:
--      bubble(HERO1,b,"","","");
      bubble(HERO1,b);
-- TT edit:
--      bubble(HERO1,"Just great... how am I supposed to get back?");
      bubble(HERO1,"Just great... how am I",
                   "supposed to get back?","","");
      wait(100);
      if (get_pidx(0) == CASANDRA) then
        if (get_progress(P_OLDPARTNER)-1 == TEMMIN) then
          bubble(HERO1,"I have to find him! I'm not letting",
-- TT edit:
--                        "him get away from me that easily.","","");
                        "him get away from me that easily.");
        else
-- TT edit:
--          bubble(HERO1,"Oh well.","","","");
          bubble(HERO1,"Oh well.");
        end
      else
        bubble(HERO1,"Sigh. I guess I'll just have",
                     "to keep going and hope I find",
-- TT edit:
--                     "a way to get back.","","");
                     "a way to get back.");
        wait(50);
      end
    end
    set_progress(P_WARPEDTOT4,2);
  end
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",205,45,205,45);

  elseif (zn == 2) then
    door_in(96,8,93,0,103,12);
    set_ent_active(6,0);

  elseif (zn == 3) then
    door_in(116,9,109,0,123,12);

  elseif (zn == 4) then
    door_in(132,9,125,0,139,12);

  elseif (zn == 5) then
    door_in(148,11,141,0,155,14);

  elseif (zn == 6) then
    door_in(100,25,93,16,107,28);

  elseif (zn == 7) then
    door_in(116,25,109,16,123,28);

  elseif (zn == 8) then
    door_in(132,25,125,16,139,28);

  elseif (zn == 9) then
    door_in(148,25,141,16,155,28);

  elseif (zn == 10) then
    door_in(98,39,93,30,103,42);

  elseif (zn == 11) then
    door_in(110,39,105,30,115,42);

  elseif (zn == 12) then
    door_in(122,43,117,30,127,46);

  elseif (zn == 13) then
    door_in(137,39,129,30,145,42);

  elseif (zn == 14) then
    door_out(22,20);

  elseif (zn == 15) then
    door_out(41,24);

  elseif (zn == 16) then
    door_out(57,26);

  elseif (zn == 17) then
    door_out(23,64);

  elseif (zn == 18) then
    door_out(16,30);

  elseif (zn == 19) then
    door_out(31,38);

  elseif (zn == 20) then
    door_out(61,41);

  elseif (zn == 21) then
    door_out(75,60);

  elseif (zn == 22) then
    door_out(76,81);

  elseif (zn == 23) then
    door_out(59,66);

  elseif (zn == 24) then
    door_out(18,79);

  elseif (zn == 25) then
    door_out(34,81);

  elseif (zn == 26) then
    shop(10);

  elseif (zn == 27) then
    shop(11);

  elseif (zn == 28) then
    shop(12);

  elseif (zn == 29) then
    inn("Seaside Inn",60,1);

  elseif (zn == 30) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 31) then
    book_talk(get_pidx(0));

  elseif (zn == 32) then
    touch_fire(get_pidx(0));

  elseif (zn == 33) then
    chest(51,I_FRUNE,1);

  elseif (zn == 34) then
    chest(52,I_EDROPS,1);

  elseif (zn == 35) then
    chest(53,I_RUBYBROOCH,1);

  elseif (zn == 36) then
    chest(81,I_B_WARP,1);
    refresh();

  elseif (zn == 37) then
    if (prompt(255,2,0,"Take a nap?","  yes","  no","") == 0) then
      for a=0,get_numchrs()-1,1 do
        set_party_hp(a,get_party_mhp(a));
        set_party_mp(a,get_party_mmp(a));
      end
      inn("House",0,0);
    end

  elseif (zn == 38) then
    set_save(1);

  elseif (zn == 39) then
    set_save(0);

  elseif (zn == 40) then
    view_range(1,93,44,102,60);
    warp(101,57,8);

  elseif (zn == 41) then
    view_range(1,93,0,103,12);
    warp(101,9,8);

  elseif (zn == 42) then
    door_out(24,16);

  elseif (zn == 43) then
    door_in(95,48,93,44,102,60);

  elseif (zn == 44) then
    view_range(1,93,44,102,60);
    warp(95,48,8);

  elseif (zn == 45) then  -- item shop door
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 46) then  -- armor shop door
    bubble(HERO1,"Locked.","","","");

  end
end

function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_TALKGELIK) == 0) then
      bubble(0,"Opal armor? I have no idea where",
               "you would find something like",
-- TT edit:
--               "that.","");
               "that.");
      bubble(0,"You should go talk to Gelik.",
               "He's old... he knows all kinds",
               "of stuff. He lives up in the",
               "north end of town.");
    else
      bubble(0,"So how do you like",
-- TT edit:
--               "our little town?","","");
               "our little town?");
    end

  elseif (en == 1) then
    bubble(1,"I planted all of the flowers",
-- TT edit:
--             "in this town. I love flowers.","","");
             "in this town. I love flowers.");
    bubble(1,"Did I mention that",
-- TT edit:
--             "I love flowers?","","");
             "I love flowers?");

  elseif (en == 2) then
    bubble(2,"Did you meet the old flower",
-- TT edit:
--             "lady? She's nuts!","","");
             "lady? She's nuts!");

  elseif (en == 3) then
    bubble(3,"You should go talk to Trezin on",
             "the other side of town. He's",
             "selling his house, and it's a",
             "really good deal.");

  elseif (en == 6) then
    if (get_progress(P_BOUGHTHOUSE) == 0) then
      bubble(6,"Hey, the name's Trezin, and have",
-- TT edit:
--               "I got a deal for you!","","");
               "I got a deal for you!");
      if (prompt(6,2,0,"You can have this lovely house",
                       "for a mere 7500 gp. Sound good?",
                       "  no","  yes") == 0) then
        bubble(6,"So be it. You know where I am",
-- TT edit:
--                 "if you change your mind.","","");
                 "if you change your mind.");
      else
        if (get_gp() >= 7500) then
          set_gp(get_gp()-7500);
        else
          bubble(6,"Hmm... looks like you're a bit",
                   "short on funds. Fear not. I'll",
                   "be here should you manage to",
                   "find enough money.");
          return
        end
        bubble(6,"Fantastic! You know a good deal",
-- TT edit:
--                 "when you see one.","","");
                 "when you see one.");
        bubble(6,"Oh, there are some things you",
-- TT edit:
--                 "should know about the house.","","");
                 "should know about the house.");
        bubble(6,"First off, you can sleep here",
                 "fully recover HP and MP for",
-- TT edit:
--                 "free!","");
                 "free!");
        bubble(6,"Secondly, there is a save spot",
                 "right in the house! How's that",
-- TT edit:
--                 "for convienience?","");
                 "for convienience?");
        bubble(6,"And lastly, I've even left",
                 "a little something inside for",
                 "you. It's a very handy spell",
                 "called Warp.");
        bubble(6,"The Warp spell lets you escape",
                 "from dungeons and other such",
-- TT edit:
--                 "places.","");
                 "places.");
        bubble(6,"As a bonus, you can use Warp",
                 "from anywhere on the world map",
                 "and it will bring you back to",
                 "the house! Great huh?");
        bubble(6,"Well, enough of my yapping.",
                 "Go in and see for yourself!,",
                 "I've got to go put this",
                 "money somewhere safe!");
        set_ent_movemode(6,2);
        set_ent_obsmode(6,0);
        if (get_ent_facing(6) == 3) then
          set_ent_script(6,"D1R1");
        else
          set_ent_script(6,"R1D1");
        end
        wait_for_entity(6,6);
        set_progress(P_BOUGHTHOUSE,1);
      end
    else
      bubble(6,"What are you waiting for?",
-- TT edit:
--               "Go in and take a look around.","","");
               "Go in and take a look around.");
    end

  elseif (en == 7) then
    if (get_progress(P_OPALHELMET) == 0) then
      if (get_progress(P_TALKGELIK) == 0) then
        bubble(7,"So, you're looking for Opal",
                 "armor eh? Well, I know of one",
-- TT edit:
--                 "piece.","");
                 "piece.");
        bubble(7,"The Opal Helmet belongs to",
                 "a wealthy man named Dungar",
                 "who lives in his own private",
                 "estate east of town.");
        bubble(7,"You should go and",
-- TT edit:
--                 "talk to him.","","");
                 "talk to him.");
        bubble(7,"If you get the Helmet, come back",
                 "here and hopefully by then I",
                 "can find out out a little bit",
                 "more about the other pieces.");
        set_progress(P_TALKGELIK,1);
      else
        bubble(7,"There isn't anything else I",
                 "can tell you about opal",
-- TT edit:
--                 "armor right now.","");
                 "armor right now.");
      end
    end
    if (get_progress(P_OPALSHIELD) == 0 and get_progress(P_OPALHELMET) > 0) then
      if (get_progress(P_TALKGELIK) == 4) then
        bubble(7,"Ah, you are back and you",
-- TT edit:
--                 "have the Helmet.","","");
                 "have the Helmet.");
        bubble(7,"Oh, in case you are curious,",
                 "you can't wear any of the armor",
                 "until you get all of the pieces",
                 "and go to the Water Shrine.");
-- TT edit:
--        bubble(HERO1,"Oh... that's why.","","","");
        bubble(HERO1,"Oh... that's why.");
        bubble(7,"Yes, well anyways, I have found",
-- TT edit:
--                 "out where the next piece is.","","");
                 "out where the next piece is.");
        bubble(7,"You see, it turns out that the",
                 "Opal Shield is the prize for",
                 "besting the warrior Trayor",
                 "at the Coliseum.");
        if (get_progress(P_SEECOLISEUM) == 0) then
-- TT edit:
--          bubble(HERO1,"Coliseum?","","","");
          bubble(HERO1,"Coliseum?");
        end
        bubble(7,"The coliseum is west of",
-- TT edit:
--                 "Sunarin. You can't miss it.","","");
                 "Sunarin. You can't miss it.");
        if (get_progress(P_SEECOLISEUM) == 1) then
          bubble(HERO1,"But the Coliseum was closed",
-- TT edit:
--                       "when I went there.","","");
                       "when I went there.");
          bubble(7,"Yes... well I'm sure it's",
-- TT edit:
--                   "open by now.","","");
                   "open by now.");
-- TT edit:
--          bubble(HERO1,"Isn't that convienent?","","","");
          bubble(HERO1,"Isn't that convienent?");
-- TT edit:
--          bubble(7,"Quite.","","","");
          bubble(7,"Quite.");
        end
        bubble(HERO1,"So, I have to go there and",
-- TT edit:
--                     "ask Trayor for the shield?","","");
                     "ask Trayor for the shield?");
        bubble(7,"No, you have to",
-- TT edit:
--                 "fight him for it.","","");
                 "fight him for it.");
        bubble(HERO1,"Fight, fight, fight.",
-- TT edit:
--                     "That's all I ever do.","","");
                     "That's all I ever do.");
        bubble(7,"Well, that's just how",
-- TT edit:
--                 "these things work.","","");
                 "these things work.");
        bubble(HERO1,"Well, I guess I'm off to fight",
                     "Trayor. Oh by the way, what kind",
                     "of battle is this? Is magic",
                     "permitted?");
        bubble(7,"Yes indeed. Anything goes... and",
                 "Trayor is a very capable",
-- TT edit:
--                 "warrior. It won't be easy.","");
                 "warrior. It won't be easy.");
        bubble(HERO1,"Oh... well nothing",
-- TT edit:
--                     "is ever easy right?","","");
                     "is ever easy right?");
-- TT edit:
--        bubble(7,"Not if it's worth doing, no.","","","");
        bubble(7,"Not if it's worth doing, no.");
        bubble(HERO1,"Thanks. I'll see you",
-- TT edit:
--                     "after I get the Shield.","","");
                     "after I get the Shield.");
        set_progress(P_TALKGELIK,5);
        set_progress(P_SEECOLISEUM,2);
        return
      else
-- TT edit:
--        bubble(7,"Good luck.","","","");
        bubble(7,"Good luck.");
      end
    end
    if (get_progress(P_OPALSHIELD) == 1 and get_progress(P_TALKGELIK) < 6) then
      bubble(7,"Oh... you're back. I'm sorry,",
               "but I haven't been able to find",
-- TT edit:
--               "out where the other pieces are.","");
               "out where the other pieces are.");
-- TT edit:
--      bubble(HERO1,"Hey, don't worry about it.","","","");
      bubble(HERO1,"Hey, don't worry about it.");
      bubble(HERO1,"My friend here brought me",
                   "information about the Band and",
                   "the Armor. We'll have no trouble",
                   "finding it now.");
      bubble(HERO1,"I appreciate all your help in",
                   "finding the Helmet and the",
-- TT edit:
--                   "Shield. Thanks!","");
                   "Shield. Thanks!");
      bubble(7,"Well, that's great... and",
               "you're welcome. I'm glad",
-- TT edit:
--               "I was able to help.","");
               "I was able to help.");
-- TT edit:
--      bubble(HERO1,"I'll see you around then.","","","");
      bubble(HERO1,"I'll see you around then.");
-- TT edit:
--      bubble(7,"Certainly. Good journey.","","","");
      bubble(7,"Certainly. Good journey.");
      set_progress(P_TALKGELIK,6);
      return
    end
    if (get_progress(P_TALKGELIK) == 6) then
-- TT edit:
--      bubble(7,"How are things going?","","","");
      bubble(7,"How are things going?");
    end

  elseif (en == 8) then
-- TT edit:
--    bubble(8,"Hello, nice day isn't it?","","","");
    bubble(8,"Hello, nice day isn't it?");

  elseif (en == 9) then
    bubble(9,"So the duck says, 200 gp...",
-- TT edit:
--             "same as in town.","","");
             "same as in town.");
-- TT edit:
--    bubble(9,"Ha ha ha ha!","","","");
    bubble(9,"Ha ha ha ha!");
-- TT edit:
--    bubble(9,"Don't you get it?","","","");
    bubble(9,"Don't you get it?");

  elseif (en == 10) then
    bubble(10,"My roommate tells",
-- TT edit:
--             "the worst jokes.","","");
             "the worst jokes.");

  elseif (en == 11) then
-- TT edit:
--    bubble(11,"... huh... oh, hi... I'm sick.","","","");
    bubble(11,"... huh... oh, hi... I'm sick.");

  elseif (en == 12) then
-- TT edit:
--    bubble(12,"zzz","","","");
    bubble(12,"zzz");

  elseif (en == 13) then
-- TT edit:
--    bubble(13,"My son is sick in bed.","","","");
    bubble(13,"My son is sick in bed.");

  end
end
