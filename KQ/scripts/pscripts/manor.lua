

-- manor - "Nostik's manor southwest of Ekla"

--
function autoexec()
  view_range(1, 5, 4, 26, 22)
  if (get_progress(100) > 0) then
    set_progress(99, 3)
  end
  if (get_progress(99) == 0) then
    -- New game; Nostik has not explained the quest yet
    LOC_setup_newgame()
  elseif (get_progress(99) == 1) then
    -- You spoke to Nostik about the purpose of your mission
  elseif (get_progress(99) == 2) then
    -- You already talked to Hunert, but you have no recruits
    for a = 0, 7, 1 do
      set_ent_active(a, 0)
    end
  elseif (get_progress(99) == 3) then
    -- You have recruited at least 1 other party member
    LOC_at_table()
    -- Put Nostik to bed (he is old and feeble)
    place_ent(8, 9, 31)
    set_ent_facing(8, 0)
  end
end
function postexec()
  local en = 8
  if (get_progress(99) == 0) then
    rest(200)
    bubble(en, "Alright everyone, I welcome you. Let me get right to the major points of why you're here.")
    LOC_explain_mission(en)
    bubble(en, "For fear of going out and being spotted, I will stay here if you need anything.")
    bubble(en, "Good luck all of you.")
    -- TT: make everyone else walk out the door
    move_entity(0, 11, 20, 1)
    move_entity(1, 11, 20, 1)
    move_entity(2, 11, 20, 1)
    move_entity(3, 11, 20, 1)
    move_entity(4, 11, 20, 1)
    move_entity(5, 11, 20, 1)
    move_entity(6, 11, 20, 1)
    move_entity(7, 11, 20, 1)
    for a = 0, 7, 1 do
      set_ent_speed(a, 4)
    end
    wait_for_entity(0, 7)
    bubble(en, "When you are ready to go, talk to Hunert and he will get you started on your journey.")
  end
end
function zone_handler(zn)
  -- Front door
  if (zn == 1) then
    change_map("main", 107, 40, 107, 40)
  -- Stairs going up
  elseif (zn == 2) then
    view_range(1, 5, 24, 26, 42)
    warp(25, 28, 8)
  -- Stairs going down
  elseif (zn == 3) then
    view_range(1, 5, 4, 26, 22)
    warp(25, 8, 8)
  -- Doors, duh
  elseif (zn == 4) then
    bubble(200, "Locked.")
  -- Bookshelves
  elseif (zn == 5) then
    bubble(200, "Wow! This guy reads weird stuff.")
  -- In front of exit
  elseif (zn == 6) then
    local en = 9
    if (get_progress(99) == 0 or get_progress(99) == 1) then
      bubble(en, "Hey! Hold on!")
      -- Turn around, see who is yelling
      set_ent_script(200, "U1")
      wait_for_entity(200, 200)
      bubble(200, "Huh?")
      -- Butler running speed
      set_ent_speed(en, 5)
      -- Check location on map
      move_entity(en, get_ent_tilex(200), get_ent_tiley(200) - 1, 0)
      -- Process movement script
      wait_for_entity(en, en)
      set_ent_facing(en, 0)
      -- Butler normal speed
      set_ent_speed(en, 3)
      bubble(en, "It might be foolish to leave without hearing what I have to say.")
      bubble(en, "First, Nostik gives you this.")
      -- TT: Added the (en) so the text bubble correctly displays
      LOC_talk_butler(en)
    end
  -- Search fireplaces
  elseif (zn == 7) then
    touch_fire(party[0])
  end
