-- town2 - "Randen"

function autoexec()
  local a;
  set_progress(P_EARLYPROGRESS, 2);
  if not LOC_manor_or_party(AJATHAR) then
-- // Make one of the ents look like Ajathar if he's not been recruited yet.
     set_ent_id(10, AJATHAR)
     set_ent_active(10,1)
     set_ent_tilex(10,15)
     set_ent_tiley(10,15)
  else
     set_ent_active(10,0)
  end
  refresh();
end


function refresh()
  if (get_treasure(3) == 1) then
    set_mtile(110, 14, 265);
  end
  if (get_treasure(4) == 1) then
    set_mtile(111, 14, 265);
  end
  if (get_treasure(5) == 1) then
    set_mtile(112, 14, 265);
  end
  if (get_treasure(7) == 1) then
    set_obs(14, 47, 0);
  end
  if (get_treasure(10) == 1) then
    set_obs(35, 38, 0);
  end
  if (get_treasure(31) == 1) then
    set_mtile(115, 33, 265);
  end
  if (get_treasure(46) == 1) then
    set_mtile(116, 33, 265);
  end
  if (get_progress(P_WARPSTONE) == 1) then
    set_mtile(35, 15, 0);
  end
end


function postexec()
  return;
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("cave1", 47, 47, 47, 47);

  elseif (zn == 2) then
    door_in(85, 7, 80, 0, 90, 10);

  elseif (zn == 3) then
    door_in(86, 21, 80, 12, 92, 24);

  elseif (zn == 4) then
    door_in(102, 7, 93, 0, 105, 10);

  elseif (zn == 5) then
    door_in(97, 20, 94, 12, 107, 23);

  elseif (zn == 6) then
    door_in(96, 35, 80, 26, 102, 49);

  elseif (zn == 7) then
    door_in(112, 7, 107, 0, 117, 10);

  elseif (zn == 8) then
    door_in(114, 37, 104, 25, 118, 40);

  elseif (zn == 9) then
    bubble(HERO1, "Locked.");

  elseif (zn == 10) then
    door_out(20, 19);

  elseif (zn == 11) then
    door_out(28, 20);

  elseif (zn == 12) then
    door_out(24, 31);

  elseif (zn == 13) then
    door_out(63, 29);

  elseif (zn == 14) then
    door_out(18, 42);

  elseif (zn == 15) then
    door_out(49, 37);

  elseif (zn == 16) then
    door_out(66, 39);

  elseif (zn == 17) then
    -- PH added code to check if you do stay over night
    -- This is done indirectly; if your gp goes down it
    -- means you must have spent some here.
    local old_gp = get_gp();
    inn("Wayside Inn", 30, 1);
    -- This means you MUST stay at the inn before the
    -- bridge gets repaired. Correct?
    if (get_gp() < old_gp) then
      if (get_progress(P_FIGHTONBRIDGE) == 4) then
        set_progress(P_FIGHTONBRIDGE, 5);
        set_progress(P_SHOWBRIDGE, 1);
      end
      if (get_progress(P_GETPARTNER) < 2) then
        set_progress(P_GETPARTNER, 3);
        autoexec();

        -- PH add: Prevents the non-interacting potential partner
        set_ent_active(10, 0);
      end
    end

  elseif (zn == 18) then
    shop(4);

  elseif (zn == 19) then
    shop(5);

  elseif (zn == 20) then
    view_range(1, 119, 0, 122, 24);
    warp(121, 3, 8);

  elseif (zn == 21) then
    view_range(1, 109, 12, 117, 21);
    warp(116, 15, 8);

  elseif (zn == 22) then
    view_range(1, 94, 12, 107, 23);
    warp(106, 16, 8);

  elseif (zn == 23) then
    view_range(1, 119, 0, 122, 24);
    warp(121, 21, 8);

  elseif (zn == 24) then
    chest(3, I_KNIFE2, 1);
    refresh();

  elseif (zn == 25) then
    chest(4, 0, 250);
    refresh();

  elseif (zn == 26) then
    chest(5, I_SALVE, 1);
    refresh();

  elseif (zn == 27) then
    chest(7, I_PURITYGEM, 1);
    refresh();

  elseif (zn == 28) then
    change_map("main", 222, 19, 222, 19);

  elseif (zn == 29) then
    chest(10, I_MHERB, 1);
    refresh();

  elseif (zn == 30) then
    book_talk(party[0]);

  elseif (zn == 31) then
    chest(11, I_LTONIC, 1);
    refresh();

  elseif (zn == 32) then
    book_talk(party[0]);

  elseif (zn == 33) then
    bubble(HERO1, "Hmmm... books about herbs.");

  elseif (zn == 34) then
    bubble(HERO1, "How to make friends through hypnosis.");

  elseif (zn == 35) then
    bubble(HERO1, "Inns always have boring books.");

  elseif (zn == 36) then
    if (get_progress(P_WARPSTONE) == 1) then
      change_map("town6", 38, 56, 38, 56);
    end

  elseif (zn == 37) then
    touch_fire(party[0]);

  elseif (zn == 38) then
    view_range(1, 104, 25, 118, 40);
    warp(116, 29, 8);

  elseif (zn == 39) then
    view_range(1, 109, 12, 117, 21);
    warp(113, 19, 8);

  elseif (zn == 40) then
    chest(31, 0, 150);
    refresh();

  elseif (zn == 41) then
    chest(46, I_STRSEED, 2);
    refresh();

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(0, "Back again, are you?");
    else
      bubble(0, "You must be an adventurer. There has been an unusual number of you people around lately.");
    end

  elseif (en == 1) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(1, "Where is that man?");
    else
      bubble(1, "My husband is late again.");
    end

  elseif (en == 2) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(2, "Welcome to Randen.");
    else
      bubble(2, "This is the town of Randen. We're mostly a coastal trade town, but with the bridge out, trade is extremely slow.");
    end

  elseif (en == 3) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(3, "Good day.");
    else
    if get_progress(P_BLADE)==0  then
        set_progress(P_BLADE, 1)
        -- PH: Just my little joke hehe
        bubble(3, "I'm just preparing some vegetables.")
        bubble(HERO1, "That's a strange knife you've got there.")
        bubble(3, "What? Oh, this. Yes, it's a Phoenix Blade")
        bubble(3, "I found it lying about under a pile of leaves in the forest.")
        bubble(HERO1, "Isn't there supposed to be be someone out looking for that?")
        bubble(3, "I heard that, too, but it's been ages and no-one has turned up.")
        bubble(3, "I would give it back, but I need it for slicing carrots!")
        bubble(HERO1, "Your secret's safe with me.")
    else
      bubble(3, "Oh, goodness, I didn't make enough for company... sorry.");
    end
    end

  elseif (en == 4) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(4, "Hi!");
    else
      bubble(4, "I'm hungry, I hope mom is done making lunch soon.");
    end

  elseif (en == 5) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(5, "Business is good.");
      return
    end
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(5, "Now that the bridge is repaired I'm back in business.");
      if (get_treasure(9) == 0) then
        bubble(5, "I hear you had something to do with speeding along the bridge's construction. I'd like you to have this.");
        chest(9, I_B_SHOCK, 1);
      end
    else
      bubble(5, "If you haven't already noticed, the bridge across Brayden river is gone.");
      bubble(5, "An altercation between some travelers and brigands resulted in a volley of magic that left the bridge in cinders.");
      wait(25);
      bubble(5, "A new bridge is supposed to be built soon. This town can't survive for long without our major trade route.");
    end

  elseif (en == 6) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(6, "I'm sorry, the mayor isn't seeing any visitors at present.");
    else
      if (get_progress(P_FOUNDMAYOR) == 0) then
        if (get_progress(P_FIGHTONBRIDGE) > 4) then
          bubble(6, "The mayor has yet to return from Andra. A few of our men are out looking for him as we speak.");
        else
          bubble(6, "The mayor was going to Andra to talk to the council there. I think that he should have been back by now.");
          bubble(6, "We may have to go and look for him soon. We need to make sure that he is safe.");
        end
      elseif (get_progress(P_FOUNDMAYOR) < 3) then
        bubble(6, "The mayor is back now, thanks to you. However, the mayor is not seeing any visitors for a while.");
        bubble(6, "He is still recovering from his ordeal.");
      else
        bubble(6, "The mayor is back now. He's been through quite a dramatic ordeal and is not seeing any visitors for awhile.");
      end
    end

  elseif (en == 7) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(7, "Are you enjoying your stay?");
    else
      if (get_progress(P_FOUNDMAYOR) ~= 0) then
        bubble(7, "Thank you!");
      else
        bubble(7, "The mayor left for Andra with an adventurer that he had just hired and his usual contingent of guards.");
        bubble(7, "I wonder if they avoided the trouble at the bridge.");
      end
    end

  elseif (en == 8) then
 --   if (get_progress(P_GETPARTNER) == 1) then
 --     LOC_partner_check(8);
 --   elseif (get_progress(P_GETPARTNER) > 1) then
      bubble(8, "I wonder how long it takes to build a bridge?");
 --   end

  elseif (en == 9) then
  --  if (get_progress(P_GETPARTNER) == 1) then
  --    LOC_partner_check(9);
  --  elseif (get_progress(P_GETPARTNER) > 1) then
      bubble(9, "How long does it take to build a bridge?");
  --  end

 elseif (en == 10) then
       LOC_meet_ajathar()
 end
