-- starting - "Various pieces of heroes' homes back in Antoria"

function autoexec()
  return;
end


function postexec()
  local a;

  if (get_progress(P_SKIPINTRO) == 1) then
    LOC_choose_hero();
    return;
  end

  set_autoparty(1);
  set_vfollow(1);

  -- TT: This makes it a hundred times faster to update scripts and see how
  -- it will appear in real time.  Just comment out everything that you do not
  -- want to sit through every time you recompile the script.
  LOC_storyline(SENSAR);
  LOC_storyline(SARINA);
  LOC_storyline(CORIN);
  LOC_storyline(AJATHAR);
  LOC_storyline(CASANDRA);
  LOC_storyline(TEMMIN);
  LOC_storyline(AYLA);
  LOC_storyline(NOSLOM);

  view_range(1, 116, 14, 141, 31);
  screen_dump();

  set_ent_active(HERO1, 0);

  -- Put a bunch of people around Hunert the butler
  place_ent(28, 129, 26);
  place_ent(29, 135, 27);
  place_ent(30, 132, 27);
  place_ent(31, 126, 19);
  place_ent(32, 133, 19);
  place_ent(33, 133, 21);
  place_ent(34, 125, 25);
  place_ent(35, 129, 24);
  for a = 0, 7, 1 do
    set_ent_id(28 + a, a);
    set_ent_active(28 + a, 1);
    set_ent_movemode(28 + a, 0);
  end
  set_noe(get_noe() + 8);
  warp(130, 24, 4);
  msg("Reisha Mountain", 255, 1500);

  bubble(15, "Thank you all for coming.");

  set_ent_script(15, "W25F2W25F0W25F3W25F0");
  wait_for_entity(15, 15);

  bubble(255, "Alright, I have made my decision...");
  wait(25);

  set_autoparty(1);
  LOC_choose_hero();
end


function zone_handler(zn)
  return;
end


function entity_handler(en)
  return;
end


function LOC_choose_hero()
--  local stop = 0;
--  local rd = 1;
--  local a, p;
--  local ptr = 0;
--
--  clear_buffer();
--  screen_dump();
--  do_fadein(16);
--  while (stop == 0) do
--    if (rd == 1) then
--      clear_buffer();
--      for a = 0, 7, 1 do
--        if (ptr == a) then
--          dark_mbox(a * 32 + 32, 16, 2, 2);
--        else
--          mbox(a * 32 + 32, 16, 2, 2);
--        end
--        drawframe(a, 0, a * 32 + 40, 24);
--      end
--      mbox(80, 64, 18, 5);
--      draw_pstat(ptr, 88, 72);
--      mbox(80, 120, 18, 8);
--      ptext(88, 128, "Strength", 5);
--      ptext(88, 136, "Agility", 5);
--      ptext(88, 144, "Vitality", 5);
--      ptext(88, 152, "Intellect", 5);
--      ptext(88, 160, "Sagacity", 5);
--      ptext(88, 168, "Speed", 5);
--      ptext(88, 176, "Aura", 5);
--      ptext(88, 184, "Spirit", 5);
--      for p = 0, 7, 1 do
--        ptext(184, p * 8 + 128, ":", 5);
--        pnum(208, p * 8 + 128, get_party_stats(ptr, p) / 100, 0);
--      end
--      screen_dump();
--    end
--    read_controls(0, 0, 1, 1, 1, 0, 0, 0);
--    rd = 0;
--    if (check_key(2) == 1) then
--      ptr = ptr - 1;
--      if (ptr < 0) then
--        ptr = 7;
--      end
--      sfx(0);
--      rd = 1;
--    elseif (check_key(3) == 1) then
--      ptr = ptr + 1;
--      if (ptr > 7) then
--        ptr = 0;
--      end
--      sfx(0);
--      rd = 1;
--    elseif (check_key(4) == 1) then
--      stop = 1;
--    end
--  end

-- TT add:
  local ptr = NOSLOM;
  add_chr(ptr);
  set_all_equip(party[0], I_KNIFE1, 0, 0, I_SUIT1, 0, 0);
  change_map("manor", 37, 7, 37, 7);
end


