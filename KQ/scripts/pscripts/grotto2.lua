-- grotto2 - second, larger part of grotto north of Ekla

function autoexec()
  refresh();
end

function refresh()
  if (get_progress(37) > 0) then
    set_btile(20,23,515);
  end
  if (get_treasure(48) > 0) then
    set_btile(17,52,517);
  end
  if (get_treasure(49) > 0) then
    set_btile(18,47,517);
  end
end
  
function postexec()
  return
end

function zone_handler(zn)
  if (zn == 0) then
    combat(24);
  
  elseif (zn == 1) then
    change_map("grotto",11,13,11,13);
  
  elseif (zn == 2) then
    bubble(200,"This door is stuck tight.","","","");
  
  elseif (zn == 3) then
    if (get_progress(37) == 0) then
      set_progress(37,1);
      sfx(5);
      msg("Rod of Cancellation procured",255,0);
      refresh();
    end
  
  elseif (zn == 4) then
    chest(48,103,1);
    refresh();
  
  elseif (zn == 5) then
    chest(49,50,1);
    refresh();

  end
end

function entity_handler(en)
  return
end
