-- cave2 - pit in grotto north of Ekla

function autoexec()
  return
end

function postexec()
  if (get_progress(11) == 0) then
    bubble(200,"Ouch!","","","");
    bubble(200,"This is no ordinary",
                 "gopher hole!","","");
    set_progress(11,1);
  end
end

function zone_handler(zn)  
  if (zn == 1) then
    bubble(200,"There's some kind of barrier",
                 "here. I can't get past it.","","");
    set_progress(11,2);
  
  elseif (zn == 2) then
    chest(16,103,1);
  
  elseif (zn == 3) then
    if (get_progress(11) == 2) then
      bubble(255,"Hey, are you all",
                 "right down there?","","");
      bubble(200,"Yeah... I'm okay.","","","");
      bubble(255,"Hang on, I've got a rope here.","","","");
      change_map("grotto",17,14,17,14);
    end
  
  end
end

function entity_handler(en)
  return
end