end
function entity_handler(en)
  -- You are talking to other party members
  if (get_ent_id(en) == 0) then
    bubble(en, "I would be useful to you, since I can use Rage in battle.")
  elseif (get_ent_id(en) == 1) then
    bubble(en, "In battle, I can attack multiple targets at once if I'm equipped with the right weapon.")
  elseif (get_ent_id(en) == 2) then
    bubble(en, "I can infuse weapons with magic during battle.")
  elseif (get_ent_id(en) == 3) then
    bubble(en, "I notice that chanting a prayer during battle can heal your party or dispells the undead monsters.")
  elseif (get_ent_id(en) == 4) then
    bubble(en, "I can use my Boost ability to strengthen spells when I am attacking.")
  elseif (get_ent_id(en) == 5) then
    bubble(en, "I am very protective of my team members and will take a beating in their place.")
  elseif (get_ent_id(en) == 6) then
    bubble(en, "I'm a thief by trade. You might be surprised what you can steal from enemies!")
  elseif (get_ent_id(en) == 7) then
    bubble(en, "I have a very keen eye. Not even enemies can hide their weaknesses from me!")
  -- Nostik
  elseif (en == 8) then
    if (get_progress(99) == 0) then
      if (prompt(en, 2, 1, "Do you need me to explain it again?",
                           "  yes",
                           "  no") == 0) then
        LOC_explain_mission(en)
        bubble(en, "I hope this helps you have a better understanding of what's going on.")
      else
        bubble(200, "No, I think I get it. But why did you choose me?")
        bubble(en, "As you may have noticed, you're not the only one. We've been friends a long time, $0, and I know I can trust you with confidence.")
        bubble(200, "Oh. I thought you selected me because of my brains or because my magic skills were more finely honed than anyone else's...")
        bubble(en, "Heh-heh. You DO have certain skills which I'm counting on will help you out on your mission, but so do the others, so don't get a big head.")
        bubble(200, "Yea, yea. So why didn't you just get everybody into one big group and just let all of us go out and find this staff?")
        bubble(en, "Groups attract attention. If you see a group of three or more people wandering around, you begin to suspect something's up. That's exactly what you'd want to avoid, if you ever want to stay ahead of Malkaron and his minions.")
        bubble(200, "Oh yea, I hadn't thought of that.")
        set_progress(99, 1)
      end
      wait(50)
      bubble(en, "You should be going. Talk to Hunert before you go. He can help start you on your way.")
    elseif (get_progress(99) == 1) then
      bubble(en, "You should be going. Talk to Hunert before you go. He can help start you on your way.")
    elseif (get_progress(99) == 2) then
      bubble(en, "Good luck, $0.")
    elseif (get_progress(99) == 3) then
      bubble(en, "Zzz... zzz... zzz...")
    else
      bubble(en, "Mine aren't the only books on the Staff of Xenarum and other treasures.")
    end
  -- Butler Hunert
  elseif (en == 9) then
    if (get_progress(99) == 0 or get_progress(99) == 1) then
      bubble(en, "Ah yes, Master Nostik asked me to give you this.")
      LOC_talk_butler(en)
    elseif (get_progress(99) == 2) then
      bubble(en, "Books are an amazing source of knowledge. Nostik has spent many years writing his own.")
    elseif (get_progress(99) >= 3) then
      bubble(en, "Welcome back, $0. The others are here waiting for you.")
      bubble(en, "You can exchange your party members here.")
      -- PH, this is where your script comes in?
      select_manor()
      LOC_at_table()
    end
  end
end
function LOC_setup_newgame()
  local a
  -- Set up entities 0-7 in manor.map as your team members
  for a = 0, 7, 1 do
    set_ent_active(a, 1)
    set_ent_id(a, a)
    set_ent_obsmode(a, 1)
    set_ent_chrx(a, 255)
  end
  -- Remove the NPC that looks like you, from the map
  set_ent_active(get_pidx(0), 0)
  -- Set the REAL hero in the old NPCs place
  place_ent(200, get_ent_tilex(get_pidx(0)),
                   get_ent_tiley(get_pidx(0)))
  -- Set your facing direction
  set_ent_facing(200, get_ent_facing(get_pidx(0)))
  -- Center map on your character coords
  calc_viewport(1)
