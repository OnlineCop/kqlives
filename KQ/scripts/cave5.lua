-- cave5 - "The new Opal cave in the mountain pass"

-- Todo: top door back in from pass, some chests on wrong
-- level inside cave cabin, obstacles for pass chests

function autoexec()
  refresh();
end


-- Show the status of a chest
function showch(x, y, tr)
  local ch
  if (get_treasure(tr) == 1) then
    ch = 41;
  else
    ch = 40;
  end
  set_mtile(x, y, ch);
end


function refresh()
  -- Modify the map to reflect bombed-out areas
  if (get_progress(P_OPALDRAGONOUT) ~= 0) then
    copy_tile_all(124, 22, 111, 39, 3, 3);
  end
  if (get_progress(P_BOMB1) ~= 0) then
    copy_tile_all(110, 82, 72, 94, 5, 5);
  end
  if (get_progress(P_BOMB2) ~= 0) then
    copy_tile_all(110, 68, 29, 84, 15, 12)
  end
  if (get_progress(P_BOMB3) ~= 0) then
    copy_tile_all(47, 118, 17, 113, 6, 5);
  end
  if (get_progress(P_BOMB4) == 3) then
    copy_tile_all(117, 60, 43, 53, 6, 7)
  end
  if (get_progress(P_BOMB5) ~= 0) then
    copy_tile_all(121, 20, 34, 25, 3, 5);
  end

  -- Modify for the chests
  showch(93, 19, 90);
  showch(10, 11, 91);
  showch(91, 61, 92);
  showch(60, 78, 93);
  showch(63, 78, 94);
  showch(95, 102, 95);
end


function postexec()
  return;
end


-- Return non-nil if the player has some dynamite to use
function has_dynamite()
  local d = use_up(190);
  if (d == 0) then
    bubble(HERO1, "I need some dynamite here.");
    return nil;
  end
  return d;
end


--
-- These are the destroy functions, one for each area.
--


function destroy1()
  -- First zone
  if (has_dynamite()) then
    hero_escape("L3D2L10D2");
    sfx(42);
    oneliner(HERO1, {"Great dynamite!",
                     "Ohh gosh!",
                     "This is risky!",
                     "If only my master could see me!",
                     "This could be fun!",
                     "Good. On to the next.",
                     "So much for stealth!",
                     "Crude, but effective..."});
    set_progress(P_BOMB1, 1);
    refresh();
  end
end


function destroy2()
  -- Second zone (cliff)
  if (has_dynamite()) then
    hero_escape("R3D3R3D5R8");
    sfx(42);
    if (get_numchrs()>1) then
      bubble(HERO2, "I think you might have overdone it!");
      oneliner(HERO1, {"Good, wasn't it?",
                       "It scared me too.",
                       "I'm afraid I have.",
                       "Do you think so?",
                       "Shut up!",
                       "No way!",
                       "I'd prefer a different way.",
                       "Let's see."});
    else
      bubble(HERO1, "Argh. I think I used too much!");
    end
    set_progress(P_BOMB2, 1);
    refresh();
  end
end


function destroy3()
  -- On left
  if (has_dynamite()) then
    hero_escape("R1U2R5U10");
    sfx(42);
    oneliner(HERO1, {"I'm enjoying this!",
                     "I wish I were home.",
                     "This is very wasteful.",
                     "My training didn't cover this.",
                     "If only life was so simple...",
                     "Satisfactory.",
                     "Too noisy!",
                     "Hmm."});
    set_progress(P_BOMB3, 1);
    refresh();
  end
end


function destroy4a()
  -- Double pillar (left)
  local p = get_progress(P_BOMB4);
  if ((p == 0 or p == 2) and has_dynamite()) then
    hero_escape("L7D2L4D4L2");
    sfx(42);
    oneliner(HERO1, {"I must have utterly destroyed it.",
                     "I wish I'd never seen this dreadful cave.",
                     "I don't like this one bit.",
                     "Perhaps I will become the first Master of Dynamite!",
                     "I don't even care if this is necessary or not.",
                     "Another goal attained.",
                     "This is almost painful.",
                     "As one sows, so shall he reap..."});
    set_progress(P_BOMB4, p + 1);
  elseif (p == 1) then
    -- Already destroyed this side
    bubble(HERO1, "I weakened it, but it might need another hit to destroy it");
  end
  refresh();
end


function destroy4b()
  -- Double pillar (right)
  local p = get_progress(P_BOMB4);
  if ((p == 0 or p == 1) and has_dynamite()) then
    hero_escape("R5D4R7D1R5");
    sfx(42);
    oneliner(HERO1, {"Another great hit!",
                     "Ow. This is so dusty.",
                     "This is totally unsafe.",
                     "I wonder if I will be able to make use of these skills?",
                     "I should have just blown the whole mountain up.",
                     "No collateral damage here.",
                     "This can't be good for my health.",
                     "I feel the need to reflect upon my actions."});
    set_progress(P_BOMB4, p + 2);
  elseif (p == 2) then
    -- Already destroyed this side
    bubble(HERO1, "I weakened it, but it might need another hit to destroy it.");
  end
  refresh();
end


function destroy5()
  if (has_dynamite()) then
    hero_escape("D1R2L2W19R20");
    sfx(42);
    oneliner(HERO1, {"I'm getting the hang of this!",
                     "I hope there's no more!",
                     "This will weaken the ceiling!",
                     "I'm not cut out for this.",
                     "That was pathetic!",
                     "Mission accomplished!",
                     "My ears are ringing!",
                     "So much destruction..."});
    set_progress(P_BOMB5, 1);
    refresh();
  end
