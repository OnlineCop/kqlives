

-- starting - "Various pieces of heroes' homes back in Antoria"

--







function autoexec()
  set_desc(0)
  return
end


function postexec()
  if (get_progress(1997) == 1) then
    LOC_choose_hero()
    return
  end

  set_autoparty(1)
  set_vfollow(1)

  -- TT: This makes it a hundred times faster to update scripts and see how
  -- it will appear in real time. Just comment out everything that you do not
  -- want to sit through every time you recompile the script.

  LOC_storyline(0)
  LOC_storyline(1)
  LOC_storyline(2)
  LOC_storyline(3)
  LOC_storyline(4)
  LOC_storyline(5)
  LOC_storyline(6)
  LOC_storyline(7)

  LOC_meet_nostik()

  LOC_choose_hero()
end


function zone_handler(zn)
  return
end


function entity_handler(en)
  return
end


function LOC_meet_nostik()
  local a

  view_range(1, 116, 12, 141, 31)
  screen_dump()

  set_ent_active(200, 0)

  -- Put a bunch of people around Hunert the butler
  place_ent(3, 127, 24)
  set_ent_facing(3, 3)
  place_ent(4, 133, 24)
  set_ent_facing(4, 2)
  place_ent(5, 127, 25)
  set_ent_facing(5, 3)
  place_ent(6, 133, 25)
  set_ent_facing(6, 2)
  place_ent(7, 128, 26)
  set_ent_facing(7, 1)
  place_ent(8, 132, 26)
  set_ent_facing(8, 1)
  place_ent(9, 129, 27)
  set_ent_facing(9, 1)
  place_ent(10, 131, 27)
  set_ent_facing(10, 1)

  for a = 0, 7, 1 do
    set_ent_id(3 + a, a)
    set_ent_active(3 + a, 1)
    set_ent_movemode(3 + a, 0)
  end
  set_noe(get_noe() + 8)
  warp(130, 24, 4)
  set_desc(1)

  bubble(0, "I would like to thank you all for coming.")
  bubble(0, "I appreciate all those willing to help. My master has summoned eight of you. Are you all accounted for?")

  set_ent_script(0, "W25F2W25F0W25F3W25F0W30")
  wait_for_entity(0, 0)

  bubble(0, "Excellent. If everyone is ready, I will take you to meet master Nostik...")
  wait(50)

  set_autoparty(1)
end

function LOC_choose_hero()
--
-- TT add:
  local ptr = 7
  add_chr(ptr)
  set_all_equip(ptr, 20, 0, 0, 60, 0, 0)
  change_map("manor", 0, 0, 0, 0)
