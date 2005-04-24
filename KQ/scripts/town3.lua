-- town3 - "Andra"

-- /*
-- {
-- P_GOBLINITEM: Possession of Goblin Item (which helps seal Monster Portal)
--   0 - Do not have it
--   1 - Got it
--   2 - Returned it to Oracle
--
-- P_PORTAL2GONE: Whether the portal in the temple is still working
--   0 - Still letting monsters through
--   1 - The Portal is sealed shut
--
-- P_TALK_TSORIN: Have you spoken to Tsorin yet (and got his seal)
--   0 - You haven't spoken to him yet.
--   1 - You have, and he gave you the seal.
--   2 - You've shown the seal to the guards at the fort.
--
-- In this town, we are going to add the 3rd PM, which will include the sub-
-- plot in the temple.
--
-- }
-- */

function autoexec()
  if (get_progress(P_TALKDERIG) > 2) then
    -- // Deactivate the Tsorin character
    set_ent_active(1, 0)
  end
  refresh()
end


function refresh()
  if (get_treasure(12) == 1) then
    set_obs(12, 41, 0)
    set_zone(12, 41, 0)
  end

  if (get_treasure(13) == 1) then
    set_zone(100, 22, 0)
  end

  if (get_treasure(14) == 1) then
    set_zone(16, 37, 0)
  end

  if (get_treasure(100) == 1) then
    set_zone(26, 36)
  end

  if (get_treasure(99) == 1) then
    set_mtile(90, 24, 0)
    set_zone(90, 24, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 261, 30, 216, 30)

  elseif (zn == 2) then
    door_in(68, 15, 58, 6, 78, 18)

  elseif (zn == 3) then
    door_out(22, 17)

  elseif (zn == 4) then
    door_in(86, 15, 79, 6, 93, 18)

  elseif (zn == 5) then
    door_out(39, 17)

  elseif (zn == 6) then
    door_in(62, 25, 58, 19, 66, 28)

  elseif (zn == 7) then
    door_out(13, 24)

  elseif (zn == 8) then
    door_in(62, 38, 58, 29, 66, 41)

  elseif (zn == 9) then
    door_out(19, 39)

  elseif (zn == 10) then
    door_in(97, 25, 93, 19, 101, 28)

  elseif (zn == 11) then
    door_out(41, 31)

  elseif (zn == 12) then
    door_in(75, 38, 67, 29, 83, 41)

  elseif (zn == 13) then
    door_out(40, 45)

  elseif (zn == 14) then
    bubble(HERO1, "Locked.")

  elseif (zn == 15) then
    inn("Riverside Inn", 35, 1)

  elseif (zn == 16) then
    shop(6)

  elseif (zn == 17) then
    shop(7)

  elseif (zn == 18) then
    shop(8)

  elseif (zn == 19) then
    shop(9)

  elseif (zn == 20) then
    door_in(62, 51, 58, 42, 66, 54)

  elseif (zn == 21) then
    door_out(19, 52)

  elseif (zn == 22) then
    chest(12, 0, 1)
    refresh()

  elseif (zn == 23) then
    chest(14, I_NPOULTICE, 1)

  elseif (zn == 24) then
    book_talk(party[0])

  elseif (zn == 25) then
    chest(13, I_CAP2, 1)

  elseif (zn == 26) then
    bubble(HERO1, "Various books about magic.")

  elseif (zn == 27) then
    bubble(HERO1, "The art of battle-magic.")

  elseif (zn == 28) then
    bubble(HERO1, "How magic can make you rich!")

  elseif (zn == 29) then
    bubble(HERO1, "Ten reasons why you should never call a wizard 'pencil-neck'.")

  elseif (zn == 30) then
    touch_fire(party[0])

  elseif (zn == 31) then
    bubble(HERO1, "Inns always have boring books.")

  elseif (zn == 32) then
    chest(99, I_B_SLEEPALL, 1)
    refresh()

  elseif (zn == 33) then
    bubble(HERO1, "The barrel is empty.")

  elseif (zn == 34) then
    chest(100, 0, 200)
    refresh()

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "This is the town of Andra.")

  elseif (en == 1) then
    if (get_progress(P_TALK_TSORIN) == 0) then
      bubble(en, "Tsorin:", "Thank you for visiting our town. May I ask of your help?")
      bubble(HERO1, "What do you need?")
      bubble(en, "To the north of here is a temple. It is nearly impenetrable from the outside, even to Malkaron's armies.")
      bubble(en, "However, monsters have somehow appeared INSIDE the temple. We need help in any way possible.")
      bubble(HERO, "Sure, I can help. What should I do?")
      bubble(en, "I heard that the monsters are coming through a portal in the cave under the temple. Seal it up and that should take care of everything.")
      bubble(HERO1, "How do I seal the portal?")
      bubble(en, "South of here, in the land of the Goblins, there is one who is called the Oracle. She lives in a tower and can help you find a way to seal it.")
      bubble(en, "I should warn you that there is unrest in Goblin territory right now. My men have orders to turn everyone back a the fort.")
      bubble(HERO1, "How can we get to the Oracle's Tower, then?")
      bubble(en, "Present this Seal to the guards, and tell them that you're acting with my authority.")
      msg("Tsorin's Seal procured!", 255, 0)
      bubble(en, "I can't send anyone to protect you - you must take the utmost care.")
      set_progress(P_TALK_TSORIN, 1)
    elseif (get_progress(P_TALK_TSORIN) == 1) then
      bubble(en, "The guards at the Fort will give you directions to the Oracle.")
    else
      bubble(en, "You can find the Oracle to the south. She will help you seal the portal in the temple.")
    end

    -- TT: The Oracle will instruct you to get the GoblinItem, which works like the StaffOfCancellation, but for only this portal.
    if (get_progress(P_GOBLINITEM) == 0) then
      bubble(en, "Any luck at the temple yet?")
    end

  elseif (en == 2) then
    bubble(en, "I wish the weapon shop sold slingshots.")

  elseif (en == 3 or en == 4) then
    bubble(en, "We're guarding against goblins.")

  elseif (en == 8) then
    bubble(en, "Caffeine, caffeine, caffeine, caffeine...")

  end
end

