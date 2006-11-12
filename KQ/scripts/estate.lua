-- estate - "Home of Dungar and Emma east of Maldea"

function autoexec()
  if (get_progress(P_OPALHELMET) == 0 and get_progress(P_TALKGELIK) < 3) then
    set_ent_active(2, 0)
  end
  if (get_progress(P_TALKGELIK) > 3) then
    local x, y = marker("house_o")
    set_zone(x, y - 1, 6)
    set_obs (x, y - 1, 1)
    set_ent_active(0, 0)
  end
  if (get_progress(P_EMBERSKEY) == 1) then
    set_ent_facing(0, FACE_UP)
    set_desc(0)
  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(0, "I'm the grounds keeper here.")

  elseif (en == 1) then
    if (get_progress(P_OPALHELMET) == 0) then
      if (get_progress(P_TALKGELIK) == 0) then
        bubble(en, "Hello, are you a friend of my husband's?")
      elseif (get_progress(P_TALKGELIK) == 1) then
        bubble(en, "You need the Opal helmet? I'm afraid you are too late... It was stolen!")
        bubble(en, "My husband left for Sunarin to try and get it back. If you go there you will probably find him.")
        set_progress(P_TALKGELIK, 2)
      elseif (get_progress(P_TALKGELIK) == 2) then
        bubble(en, "Did you find my husband?")
      elseif (get_progress(P_TALKGELIK) == 3) then
        bubble(en, "Good luck!")
      else
        bubble(en, "Hello, I'm Emma.")
      end
    else
      if (get_progress(P_TALKGELIK) == 3) then
        bubble(en, "Fabulous!")
      else
        bubble(en, "...")
      end
    end

  elseif (en == 2) then
    if (get_progress(P_OPALHELMET) == 0) then
      bubble(en, "Just return here with the Helmet once you find it. I'd like to see it before you borrow it.")
    else
      LOC_murder(en)
    end

  end
end


function postexec()
  if (get_progress(P_EMBERSKEY) == 1) then
    local wife = 1
    local en = 2
    wait(50)
    bubble(en, "Well, you can get into the Ember's guild through the house in the south-west part of town. The door is locked, so you'll need this.")
    set_progress(P_EMBERSKEY, 2)
    sfx(5)
    msg("Ember's key procured", 255, 0)
    bubble(en, "In the house are some stairs leading to a small basement.")
    bubble(en, "On the center bookshelf is a book titled 'How to Enter the Embers' Secret Hideout'.")
    bubble(en, "Just pull on the book and the bookshelf will slide over revealing an entrance to the guild.")
    bubble(en, "The rest is up to you after that.")
    bubble(HERO1, "How do you know all this?")
    bubble(en, "Well, I... umm... that is to say...")
    bubble(wife, "Dungar is a retired thief...")
    bubble(en, "Emma!")
    bubble(wife, "Well it's true! In fact Dungar was only one step away from becoming Guildmaster!")
    bubble(en, "Yes, but I got out! The guild was getting too big, and too violent. I just had to leave.")
    bubble(en, "I may have been a thief, but I refused to kill innocent people!")
    bubble(HERO1, "So why are the Embers trying to get this Helmet back? Did you take it from them?")
    bubble(en, "Well, more or less... but it's mine. The guild wanted it, of course, but it's mine and I do not wish to part with it.")
    bubble(en, "It has sentimental value to me.")
    bubble(HERO1, "Well, that's good enough for me. I will be off then.")
    bubble(en, "Great! Oh, could you please bring the Helmet back here before you continue on your quest?")
    bubble(en, "I just want to make sure that they haven't damaged it.")
    bubble(HERO1, "No problem. I'll be back in a bit.")
  end
end


