-- fort - "Mountain fort south of Andra"

function autoexec()
  if (get_progress(P_TALKDERIG) > 2) then
    set_ent_active(4,0);
  end
  if (get_progress(P_TALKDERIG) == 3) then
    set_ent_active(5,1);
    if (get_numchrs() == 2) then
      set_ent_tilex(HERO2,get_ent_tilex(HERO2)+1);
      orient_heroes();
    end
    set_desc(0);
  else
    set_ent_active(5,0);
  end
end

function postexec()
  if (get_progress(P_TALKDERIG) == 3) then
    view_range(1,36,11,44,19);
    set_ent_facing(HERO1,1);
    if (get_numchrs() == 2) then
      set_ent_facing(HERO2,1);
    end
    drawmap();
    screen_dump();
    do_fadein(4);
    set_holdfade(0);
    bubble(5,"So that's about it... cross the",
             "river and head for the small",
             "group of mountains due west from",
             "there.");
    bubble(5,"You just have to go through some",
             "caves to reach the valley beyond.","","");
    bubble(5,"Then make your way to the Oracle's",
             "tower. Is that understood?","","");
    bubble(HERO1,"Yeah, I guess so... it all",
                 "seems clear enough.","","");
    bubble(5,"Great! Head out whenever you're",
             "ready.","","");
    set_progress(P_TALKDERIG,4);
  end
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",261,33,261,33);

  elseif (zn == 2) then
    change_map("main",261,36,261,36);

  elseif (zn == 3) then
    door_in(40,16,36,10,44,18);

  elseif (zn == 4) then
    door_out(17,31);

  end
end

function entity_handler(en)
  if (en == 0) then
    bubble(0,"We have no outposts in goblin",
             "territory. If you get into real",
             "trouble, you'll have to go all",
             "the way back to Andra.");

  elseif (en == 1) then
    bubble(1,"Make sure to stock",
             "up on supplies.","","");

  elseif (en == 2) then
    if (get_progress(P_TALKDERIG) < 3) then
      bubble(2,"We cannot let you pass.","","","");
    else
      bubble(2,"You are free to come and",
               "go as you please.","","");
    end

  elseif (en == 3) then
    if (get_progress(P_TALKDERIG) < 3) then
      bubble(3,"No one is allowed",
               "through right now.","","");
    else
      bubble(3,"Be careful in the goblin lands.","","","");
    end

  elseif (en == 4) then
    bubble(4,"Only the captain may authorize",
             "passage through here.","","");

  elseif (en == 5) then
    bubble(5,"Best of luck to you.","","","");

  end
end
