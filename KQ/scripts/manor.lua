-- manor - "Nostik's manor southwest of Ekla"

-- /*
-- Which globals should we have for manor?
--
-- P_MANOR
--   (0) New game; Nostik hasn't spoken yet
--   (1) Nostik explained the quest, but you haven't yet spoken to the butler
--   (2) Butler has already spoken to you
--   (3) You have recruited others and they are waiting around the table
--    *  We have to somehow "set_progress(P_MANOR, 3)"... it's not active yet.
--
-- P_PLAYERS
--   (0) You haven't recruited anyone yet
--   (1..7) You have others in your party
--
-- When the game starts, Nostik debriefs everybody quickly and then offers to
-- go into detail for whoever wants to stay.  We should script it so everyone
-- else leaves immediately (walks out the door) and you're left at the table.
--
-- Before you can leave yourself, the butler, Hunert, will stop to talk to you
-- and fill you in, as well as give you some money to start the quest.
--
-- When you return to the manor, the dining room should be empty unless you
-- have 'recruited' other party members into your team.  If you have, then
-- those members are sitting in their places around the table.  Even the
-- person that is trailing you will be sitting there; no one will join your
-- little "procession" until you leave the manor.  When you talk to the party
-- members around the table, PH's team selector will be activated.
-- Irregardless of who you choose, they don't 'jump up' and start trailing you
-- immediately; they stay around the table until you leave.  This prevents
-- 'jumping' when a character is traded.  The 'removed' character would
-- otherwise magically disappear from behind you and appear at the table, and
-- the person you're recruiting will disappear from their seat and be standing
-- behind/under you.  I just don't like that; it just looks like bad coding.
--
-- Quick reference:
--
--  SENSAR   0
--  SARINA   1
--  CORIN    2
--  AJATHAR  3
--  CASANDRA 4
--  TEMMIN   5
--  AYLA     6
--  NOSLOM   7
--  Nostik   8
--  Hunert   9
-- */


function autoexec()
  local a;

  -- There needs to be a check to see if there are any recruits.  If so, we
  -- need to set P_MANOR=3 so the code below can function correctly.

  -- Nostik has not explained the quest yet
  if (get_progress(P_MANOR) == 0) then
    -- Init all 8 heroes
    for a = 0, 7, 1 do
      -- Set up entities 0-7 in manor.map as your team members
      set_ent_id(a, a);
      -- They should all be obstacles :)
      set_ent_obsmode(a, 1);
      -- What this sprite looks like
      set_ent_chrx(a, 255);
    end

    -- Remove the NPC that looks like you, from the map
    set_ent_active(get_pidx(0), 0);
    -- Set the REAL hero in the old NPCs place
    place_ent(HERO1, get_ent_tilex(get_pidx(0)),
              get_ent_tiley(get_pidx(0)));
    -- Set your facing direction
    set_ent_facing(HERO1, get_ent_facing(get_pidx(0)));
    -- Center map on your character coords
    calc_viewport(1);

  -- P_MANOR should NEVER ==1 when entering manor.map

  -- P_MANOR ==2 when you already talked to Hunert, but you have no recruits
  elseif (get_progress(P_MANOR) == 2) then
    for a = 0, 7, 1 do
      set_ent_active(a, 0);
    end

  -- You have recruited at least 1 other party member
  elseif (get_progress(P_MANOR) == 3) then
     LOC_at_table()
-- **HERE** is the trouble code:
-- What is the best way that we want to keep track of which members have
-- joined your party?  One way is to make use of a bitfield so you can have
-- players 'a', 'c', and 'f' but not players 'b', 'd', 'e', or 'g'.  Another
-- way is to create a struct with '.in_party == 1' or something.  A third is
-- to only set char_array[x] to the characters ID# if they joined, null if
-- not.
    -- Put Nostik to bed (he is old and feeble)
    place_ent(8, 9, 37);

  end
end