end
function LOC_explain_mission(en)
    local a
    a = prompt(en, 3, 1, "Do you want the long version or the short version?",
                         "  long",
                         "  short",
                         "  neither")
    if (a == 0) then
      -- long
      bubble(en, "The world is in an upheaval right now. Malkaron is a military general who, quite suddenly, became unstoppable. His forces were practically invincible. The world was plagued with his destruction. Then just a few months ago, Malkaron withdrew and all this frenzy stopped. He and his armies seemed to have just vanished.")
      bubble(en, "Peace only lasted only until now, when monsters have begun to appear everywhere. They kill livestock, attack towns and ransack villages. They are looking for something.")
      wait(75)
      bubble(en, "The fact is, I had, until recently, been an advisor to him in his army. He is furious with me, however, and he's sent out scouts to find me.")
      bubble(en, "Malkaron had a magical staff which gave him and his armies total invincibility. It turned the weapons and armour of his armies practically indestructible, merely by being in close proximity to the staff. That is why his army was unstoppable.")
      bubble(en, "Naturally, he kept this a well-guarded secret, but since he carried this staff with him everywhere his armies went, I began to piece the clues together. Malkaron became suspicious, and sent me to the dungeon.")
      LOC_explain_mission2(en)
    elseif (a == 1) then
      -- short
      bubble(en, "Malkaron is a general whose armies became invincible. They practically tore the world apart until just recently.")
      bubble(en, "Their forces unexpectedly withdrew a few months ago and it's been peaceful until now.")
      bubble(en, "It appears that there are monsters springing up everywhere, looking for something.")
      bubble(en, "Malkaron had a magical staff that made his armies' weapons and armour practically indestructible by mere proximity to it.")
      bubble(en, "He found out that I knew about this 'little secret', and threw me into the dungeon.")
      LOC_explain_mission2(en)
    else
      bubble(en, "Fine, I'll tell you the minimal amount. Malkaron captured me.")
    end
    bubble(en, "I escaped by use of my magic, and now I am here.")
end
function LOC_explain_mission2(en)
    wait(75)
    bubble(en, "This is where you come in. I believe that Malkaron has lost the staff and is now looking for it. Those monsters are his scouts.")
    bubble(en, "If this is true, I need your help to find it before he does.")
    bubble(en, "Each of you have a different skill that I believe will be very beneficial to your search.")
    bubble(en, "You will need to stop the monsters. They are Malkaron's eyes and ears, and if they find the staff before you do, we may be in trouble again.")
    wait(50)
end
function LOC_talk_butler(en)
  drawmap()
  screen_dump()
  sfx(6)
  msg("You've acquired 200 gp!", 255, 0)
  set_gp(get_gp() + 200)
  drawmap()
  screen_dump()
  bubble(en, "Since there are so many monsters wandering around, you may be attacked at random.")
  bubble(200, "You mean monsters will go on a wild rampage and just start gouging me for no reason?")
  bubble(en, "Well, yes. That's a good way of putting it.")
  bubble(200, "That will sure make it hard to sleep at night.")
  bubble(en, "Heh heh. Maybe so. Try sleeping in a town or village inn. Monsters avoid populated places for some reason.")
  bubble(en, "Here, I'll put a monster repellant on you that will last long enough for you to make it into town.")
  msg("Hunert sprinkles some strong-scented oils on you.", 255, 0)
  bubble(200, "Whew! That reeks! Why does it have to smell so bad?")
  msg("Hunert shrugs", 255, 0)
  bubble(en, "Hopefully, it will give you a chance to buy some better weapons and armour.")
  bubble(200, "Well, thank you for the information.")
  set_progress(99, 2)
end
-- Decide who should be sitting around the table
function LOC_at_table()
  local id, a
  for a = 0, 7 do
    -- You have not recruited this person into your team
    id = get_progress(a + 91) - 1
    if (id == get_pidx(0)) then
      id = -1
    end
    if (get_numchrs() == 2 and id == get_pidx(1)) then
      id = -1
    end
    if (id < 0) then
      -- Remove entity from the map
      set_ent_active(a, 0)
    else
      -- Place around the table
      set_ent_active(a, 1)
      set_ent_chrx(a, 255)
      set_ent_id(a, id)
      set_ent_obsmode(a, 1)
    end
  end
end
