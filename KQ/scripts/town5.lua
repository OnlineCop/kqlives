-- town5 - "Sunarin"
--/* 
-- This is where the entrance to the Embers Guild is located
-- It is also the start of Ayla's quest - robbing the palace in 
-- the inner city.
-- controlled by P_AYLA_QUEST:
-- 	0: Nothing done
-- 	1: Talked to the girl near the gates
-- 	2: Talked to the folks in the bar
-- 	3: Talked to both
--	4: Got the disguise and ready to go
--	5: Finished/given up
--*/

function autoexec()
  if (get_progress(P_TALKGELIK) ~= 2) then
    set_ent_active(7, 0);
  end
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_desc(0);
  end
  refresh();
end


function refresh()
  if (get_treasure(32) > 0) then
    set_obs(21, 27, 0);
  end
end


function postexec()
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_progress(P_FOUGHTGUILD, 3);
    set_progress(P_EMBERSKEY, 0);
    bubble(HERO1, "Oh no!");
    bubble(HERO1, "I must have dropped that key inside!");
    bubble(HERO1, "Oh well, I got what I came for.");
  end
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 247, 115, 247, 115);

  elseif (zn == 2) then
    bubble(HERO1, "Locked.");

  elseif (zn == 3) then
    door_in(65, 16, 61, 5, 77, 19);

  elseif (zn == 4) then
    door_in(73, 16, 61, 5, 77, 19);

  elseif (zn == 5) then
    door_in(65, 36, 61, 25, 75, 39);

  elseif (zn == 6) then
    door_in(71, 36, 61, 25, 75, 39);

  elseif (zn == 7) then
    door_out(24, 12);

  elseif (zn == 8) then
    door_out(28, 12);

  elseif (zn == 9) then
    door_out(37, 34);

  elseif (zn == 10) then
    door_out(42, 34);

  elseif (zn == 11) then
    inn("River's Way Inn", 70, 1);

  elseif (zn == 12) then
    shop(13);

  elseif (zn == 13) then
    shop(14);

  elseif (zn == 14) then
    bubble(HERO1, "Worst books ever.");

  elseif (zn == 15) then
    bubble(HERO1, "What language is this?");

  elseif (zn == 16) then
    chest(32, I_PRIESTESS, 1);
    refresh();

  elseif (zn == 17) then
    bubble(HERO1, "The gates are",
                 "closed and locked.");

  elseif (zn == 18) then
    if (get_progress(P_EMBERSKEY) == 0) then
      bubble(HERO1, "Locked.");
    else
      bubble(HERO1, "Key goes in...");
      change_map("guild", 0, 0, 0, 0);
    end

  elseif (zn == 19) then
    touch_fire(party[0]);

  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(0, "Do you like this shirt?");

  elseif (en == 1) then
    bubble(1, "I really like that guy's shirt.");

  elseif (en == 2) then
    bubble(2, "I have to stop bringing my wife with me.");

  elseif (en == 3) then
    bubble(3, "I must have had too many drinks.",
              "My husband is starting to look handsome.");

  elseif (en == 4) then
    bubble(4, "These two are weird... but at least I don't have to sit alone.");

  elseif (en == 5) then
    if party[0]==Ayla then
        LOC_ayla_mission()
    else
        bubble(5, "Those gates to the inner city never open. Strangers are not allowed in.");
    end
  elseif (en == 6) then
    bubble(6, "Welcome to Sunarin!",
              "Please feel free to leave at any time.");

  elseif (en == 7) then
    set_progress(P_TALKGELIK, 3);
    bubble(7, "Please... help me... I brought soldiers with me... but the Embers killed them. I barely escaped with my life!");
    bubble(HERO1, "Who are you? And who are the Embers?");
    bubble(7, "My name is Dungar. The Embers is the short name for the Thieves of the Ember Crest. They run this town.");
    bubble(7, "I came to get back something they stole from me...");
    bubble(HERO1, "The Opal Helmet?");
    bubble(7, "Why yes! The Opal Helmet.",
              "Who are you? You're not one of them are you?");
    bubble(HERO1, "No. My name is $0. I am on a quest, and I need to borrow that helmet to traverse the underwater passage.");
    bubble(7, "I see. Well, if you can get it back, I will certainly let you use it.");
    bubble(HERO1, "Well, how do we get it back?");
    bubble(7, "We can discuss that back at my place. It is not safe here.");
    bubble(HERO1, "Okay.");
    set_progress(P_EMBERSKEY, 1);
    change_map("estate", 57, 17, 57, 17);

  end
end

function LOC_ayla_mission()
local pp
pp=get_progress(P_AYLA_QUEST)
if pp==0 then
    bubble(5, "Those gates to the inner city never open. Strangers are not allowed in.");
    bubble(HERO1, "I suppose ... there must be some valuable stuff in there?")
    bubble(5, "Oh, yes, this town should be prosperous, but our ruler takes all our gold in taxes and keeps it walled up in there.")
    bubble(5, "That's why everyone here is so surly.")
    bubble(HERO1, "You said strangers weren't allowed in. Can't locals come and go as they please?")
    bubble(5, "Only servants, they ... hey, why are you so interested?")
    bubble(HERO1, "No reason. I'm just passing through this town and I was curious, that's all")
    thought(HERO1, "Hmm... curious about the riches they've got in there. Surely the mission can wait for a while.")
elseif pp==1 then
    bubble(5, "You know, I'll call the guards if I see you round here again.")
elseif pp==2 then
    
elseif pp==3 then
elseif pp==4 then
else 
end
end
