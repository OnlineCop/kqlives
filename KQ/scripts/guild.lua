-- guild - home of the Embers in Sunarin

function autoexec()
  refresh();
end

function refresh()
  if (get_progress(P_GUILDSECRET) == 1) then
    set_ftile(12,25,0);
    set_ftile(12,26,0);
    set_obs(12,26,0);
    set_zone(12,26,0);
    set_ftile(13,25,242);
    set_ftile(13,26,241);
  end
  if (get_progress(P_FOUGHTGUILD) == 1) then
    set_ent_active(0,0);
    set_ent_active(1,0);
  end
  if (get_progress(P_OPALHELMET) == 1) then
    set_ftile(58,24,265);
  end
end

function postexec()
  return
end

-- updated 20020929 PH
-- added test for P_OPALHELMET so
-- that you can re-enter the guild
-- if you do not have the helmet yet
function zone_handler(zn)  
  if (zn == 1) then
    if (get_progress(P_OPALHELMET)==1) then
      set_progress(P_FOUGHTGUILD,2);
    end
    change_map("town5",18,37,18,37);

  elseif (zn == 2) then
    warp(10,5,8);

  elseif (zn == 3) then
    if (get_progress(P_GUILDSECRET) == 0) then
      bubble(HERO1,"Well I'll be...","","","");
      bubble(HERO1,"The book really IS called 'How",
                   "to Enter the Ember's Secret",
                   "Hideout'. Heh... brilliant in",
                   "its simplicity, I suppose.");
      set_progress(P_GUILDSECRET,1);
      refresh();
    end

  elseif (zn == 4) then
    if (get_pidx(0) == AYLA) then
      bubble(HERO1,"I'm a thief... but even I",
                   "wouldn't read this garbage!","","");
    else
      bubble(HERO1,"Thieves read?!","","","");
    end

  elseif (zn == 5) then
    chest(47,0,500);

  elseif (zn == 6) then
    warp(37,7,8);

  elseif (zn == 7) then
    warp(23,19,8);

  elseif (zn == 8) then
    chest(50,I_SWORD4,1);

  elseif (zn == 9) then
    bubble(HERO1,"These look interesting.","","","");

  elseif (zn == 10) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 11) then
    local a,b;

    if (get_progress(P_FOUGHTGUILD) == 0) then
      a = get_vx();
      b = get_vy();
      move_camera(816,288,2);
      bubble(HERO1,"Ooh... a chest! But",
                   "who are these clowns?","","");
      wait(50);
      bubble(0,"I'll follow the others to the",
               "Coliseum and you wait here for",
               "the guildmaster to return from",
               "his visit.");
      bubble(1,"Yes sir! I'll...","","","");
      bubble(0,"Who is that?","","","");
      set_ent_facing(1,3);
      drawmap();
      screen_dump();
      move_camera(a,b,1);
      drawmap();
      screen_dump();
      bubble(0,"Guards!","","","");
      bubble(HERO1,"Oh oh!","","","");
      wait(50);
      set_ftile(65,19,369);
      set_ftile(64,20,372);
      set_ftile(66,20,371);
      set_ftile(65,21,370);
      wait(50);
      bubble(HERO1,"What the...","","","");
      drawmap();
      screen_dump();
      set_run(0);
      combat(26);
      set_run(1);
      if (get_alldead() == 1) then
        return
      end
      set_progress(P_FOUGHTGUILD,1);
      set_ftile(65,19,0);
      set_ftile(64,20,0);
      set_ftile(66,20,0);
      set_ftile(65,21,0);
      refresh();
      drawmap();
      screen_dump();
      wait(50);
      bubble(HERO1,"Those guys are gone!","","","");
      bubble(HERO1,"But they left the chest behind.","","","");
    end

  elseif (zn == 12) then
    if (get_progress(P_OPALHELMET) == 0) then
      sfx(5);
      msg("Opal Helmet procured",255,0);
      set_progress(P_OPALHELMET,1);
      refresh();
      drawmap();
      screen_dump();
      wait(50);
      bubble(HERO1,"I don't like this.",
                   "It was just too easy.","","");
    end

  elseif (zn == 13) then
    warp(10,27,8);

  end
end

function entity_handler(en)
  return
end
