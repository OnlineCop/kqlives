-- town2 - Randen

function autoexec()
  local a;

  if (get_progress(P_GETPARTNER) == 0) then
    while (get_progress(P_PARTNER1) == 0) do
      a = krnd(8);
      if (a ~= get_pidx(0)) then
        set_progress(P_PARTNER1,a+1);
      end
    end
    while (get_progress(P_PARTNER2) == 0) do
      a = krnd(8);
      if (a ~= get_pidx(0) and a ~= get_progress(P_PARTNER1)-1) then
        set_progress(P_PARTNER2,a+1);
      end
    end
    for a=0,7,1 do
      if (a ~= get_pidx(0)) then
        give_xp(a,300+krnd(100),1);
      end
      if (a == SENSAR or a == SARINA or a == TEMMIN or a == AYLA) then
        set_all_equip(a,I_SWORD2,I_SHIELD1,I_HELM1,I_ARMOR2,I_BAND1,0);
      elseif (a == AJATHAR) then
        set_all_equip(a,I_MACE2,I_SHIELD1,I_HELM1,I_ROBE2,I_BAND1,0);
      elseif (a == CORIN or a == CASANDRA or a == NOSLOM) then
        set_all_equip(a,I_ROD1,I_SHIELD1,I_CAP1,I_ROBE2,I_BAND1,0);
      end
    end
    set_progress(P_GETPARTNER,1);
  end
  if (get_progress(P_GETPARTNER) == 1) then
    if (get_progress(P_PARTNER1) > 0) then
      set_ent_id(8,get_progress(P_PARTNER1)-1);
      set_ent_active(8,1);
    end
    if (get_progress(P_PARTNER2) > 0) then
      set_ent_id(9,get_progress(P_PARTNER2)-1);
      set_ent_active(9,1);
    end
  elseif (get_progress(P_GETPARTNER) > 1) then
    set_ent_active(8,0);
    set_ent_active(9,0);
  end
  if (get_progress(P_FOUNDMAYOR) == 1) then
    set_ent_id(10,get_progress(P_PARTNER3)-1);
    set_ent_active(10,1);
  else
    set_ent_active(10,0);
  end
  refresh();
end
  
function refresh()
  if (get_treasure(7) == 1) then
    set_obs(14,47,0);
  end
  if (get_treasure(10) == 1) then
    set_obs(35,38,0);
  end
  if (get_treasure(3) == 1) then
    set_ftile(110,14,265);
  end
  if (get_treasure(4) == 1) then
    set_ftile(111,14,265);
  end
  if (get_treasure(5) == 1) then
    set_ftile(112,14,265);
  end
  if (get_progress(P_WARPSTONE) == 1) then
    set_mtile(35,15,0);
  end
end
  
