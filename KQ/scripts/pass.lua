-- "pass - Karnok's pass, between Sunarin and Pulcannen"

function autoexec()
  refresh();
end
  
function refresh()
  if (get_progress(P_SAVEBREANNE) > 0) then
    place_ent(0,get_ent_tilex(0)-1,get_ent_tiley(0));
    set_ent_facing(0,3);
    place_ent(1,get_ent_tilex(1)+1,get_ent_tiley(1));
    set_ent_facing(1,2);
  end
if (get_progress(P_CAVEKEY)>0) then
 place_ent(2,152,12);
end
  if (get_treasure(73) == 1) then
    set_mtile(74,47,39);
  end
  if (get_treasure(74) == 1) then
    set_mtile(75,47,39);
  end
  if (get_progress(P_PASSDOOR2) == 1) then
    set_mtile(78,38,57);
    set_mtile(78,39,33);
    set_obs(78,38,0);
  end
  if (get_progress(P_PASSDOOR3) == 1) then
    set_mtile(106,35,57);
    set_mtile(106,36,33);
    set_obs(106,35,0);
  end
end

function postexec()
  return
end
  
function zone_handler(zn)
--  if (zn == 0) then
--    combat(50);

--  elseif (zn == 1) then
  if (zn == 1) then
    change_map("main",268,103,268,103);
  
  elseif (zn == 2) then
    change_map("main",271,100,271,100);
  
  elseif (zn == 3) then
    bubble(HERO1, "It only opens", "from the inside", "","");
--  if (get_progress(P_CAVEKEY) == 0) then
--    bubble(HERO1,"Locked.","","","");
--    return
--  else
--      sfx(26);
--    set_mtile(83,27,57);
--    set_mtile(83,28,33);
--    drawmap();
--    screen_dump();
--  end
--  change_map("cave5",35,4,0,0);
  
  elseif (zn == 4) then
    chest(73,I_VITSEED,1);
    refresh();
  
  elseif (zn == 5) then
    chest(74,I_ERUNE,1);
    refresh();
  
  elseif (zn == 6) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
      return
    else
      if (get_progress(P_PASSDOOR2) == 0) then
        set_progress(P_PASSDOOR2,1);
        sfx(26);
        refresh();
      end
    end
    change_map("cave5",15,147,0,0);
  
  elseif (zn == 7) then
    if (get_progress(P_CAVEKEY) == 0) then
      bubble(HERO1,"Locked.","","","");
      return
    else
      if (get_progress(P_PASSDOOR3) == 0) then
        set_progress(P_PASSDOOR3,1);
        sfx(26);
        refresh();
      end
    end
    change_map("cave5",84,147,0,0);
  elseif (zn==8) then -- door into cabin
   door_in(150,14,148,8,154,15);
  elseif (zn==9) then -- door out of cabin
   door_out(83,50);
  elseif (zn==10) then --treasure
  elseif (zn==11) then --treasure
  end
end

function miner( en)
if (get_progress(P_TALKRUFUS)==0) then
 bubble(en, "Howdy!");
 bubble(HERO1, "Hello. Is this your cabin?");
 bubble(en, "It sure is! My name's Rufus. I work in the abandoned mines in these parts.");
 bubble(en, "But I'll warn you now. There's something nasty hiding in them there mines.");
 bubble(HERO1,"Like what?");
 bubble(en, "I guess I've said enough...");
 set_progress(P_TALKRUFUS,1);
 if (get_progress(P_CAVEKEY)==0) then
  bubble(HERO1, "Don't worry. I was just passing through, anyway.");
  return
 end
elseif (get_progress(P_TALKRUFUS)==1) then
 bubble(HERO1, "Hello again");
 bubble(en, "Don't forget what I told you about the mines.");
 if (get_progress(P_CAVEKEY)==0) then
  bubble(HERO1, "I won't.");
  return
 end
end

if (get_progress(P_TALKRUFUS)<2) then 
 bubble(HERO1, "But, I really need to get in there.");
 set_progress(P_TALKRUFUS, 2);
 bubble(en, "We-e-ell,");
 bubble(en, "I reckon that you and me might be able to do a little business here.");
else
 bubble(en, "So, back for some more of the dynamite, are you?");
end
shop(23);
end

function entity_handler(en)  
  if (en == 0 or en == 1) then
    if (get_progress(P_SAVEBREANNE) == 0) then
      bubble(en,"This pass is reserved for",
                "use by caravans only.","","");
    else
      bubble(en,"You are free to use the pass.",
                "Just be careful.","","");
    end
  elseif (en==2) then -- cabin bloke
   miner(en);
  end
end
