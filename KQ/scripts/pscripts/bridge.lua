-- bridge - on Brayden river Randen and Andra

function autoexec()
  if (get_treasure(8) == 1) then
    set_obs(19,27,0);
    set_zone(19,27,0);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(10) == 0 or get_progress(10) > 2) then
      if (get_progress(10) == 3) then
        set_progress(10,4);
      end
      change_map("main",240,29,240,29);
      return
    end
    if (get_progress(10) == 1) then
      bubble(255,"Ahhhhh!!!","","","");
      bubble(255,"Help!","","","");
      set_ent_movemode(2,2);
      set_ent_movemode(3,2);
      set_ent_movemode(4,2);
      set_ent_movemode(5,2);
      set_ent_movemode(6,2);
      wait_for_entity(2,6);
      set_ent_movemode(2,0);
      set_ent_movemode(3,0);
      set_ent_movemode(4,0);
      set_ent_movemode(5,0);
      set_ent_movemode(6,0);
      set_progress(10,2);
      set_btile(29,17,176);
      set_obs(29,17,1);
      set_zone(29,17,3);
    end
  
  elseif (zn == 2) then
    chest(8,105,2);
    autoexec();
  
  elseif (zn == 3) then
    if (get_progress(10) == 2) then
      can_run = 0;
      combat(0);
      can_run = 1;
      set_progress(10,3);
      set_btile(29,17,160);
      set_obs(29,17,1);
      set_zone(29,17,0);
    end

  end
end
  
function entity_handler(en)
  local a = get_progress(10);
  
  if (en >= 2) then
    if (a == 0) then
      set_progress(10,1);
    elseif (a == 2) then
      bubble(en,"There's some weird creatures",
                "in the water.","","");
      return
    elseif (a == 3) then
      bubble(en,"I think we'll take a break",
                "before getting back to work.","","");
      return
    end
  end
  
  if (en == 0) then
    if (a == 0) then
      bubble(0,"We're on the lookout",
               "for bandits.","","");
    elseif (a == 2) then
      bubble(0,"Um... we don't really have",
               "any experience dealing with",
               "anything hostile... could you",
               "take a look for us?");
    elseif (a == 3) then
      bubble(0,"Very impressive... it's a good",
               "thing you came along. Thanks!","","");
    elseif (a == 4) then
      bubble(0,"We should be done soon...",
               "come back a little later.","","");
    end
  
  elseif (en == 1) then
    if (a == 0) then
      if (get_progress(13) == 0) then
        bubble(1,"Those bandits better not show",
                 "their faces around here again!","","");
        bubble(200,"Or you'll thrash 'em right?","","","");
        bubble(1,"No... they'd better not show up",
                 "because I forgot my sword!","","");
        wait(50);
        bubble(1,"I probably shouldn't",
                 "have told you that.","","");
        set_progress(13,1);
      else
        bubble(1,"Let me know if you see",
                 "any bandits, will ya?","","");
      end
    elseif (a == 2) then
      bubble(1,".....","","","");
    elseif (a == 3) then
      bubble(1,"Wow!","","","");
    end
  
  elseif (en == 2) then
    if (a == 4 or a < 2) then
      bubble(2,"This is very hard work!","","","");
    end
  
  elseif (en == 3) then
    if (a == 4 or a < 2) then
      bubble(3,".....","","","");
      if (get_progress(14) == 0) then
        wait(50);
        bubble(3,"Wha...?","","","");
        set_ent_facing(3,2);
        bubble(3,"I wasn't sleeping!","","","");
        set_ent_facing(3,1);
        set_progress(14,1);
      end
    end
  
  elseif (en == 4) then
    if (a == 4 or a < 2) then
      bubble(4,"I think that other",
               "guy is asleep.","","");
    end
  
  elseif (en == 5) then
    if (a == 4 or a < 2) then
      bubble(5,"I really could use a break.",
               "It's been almost 15 minutes",
               "since the last one!","");
    end
  
  elseif (en == 6) then
    if (a == 4 or a < 2) then
      bubble(6,"I never should have listened",
               "to my mother when she told",
               "me to take up construction.","");
      bubble(6,"I should've been a florist",
                "like my dad.","","");
      if (a == 0) then
        set_progress(10,1);
      end
    end

  end
end  
