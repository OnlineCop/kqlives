

-- -*- lua -*-
function ch1()
set_progress(64,1);
set_progress(71,1);
set_progress(87,2);
change_map("pass",83,50,0,0);
end
function ch2()
set_progress(89, 0);
change_map("cave5", 35,6,0,0);
end
function ch3()
if get_progress(505)==0 then
-- change_map("para",0,0,0,0)
end
 bubble(200, "Map mode is now "..get_progress(505))
 set_map_mode(get_progress(505))
 set_progress(505, get_progress(505)+1)
end
function ch4()
change_map("cave3b",0,0,0,0);
end
function ch5()
   set_party_lvl(get_pidx(0), 49)
   set_party_mhp(get_pidx(0), 900)
   set_party_lvl(get_pidx(1), 49)
   set_party_mhp(get_pidx(1), 900)
   set_progress(59, 7)
   set_progress(50, 0)
   set_progress(60, 0)
   change_map("coliseum", 0,0,0,0)
end
function ch6()
set_progress(88, 0);
set_progress(9, 0);
change_map("grotto",0,0,0,0);
end
function ch7()
   change_map("shrine", 20,70,0,0)
end
function ch8()
 msg "How about a very long text message. Will it wrap or just appear as one long crappy line? I wonder."
end
function cheat()
set_progress(38, 1)
   change_map("town2", 20, 20, 0, 0)
end

function rtmomt()
--set_progress(85, 0);
--change_map("cave5", 32,30,128,128);
--set_progress(64,1);
--set_progress(71,1);
--set_progress(86, 6);
--change_map("cave5",0,0,0,0);
--combat(56);
--bubble(200, "$0 ",get_party_name(get_pidx(0)));
end
function ch9()
local s
-- msg(Corin.name .. " is at ("..Corin.tilex .. "," .. Corin.tiley .. ")")
 s="There are "
   for i,v in pairs(entity) do
      s=s.." "..(v.name or ("#"..v.eid))..(v.id or "")
  end
    log(s)
end
