-- pass - "Karnok's pass, between Sunarin and Pulcannen"

function autoexec()
  if (get_progress(P_SAVEBREANNE) > 0) then
    -- Move guard on left side of pass
    place_ent(0, get_ent_tilex(0) - 1, get_ent_tiley(0));
    set_ent_facing(0, 3);
    -- Move guard on right side of pass
    place_ent(1, get_ent_tilex(1) + 1, get_ent_tiley(1));
    set_ent_facing(1, 2);
  end
  refresh();
end


-- show the status of a chest
function showch(x, y, tr)
  local ch
  if (get_treasure(tr) == 1) then
    ch = 39;
  else
    ch = 38;
  end
  set_mtile(x, y, ch);
end


function refresh()
  if (get_progress(P_CAVEKEY)>0) then
    -- Move Rufus into his house
    place_ent(2, 152, 12);
  end

  showch(74, 47, 73);
  showch(75, 47, 74);
  showch(84, 23, 82);
  showch(93, 21, 83);
end


function postexec()
  return;
end


function zone_handler(zn)
--  if (zn == 0) then
--    combat(50);

--  elseif (zn == 1) then
  if (zn == 1) then
    change_map("main", 268, 103, 268, 103);

  elseif (zn == 2) then
    change_map("main", 271, 100, 271, 100);

  elseif (zn == 3) then -- northern door
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1, "What's wrong with the code?",
                    "I just walked through here.",
                    "Therefore, I have the key.",
                    "Check the code for bugs.");
    else
      -- Open the door before going in
      sfx(26);
      set_mtile(83, 27, 57);
      set_mtile(83, 28, 33);
      drawmap();
      screen_dump();

      change_map("cave5", 4, 93, 0, 0);
    end
  elseif (zn == 4) then
    chest(73, I_VITSEED, 1);
    refresh();

  elseif (zn == 5) then
    chest(74, I_ERUNE, 1);
    refresh();

  -- Western door
  elseif (zn == 6) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1, "Locked.");
    else
      -- Open the door before going in
      sfx(26);
      set_mtile(78, 38, 57);
      set_mtile(78, 39, 33);
      drawmap();
      screen_dump();

      change_map("cave5", 15, 147, 0, 0);
    end

  elseif (zn == 7) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1, "Locked.");
    else
      -- Open the door before going in
      sfx(26);
      set_mtile(106, 35, 57);
      set_mtile(106, 36, 33);
      drawmap();
      screen_dump();

      change_map("cave5", 84, 147, 0, 0);
    end

  -- Door into cabin
  elseif (zn == 8) then
    door_in(150, 14, 147, 7, 155, 18);

  -- Door out of the cabin
  elseif (zn == 9) then
    door_out(83, 50);

  -- Treasure
  elseif (zn == 10) then
    chest(82, 0, 500);
    refresh();

  -- Treasure
  elseif (zn == 11) then
    chest(83, I_SALVE, 1);
    refresh();

  elseif (zn == 12) then
    touch_fire(party[0]);
  end
end


function miner(en)
  if (get_progress(P_TALKRUFUS) == 0) then
    bubble(en, "Howdy!");
    bubble(HERO1, "Hello. Is this your cabin?");
    bubble(en, "It sure is! Th' name's Rufus. I work in the abandoned mines in these parts.");
    bubble(en, "But I'll warn ya now. There's something nasty hiding in them thar mines.");
    bubble(HERO1, "Like what?");
    bubble(en, "I guess I've said enough...");
    set_progress(P_TALKRUFUS, 1);
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1, "Don't worry. I was just passing through, anyway.");
    end
  elseif (get_progress(P_TALKRUFUS) == 1) then
    bubble(HERO1, "Hello again.");
    bubble(en, "Don't forget what I told ya about them mines.");
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1, "I won't.");
    end
  end

  if (get_progress(P_TALKRUFUS)<2) then
    bubble(HERO1, "But, I really need to get in there.");
    set_progress(P_TALKRUFUS, 2);
    bubble(en, "We-e-ell...");
    bubble(en, "I reckon that you and me might be able to do a little business here.");
  else
    bubble(en, "So, back for some more of the dynamite, are you?");
  end

  shop(23);
end


function entity_handler(en)
  if (en == 0 or en == 1) then
    if (get_progress(P_SAVEBREANNE) == 0) then
      bubble(en, "This pass is reserved for use by caravans only.");
    else
      bubble(en, "You are free to use the pass. Just be careful.");
    end
  elseif (en == 2) then
    -- Cabin bloke
    miner(en);
  end
end
