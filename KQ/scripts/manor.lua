-- manor - "Nostik's manor southwest of Ekla"

-- "{
-- Which globals should we have for manor?
--
-- P_INTRO
--   (0) New game; Nostik hasn't spoken yet
--   (1) Nostik explained the quest, but you haven't yet spoken to the butler
--   (2) Butler has already spoken to you
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
-- person that is trailing you will be sitting there; they will join you when
-- you leave the manor.  If you talk to the butler, he will allow you to pick
-- them to join you or switch around your party.  Irregardless of who you
-- choose, they don't 'jump up' and start trailing you immediately; they stay
-- around the table until you leave.  This prevents 'jumping' when a character
-- is traded.  The 'removed' character would otherwise magically disappear
-- from behind you and appear at the table, and the person you're recruiting
-- will disappear from their seat and be standing behind/under you.  I just
-- don't like that; it just looks like bad coding.
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
-- }"


function autoexec()
  local a;

  -- There needs to be a check to see if there are any recruits.  If so, we
  -- need to set P_INTRO=3 so the code below can function correctly.

  -- Nostik has not explained the quest yet
  if (get_progress(P_INTRO) == 0) then
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

  -- P_INTRO should NEVER ==1 when entering manor.map

  -- P_INTRO ==2 when you already talked to Hunert, but you have no recruits
  elseif (get_progress(P_INTRO) == 2) then
    for a = 0, 7, 1 do
      set_ent_active(a, 0);
    end

  -- You have recruited at least 1 other party member
  elseif (get_progress(P_INTRO) == 3) then
    for a = 1, 7, 1 do

-- **HERE** is the trouble code:
-- What is the best way that we want to keep track of which members have
-- joined your party?  One way is to make use of a bitfield so you can have
-- players 'a', 'c', and 'f' but not players 'b', 'd', 'e', or 'g'.  Another
-- way is to create a struct with '.in_party == 1' or something.  A third is
-- to only set char_array[x] to the characters ID# if they joined, null if
-- not.

      -- You have not recruited this person into your team
      if (in_party(get_ent_id(a)) == 0) then
        -- Remove entity from the map
        set_ent_active(a, 0);

      -- This person has been recruited
      else
        -- Place around the table
        set_ent_active(a, 1);
      end
    end
    -- Put Nostik to bed (he is old and feeble)
    place_ent(8, 9, 37);

  end
end


-- Why are these 3 lines in here if immediately followed with another postexec() ?
function postexec()
  return
end


function postexec()
  local player_response, done_talking;

  if (get_progress(P_INTRO) == 0) then
    rest(200);
    bubble(8, "Welcome all.");
--    bubble(8, "I am the legendary Nostik. I am a wizard. A very old wizard in fact. I know the secret of the Great Magic, and you have come to learn.");
--    bubble(8, "Be heretofore warned however: my words are not to be meddled with, for you are crunchy and taste good with...");
      bubble(8, "Well, never mind. I shall get right to the point.");
