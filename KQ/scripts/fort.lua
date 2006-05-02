-- fort - "Mountain fort south of Andra"

-- /*
-- {
-- P_GOBLINITEM: Possession of Goblin Item (which helps seal Monster Portal)
--   (0) Do not have it
--   (1) Got it
--   (2) Returned it to Oracle
--
-- P_PORTAL2GONE: Whether the portal in the temple is still working
--   (0) Still letting monsters through
--   (1) The Portal is sealed shut
--
-- P_TALK_TSORIN: If you've spoken to Tsorin in Andra (and got his seal)
--   (0) You haven't spoken to him yet
--   (1) Tsorin gave you a note to give to Derig
--   (2) Derig gave you a note to return to Tsorin
--   (3) Tsorin gave you his seal to get through the fort
--   (4) You've shown the seal to the guards at the fort
--   (5) You are free pass through the fort anytime (no contention in goblin lands)
--
-- In this town, we are going to add the 3rd PM, which will include the sub-
-- plot in the temple.
--
-- }
-- */


function autoexec()
  refresh()
end


function refresh()
  if (get_progress(P_TALK_TSORIN) > 3) then
    place_ent(4, 39, 14)
    set_ent_facing(4, FACE_DOWN)
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
    change_map("main", "fort_n")

  elseif (zn == 2) then
    change_map("main", "fort_s")

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
    if (get_progress(P_TALK_TSORIN) < 3) then
      bubble(en, "We cannot let you pass.")
    elseif (get_progress(P_TALK_TSORIN) == 3) then
      bubble(en, "The Lieutenant requires Tsorin's authorization before anyone can pass through here.")
    else
      bubble(en, "You are free to come and go as you please.")
    end

  elseif (en == 3) then
    if (get_progress(P_TALK_TSORIN) < 3) then
      bubble(en, "No one is allowed through right now.")
    elseif (get_progress(P_TALK_TSORIN) == 3) then
      bubble(en, "Show Tsorin's seal to this man next to me. He will inform the Lieutenant that you have it.")
    else
      bubble(en, "Be careful in the goblin lands.")
    end

  elseif (en == 4) then
    if (get_progress(P_TALK_TSORIN) < 3) then
      bubble(en, "Only Captain Tsorin can authorize you to come through here.")
    elseif (get_progress(P_TALK_TSORIN) == 3) then
      if (get_numchrs() == 1) then
        bubble(HERO1, "Tsorin sends his greetings to you. You are to let me pass through to the Oracle's Tower.")
      else
        bubble(HERO1, "Tsorin sends his greetings to you. You are to let us pass through to the Oracle's Tower.")
      end
      bubble(en, "I see that you are bearing his seal. Very well.")
      bubble(en, "I'll tell the Lieutenant you're here.")
      set_ent_speed(en, 4)
      set_ent_script(en, "D10")
      wait_for_entity(en, en)
      set_progress(P_TALK_TSORIN, 4)
      refresh()
    elseif (get_progress(P_TALK_TSORIN) == 4) then
      bubble(en, "The Lieutenant says you are free to go into goblin lands. Be cautious.")
    else
      bubble(en, "We have received word that the goblin lands are peaceful now. Thank you for your help.")
    end

  elseif (en == 5) then
    if (get_progress(P_TALK_TSORIN) == 4) then
      bubble(en, "Lieutenant:", "South of here is a river you'll need to cross. Head for the tower in the small group of mountains due west from there. You just have to go through some caves to reach it. Then make your way up to the Oracle. Is that understood?")
      bubble(HERO1, "Yeah, I guess so... it all seems clear enough.")
      bubble(en, "Good. Head out whenever you're ready.")
    else
      bubble(en, "Best of luck to you.")
    end

  end
end
