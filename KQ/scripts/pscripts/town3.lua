

-- town3 - "Andra"

--
function autoexec()
  if (get_progress(108) == 4) then
    --
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
    bubble(200, "Locked.")
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
    chest(14, 115, 1)
  elseif (zn == 24) then
    book_talk(party[0])
  elseif (zn == 25) then
    chest(13, 46, 1)
  elseif (zn == 26) then
    bubble(200, "Various books about magic.")
  elseif (zn == 27) then
    bubble(200, "The art of battle-magic.")
  elseif (zn == 28) then
    bubble(200, "How magic can make you rich!")
  elseif (zn == 29) then
    bubble(200, "Ten reasons why you should never call a wizard 'pencil-neck'.")
  elseif (zn == 30) then
    touch_fire(party[0])
  elseif (zn == 31) then
    bubble(200, "Inns always have boring books.")
  elseif (zn == 32) then
    chest(99, 146, 1)
    refresh()
  elseif (zn == 33) then
    bubble(200, "The barrel is empty.")
  elseif (zn == 34) then
    chest(100, 0, 200)
    refresh()
  end
end
function entity_handler(en)
  if (en == 0) then
    bubble(en, "This is the town of Andra.")
  elseif (en == 1) then
    if (get_progress(108) == 0) then
      bubble(en, "Tsorin:", "Thank you for visiting our town. What do you need?")
      bubble(200, "I'm not sure. To get through the pass to the south, I guess.")
      bubble(en, "I'm afraid I cannot allow that. There is a civil upheaval in the Goblin Lands and so cannot allow anyone to pass.")
      if (get_numchrs() == 1) then
        bubble(200, "I'm pretty sure I need to get through there.")
      else
        bubble(200, "We're pretty sure we need to get through there.")
      end
      msg("Tsorin eyes you warily.", 255, 0)
      bubble(en, "Well, I have to know if I can trust you first. Here, take this note to Derig.")
      msg("Tsorin hands you an envelope with his seal on it.", 18, 0)
      bubble(en, "He lives back in Ekla, and if he says you can pass, I shall let you pass.")
      set_progress(108, 1)
    elseif (get_progress(108) == 1) then
      bubble(en, "If you have trouble finding Derig, ask around town. Someone's bound to know where he's wandered off to.")
    elseif (get_progress(108) == 2) then
      msg("You hand the note to Tsorin.", 18, 0)
      bubble(en, "Hmm, I see. Since Derig says you are trustworthy, I can let you pass through the fort.")
      if (get_numchrs() == 1) then
        bubble(200, "Can you tell me why I need this security clearance?")
      else
        bubble(200, "Can you tell us why we need this security clearance?")
      end
      bubble(en, "The Oracle's Statue was stolen from a village shrine south of here. It caused such problems that civil war has ensued. As a safety to our citizens, we have closed the border to try to capture the thief or thieves.")
      if (get_numchrs() == 1) then
        bubble(200, "If you want, I can help find this missing statue.")
      else
        bubble(200, "If you want, we can help you find this missing statue.")
      end
      bubble(en, "That would be wonderful! Find the Oracle to the south and ask her what she would like you to do.")
      bubble(en, "Present this Seal to the guards, and tell them that you're acting with my authority.")
      msg("Tsorin's Seal procured!", 25, 0)
      bubble(en, "I'm fear I cannot send anyone to protect you - you must take the utmost care.")
      set_progress(108, 3)
    elseif (get_progress(108) == 3) then
      bubble(en, "You can find the Oracle to the south. She will help you find this missing statue.")
    elseif (get_progress(108) == 4) then
      bubble(en, "Thank you for all you've done.")
    end -- 108
  elseif (en == 2) then
    bubble(en, "I wish the weapon shop sold slingshots.")
  elseif (en == 3 or en == 4) then
    bubble(en, "We're guarding against goblins.")
  elseif (en == 8) then
    bubble(en, "Caffeine, caffeine, caffeine, caffeine...")
  elseif (en == 9) then
    if (get_progress(17) == 0) then
      bubble(en, "To the north of here is a temple. It is nearly impenetrable from the outside, even to Malkaron's armies.")
      bubble(en, "However, monsters have somehow appeared INSIDE the temple. They need help in any way possible.")
      bubble(200, "I can probably help. What can I do?")
      bubble(en, "I hear that the monsters are coming through a portal in the caves under the temple. Seal it up to stop the monsters.")
      bubble(200, "How do I seal the portal?")
      bubble(en, "I don't know. You should ask someone in the temple.")
    elseif (get_progress(17) == 1) then
      bubble(en, "Use that Goblin Item to seal the portal in the temple.")
    else
      bubble(en, "Good work in the temple! We thank you most graciously!")
    end
  end
end
