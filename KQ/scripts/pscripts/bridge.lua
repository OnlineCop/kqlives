

-- bridge - "On Brayden river Randen and Andra, incomplete"

--
function autoexec()
  refresh()
end
function postexec()
  return
end
function refresh()
  if (get_treasure(8) == 1) then
    set_obsm("treas_1", 0)
    set_zonem("treas_1", 0)
  end
end
function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(10) == 0 or get_progress(10) > 2) then
      if (get_progress(10) == 3) then
        set_progress(10, 4)
      end
      change_map("main", 240, 29, 240, 29)
      return
    end
    if (get_progress(10) == 1) then
      bubble(255, "Ahhhhh!!!")
      bubble(255, "Help!")
      set_ent_movemode(2, 2)
      set_ent_movemode(3, 2)
      set_ent_movemode(4, 2)
      set_ent_movemode(5, 2)
      set_ent_movemode(6, 2)
      wait_for_entity(2, 6)
      set_ent_movemode(2, 0)
      set_ent_movemode(3, 0)
      set_ent_movemode(4, 0)
      set_ent_movemode(5, 0)
      set_ent_movemode(6, 0)
      set_progress(10, 2)
      set_btile(29, 17, 176)
      set_obs(29, 17, 1)
      set_zone(29, 17, 3)
    end
  elseif (zn == 2) then
    chest(8, 105, 2)
    refresh()
  elseif (zn == 3) then
    if (get_progress(10) == 2) then
      can_run = 0
      combat(0)
      can_run = 1
      set_progress(10, 3)
      set_btile(29, 17, 160)
      set_obs(29, 17, 1)
      set_zone(29, 17, 0)
    end
  end
end
function entity_handler(en)
  local a = get_progress(10)
  -- TT comments:
  -- a ==0 before talking to any worker (and before monster)
  -- a ==1 talked to workers but you have not tried to leave
  -- a ==2 tried to leave after a==1 and monster has appeared
  -- a ==3 you defeated the monster
  -- a ==4 after a==3 and you left map and returned
  -- a !=5 in this map; you will use bridge2.(lua|map)
  if (en >= 2) then
    if (a == 0) then
      set_progress(10, 1)
    elseif (a == 2) then
      bubble(en, "There are some weird creatures in the water!")
      return
    elseif (a == 3) then
      bubble(en, "I think we'll take a break before getting back to work.")
      return
    end
  end
  if (en == 0) then
    if (a == 0 or a == 1) then
      bubble(en, "We're on the lookout for bandits.")
    elseif (a == 2) then
      bubble(en, "Um... we don't really have any experience dealing with anything hostile... could you take a look for us?")
    elseif (a == 3) then
      bubble(en, "Very impressive... it's a good thing you came along. Thanks!")
    elseif (a == 4) then
      bubble(en, "We should be done soon... come back a little later.")
    end
  elseif (en == 1) then
    if (a < 2) then
      if (get_progress(13) == 0) then
        bubble(en, "Those bandits better not show their faces around here again!")
        bubble(200, "Or you'll thrash 'em right?")
        bubble(en, "No... they'd better not show up because I forgot my sword!")
        wait(50)
        bubble(en, "I probably shouldn't have told you that.")
        set_progress(13, 1)
      else
        bubble(en, "Let me know if you see any bandits, will ya?")
      end
    elseif (a == 2) then
      bubble(en, ".....")
    elseif (a == 3) then
      bubble(en, "Wow!")
    elseif (a == 4) then
      bubble(en, "The bridge looks like it might be done by tomorrow. Why don't you go have a rest at the inn?")
    end
  elseif (en == 2) then
    if (a < 2) then
      bubble(en, "This is very hard work!")
    elseif (a == 4) then
      bubble(en, "That monster popped up right under my nose. Thanks for everything!")
    end
  elseif (en == 3) then
    if (a < 2) then
      bubble(en, ".....")
      if (get_progress(14) == 0) then
        wait(50)
        bubble(en, "Wha...?")
        set_ent_facing(3, 2)
        bubble(en, "I wasn't sleeping!")
        set_ent_facing(3, 1)
        set_progress(14, 1)
      end
    elseif (a == 4) then
      bubble(en, "Zzz...")
    end
  elseif (en == 4) then
    if (a < 2) then
      bubble(en, "I think that guy up there is asleep.")
    elseif (a == 4) then
      bubble(en, "Lousy, no-good... sleeping on the job...")
    end
  elseif (en == 5) then
    if (a < 2) then
      bubble(en, "I really could use a break. It's been almost 15 minutes since the last one!")
    elseif (a == 4) then
      bubble(en, "Now that the bridge is almost done, I think I'm due for another break.")
    end
  elseif (en == 6) then
    if (a < 2) then
      bubble(en, "I never should have listened to my mother when she told me to take up construction.")
      bubble(en, "I should've been a florist like my dad.")
    elseif (a == 4) then
      bubble(en, "Now you see why I should've been a florist...")
    end
  end
end
