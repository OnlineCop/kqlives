-- cave2 - "Pit in grotto north of Ekla"

function autoexec()
  return;
end


function postexec()
  if (get_progress(P_FELLINPIT) == 0) then
    bubble(HERO1, "Ouch!");
    bubble(HERO1, "This is no ordinary gopher hole!");
    set_progress(P_FELLINPIT, 1);
  end
end


function zone_handler(zn)
  if (zn == 1) then
    bubble(HERO1, "There's some kind of barrier here. I can't get past it.");
    set_progress(P_FELLINPIT, 2);

  elseif (zn == 2) then
    chest(16, I_MHERB, 1);

  elseif (zn == 3) then
    if (get_progress(P_FELLINPIT) == 2) then
      bubble(255, "Hey, are you alright down there?");
      bubble(HERO1, "Yeah... I'm okay.");
      bubble(255, "Hang on, I've got a rope here.");
      change_map("grotto", 17, 14, 17, 14);
    end

  end
end


function entity_handler(en)
  return;
end