function refresh()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "estate")

  elseif (zn == 2) then
    door_in("house_i")

  elseif (zn == 3) then
    door_in("hut_i")

  elseif (zn == 4) then
    chest(54, I_EDROPS, 1)
    refresh()

  elseif (zn == 5) then
    door_out("house_o")

  elseif (zn == 6) then
    bubble(HERO1, "Locked.")

  elseif (zn == 7) then
    warp("dstairs1", 8)

  elseif (zn == 8) then
    if (get_progress(P_TALKGELIK) < 4) then
      bubble(HERO1, "That scream came from up here somewhere! I can't leave!")
    else
      warp("ustairs1", 8)
    end

  elseif (zn == 9) then
    bubble(HERO1, "These look rare.")

  elseif (zn == 10) then
    door_out("hut_o")

  elseif (zn == 11) then
    if (get_progress(P_TALKGELIK) < 4) then
      if (get_numchrs() == 1) then
        set_ent_script(HERO1, "U1F0")
        wait_for_entity(HERO1, HERO1)
      else
        set_ent_script(HERO1, "U1F0")
        set_ent_script(HERO2, "U1F0")
        wait_for_entity(HERO1, HERO2)
      end
    end

  elseif (zn == 12) then
    if (get_progress(P_TALKGELIK) < 4) then
      local wife = 1

      set_progress(P_TALKGELIK, 4)
      set_ent_facing(en, FACE_DOWN)
      bubble(wife, "Oh $0!")
      bubble(wife, "I woke up because I thought I heard fighting in your room.")
      bubble(wife, "When I turned to wake Dungar, I realized that he was dead.")
      bubble(wife, "Someone stabbed him in his sleep!")
      bubble(HERO1, "Oh no! You were right though, I was fighting an assassin who was sent here to kill me. He obviously was not alone.")
      bubble(HERO1, "This is all my fault!")
      bubble(wife, "No it's not... this is all because of that damn guild!")
      bubble(HERO1, "Yes, but when I was there to get the Helmet back, it all seemed as though it was too easy.")
      bubble(HERO1, "They set me up. They wanted to follow me so that they could kill Dungar!")
      bubble(wife, "That may be true, $0, but you were doing the right thing. You didn't know.")
      bubble(HERO1, "I'm so sorry, Emma!")
      bubble(wife, "It's okay... I guess I've been expecting something like this to happen for a long time.")
      bubble(wife, "You can't marry an ex-thief and expect to have a normal life. I'll be alright.")
      bubble(HERO1, "Are you sure... is there anything I can do?")
      bubble(HERO1, "I know... I'll take care of that guild once and for all!")
      bubble(wife, "No! Don't do that. You'll just get yourself killed! You have a quest to continue.")
      bubble(wife, "What you can do for me is take that damn Helmet far away from here... for good.")
      bubble(HERO1, "Are you sure?")
      bubble(wife, "I'm very sure. As long as I have that thing I too will be in danger. You keep it.")
      bubble(wife, "And don't worry about the guild. They won't be able to find you if you keep moving around.")
      bubble(HERO1, "Okay... but what about you? It's not safe here. You have to leave!")
      bubble(wife, "I will. I'll gather my things and leave right after I give my husband a proper funeral.")
      bubble(wife, "You should leave right away. They know you're here.")
      bubble(HERO1, "Okay... you be careful, alright?")
      bubble(wife, "Don't worry about me. I'll be fine. Now go!")
      bubble(HERO1, "Goodbye.")
      orient_heroes()
    end

  elseif (zn == 13) then
    bubble(HERO1, "Goodbye Dungar. I'm sorry.")

  elseif (zn == 14) then
    touch_fire(party[0])

  end
end


