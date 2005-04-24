-- fort - "Mountain fort south of Andra"

-- /*
-- On this one, it might not be necessary to have this closed, unless we were
-- to have the main plot include a requirement to speak with Nostik, who gives
-- you authorization to pass through there. It should be fairly simple, but
-- lengthen out the game a bit.
-- PH: according to the Game Plot, it's _Tsorin_ who give you the authorization
-- here, not Nostik.
-- */


function autoexec()
   if (get_progress(P_TALK_TSORIN) == 2) then
      set_ent_active(4, 0)
   end
end


function postexec()
/*
  local en = 5

  if (get_progress(P_TALKDERIG) == 3) then
    view_range(1, 36, 11, 44, 19)
    set_ent_facing(HERO1, FACE_UP)
    if (get_numchrs() == 2) then
      set_ent_facing(HERO2, FACE_UP)
    end
    drawmap()
    screen_dump()
    do_fadein(4)
    set_holdfade(0)
    bubble(en, "So that's about it... cross the river and head for the small group of mountains due west from there.")
    bubble(en, "You just have to go through some caves to reach the valley beyond.")
    bubble(en, "Then make your way to the Oracle's tower. Is that understood?")
    bubble(HERO1, "Yeah, I guess so... it all seems clear enough.")
    bubble(en, "Great! Head out whenever you're ready.")
    set_progress(P_TALKDERIG, 4)
  end
*/
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 261, 33, 261, 33)

  elseif (zn == 2) then
    change_map("main", 261, 36, 261, 36)

  elseif (zn == 3) then
    door_in(40, 16, 36, 10, 44, 18)

  elseif (zn == 4) then
    door_out(17, 31)

  elseif (zn == 5) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "We have no outposts in goblin territory. If you get into real trouble, you'll have to go all the way back to Andra.")

  elseif (en == 1) then
    bubble(en, "Make sure to stock up on supplies.")

  elseif (en == 2) then
    if (get_progress(P_TALK_TSORIN) < 2) then
      bubble(en, "We cannot let you pass.")
    else
      bubble(en, "You are free to come and go as you please.")
    end

  elseif (en == 3) then
    if (get_progress(P_TALK_TSORIN) < 2) then
      bubble(en, "No one is allowed through right now.")
    else
      bubble(en, "Be careful in the goblin lands.")
    end

  elseif (en == 4) then
    if (get_progress(P_TALK_TSORIN) == 0) then
      bubble(en, "Only Captain Tsorin can authorize you to come through here.")
    else
      bubble(HERO1, "Tsorin sends his greetings to you. You are to let us pass through to the Oracle's Tower.")
      bubble(en, "I see that you are bearing his seal. Very well.")
      bubble(en, "I'll tell the Lieutenant you're here.")
      set_ent_script(en, "D10")
      wait_for_entity(en, en)
      set_progress(P_TALK_TSORIN, 2)
      set_ent_active(en, 0)
    end

  elseif (en == 5) then
    if (get_progress(P_TALK_TSORIN) == 1) then
      bubble(en, "Cross the river and head for the small group of mountains due west from there.")
      bubble(en, "You just have to go through some caves to reach the valley beyond.")
      bubble(en, "Then make your way to the Oracle's tower. Is that understood?")
      bubble(HERO1, "Yeah, I guess so... it all seems clear enough.")
      bubble(en, "Good. Head out whenever you're ready.")
    else
      bubble(en, "Best of luck to you.")
    end

  end
end