--    bubble(8, "I have said that I am in need of adventurers to partake in a quest for me. You eight have shown great courage to stay. Courage alone isn't enough, however. You must prove your worthiness of this Great Magic. Does anyone wish to leave?");
--    set_ent_script(8, "W25F1W35F3W35F0W35F3W50");
--    wait_for_entity(8, 8);
    if (prompt(8, 2, 1, "Very well then. Do you need a",
                     "background?", "  yes", "  no") == 0) then
      -- yes
      while(not done_talking) do
        player_response = prompt(8, 3, 0, "What do you want to learn about?",
                                       "  Staff of Xenarum",
                                       "  Malkaron",
                                       "  Done");
        -- Staff of Xenarum
        if (player_response == 0) then
          bubble(8, "A very long time ago, a great priest appeared at the Xenarum shrine. There he chose three Disciples to follow and learn of a better life. Some say that this priest was the very Adrial the Messiah.");
          bubble(8, "Taking a non-violent approach to everything, Adrial chose Three Disciples to follow him and spread peace and knowledge around the land. Adrial taught of becoming the best that you can be in the followings of `The God'. The legend suggests that his Three Disciples were jealous of his connection with `The God' and betrayed him.");
          bubble(8, "This is where you come in. Supposedly one Adrial's Disciples, Chaiman, kept a journal of his voyage. Inside the journal speaks of what actually happened with the miracles and everything. No one has ever seen this journal but word has it that it is kept somewhere in Xenarum. That land is inaccessible except with the Staff of Xenarum, which is the key to the Shifting Mounts.");
          bubble(8, "Your journey is to find this Staff. Bring it to me; I must see what is in that journal! I am offering to teach any of you the Great Magic in exchange for that Staff.");
          bubble(8, "You see now why I have chosen only the eight of you; this journey must be as inconspicuous as possible, as the Three Disciples may have minions everywhere. You can travel with anyone you wish, but please stay in groups of no more than two. More than that will draw attention to you, and may put all of us in jeopardy.");
        -- Malkaron
        elseif (player_response == 1) then
          -- explain the whole Maklaron bit
          thought(8, "Oh great. How will I explain THIS one? (add your own script here)");
        -- Finished with options
        elseif (player_response == 2) then
          done_talking = 1;
        -- We should never reach this one:
        else
          bubble(8, "That's not an option, Einstein.");
        end
      end
    end
    bubble(8, "I thank you for your help. Talk to my butler before you leave and he will help you get started on your journey.");
    set_progress(P_INTRO, 1);
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
    if (get_progress(P_INTRO) == 1) then
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
      bubble(9, "Maybe you didn't hear Master",
                "Nostik when he said that you",
                "were supposed to see me before",
                "you left.");
      bubble(9, "Anyways, Master Nostik asked",
                "me to give you this.");
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
    if (get_progress(P_INTRO) == 0) then
      bubble(8, "Talk to my butler before you leave and he will help you get started on your journey.");
    elseif (get_progress(P_INTRO) == 3) then
      bubble(8, "Zzz... zzz... zzz...");
    else
      bubble(8, "Feel free to look through my books and scrolls. I have made a study about this Disciple's journal and you may learn much.");
    end

  -- Butler Hunert
  elseif (en == 9) then
    if (get_progress(P_INTRO) == 1) then
      bubble(9, "Ah yes, Master Nostik asked",
                "me to give you this.");
      LOC_talk_butler();
    elseif (get_progress(P_INTRO) == 2) then
      bubble(9, "Good luck.");
    elseif (get_progress(P_INTRO) >= 3) then
      bubble(9, "Your friends are here waiting for you. Nostik is asleep upstairs. Don't wake him.");
      bubble(9, "Who do you want in your party?");
      -- PH, this is where your script comes in?
    end

  end
end

function LOC_talk_butler()
  drawmap();
  screen_dump();
  sfx(6);
  msg("You've acquired 200 gp!", 255, 0);
  set_gp(get_gp()+200);
  drawmap();
  screen_dump();
--  bubble(9, "Oh and there is one thing",
--            "that I better warn you about.");
--  bubble(9, "It's nothing major really, but",
--            "this world just happens to be",
--            "full of monsters that will",
--            "constantly attack you.");
--  bubble(HERO1, "Well, that's not so",
--                "weird actually.");
--  bubble(9, "These creatures are not your",
--            "ordinary violent miscreants.");
--  bubble(HERO1, "Are they ever?");
--  bubble(9, "These creatures are the",
--            "manifestations of evil spirits.");
--  bubble(9, "These creatures exist in a realm",
--            "between the world of the living",
--            "and the world of the dead.");
--  bubble(9, "They have no qualms about",
--            "attacking anyone or anything.");
--  bubble(HERO1, "That kind of thing happens",
--                "everywhere. I didn't think",
--                "it would be any different here.");
--  bubble(9, "Yes, but in your world, these",
--            "wandering monsters are real and",
--            "visible... even avoidable. That",
--            "is not the case here.");
--  bubble(HERO1, "Why not?");
--  bubble(9, "Well, like I said, the monsters",
--            "here are spirits. That means",
--            "that you can't see them.");
--  bubble(9, "You will see them once they",
--            "attack you, but then it's",
--            "already too late.");
--  bubble(HERO1, "So, do you mean to say that I",
--                "could be attacked at any time",
--                "without any warning whatsoever?");
--  bubble(9, "Well, not necessarily. For some",
--            "unknown reason, these spirits",
--            "do not enter the towns and",
--            "villages of the land.");
--  bubble(9, "In most other places though,",
--            "these encounters are highly",
--            "probable.");
--  if (get_pidx(0) == AYLA) then
--    bubble(HERO1, "This reward your boss is",
--                  "offering better be a lot",
--                  "more than just handsome.");
--  else
--    bubble(HERO1, "Oh well, I didn't think this",
--                  "would be a walk in the park.");
--  end
--  bubble(9, "Yes, well, you had better be leaving now.");
  bubble(HERO1, "Certainly, thanks.");
  set_progress(P_INTRO, 2);
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
-- TT debug add:
  -- This causes some glitches that we are going to have to fix; it is perfect
  -- here because everyone is conveniently located right here.  Now I need
  -- to fix it so it properly handles multiple party members.
  add_chr(b);
end