function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("cave1",47,47,47,47);
  
  elseif (zn == 2) then
    door_in(85,7,80,0,90,10);
  
  elseif (zn == 3) then
    door_in(86,21,80,12,92,24);
  
  elseif (zn == 4) then
    door_in(102,7,93,0,105,10);
  
  elseif (zn == 5) then
    door_in(97,20,94,12,107,23);
  
  elseif (zn == 6) then
    door_in(96,35,80,26,102,49);
  
  elseif (zn == 7) then
    door_in(112,7,107,0,117,10);
  
  elseif (zn == 8) then
    door_in(114,37,104,25,117,40);
  
  elseif (zn == 9) then
    bubble(HERO1,"Locked.","","","");
  
  elseif (zn == 10) then
    door_out(20,19);
  
  elseif (zn == 11) then
    door_out(28,20);
  
  elseif (zn == 12) then
    door_out(24,31);
  
  elseif (zn == 13) then
    door_out(44,47);
  
  elseif (zn == 14) then
    door_out(18,42);
  
  elseif (zn == 15) then
    door_out(49,37);
  
  elseif (zn == 16) then
    door_out(66,39);
  
  elseif (zn == 17) then
    inn("Wayside Inn",30,1);
    if (get_progress(P_FIGHTONBRIDGE) == 4) then
      set_progress(P_FIGHTONBRIDGE,5);
      set_progress(P_SHOWBRIDGE,1);
    end
    if (get_progress(P_GETPARTNER) < 2) then
      set_progress(P_GETPARTNER,3);
      autoexec();
    end
  
  elseif (zn == 18) then
    shop(4);
  
  elseif (zn == 19) then
    shop(5);
  
  elseif (zn == 20) then
    view_range(1,119,0,122,24);
    warp(121,21,8);
  
  elseif (zn == 21) then
    view_range(1,94,12,107,23);
    warp(106,16,8);
  
  elseif (zn == 22) then
    view_range(1,109,12,117,19);
    warp(116,15,8);
  
  elseif (zn == 23) then
    view_range(1,119,0,122,24);
    warp(121,3,8);
  
  elseif (zn == 24) then
    chest(3,I_KNIFE2,1);
    refresh();
  
  elseif (zn == 25) then
    chest(4,0,250);
    refresh();
  
  elseif (zn == 26) then
    chest(5,I_SALVE,1);
    refresh();
  
  elseif (zn == 27) then
    chest(7,I_PURITYGEM,1);
    refresh();
  
  elseif (zn == 28) then
    change_map("main",222,19,222,19);
  
  elseif (zn == 29) then
    chest(10,I_MHERB,1);
    refresh();
  
  elseif (zn == 30) then
    book_talk(get_pidx(0));
  
  elseif (zn == 31) then
    chest(11,I_LTONIC,1);
  
  elseif (zn == 32) then
    book_talk(get_pidx(0));
  
  elseif (zn == 33) then
    bubble(HERO1,"Hmmm... books about herbs.","","","");
  
  elseif (zn == 34) then
    bubble(HERO1,"How to make friends",
                 "through hypnosis.","","");
  
  elseif (zn == 35) then
    bubble(HERO1,"Inns always have boring books.","","","");
  
  elseif (zn == 36) then
    if (get_progress(P_WARPSTONE) == 1) then
      change_map("town6",38,56,38,56);
    end
  
  end
end

