-- fort - "Mountain fort south of Andra"

-- /*
-- On this one, it might not be necessary to have this closed, unless we were
-- to have the main plot include a requirement to speak with Nostik, who gives
-- you authorization to pass through there. It should be fairly simple, but
-- lengthen out the game a bit.
-- PH: according to the Game Plot, it's _Tsorin_ who give you the authorisation here
-- not Nostik.
-- */


function autoexec()
   if (get_progress(P_TALK_TSORIN)==2) then
      set_ent_active(4, 0)
   end
end


function postexec()

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
    bubble(0, "We have no outposts in goblin territory. If you get into real trouble, you'll have to go all the way back to Andra.")

  elseif (en == 1) then
    bubble(1, "Make sure to stock up on supplies.")

  elseif (en == 2) then
    if (get_progress(P_TALK_TSORIN) < 2) then
      bubble(2, "We cannot let you pass.")
    else
      bubble(2, "You are free to come and go as you please.")
    end

  elseif (en == 3) then
    if (get_progress(P_TALK_TSORIN) < 2) then
      bubble(3, "No one is allowed through right now.")
    else
      bubble(3, "Be careful in the goblin lands.")
    end

  elseif (en == 4) then
     if (get_progress(P_TALK_TSORIN)==0) then
	bubble(en, "Only Captain Tsorin can authorize you to come through here")
     else
	bubble(HERO1, "Tsorin sends his greetings to you. You are to let us pass through to the Oracle's Tower.")
	bubble(en, "I see that you are bearing his seal. Very well")
	bubble(en, "I'll tell the Lieutentant you're here")
	set_ent_script(en, "D10")
	wait_for_entity(en,en)
	set_progress(P_TALK_TSORIN, 2)
	set_ent_active(en, 0)
     end
  elseif (en == 5) then
     if (get_progress(P_TALK_TSORIN)==1) then
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
