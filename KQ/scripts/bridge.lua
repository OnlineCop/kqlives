-- bridge - "On Brayden river Randen and Andra"

function autoexec()
  if (get_treasure(8) == 1) then
    set_obs(19, 27, 0);
    set_zone(19, 27, 0);
  end
end


function postexec()
  return;
end


function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(P_FIGHTONBRIDGE) == 0 or get_progress(P_FIGHTONBRIDGE) > 2) then
      if (get_progress(P_FIGHTONBRIDGE) == 3) then
        set_progress(P_FIGHTONBRIDGE, 4);
      end
      change_map("main", 240, 29, 240, 29);
      return;
    end
    if (get_progress(P_FIGHTONBRIDGE) == 1) then
      bubble(255, "Ahhhhh!!!");
      bubble(255, "Help!");
      set_ent_movemode(2, 2);
      set_ent_movemode(3, 2);
      set_ent_movemode(4, 2);
      set_ent_movemode(5, 2);
      set_ent_movemode(6, 2);
      wait_for_entity(2, 6);
      set_ent_movemode(2, 0);
      set_ent_movemode(3, 0);
      set_ent_movemode(4, 0);
      set_ent_movemode(5, 0);
      set_ent_movemode(6, 0);
      set_progress(P_FIGHTONBRIDGE, 2);
      set_btile(29, 17, 176);
      set_obs(29, 17, 1);
      set_zone(29, 17, 3);
    end

  elseif (zn == 2) then
    chest(8, I_OSEED, 2);
    autoexec();

  elseif (zn == 3) then
    if (get_progress(P_FIGHTONBRIDGE) == 2) then
      can_run = 0;
      combat(0);
      can_run = 1;
      set_progress(P_FIGHTONBRIDGE, 3);
      set_btile(29, 17, 160);
      set_obs(29, 17, 1);
      set_zone(29, 17, 0);
    end

  end
end


function entity_handler(en)
  local a = get_progress(P_FIGHTONBRIDGE);
  -- TT comments:
  -- a ==0 before talking to any worker (and before monster)
  -- a ==1 talked to workers but you have not tried to leave
  -- a ==2 tried to leave after a==1 and monster has appeared
  -- a ==3 you defeated the monster
  -- a ==4 after a==3 and you left map and returned

  if (en >= 2) then
    if (a == 0) then
      set_progress(P_FIGHTONBRIDGE, 1);
    elseif (a == 2) then
      bubble(en, "There are some weird creatures in the water!");
      return;
    elseif (a == 3) then
      bubble(en, "I think we'll take a break before getting back to work.");
      return;
    end
  end

  if (en == 0) then
    if (a == 0 or a == 1) then
      bubble(0, "We're on the lookout for bandits.");
    elseif (a == 2) then
      bubble(0, "Um... we don't really have any experience dealing with anything hostile... could you take a look for us?");
    elseif (a == 3) then
      bubble(0, "Very impressive... it's a good thing you came along. Thanks!");
    elseif (a == 4) then
      bubble(0, "We should be done soon... come back a little later.");
    end

  elseif (en == 1) then
    if (a < 2) then
      if (get_progress(P_LOSERONBRIDGE) == 0) then
        bubble(1, "Those bandits better not show their faces around here again!");
        bubble(HERO1, "Or you'll thrash 'em right?");
        bubble(1, "No... they'd better not show up because I forgot my sword!");
        wait(50);
        bubble(1, "I probably shouldn't have told you that.");
        set_progress(P_LOSERONBRIDGE, 1);
      else
        bubble(1, "Let me know if you see any bandits, will ya?");
      end
    elseif (a == 2) then
      bubble(1, ".....");
    elseif (a == 3) then
      bubble(1, "Wow!");
    elseif (a == 4) then
      bubble(1, "The bridge looks like it might be done by tomorrow. Why don't you go have a rest at the inn?");
    end

  elseif (en == 2) then
    if (a < 2) then
      bubble(2, "This is very hard work!");
    elseif (a == 4) then
      bubble(2, "That monster popped up right under my nose.  Thanks for everything!");
    end

  elseif (en == 3) then
    if (a < 2) then
      bubble(3, ".....");
      if (get_progress(P_ASLEEPONBRIDGE) == 0) then
        wait(50);
        bubble(3, "Wha...?");
        set_ent_facing(3, 2);
        bubble(3, "I wasn't sleeping!");
        set_ent_facing(3, 1);
        set_progress(P_ASLEEPONBRIDGE, 1);
      end
    elseif (a == 4) then
      bubble(3, "Zzz...");
    end

  elseif (en == 4) then
    if (a < 2) then
      bubble(4, "I think that guy up there is asleep.");
    elseif (a == 4) then
      bubble(4, "Lousy, no-good... sleeping on the job...");
    end

  elseif (en == 5) then
    if (a < 2) then
      bubble(5, "I really could use a break. It's been almost 15 minutes since the last one!");
    elseif (a == 4) then
      bubble(5, "Now that the bridge is almost done, I think I'm due for another break.");
    end

  elseif (en == 6) then
    if (a < 2) then
      bubble(6, "I never should have listened to my mother when she told me to take up construction.");
      bubble(6, "I should've been a florist like my dad.");
    elseif (a == 4) then
      bubble(6, "Now you see why I should've been a florist...");
    end

  end
end