function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(0,"Back again, are you?","","","");
    else
      bubble(0,"You must be an adventurer. There",
               "has been an unusual number of",
               "you people around lately.","");
    end
  
  elseif (en == 1) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(1,"Where is that man?","","","");
    else
      bubble(1,"My husband is late again.","","","");
    end
  
  elseif (en == 2) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(2,"Welcome to Randen.","","","");
    else
      bubble(2,"This is the town of Randen.",
               "We're mostly a coastal trade",
               "town, but with the bridge out,",
               "trade is extremely slow.");
    end
  
  elseif (en == 3) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(3,"Good day.","","","");
    else
      bubble(3,"Oh, goodness, I didn't make",
               "enough for company... sorry.","","");
    end
  
  elseif (en == 4) then
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(4,"Hi!","","","");
    else
      bubble(4,"I'm hungry, I hope mom is",
               "done making lunch soon.","","");
    end
  
  elseif (en == 5) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(5,"Business is good.","","","");
      return
    end
    if (get_progress(P_FIGHTONBRIDGE) > 4) then
      bubble(5,"Now that the bridge is repaired",
               "I'm back in business.","","");
      if (get_treasure(9) == 0) then
        bubble(5,"I hear you had something to do",
                 "with speeding along the bridge's",
                 "construction. I'd like you to",
                 "have this.");
        chest(9,I_B_SHOCK,1);
      end
    else
      bubble(5,"If you haven't already noticed,",
               "the bridge across Brayden river",
               "is gone.","");
      bubble(5,"An altercation between some",
               "travellers and brigands resulted",
               "in a volley of magic that left",
               "the bridge in cinders.");
      wait(25);
      bubble(5,"A new bridge is supposed to be",
               "built soon. This town can't",
               "survive for long without our",
               "major trade route.");
    end
  
  elseif (en == 6) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(6,"I'm sorry, the mayor isn't",
               "seeing any visitors at present.","","");
    else
      if (get_progress(P_FOUNDMAYOR) == 0) then
        if (get_progress(P_FIGHTONBRIDGE) > 4) then
          bubble(6,"The mayor has yet to return from",
                   "Andra. A few of our men are out",
                   "looking for him as we speak.","");
        else
          bubble(6,"The mayor was going to Andra to",
                   "talk to the council there. I",
                   "think that he should have been",
                   "back by now.");
          bubble(6,"We may have to go and look for",
                   "him soon. We need to make sure",
                   "that he is safe.","");
        end
      elseif (get_progress(P_FOUNDMAYOR) < 3) then
        bubble(6,"The mayor is back now, thanks to",
                 "you. However, the mayor is not",
                 "seeing any visitors for a while.","");
        bubble(6,"He is still recovering",
                 "from his ordeal.","","");
      else
        bubble(6,"The mayor is back now. He's",
                 "been through quite a dramatic",
                 "ordeal and is not seeing any",
                 "visitors for awhile.");
      end
    end
  
  elseif (en == 7) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(7,"Are you enjoying your stay?","","","");
    else
      if (get_progress(P_FOUNDMAYOR) ~= 0) then
        bubble(7,"Thank you!","","","");
      else
        bubble(7,"The mayor left for Andra with an",
                 "adventurer that he had just",
                 "hired and his usual contingent",
                 "of guards.");
        bubble(7,"I wonder if they avoided the",
                 "trouble at the bridge.","","");
      end
    end
  
  elseif (en == 8) then
    if (get_progress(P_GETPARTNER) == 1) then
      LOC_partner_check(8);
    elseif (get_progress(P_GETPARTNER) > 1) then
      bubble(8,"How long does it take to",
               "build a bridge?","","");
    end
  
  elseif (en == 9) then
    if (get_progress(P_GETPARTNER) == 1) then
      LOC_partner_check(9);
    elseif (get_progress(P_GETPARTNER) > 1) then
      bubble(9,"How long does it take to",
               "build a bridge?","","");
    end
  
  elseif (en == 10) then
    if (get_progress(P_GETPARTNER) ~= 3) then
      if (get_numchrs() == 1) then
        bubble(10,"Wow!. You were great back there.",
                  "I really appreciate what you did",
                  "for me. In return, I shall",
                  "accompany you.");
        bubble(eno,"Oh... and here... take this.","","","");
        set_gp(get_gp()+1000);
        sfx(6);
        msg("You received 1000 gp!",255,0);
        bubble(10,"That's my pay from the mayor.",
                  "We should use it to get some",
                  "new equipment and such.","");
        add_chr(progress[P_PARTNER3]-1);
        progress[P_GETPARTNER] = 3;
        copy_ent(10,HERO2);
        set_ent_active(10,0);
        orient_heroes();
        give_xp(get_pidx(numchrs()-1),470+krnd(50),1);
        msg("$1 joined!",255,0,xofs,yofs);
        set_progress(P_FOUNDMAYOR,2);
      else
        bubble(10,"Wow!. You were great back there.",
                  "I really appreciate what you did",
                  "for me.","");
        bubble(10,"I would like to come along with",
                  "you, but you already have a partner.","","");
        bubble(10,"At any rate, you've earned this.","","","");
        set_gp(get_gp()+500);
        sfx(6);
        msg("You received 500 gp!",255,0,xofs,yofs);
        bubble(10,"That's half of what the mayor",
                  "paid me. I'm actually surprised",
                  "that he still thought I deserved",
                  "it, but I'm not one to complain.");
        bubble(10,"See ya!","","","");
        if (get_ent_facing(10) == 0) then
          set_ent_script(10,"L1D10");
        else
          set_ent_script(10,"D10");
        end
        wait_for_entity(10,10);
        set_ent_active(10,0);
        set_progress(P_FOUNDMAYOR,2);
        set_progress(P_GETPARTNER,3);
      end
    end

  end
end
  
function LOC_partner_check(who)
  if (prompt(who,2,0,"Hi $0, want to team up?","  yes","  no","") == 0) then
    bubble(who,"Great! Let's go.","","","");
    if (who == 8) then
      add_chr(get_progress(P_PARTNER1)-1);
    else
      add_chr(get_progress(P_PARTNER2)-1);
    end
    set_progress(P_GETPARTNER,2);
    give_item(I_MHERB,3);
    give_item(I_NLEAF,1);
    copy_ent(who,HERO2);
    set_ent_active(who,0);
    orient_heroes();
    msg("$1 joined!",255,0);
  else
    bubble(who,"Oh... ok. Well maybe we'll",
               "run into each other again",
               "some time.","");
  end
end
