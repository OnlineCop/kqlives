-- camp - orc forest encampment south of Randen

function autoexec()
  local a;
  
  set_ent_active(37,0);
  if (get_progress(P_FOUNDMAYOR) == 0 and get_progress(P_PARTNER3) == 0) then
    while (get_progress(P_PARTNER3) == 0) do
      a = krnd(8);
      if (a ~= get_pidx(0) and a ~= get_progress(P_PARTNER1)-1 and a ~= get_progress(P_PARTNER2)-1) then
        set_progress(P_PARTNER3,a+1);
      end
    end
  end
  if (get_progress(P_FOUNDMAYOR) == 0 and get_progress(P_PARTNER3) > 0) then
    set_ent_active(37,1);
    set_ent_id(37,get_progress(P_PARTNER3)-1);
  end
  refresh();
end
  
function refresh()
  if (get_treasure(55) == 1) then
    set_ftile(66,45,265);
  end
  if (get_treasure(56) == 1) then
    set_ftile(73,43,265);
  end
  if (get_treasure(57) == 1) then
    set_ftile(81,24,265);
  end
  if (get_treasure(58) == 1) then
    set_ftile(83,11,265);
  end
  if (get_treasure(59) == 1) then
    set_ftile(84,11,265);
  end
  if (get_treasure(60) == 1) then
    set_ftile(13,57,265);
  end
  if (get_treasure(61) == 1) then
    set_ftile(14,57,265);
  end
end

function postexec()
  return
end
  
function zone_handler(zn)
  if (zn == 1) then
    change_map("main",235,36,235,36);
  
  elseif (zn == 2) then
    change_map("main",235,38,235,38);
  
  elseif (zn == 3) then
    set_ent_script(22,"L1U2");
    set_ent_script(23,"L1U3");
    wait_for_entity(22,23);
    bubble(22,"You're not welcome here!","","","");
    drawmap();
    screen_dump();
    set_run(0);
    combat(2);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(22,0);
    set_ent_active(23,0);
    set_zone(23,24,0);
    set_zone(24,24,0);
  
  elseif (zn == 4) then
    set_ent_script(14,"R1U6L2F1");
    set_ent_script(15,"L2U3");
    set_ent_script(16,"U1L2U1");
    set_ent_script(17,"U2L2U2");
    wait_for_entity(14,17);
    set_run(0);
    combat(4);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(14,0);
    set_ent_active(15,0);
    set_ent_active(16,0);
    set_ent_active(17,0);
    set_zone(20,43,0);
    set_zone(21,43,0);
  
  elseif (zn == 5) then
    set_ent_script(10,"R1D4L3D3");
    set_ent_script(11,"L8U2");
    set_ent_script(12,"U1L8U2");
    wait_for_entity(10,12);
    set_run(0);
    combat(3);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(10,0);
    set_ent_active(11,0);
    set_ent_active(12,0);
    set_zone(32,48,0);
    set_zone(34,52,0);
    set_zone(34,53,0);
  
  elseif (zn == 6) then
    set_ent_script(6,"D4R1D2");
    set_ent_script(7,"D6R2F0");
    set_ent_script(8,"D1R1D1");
    set_ent_script(9,"R1D3");
    wait_for_entity(6,9);
    set_run(0);
    combat(4);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(6,0);
    set_ent_active(7,0);
    set_ent_active(8,0);
    set_ent_active(9,0);
    set_zone(36,28,0);
    set_zone(37,28,0);
  
  elseif (zn == 7) then
    set_ent_script(10,"R1D4L3D7");
    set_ent_script(11,"L2D1L2");
    set_ent_script(12,"U1L5");
    wait_for_entity(10,12);
    set_run(0);
    combat(3);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(10,0);
    set_ent_active(11,0);
    set_ent_active(12,0);
    set_zone(32,48,0);
    set_zone(34,52,0);
    set_zone(34,53,0);
  
  elseif (zn == 8) then
    chest(55,I_NLEAF,2);
    refresh();
  
  elseif (zn == 9) then
    chest(56,0,250);
    refresh();
  
  elseif (zn == 10) then
    chest(57,I_RRUNE,1);
    refresh();
  
  elseif (zn == 11) then
    chest(58,I_SALVE,2);
    refresh();
  
  elseif (zn == 12) then
    chest(59,I_EDROPS,1);
    refresh();
  
  elseif (zn == 13) then
    chest(60,I_SPEAR1,1);
    refresh();
  
  elseif (zn == 14) then
    chest(61,I_ARMOR2,1);
    refresh();

  end
end

