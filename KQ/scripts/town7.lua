-- town7 - "Pulcannen"

function autoexec()
  refresh();
end


function refresh()
  if (get_treasure(72) > 0) then
    set_obs(42, 18, 0);
  end
end


function postexec()
  return;
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 293, 82, 293, 82);

  elseif (zn == 2) then
    door_in(74, 32, 69, 18, 97, 35);

  elseif (zn == 3) then
    door_in(83, 32, 69, 18, 97, 35);

  elseif (zn == 4) then
    door_in(92, 32, 69, 18, 97, 35);

  elseif (zn == 5) then
    door_in(75, 46, 69, 36, 81, 49);

  elseif (zn == 6) then
    door_in(89, 46, 83, 36, 95, 49);

  elseif (zn == 7) then
    door_out(32, 33);

  elseif (zn == 8) then
    door_out(37, 33);

  elseif (zn == 9) then
    door_out(42, 33);

  elseif (zn == 10) then
    door_out(31, 21);

  elseif (zn == 11) then
    door_out(49, 43);

  elseif (zn == 12) then
    bubble(HERO1, "Locked.");

  elseif (zn == 13) then
    shop(19);

  elseif (zn == 14) then
    shop(20);

  elseif (zn == 15) then
    shop(21);

  elseif (zn == 16) then
    inn("Pulcannen Inn", 200, 1);

  elseif (zn == 17) then
    view_range(1, 69, 0, 97, 17);
    warp(79, 4, 8);

  elseif (zn == 18) then
    view_range(1, 69, 0, 97, 17);
    warp(87, 4, 8);

  elseif (zn == 19) then
    view_range(1, 69, 18, 97, 35);
    warp(79, 22, 8);

  elseif (zn == 20) then
    view_range(1, 69, 18, 97, 35);
    warp(87, 22, 8);

  elseif (zn == 21) then
    chest(71, I_SALVE, 1);

  elseif (zn == 22) then
    chest(72, I_MPUP, 1);
    refresh();

  elseif (zn == 23) then
    book_talk(party[0]);

  elseif (zn == 24) then
    touch_fire(party[0]);

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "You'll find great things here.");

  elseif (en == 1) then
    bubble(en, "Things here are rather pricey.");

  elseif (en == 2) then
    bubble(en, "It's quite peaceful here.", "I like it.");

  elseif (en == 3) then
    bubble(en, "I love the sea.");

  elseif (en == 4) then
    bubble(en, "Nothing exciting ever happens here.");

  elseif (en == 5) then
    bubble(en, "This town does not get a lot of visitors.");

  end
end
