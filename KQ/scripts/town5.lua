-- town5 - "Sunarin"

function autoexec()
  if (get_progress(P_TALKGELIK) ~= 2) then
    set_ent_active(7,0);
  end
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_desc(0);
  end
  refresh();
end

function refresh()
  if (get_treasure(32) > 0) then
    set_obs(21,27,0);
  end
end

function postexec()
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_progress(P_FOUGHTGUILD,3);
    set_progress(P_EMBERSKEY,0);
    bubble(HERO1,"Oh no!","","","");
    bubble(HERO1,"I must have dropped",
                 "that key inside!","","");
    bubble(HERO1,"Oh well, I got what I came for.","","","");
  end
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",247,115,247,115);

  elseif (zn == 2) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 3) then
    door_in(65,16,61,5,77,19);

  elseif (zn == 4) then
    door_in(73,16,61,5,77,19);

  elseif (zn == 5) then
    door_in(65,36,61,25,75,39);

  elseif (zn == 6) then
    door_in(71,36,61,25,75,39);

  elseif (zn == 7) then
    door_out(24,12);

  elseif (zn == 8) then
    door_out(28,12);

  elseif (zn == 9) then
    door_out(37,34);

  elseif (zn == 10) then
    door_out(42,34);

  elseif (zn == 11) then
    inn("River's Way Inn",70,1);

  elseif (zn == 12) then
    shop(13);

  elseif (zn == 13) then
    shop(14);

  elseif (zn == 14) then
    bubble(HERO1,"Worst books ever.","","","");

  elseif (zn == 15) then
    bubble(HERO1,"What language is this?","","","");

  elseif (zn == 16) then
    chest(32,I_PRIESTESS,1);
    refresh();

  elseif (zn == 17) then
    bubble(HERO1,"The gates are",
                 "closed and locked.","","");

  elseif (zn == 18) then
    if (get_progress(P_EMBERSKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
    else
      bubble(HERO1,"Key goes in...","","","");
      change_map("guild",0,0,0,0);
    end

  elseif (zn == 19) then
    touch_fire(get_pidx(0));

  end
end

function entity_handler(en)
  if (en == 0) then
    bubble(0,"Do you like this shirt?","","","");

  elseif (en == 1) then
    bubble(1,"I really like that guy's shirt.","","","");

  elseif (en == 2) then
    bubble(2,"I have to stop bringing",
             "my wife with me.","","");

  elseif (en == 3) then
    bubble(3,"I must have had too many drinks.",
             "My husband is starting",
             "to look handsome.","");

  elseif (en == 4) then
    bubble(4,"These two are weird... but at",
             "least I don't have to sit alone.","","");

  elseif (en == 5) then
    bubble(5,"Those gates to the inner city",
             "never open. Strangers are not",
             "allowed in.","");

  elseif (en == 6) then
    bubble(6,"Welcome to Sunarin!",
             "Please feel free to",
             "leave at any time.","");

  elseif (en == 7) then
    set_progress(P_TALKGELIK,3);
    bubble(7,"Please... help me... I brought",
             "soldiers with me... but the",
             "Embers killed them. I barely",
             "escaped with my life!");
    bubble(HERO1,"Who are you? And who",
                 "are the Embers?","","");
    bubble(7,"My name is Dungar. The Embers is",
             "the short name for the Thieves",
             "of the Ember Crest. They run",
             "this town.");
    bubble(7,"I came to get back something",
             "they stole from me...","","");
    bubble(HERO1,"The Opal Helmet?","","","");
    bubble(7,"Why yes! The Opal Helmet.",
             "Who are you? You're not",
             "one of them are you?","");
    bubble(HERO1,"No. My name is $0. I am",
                 "on a quest, and I need to borrow",
                 "that helmet to traverse the",
                 "underwater passage.");
    bubble(7,"I see. Well, if you can get",
             "it back, I will certainly",
             "let you use it.","");
    bubble(HERO1,"Well, how do we get it back?","","","");
    bubble(7,"We can discuss that",
             "back at my place.","","");
    bubble(HERO1,"Okay.","","","");
    set_progress(P_EMBERSKEY,1);
    change_map("estate",57,17,57,17);

  end
end