function LOC_murder(en)
  local wife = 1
  local assassin = 3
  local pp, os

  if (get_numchrs() == 1) then
    pp = "I"
    os = ""
  else
    pp = "we"
    os = "s"
  end

  bubble(en, "Excellent work! The Helmet appears to be undamaged... this is wonderful.")
  bubble(wife, "Dungar, why don't you invite our young friend"..os.." to stay the night? It's getting awfully late.")
  bubble(HERO1, "Well, "..pp.."'d love to, but "..pp.." really should be going.")
  bubble(en, "Nonsense! I insist you spend the night with us and celebrate. ")
  bubble(HERO1, "Well, alright... but "..pp.."'ll have to leave first thing in the morning.")
  bubble(en, "Stupendous! Now let's start the celebration.")
  do_fadeout(4)
  wait(100)
  set_holdfade(1)
  set_autoparty(1)

  x, y = marker("after_party")
  warp(x, y, 16)

  x, y = marker("door")
  set_ftile(x, y, 0)
  set_zone (x, y, 0)
  set_obs  (x, y, 0)
  set_btile(x, y + 1, 201)

  set_ent_active(assassin, 0)

  -- This is one of the few exceptions where we actually USE view_range
  view_range(1, 80, 5, 87, 12)
  set_holdfade(0)
  set_ent_facing(HERO1, FACE_UP)
  drawmap()
  screen_dump()
  do_fadein(4)
  msg("Later...", 255, 1000)
  bubble(HERO1, "Ugh... what a party!")
  set_ent_script(HERO1, "R3U1L1W50F1")
  if (get_numchrs() == 1) then
    wait_for_entity(HERO1, HERO1)
  else
    set_ent_script(HERO2, "W50R4U1R1W60F3")
    wait_for_entity(HERO1, HERO2)
  end
  bubble(HERO1, "Ah... sleep.")
  drawmap()
  screen_dump()
  stop_song()
  sfx(36)
  do_fadeout(2)
  rest(3000)
  set_ent_active(assassin, 1)
  set_ent_active(wife, 1)
  x, y = marker("by_bed")
  place_ent(wife, x, y)
  set_ent_facing(wife, FACE_UP)
  set_ent_active(en, 1)
  set_ent_facing(en, FACE_DOWN)
  drawmap()
  screen_dump()
  do_fadein(16)
  bubble(HERO1, "Huh... what was that?")
  set_ent_facing(HERO1, FACE_RIGHT)
  drawmap()
  screen_dump()
  bubble(assassin, "Oops!")
  bubble(HERO1, "Hey!")
  set_ent_script(HERO1, "R1D1R1")
  wait_for_entity(HERO1, HERO1)
  bubble(HERO1, "Who are you?")
  bubble(assassin, "I'm an assassin, you moron.")
  bubble(HERO1, "So you're here to kill me?")
  bubble(assassin, "No, I'm selling tickets to the assassin's jamboree. Of course I'm here to kill you!")
  if (get_numchrs() == 1) then
    bubble(HERO1, "Well there's no need to be so rude about it.")
    bubble(assassin, "Shut up!")
  else
    bubble(HERO1, "$1! Wake up!")
    bubble(HERO2, "Wha...?")
    set_ent_script(HERO2, "L1D1L1")
    wait_for_entity(HERO2, HERO2)
    set_ent_facing(HERO2, FACE_RIGHT)
    orient_heroes()
  end
  drawmap()
  screen_dump()
  set_run(0)
  combat(23)
  set_run(1)
  if (get_alldead() == 1) then
    return
  end
  -- Now restore the view
  view_range(0, 0, 0, 0, 0)
  set_ent_active(assassin, 0)
  set_ent_active(en, 0)

  x, y = marker("by_bed")
  set_mtile(x - 3, y, 288)
  place_ent(wife, x, y)

  set_ent_facing(wife, FACE_LEFT)
  drawmap()
  screen_dump()
  wait(50)
  if (get_numchrs() == 1) then
    bubble(HERO1, "Gee... good thing I wore all my equipment to bed.")
  else
    bubble(HERO1, "Gee... good thing we wore all our equipment to bed.")
  end
  wait(50)
  bubble(wife, "Aaaahhhh!")
  set_ent_facing(HERO1, FACE_LEFT)
  drawmap()
  screen_dump()
  bubble(HERO1, "Oh oh, that sounded like Dungar's wife!")
  set_vfollow(1)
  set_autoparty(0)
end
