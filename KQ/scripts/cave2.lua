-- cave2 - "Pit in grotto north of Ekla"

function autoexec()
  if (get_progress(P_FELLINPIT) > 0) then
    set_mtile(15, 19, 360)
    set_zone(15, 19, 3)
  end
  return
end


function postexec()
  if (get_progress(P_FELLINPIT) == 0) then
    bubble(HERO1, "Ouch!")
    bubble(HERO1, "This is no ordinary gopher hole!")
  end
end


-- // P_TALKDERIG will ALWAYS be > 0!
function zone_handler(zn)
  if (zn == 1) then
    -- // This is the first time you try the door
    if (get_progress(P_FELLINPIT) == 0) then
      bubble(HERO1, "There's some kind of barrier here. I can't get past it.")
      set_mtile(15, 19, 360)
      set_zone(15, 19, 3)
      bubble(HERO1, "Hey... What's that thing down there?")
      set_progress(P_FELLINPIT, 1)
    -- // You have already tried the door
    else
      bubble(HERO1, "This barrier is still here. I can't get out.")
    end

  elseif (zn == 2) then
    chest(16, I_MHERB, 1)

  elseif (zn == 3) then
    if (get_progress(P_UCOIN) == 0) then
      if (get_progress(P_TALKDERIG) == 1) then
        -- // I have never seen Derig; he has never helped me.
        bubble(HERO1, "This doesn't do anything; I can't get out of here. I'll give up and just go to sleep.")
        set_progress(P_TALKDERIG, 2)
        change_map("grotto", 19, 16, 19, 16)
      elseif (get_progress(P_TALKDERIG) == 2) then
        -- // Derig helped you out, when you were sleeping.  You re-entered the pit before speaking to Jen.
        bubble(HERO1, "Oops, I'm stuck down here again. I'll try sleeping again to get back out.")
        change_map("grotto", 19, 16, 19, 16)
      end
    elseif (get_progress(P_UCOIN) == 1) then
      bubble(HERO1, "Hello? Is anyone there?")
      bubble(255, "Hey, are you alright down there?")
      bubble(HERO1, "Yeah... I'm okay.")
      bubble(255, "Hang on, let me get you out of there.")
      set_progress(P_TALKDERIG, 3)
      change_map("grotto", 17, 14, 17, 14)
    else
      -- // Derig has helped you out of here before
      bubble(HERO1, "Derig, can you help me out of here?")
      bubble(255, "Sure thing. Here you go.")
      change_map("grotto", 19, 16, 19, 16)
    end
  end
end


function entity_handler(en)
  return
end
