-- cave5 - The new Opal cave in the mountain pass

function autoexec()
end

function refresh()

end

function postexec()
  
end

function destroy1()
 set_ent_script(HERO1, "L3D2L10");
 spd=get_ent_speed(HERO1);
 set_ent_speed(HERO1, 5);
 if (get_numchrs()==1) then
  wait_for_entity(HERO1, HERO1);
 else
  set_ent_script(HERO2, "L3D2L10");
  wait_for_entity(HERO1, HERO2);
 end
 set_ent_speed(HERO1, spd);
 copy_tile_all(110,80,72,94,5,5);
 bubble(HERO1, "Whew!","","","");
end

function destroy2()
copy_tile_all(110,68,26,74,15,12)
end
function destroy3()
copy_tile_all(110,85,29,85,15,12)
end
--function destroy4()
-- copy_tile_all(118,75,0,0,0,0)
--end

function zone_handler(zn)
if (zn==1) then 
 destroy1();
elseif (zn==2) then
 destroy2();
elseif (zn==3) then
 destroy3();
end
end

function entity_handler(en)
  return
end


