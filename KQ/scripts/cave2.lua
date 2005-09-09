-- cave2 - "Pit in grotto north of Ekla"

-- /*
-- {
-- P_UCOIN: Spoke to Jen, the granddaughter in Ekla
--   (0) You don't know Derig is here; you "somehow" get out
--   (1) You know Derig can help you out
--   (2) (not used)
--   (3) Derig is back; he can help you out again
-- P_TALKDERIG: When you make contact with Derig
--   (0) (not used)
--   (1) Haven't met Derig; have never gotten out of Pit before
--   (2) Got out of Pit (DIDN'T meet Derig)
--   (3) Got out of Pit (DID meet Derig)
--   (4) (not used)
--   (5) (not used)
--   (6) Derig is back; he can help you out again.
-- P_FELLINPIT: Set when you fall down the pit
--   (0) Haven't fallen down the pit
--   (1) Fell into the pit, did NOT speak to Derig
--   (2) Fell into the pit, DID speak to Derig
--   (3) You won't be able to fall down into the Pit
--   (4) You may fall down into the pit again
-- }
-- */


function autoexec()
  refresh()
end


function postexec()
  if (get_progress(P_FELLINPIT) == 0) then
    bubble(HERO1, "Ouch!")
    bubble(HERO1, "This is no ordinary gopher hole!")
  end
end


function refresh()
  -- Show the shiny, glowing rescue spot
  if (get_progress(P_FELLINPIT) > 0) then
    set_btile(15, 18, 30)
    set_zone(15, 18, 3)
  end
end


function zone_handler(zn)
  if (zn == 1) then
    -- // This is the first time you try the door
    if (get_progress(P_FELLINPIT) == 0) then
      bubble(HERO1, "There's some kind of barrier here. I can't get past it.")
      set_progress(P_FELLINPIT, 1)
      refresh()
      bubble(HERO1, "Hey... What's that thing down there?")
    -- // You have already tried the door
    else
      bubble(HERO1, "This barrier is still here. I can't get out.")
    end

  elseif (zn == 2) then
    chest(16, I_MHERB, 1)
    refresh()

  elseif (zn == 3) then
    if (get_progress(P_UCOIN) == 0) then
      if (get_progress(P_TALKDERIG) == 1) then
        -- // I have never seen Derig; he has never helped me.
        bubble(HERO1, "This doesn't do anything; I can't get out of here. I'll give up and just go to sleep.")
        set_progress(P_TALKDERIG, 2)
        inn("You decide to sleep.", 0, 0)
      elseif (get_progress(P_TALKDERIG) == 2) then
        -- // Derig helped you out, when you were sleeping.  You re-entered the pit before speaking to Jen.
        bubble(HERO1, "Oops, I'm stuck down here again. I'll try sleeping again to get back out.")
        inn("You decide to sleep.", 0, 0)
      end
      set_ent_facing(HERO1, FACE_RIGHT)
      change_mapm("grotto", "by_fire")
      -- change_map("grotto", 19, 16, 19, 16)
    elseif (get_progress(P_UCOIN) == 1) then
      bubble(HERO1, "Hmm... this doesn't look like it does anything.")
      msg("Hello? Who's there? Is that somebody down there?", 255, 0)
      bubble(HERO1, "Yeah... I fell down here and the door's blocked.")
      msg("Hang on, let me get you out of there.", 255, 0)
      set_progress(P_TALKDERIG, 3)
      change_mapm("grotto", "by_pit")
      -- change_map("grotto", 17, 14, 17, 14)
    else
      -- // Derig has helped you out of here before
      bubble(HERO1, "Derig, can you help me out of here?")
      msg("Sure thing. Here you go.", 255, 0)
      set_ent_facing(HERO1, FACE_RIGHT)
      change_mapm("grotto", "by_fire")
      -- change_map("grotto", 19, 16, 19, 16)
    end
  end
end


function entity_handler(en)
  return
end