function entity_handler(en)
  if (en == 5) then
    bubble(5,"You have to get through us to",
             "get to them.","","");
    drawmap();
    set_run(0);
    combat(5);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(5,0);
    set_ent_active(34,0);
    set_ent_active(35,0);
    set_ent_active(36,0);
  
  elseif (en == 19) then
    if (get_numchrs() > 1) then
      bubble(19,"Intruders!","","","");
    else
      bubble(19,"Intruder!","","","");
    end
    set_ent_script(20,"D1R6U1");
    set_ent_script(21,"D1R3U2");
    wait_for_entity(E_G1L,E_G1R);
    set_run(0);
    combat(3);
    set_run(1)
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(19,0);
    set_ent_active(20,0);
    set_ent_active(21,0);
  
  elseif (en == 25) then
    bubble(25,"Thanks for your help! We Should",
              "be able to find our way out.","","");
  
  elseif (en == 26) then
    bubble(26,"Thanks for your help! We Should",
              "be able to find our way out.","","");
  
  elseif (en == 37) then
    local a;
  
    a = get_progress(P_PARTNER3)-1;
    bubble(37,"Hey, $0! Am I ever",
              "glad to see you! We're in a bit",
              "of a jam here.","");
    bubble(37,"I took a small job to help guard",
              "the mayor on his trip to Andra.","","");
    bubble(37,"We were on the Brayden bridge",
              "when we were ambushed by these",
              "orcs. During the fight, one",
              "of them used a fire spell.");
    bubble(37,"The bridge was engulfed in",
              "flames and we were trapped.","","");
    bubble(HERO1,"But how did you end up here?","","","");
    bubble(37,"Well, we had no choice but to go",
              "back the way we came, and we ran",
              "into the whole tribe.","");
    bubble(37,"We were horribly out-numbered",
              "and forced to surrender. They",
              "brought us here and then most of",
              "them left for somewhere else.");
    bubble(HERO1,"Hmm... this almost sounds as though",
                 "the whole thing was planned.","","");
    bubble(37,"Hmm...","","","");
    bubble(HERO1,"Excuse me, Mr. Mayor.","","","");
    bubble(4,"Yes?","","","");
    bubble(HERO1,"Do you think that this could",
                 "have possibly been a planned",
                 "attack with the purpose of",
                 "trying to kidnap you?");
    bubble(4,"I can't see what these vermin",
             "would hope to gain by",
             "imprisoning me, but I suppose",
             "it's possible.");
    bubble(37,"That's got to be it, there was",
              "far too many of them, for it to",
              "be a random encounter with",
              "brigands.");
    bubble(37,"And here I was blaming myself",
              "for the whole thing.","","");
    bubble(HERO1,"It's not your fault. You did",
                 "your job, the mayor is unharmed.","","");
    bubble(4,"So, can we get out of",
             "here now or what?","","");
    set_ent_facing(37,1);
    bubble(37,"Yes sir!","","","");
    set_ent_facing(37,0);
    bubble(37,"I'll go back to Randen with",
              "the mayor. Why don't you meet",
              "me there after.","");
    bubble(HERO1,"Sure.","","","");
    set_ftile(41,14,0);
    set_autoparty(1);
    set_ent_script(HERO1,"D2R1F2");
    if (get_numchrs() == 2) then
      set_ent_script(HERO2,"D2R1F2");
      wait_for_entity(HERO1,HERO2);
    else
      wait_for_entity(HERO1,HERO1);
    end
    set_autoparty(0);
    set_ent_script(37,"D5");
    wait_for_entity(37,37);
    set_ent_script(3,"L1D4");
    wait_for_entity(3,3);
    set_ent_script(4,"D4");
    wait_for_entity(4,4);
    set_ent_script(2,"L1D3");
    wait_for_entity(2,2);
    set_ent_script(1,"R1D2");
    wait_for_entity(1,1);
    set_ent_script(0,"R2D1");
    wait_for_entity(0,0);
    set_ent_script(37,"L4D10");
    set_ent_script(3,"D1L4D10");
    set_ent_script(4,"D2L4D10");
    set_ent_script(2,"D3L4D10");
    set_ent_script(1,"D4L4D10");
    set_ent_script(0,"D5L4D10");
    for a=6,36,1 do
      set_ent_movemode(a,0);
    end
    wait_for_entity(0,37);
    set_ent_active(37,0);
    set_ent_active(3,0);
    set_ent_active(4,0);
    set_ent_active(2,0);
    set_ent_active(1,0);
    set_ent_active(0,0);
    set_progress(P_FOUNDMAYOR,1);

  elseif (en == 13 or en == 18 or en == 24 or (en >= 27 and en <= 33)) then
    if (get_numchrs() > 1) then
      bubble(en,"Intruders!","","","");
    else
      bubble(en,"Intruder!","","","");
    end
    drawmap();
    set_run(0);
    combat(1);
    set_run(1);
    if (get_alldead() == 1) then
      return
    end
    set_ent_active(en,0);

  end
end