function postexec()
  local player_response, done_talking;

  if (get_progress(P_MANOR) == 0) then
    rest(200);

    -- SLF: Hey, I want to add some contributions here. They will be as simple as
    -- possible and whoever wants to do more can make them actually sound good.
    bubble(8, "Alright everyone, I welcome you. Let me get right to the major points of why you're here.");
    bubble(8, "Monsters have started attacking us from out of nowhere. We need your help to find their origins and put a stop to it!");
    bubble(8, "Go out, discover where they came from, and get rid of them so we can enjoy peace once again. I have two hunches as to where they came from.");
    if (prompt(8, 2, 1, "Do you want to hear what",
                        "they are?", "  yes", "  no") == 0) then
      -- yes
      while (not done_talking) do
        player_response = prompt(8, 3, 0, "These are my ideas:",
                                          "  Staff of Xenarum",
                                          "  Malkaron",
                                          "  Done");

        if (player_response == 0) then
          -- Staff of Xenarum
          bubble(8, "I was captured and tortured by Malkaron. He stole my Staff of Xenarum, which I feel holds great power.");
          bubble(8, "I had learned of the staff from years of study, read about its amazing power, and found it in ancient ruins.");
          bubble(8, "I never tapped its full potential but know it has been a very powerful tool for good or evil.");
          bubble(8, "It may be in Malkaron's hands, and he may be controlling the monsters.");
        elseif (player_response == 1) then
          -- Malkaron
          bubble(8, "A corrupt man, Malkaron was a power-hungry nobleman in a frozen country. Because of that, many have dubbed him the Ice Lord.");
          bubble(8, "15 years ago, he captured me and took my Staff of Xenarum. He mysteriously became very powerful very quickly. I believe it was the staff that gave him such great power.");
          bubble(8, "Rebels in his army found me and helped me escape. Sadly, I lost one of my eyes during the ordeal, but Hunert, my butler, has been able to help me a great deal. He is very useful, and he will be a valuable resource for you.");
        elseif (player_response == 2) then
          done_talking = 1;
        else
          -- We should never reach this one; it is just error-checking:
          bubble(8, "That's not an option, Einstein!");
        end
      end
	end

    bubble(8, "I cannot help you by going out myself, but I shall be here if you would need me.");
    bubble(8, "Good luck all of you.");

    -- TT: make everyone else walk out the door
    set_ent_script(0, "R3D1L1D6R3");
    set_ent_script(1, "R2D1L1D6R4");
    set_ent_script(2, "R1D1L1D6R4D1");
    set_ent_script(3, "D1L1D6R4D2");
    set_ent_script(4, "R1D2L1D1L1D6R2");
    set_ent_script(5, "R2D2L1D1L1D6R1");
    set_ent_script(6, "R3D2L1D1L1D6");
    wait_for_entity(0, 6);

    for a = 0, 6, 1 do
      set_ent_active(a, 0);
    end

    bubble(8, "When you are ready to go, talk to Hunert and he will get you started on your journey.");
    set_progress(P_MANOR, 1);
  end
end


function zone_handler(zn)
  -- Front door
  if (zn == 1) then
    change_map("main", 107, 40, 107, 40);

  -- Stairs going up
  elseif (zn == 2) then
    warp(25, 34, 8);

  -- Stairs going down
  elseif (zn == 3) then
    warp(25, 5, 8);

  -- Doors, duh
  elseif (zn == 4) then
    bubble(HERO1, "Locked.");

  -- Bookshelves
  elseif (zn == 5) then
    bubble(HERO1, "Wow! This guy reads weird stuff.");

  -- In front of exit
  elseif (zn == 6) then
    if (get_progress(P_MANOR) == 1) then
      bubble(9, "Hey! Hold on!");
      -- Turn around, see who is yelling
      set_ent_facing(HERO1, 1);
      -- Butler running speed
      set_ent_speed(9, 5);
      -- Check location on map
      if (get_ent_tilex(HERO1) == 16) then
        -- Hero is in front of right door
        set_ent_script(9, "D1R3D2L5D7L1D2");
      else
        -- Hero is in front of left door
        set_ent_script(9, "D1R3D2L5D7L2D2");
      end
      -- Process movement script
      wait_for_entity(9, 9);
      -- Butler normal speed
      set_ent_speed(9, 3);
      bubble(9, "You would be a fool to leave without hearing what I have to say.");
      bubble(9, "First, Nostik gives you this.");
      LOC_talk_butler();
    end

  -- Search fireplaces
  elseif (zn == 7) then
    touch_fire(get_pidx(0));
  end