end


function oneliner(id, tbl)
  -- Display a one-line Bruce Willis-style quip; a different one for each hero
  local pid = get_pidx(id - HERO1);
  local str = tbl[pid + 1];
  bubble(id, str or "What?!?");
end


-- Handles the heroes running away from the explosion
-- HERO2 (if present) is always one square away from HERO1
-- so the first action is for HERO2 to move onto that square.
-- Then HERO1 takes one more step at the end.
function hero_escape(script)
  if (get_numchrs() > 1) then
    local dx = get_ent_tilex(HERO2) - get_ent_tilex(HERO1);
    local dy = get_ent_tiley(HERO2) - get_ent_tiley(HERO1);
    if (dx < 0) then
      onestep = "R1"
    elseif (dx > 0) then
      onestep = "L1"
    elseif (dy < 0) then
      onestep = "D1"
    elseif (dy > 0) then
      onestep = "U1"
    else
      onestep = ""
    end
    set_autoparty(1);
    set_ent_script(HERO1, script..onestep);
    set_ent_script(HERO2, onestep..script);
    wait_for_entity(HERO1, HERO2);
  else
    set_ent_script(HERO1, script);
    wait_for_entity(HERO1, HERO1);
  end
  set_autoparty(0);
  orient_heroes();
end


function opaldragon()
  local spd;
  if (get_progress(P_OPALDRAGONOUT) == 0) then
    if (get_numchrs() > 1) then
      set_autoparty(1);
      set_ent_script(HERO2, "L1");
      wait_for_entity(HERO2, HERO12);
      -- Face 'up'
      set_ent_facing(HERO2, 1);
      set_autoparty(0);
    end
    bubble(HERO1, "Ohhh!");
    bubble(HERO1, "The legend was true!");
    spd = get_ent_speed(HERO1);
    set_autoparty(1);
    if (get_numchrs() > 1) then
      set_ent_speed(HERO1, 1);
      set_ent_speed(HERO2, 1);
      set_ent_script(HERO1, "U7");
      set_ent_script(HERO2, "U7");
      wait_for_entity(HERO1, HERO2);
      set_ent_speed(HERO1, spd);
      set_ent_speed(HERO2, spd);
    else
      set_ent_speed(HERO1, 1);
      set_ent_script(HERO1, "U7");
      wait_for_entity(HERO1, HERO1);
    end
    set_autoparty(0);

    set_run(0);
    combat(58);
    set_run(1);
    set_progress(P_OPALDRAGONOUT, 1);

    orient_heroes();
    refresh();
  end
end


function zone_handler(zn)
  if (zn == 0) then
    combat(57);
  elseif (zn == 1) then
    destroy1();
  elseif (zn == 2) then
    destroy2();
  elseif (zn == 3) then
    destroy3();
  elseif (zn == 4) then
    destroy4a();
  elseif (zn == 5) then
    destroy4b();
  elseif (zn == 6) then
    destroy5();
  elseif (zn == 7) then
    -- Save point south
    set_save(1);
    warp(119, 139, 10);
  elseif (zn == 8) then
    -- Save point north
    set_save(1);
    warp(119, 132, 10);
  elseif (zn == 9) then
    change_map("pass", 78, 39, 0, 0);
  elseif (zn == 10) then
    change_map("pass", 106, 36, 0, 0);
  elseif (zn == 11) then
    -- Dragon
    sfx(26);
    warp(112, 49, 10);
    opaldragon();
  elseif (zn == 12) then
    change_map("pass", 83, 28, 0, 0);
  elseif (zn == 13) then
    -- Behind pillar
    set_save(0);
    warp(35, 24, 10);
  elseif (zn == 14) then
    -- Long room south
    set_save(0);
    warp(35, 10, 10);
  elseif (zn == 15) then
    -- Long room north
    warp(35, 4, 10);
  elseif (zn == 16) then
    chest(90, I_PCURING, 1);
    refresh();
  elseif (zn == 17) then
    chest(91, I_B_VISION, 1);
    refresh();
  elseif (zn == 18) then
    chest(92, I_WATERRING, 1);
    refresh();
  elseif (zn == 19) then
    chest(93, I_KBREW, 1);
    refresh();
  elseif (zn == 20) then
    chest(94, 0, 1000);
    refresh();
  elseif (zn == 21) then
    chest(95, I_VITSEED, 2);
    refresh();
  elseif (zn == 22) then
    if (get_progress(P_OPALARMOUR) == 0) then
      set_progress(P_OPALARMOUR, 1)
      sfx(5);
      msg("Opal Armour procured!", 255, 0);
      refresh();
    end;
  elseif (zn == 23) then
    combat(59);
    -- Clear the zone so it does not repeat the combat
    set_zone(get_ent_tilex(HERO1), get_ent_tiley(HERO1), 0);
  end
end


function entity_handler(en)
  return;
end


-- zone list
-- 1 first bomb leading to treasure
-- 2 cliff bomb
-- 3 bomb on lhs
-- 4 left of double bomb area
-- 5 right of ditto
-- 6 small pillar bomb
-- 7 behind small pillar
-- 8 south of long room
-- 9 left-bottom exit
-- 10 right-bottom exit
-- 11 north of long room
-- 12 door at extreme left
-- 13 save point room (south)
-- 14 save point room (north)
-- 15 opal dragon room south
-- 16..21 chests
-- 22 opal armour
-- 23 ghosts


-- character crib for PH
-- SENSAR     0
-- SARINA     1
-- CORIN      2
-- AJATHAR    3
-- CASANDRA   4
-- TEMMIN     5
-- AYLA       6
-- NOSLOM     7