function LOC_storyline(en)
  add_chr(en);

  if (en == SENSAR) then
    set_ent_facing(HERO1, 1);

    view_range(1, 8, 10, 22, 28);
    warp(15, 27, 16);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "W50U3");
    wait_for_entity(HERO1, HERO1);

    bubble(HERO1, "Hey, what's that?");

    set_ent_script(HERO1, "L2U5");
    wait_for_entity(HERO1, HERO1);

    -- TT: These are formatted specifically for looks: do not one-line them!
    thought(HERO1, "$0:",
                   "`The great magician Nostik doth   ",
                   " formally request audience of all ",
                   " brave adventurers who wish to    ");
    thought(HERO1, " rid the land of the infestation  ",
                   " of monsters. Honorable men and   ",
                   " women may be rewarded with a     ",
                   " Great Magic for their service.   ");
    thought(HERO1, " Gather ye together to the mystic ",
                   " Reisha Mountain in a fortnight.  ",
                   " Haste and discression is of the  ",
                   " utmost importance.'              ");
    set_ent_facing(HERO1, 0);
    wait(50);

    thought(HERO1, "Great Magic?!");

    set_ent_facing(HERO1, 1);
    wait(50);

    bubble(HERO1, "Wait a minute! This Great Magic may be the answer I've been looking for to reverse this curse!");
    bubble(HERO1, "I'm there man!");

    set_ent_facing(HERO1, 0);
    wait(50);

    thought(HERO1, "Who am I talking to?",
                   "Oh well, who cares? I'm off!");

    set_ent_speed(HERO1, 5);
    set_ent_script(HERO1, "D4R2D4");
    wait_for_entity(HERO1, HERO1);

    sfx(25);

  elseif (en == SARINA) then
    view_range(0, 0, 0, 0, 0);
    warp(27, 43, 4);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "L17");
    wait_for_entity(HERO1, HERO1);

    bubble(HERO1, "What?");

    set_ent_facing(HERO1, 0);
    wait(10);

    thought(HERO1, "That wasn't there this morning.");

    set_ent_script(HERO1, "R2U1");
    wait_for_entity(HERO1, HERO1);

    bubble(HERO1, "$0:",
                  "Wow, what a strange request.");
    thought(HERO1, "Hey wait a minute. Perhaps this is what I need to help Mom and Dad.");
    thought(HERO1, "I'll have to let them know right away!");

    set_ent_speed(HERO1, 5);
    set_ent_script(HERO1, "D1R15");
    wait_for_entity(HERO1, HERO1);

  elseif (en == CORIN) then
    view_range(0, 0, 0, 0, 0);
    warp(37, 27, 4);

    set_ent_speed(HERO1, 4);
    set_ent_script(HERO1, "U7");
    wait_for_entity(HERO1, HERO1);

    thought(HERO1, "$0:",
                   "Hmm... a note from Master Largon. Some crazy magician needs some toadies.");

    wait(50);
    set_mtile(37, 19, 0);
    wait(50);

    set_ent_facing(HERO1, 0);
    wait(50);

    thought(HERO1, "I don't like the sound of this one bit. I wonder who this Nostik guy is?");
    thought(HERO1, "Well, I'd better get packed. It's a long way to Reisha Mountain.");

  elseif (en == AJATHAR) then
    view_range(1, 50, 0, 64, 16);
    warp(57, 15, 4);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "U7");
    wait_for_entity(HERO1, HERO1);

    bubble(HERO1, "$0:",
                  "You wanted to see me sir?");
    bubble(24, "Yes $0. It is time for you to leave the temple and discover the world. I have found the perfect way for you to do that.");
    bubble(HERO1, "Excellent sir. What am I to do?");
    bubble(24, "One of the others brought in a notice that speaks of a man named Nostik.");
    bubble(24, "This man makes strange claims to his identity as well as to give or teach others some `Great Magic'.");
    bubble(24, "I am giving you an opportunity to investigave this quest. This will test your training, as well as get you out on a little adventure.");
    bubble(HERO1, "I would be honored sir. Where do I go first?");
    bubble(24, "The note says to go to Reisha Mountain where you will receive further instruction.");
    bubble(HERO1, "I'm on my way.");
    bubble(24, "Fare thee well $0.");

    set_ent_script(HERO1, "D5");
    wait_for_entity(HERO1, HERO1);

  elseif (en == CASANDRA) then
    set_ent_facing(HERO1, 1);

    view_range(0, 0, 0, 0, 0);
    warp(55, 48, 4);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "U9");
    wait_for_entity(HERO1, HERO1);

    thought(HERO1, "$0:",
                   "Nella was right. This is an extremely stupid place to put a public notice.");
    bubble(HERO1, "Wait a minute!");
    bubble(HERO1, "Great Magic!");
    thought(HERO1, "I could definitely use that.");
    thought(HERO1, "Looks like I'm going to Reisha Mountain.");

    set_ent_speed(HERO1, 5);

    set_ent_script(HERO1, "D9");
    wait_for_entity(HERO1, HERO1);

  elseif (en == TEMMIN) then
    view_range(0, 0, 0, 0, 0);
    warp(88, 29, 4);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "L9");
    wait_for_entity(HERO1, HERO1);

    bubble(HERO1, "$0:",
                  "I was told you had a new mission for me, Sir.");
    bubble(25, "Yes $0, you are going to Reisha Mountain. You will likely find others there who are preparing for a quest.");
    bubble(25, "Reports say that a wizard is looking for adventurers. You are to find out what this is all about and report back to us.");
    bubble(25, "This man may pose no threat, but we will be safe instead of sorry. Is that understood?");
    bubble(HERO1, "Yes sir. I'll leave immediately, sir.");

    set_ent_script(HERO1, "R9");
    wait_for_entity(HERO1, HERO1);

  elseif (en == AYLA) then
    view_range(1, 101, 12, 113, 23);
    warp(107, 22, 4);

    set_ent_speed(HERO1, 4);

    set_ent_script(HERO1, "U6R2F3W50");
    wait_for_entity(HERO1, HERO1);

    set_ftile(110, 16, 312);
    wait(50);

    bubble(HERO1, "$0:",
                   "This is so perfect.");
    bubble(HERO1, "Just as my luck starts running out, along comes an opportunity to get out of town and make some serious coin at the same time.");
    bubble(HERO1, "Life is good.");

    set_ent_facing(HERO1, 0);
    wait(50);

    bubble(HERO1, "I can just imagine the fame and glory I'd get for getting rid of all these monsters.");
    bubble(HERO1, "I would be the richest thief on the block. No! I'd be the richest thief... IN MY OWN CITY! Ha ha ha!");

    wait(50);

  elseif (en == NOSLOM) then
    set_ent_facing(HERO1, 1);
    view_range(1, 101, 31, 111, 49);
    warp(106, 35, 4);

    set_ent_speed(HERO1, 4);

    thought(HERO1, "$0:",
                   "This does not look good. There is a really good chance that it could be the same man.");
    set_ent_facing(HERO1, 0);
    wait(50);
    thought(HERO1, "I have no choice but to check it out. I'd best be on my way.");
    set_ent_script(HERO1, "D12");
    wait_for_entity(HERO1, HERO1);
    set_autoparty(0);
  end

  remove_chr(en);
  return;
end