end


function entity_handler(en)
  -- You are talking to other party members
  if (en >= 0 and en <= 7) then
    LOC_chit_chat(en);

  -- Nostik
  elseif (en == 8) then
    if (get_progress(P_MANOR) == 0) then
      bubble(8, "Talk to my butler before you leave and he will help you get started on your journey.");
    elseif (get_progress(P_MANOR) == 3) then
      bubble(8, "Zzz... zzz... zzz...");
    else
      bubble(8, "You may find a lot of information about the Staff of Xenarum in the books and scrolls in this manor. I have written many of them myself, actually!");
    end

  -- Butler Hunert
  elseif (en == 9) then
    if (get_progress(P_MANOR) == 1) then
      bubble(9, "Ah yes, Master Nostik asked me to give you this.");
      LOC_talk_butler();
    elseif (get_progress(P_MANOR) == 2) then
      bubble(9, "Welcome back, $0. Have you spoken with the other members of your group? The abilities they possess may come in useful in your work.");
    elseif (get_progress(P_MANOR) == 3) then
      bubble(9, "The others in your group are here. I'm sure if you talk to them, they will join your party.");
      bubble(9, "Welcome back. Nostik has retired to his room. Please don't wake him.");
      select_manor()-- PH, this is where your script comes in?
      LOC_at_table()
    end
  end

end


function LOC_talk_butler()
  drawmap();
  screen_dump();
  sfx(6);
  msg("You've acquired 200 gp!", 255, 0);
  set_gp(get_gp() + 200);
  drawmap();
  screen_dump();
  bubble(9, "The source of the monsters is a mystery. They appear out of nowhere and may attack randomly.");
  bubble(HERO1, "You mean they will just go on a wild rampage and start gouging us for no reason?");
  bubble(9, "Heh heh. That's a good way of putting it. It seems to me that they have no qualms about attacking anyone or anything.");
  bubble(HERO1, "That will sure make it hard to sleep at night.");
  bubble(9, "Maybe so. Try sleeping in a town or village inn. Monsters seem to avoid populated places for some reason.");
  bubble(HERO1, "I wonder why that is?");
  msg("Hunert shrugs", 255, 0);
  bubble(HERO1, "Well, thank you for the information.");
  set_progress(P_MANOR, 2);
end


function LOC_chit_chat(a)
  local b;
  local c;

  b = get_ent_id(a);
  if (b == SENSAR) then
    bubble(a, "Good luck to you friend.");
  elseif (b == SARINA) then
    bubble(a, "I really need that reward.");
  elseif (b == CORIN) then
    bubble(a, "Hmmm... this all seems very odd.");
  elseif (b == AJATHAR) then
    bubble(a, "I hope to learn much on this journey.");
  elseif (b == CASANDRA) then
    bubble(a, "That Staff is as good as mine.");
  elseif (b == TEMMIN) then
    bubble(a, "This will certainly not be easy.");
  elseif (b == AYLA) then
    bubble(a, "I am confident we will all do well.");
  elseif (b == NOSLOM) then
    bubble(a, "I am fortunate to have been selected.");
  end
end

function LOC_at_table()
   local id,a
   log("HERO1 "..get_pidx(0)..", HERO2 "..get_pidx(1))
   for a=0,7 do
      -- You have not recruited this person into your team
      id=get_progress(a+P_MANORPARTY)-1
      if (id==get_pidx(0)) then
	 id=-1;
      end
      if (get_numchrs()==2 and id==get_pidx(1)) then
	 id=-1;
      end
      log("Ent "..a.." with ID "..id)
      if (id < 0) then
        -- Remove entity from the map
	 set_ent_active(a, 0);
      else
        -- Place around the table
        set_ent_active(a, 1)
	set_ent_id(a, id)
	set_ent_chrx(a,255)
	set_ent_obsmode(a,1)
      end
   end
end