end


function LOC_partner_check(who)
  if (prompt(who, 2, 0, "Hi $0, want to team up?",
                        "  yes",
                        "  no") == 0) then
    bubble(who, "Great! Let's go.");
    if (who == 8) then
      add_chr(get_progress(P_PARTNER1) - 1);
    else
      add_chr(get_progress(P_PARTNER2) - 1);
    end
    set_progress(P_GETPARTNER, 2);
    give_item(I_MHERB, 3);
    give_item(I_NLEAF, 1);
    copy_ent(who, HERO2);
    set_ent_active(who, 0);
    orient_heroes();
    msg("$1 joined!", 255, 0);
  else
    bubble(who, "Oh... ok. Well maybe we'll run into each other again some time.");
  end
end

-- Checks if this ent is in the party, or in the manor,
-- or has never been recruited.
-- who: hero id
-- returns "manor" if in manor, "party" if in party, nil otherwise
function LOC_manor_or_party(who)
   local a
   if get_pidx(0)==who then
      return "party"
   elseif get_numchrs()>1 and get_pidx(1)==who then
      return party
   end
   for a=P_MANORPARTY,P_MANORPARTY7 do
      if get_progress(a)-1==who then
	 return "manor"
      end
   end
   return nil
end

function LOC_meet_ajathar()
   local ta, id;
   ta=get_progress(P_TALK_AJATHAR);
   if (get_progress(P_PORTALGONE)==0) then
      if (ta==0) then
	 bubble(HERO1, "Hello! You haven't ventured very far!")
	 bubble(10, "I have been maintaining a constant prayer vigil at this point. It should discourage the monsters from emerging into the town.")
	 bubble(HERO1, "And what if they DO come out?")
	 bubble(10, "My training also included swordsmanship")
	 set_progress(P_TALK_AJATHAR, 1)
      elseif (ta==1) then
	 bubble(10, "I hope I am doing the right thing here")
      end
   else
      if (ta==0) then 
	 bubble(10, "Be careful! That tunnel is infested with monsters")
	 bubble(HERO1, "Fortunately, I been successful in closing the Portal. No more monsters will trouble us now")
	 set_progress(P_TALK_AJATHAR, 1)
      else
	 bubble(HERO1, "You can rest easy now. I have closed the Portal that let the monsters through.")
      end
      bubble(10, "Great! Can I offer my services?")
-- Give ajathar his default equipment
      set_all_equip(AJATHAR, I_MACE2, I_SHIELD1, I_HELM1, I_ROBE2, I_BAND1, 0);
      id=select_team{AJATHAR}
--// add the characters that weren't selected to the manor
      add_to_manor(id)
      if (id[1]) then
	 set_ent_id(10, id[1])
	 if (id[2]) then
	    -- two heroes were de-selected
	    set_ent_id(9,id[2])
	    set_ent_active(9,1)
	    set_ent_tilex(9,get_ent_tilex(10))
	    set_ent_tiley(9, get_ent_tiley(10)+1)
	    bubble(10, "If you need us, we'll be back at the manor")
	    set_ent_script(10, "L1U1L1U2L2U1K")
	    set_ent_script(9,  "L1U2L1U2L2U1K")
	    wait_for_entity(9,10)
	    set_ent_active(9,0)
	 else
	    -- one hero was de-selected
	    bubble(10, "If you need me, I'll be back at the manor")
	    set_ent_script(10, "L1U2L2U1L1U1")
	    wait_for_entity(10,10)
	 end
      end
      set_ent_active(10, 0)
   end
end