end
function LOC_storyline(en)
  local a, b
  add_chr(en)
  if (en == 0) then
    set_ent_facing(200, 1)
    view_range(1, 8, 10, 22, 27)
    warp(10, 16, 16)
    set_ent_speed(200, 4)
    wait(90)
    for a = 0, 1, 1 do
      set_ftile(11, 17, 537)
      wait(5)
      set_ftile(11, 17, 538)
      wait(5)
      set_ftile(11, 17, 539)
      wait(5)
      set_ftile(11, 17, 538)
      wait(5)
    end
    set_ftile(11, 17, 537)
    wait(10)
    set_ftile(11, 17, 0)
    set_ent_script(200, "W40D1")
    wait_for_entity(200, 200)
    bubble(200, "What the..? What was that?")
    wait(25)
    set_ftile(11, 17, 311)
    set_ent_script(200, "W25F3W25")
    wait_for_entity(200, 200)
    bubble(200, "Hey, a note!")
    set_ftile(11, 17, 0)
    bubble(200, "The note is addressed to me!")
    a = get_vx()
    b = get_vy()
    move_camera(a + 64, b + 48, 1)
    thought(200, "`$0, I am an old friend.",
                   "This note is intended for your   ",
                   "eyes only. Haste and discretion  ",
                   "are of the utmost importance.    ")
    thought(200, "I implore you to travel to Reisha",
                   "Mountain with all possible speed.",
                   "There, a man shall be waiting for",
                   "your arrival. I beg that you come")
    thought(200, "at once. Tell no one, as both my ",
                   "life and yours will be in danger.",
                   "Please trust me.'                ",
                   "  -Anonymous                     ")
    move_camera(a, b, 1)
    bubble(200, "`An old friend?' `Tell no one?' This looks really fishy. And strangely important. I'd better check this out.")
    set_ent_speed(200, 5)
    set_ent_script(200, "D6R4D4")
    wait_for_entity(200, 200)
    sfx(25)
  elseif (en == 1) then
    view_range(1, 0, 31, 41, 59)
    warp(27, 43, 4)
    set_ent_speed(200, 4)
    set_ent_script(200, "L17")
    wait_for_entity(200, 200)
    for a = 0, 1, 1 do
      set_ftile(12, 41, 537)
      wait(10)
      set_ftile(12, 41, 538)
      wait(10)
      set_ftile(12, 41, 539)
      wait(10)
      set_ftile(12, 41, 538)
      wait(10)
    end
    set_ftile(12, 41, 537)
    wait(20)
    set_ftile(12, 41, 0)
    bubble(200, "..huh?")
    set_ent_script(200, "R2F1W30")
    wait_for_entity(200, 200)
    bubble(200, "$0:",
                  "What was that?")
    set_ftile(12, 41, 310)
    wait(30)
    bubble(200, "$0:",
                  "That wasn't there a second ago!")
    set_ent_script(200, "U1")
    wait_for_entity(200, 200)
    bubble(200, "Wow, what a strange request.")
    bubble(200, "On the other hand, maybe there's something here that I can use to help Mom and Dad.")
    bubble(200, "I'll have to let them know right away!")
    set_ent_speed(200, 5)
    set_ftile(12, 41, 0)
    set_ent_script(200, "W25D1R15")
    wait_for_entity(200, 200)
  elseif (en == 2) then
    view_range(1, 26, 10, 49, 35)
    warp(37, 27, 4)
    set_ent_speed(200, 4)
    set_ent_script(200, "U7")
    wait_for_entity(200, 200)
    bubble(200, "$0:",
                  "Oh, no. I hope this isn't another eviction notice.")
    wait(25)
    set_mtile(37, 19, 0)
    wait(50)
    bubble(200, "Hey now, what's this?")
    wait(50)
    set_ent_facing(200, 0)
    wait(50)
    bubble(200, "Hmm... I don't like the sound of this one bit. I wonder if I even know who this anonymous fellow is?")
    bubble(200, "Well, I'd better get packed. It's a long way to Reisha Mountain.")
    set_ent_speed(200, 5)
    set_ent_script(200, "D7")
    wait_for_entity(200, 200)
  elseif (en == 3) then
    view_range(1, 50, 0, 64, 16)
    warp(57, 15, 4)
    set_ent_speed(200, 4)
    set_ent_script(200, "U7")
    wait_for_entity(200, 200)
    bubble(200, "$0:",
                  "You wanted to see me sir?")
    bubble(1, "Yes $0. It is time for you to leave the temple and discover the world. I have found the perfect way for you to do that.")
    bubble(200, "Excellent sir. What am I to do?")
    bubble(1, "A note addressed to you has mysteriously appeared on the door of your private chambers. It was brought to me.")
    bubble(1, "Read over this and tell me what you may know about the sender.")
    set_ftile(57, 6, 311)
    set_ent_script(200, "U1W50")
    wait_for_entity(200, 200)
    set_ftile(57, 6, 0)
    wait(50)
    set_ent_facing(200, 0)
    wait(100)
    set_ent_facing(200, 1)
    wait(50)
    bubble(200, "I cannot say, sir. I know no one who lives at Reisha Mountain.")
    bubble(200, "My knowledge of geography dictates that it is in a secluded mountain range, but that is all.")
    bubble(1, "Very well, $0. I am giving you an opportunity to investigate this quest.")
    bubble(1, "Among other things, this will test your training as well as get you out on a little adventure.")
    bubble(200, "I would be honored sir. Where do I go first?")
    bubble(1, "Proceed directly to Reisha Mountain. Let's not keep our mystery host waiting.")
    bubble(200, "I'm on my way.")
    bubble(1, "Fare thee well then, $0.")
    set_ent_script(200, "D5")
    wait_for_entity(200, 200)
  elseif (en == 4) then
    set_ent_facing(200, 1)
    view_range(1, 37, 29, 70, 59)
    warp(53, 58, 16)
    set_ent_speed(200, 5)
    set_ent_script(200, "U10")
    wait_for_entity(200, 200)
    set_ent_speed(200, 4)
    set_ent_script(200, "F2W30F1W30F3W50U2R1")
    wait_for_entity(200, 200)
    bubble(200, "$0:",
                  "Come out, come out, wherever you are, Nella!")
    set_ent_script(200, "R1U1W30F2W30F1W30F3W30")
    wait_for_entity(200, 200)
    bubble(200, "Nella, YOU ARE SO DEAD MEAT WHEN I FIND YOU!")
    set_ftile(55, 38, 537)
    wait(10)
    set_ftile(55, 38, 538)
    wait(10)
    set_ftile(55, 38, 539)
    wait(10)
    set_ftile(55, 38, 538)
    wait(10)
    set_ftile(55, 38, 537)
    wait(10)
    set_ent_script(200, "U1W50")
    wait_for_entity(200, 200)
    set_ftile(55, 38, 310)
    wait(50)
    bubble(200, "Wait a minute...")
    set_ent_script(200, "U5W75")
    wait_for_entity(200, 200)
    bubble(200, "Who put this letter here? Wait a second... I've heard of this `Reisha Mountain' before...")
    set_ftile(55, 38, 0)
    bubble(200, "I've got to take a look into this right away!")
    set_ent_speed(200, 5)
    set_ent_script(200, "D7L2D13W100K")
    wait_for_entity(200, 200)
    set_ent_id(1, 255)
    set_ent_chrx(1, 8)
    set_ent_speed(1, 3)
    set_ent_movemode(1, 2)
    set_ent_tilex(1, 48)
    set_ent_tiley(1, 54)
    set_ent_script(1, "R2F0W50")
    wait_for_entity(1, 1)
    bubble(1, "$0? Hello?")
  elseif (en == 5) then
    view_range(1, 67, 20, 98, 41)
    warp(88, 29, 4)
    set_ent_speed(200, 4)
    set_ent_script(200, "L9")
    wait_for_entity(200, 200)
    bubble(200, "$0:",
                  "I was told you had a new mission for me, sir.")
    bubble(2, "Yes $0. A message from Military Intelligence came for you today. You are to go to Reisha Mountain.")
    bubble(200, "Yes, sir. What are my orders?")
    bubble(2, "This is the letter which we received.")
    set_ent_script(200, "D1L2F1W60R2U1F2W30")
    wait_for_entity(200, 200)
    bubble(200, "Understood, sir.")
    bubble(2, "Once you find out what this is all about, report back to us if there is a threat of another invasion.")
    bubble(2, "We will be safe instead of sorry, is that understood?")
    bubble(200, "Yes sir. I'll leave immediately, sir.")
    set_ent_script(200, "R9")
    wait_for_entity(200, 200)
  elseif (en == 6) then
    view_range(1, 101, 12, 113, 22)
    warp(107, 22, 4)
    set_ent_speed(200, 4)
    set_ent_script(200, "U6R2W50")
    wait_for_entity(200, 200)
    set_ftile(110, 16, 312)
    wait(50)
    bubble(200, "$0:",
                   "This is so perfect.")
    bubble(200, "Just as my luck starts running out, along comes an opportunity to get out of town, and possibly make some serious coin on the way.")
    bubble(200, "Anyone this desperate must have a lot of dough to back it up, and I'm just the one to steal it from him!")
    bubble(200, "I can just imagine...")
    set_ent_facing(200, 0)
    wait(50)
    bubble(200, "I'd be the richest thief on the block.")
    bubble(200, "No! I'd be the richest thief in my OWN CITY! Ha ha ha!")
    wait(50)
  elseif (en == 7) then
    set_ent_facing(200, 1)
    view_range(1, 101, 31, 111, 47)
    warp(106, 35, 4)
    set_ent_speed(200, 5)
    bubble(200, "$0:",
                  "This does not look good. I'm almost positive the handwriting on this note is the same as...")
    wait(50)
    set_ent_facing(200, 0)
    wait(50)
    bubble(200, "Hmm...")
    wait(50)
    bubble(200, "I have no choice but to check it out. I'd must be getting on my way immediately!")
    set_ent_script(200, "D12")
    wait_for_entity(200, 200)
  end
  set_autoparty(0)
  remove_chr(en)
  return
end
