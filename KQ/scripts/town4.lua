-- town4 - "Maldea"

-- /*
-- {
-- Need a rewrite so we accurately lose the character.
-- You also find out that you can't wear the Opal armour because it's cursed.
-- }
-- */


function autoexec()
  if (get_progress(P_WARPEDTOT4) == 1) then
    if (get_numchrs() > 1) then
      set_progress(P_OLDPARTNER, get_pidx(1) + 1)
      remove_chr(get_pidx(1))
      set_ent_active(HERO2, 0)
    else
      set_progress(P_OLDPARTNER, 0)
    end
    set_progress(P_WARPEDTOT4, 2)
    set_desc(0)
  end

  -- We do not want to call this in refresh() because we want to keep the
  -- guy on the screen until you walk into the house
  if (get_progress(P_BOUGHTHOUSE) == 1) then
    set_ent_active(6, 0)
  end

  -- Warped inside "bounded area #0", the house of the player
  if (get_bounds(HERO1) == 0) then
    set_sstone(0)
    set_desc(0)
  end
  refresh()
end


-- Show the status of a treasures
function showch(which_marker, which_chest)
  -- Set tiles if -1 passed in as 'which_chest' or if chest already opened
  if (which_chest < 0 or get_treasure(which_chest) == 1) then
    set_zone(which_marker, 0)
  end
  if (which_chest == 81) then
    set_mtile(which_marker, 237)
  end
end


function refresh()
  showch("treasure1", 51)
  showch("treasure2", 52)
  showch("treasure3", 53)
  showch("warp_spell", 81)
end


function postexec()
  if (get_progress(P_WARPEDTOT4) == 2) then
    LOC_lost_partner()
  end
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "town4")

  elseif (zn == 2) then
    door_in("room_1i")
    set_ent_active(6, 0)

  elseif (zn == 3) then
    door_in("gelik_i")

  elseif (zn == 4) then
    door_in("shop_1i")

  elseif (zn == 5) then
    door_in("room_2i")

  elseif (zn == 6) then
    door_in("room_3i")

  elseif (zn == 7) then
    door_in("room_4i")

  elseif (zn == 8) then
    door_in("room_5i")

  elseif (zn == 9) then
    door_in("room_6i")

  elseif (zn == 10) then
    door_in("room_7i")

  elseif (zn == 11) then
    door_in("shop_2i")

  elseif (zn == 12) then
    door_in("shop_3i")

  elseif (zn == 13) then
    door_in("inn_i")

  elseif (zn == 14) then
    door_out("room_1o")

  elseif (zn == 15) then
    door_out("gelik_o")

  elseif (zn == 16) then
    door_out("shop_1o")

  elseif (zn == 17) then
    door_out("room_2o")

  elseif (zn == 18) then
    door_out("room_3o")

  elseif (zn == 19) then
    door_out("room_4o")

  elseif (zn == 20) then
    door_out("room_5o")

  elseif (zn == 21) then
    door_out("room_6o")

  elseif (zn == 22) then
    door_out("room_7o")

  elseif (zn == 23) then
    door_out("shop_2o")

  elseif (zn == 24) then
    door_out("shop_3o")

  elseif (zn == 25) then
    door_out("inn_o")

  elseif (zn == 26) then
    shop(10)

  elseif (zn == 27) then
    shop(11)

  elseif (zn == 28) then
    shop(12)

  elseif (zn == 29) then
    inn("Seaside Inn", 60, 1)

  elseif (zn == 30) then
    bubble(HERO1, "Locked.")

  elseif (zn == 31) then
    book_talk(party[0])

  elseif (zn == 32) then
    touch_fire(party[0])

  elseif (zn == 33) then
    chest(51, I_FRUNE, 1)

  elseif (zn == 34) then
    chest(52, I_EDROPS, 1)

  elseif (zn == 35) then
    chest(53, I_RUBYBROOCH, 1)

  elseif (zn == 36) then
    chest(81, I_B_WARP, 1)
    refresh()

  elseif (zn == 37) then
    if (prompt(255, 2, 0, "Take a nap?",
                          "  yes",
                          "  no") == 0) then
      do_inn_effects(1)
    end

  elseif (zn == 38) then
    set_save(1)

  elseif (zn == 39) then
    set_save(0)

  elseif (zn == 40) then
    warp("room_8stairs1", 8)

  elseif (zn == 41) then
    warp("room_1stairs", 8)

  elseif (zn == 42) then
    door_out("room_1escape")

  elseif (zn == 43) then
    door_in("room_8stairs2")

  elseif (zn == 44) then-- item shop door
    bubble(HERO1, "Locked.")

  elseif (zn == 45) then-- armor shop door
    bubble(HERO1, "Locked.")

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_TALKGELIK) == 0) then
      bubble(en, "Opal armor? I have no idea where you would find something like that.")
      bubble(en, "You should go talk to Gelik. He's old... he knows all kinds of stuff. He lives up in the north end of town.")
    else
      bubble(en, "So how do you like our little town?")
      if (party[0] == CASANDRA) then
        bubble(HERO1, "Oh, not too bad. Needs more flowers, though...")
      end
    end

  elseif (en == 1) then
    bubble(en, "I planted all of the flowers in this town. I love flowers.")
    if (party[0] == CASANDRA) then
      bubble(HERO1, "Well, you've certainly done a good job on them. Have you ever considered birchleaf fertilizer for extra color?")
      wait(50)
    end
    bubble(en, "Did I mention that I love flowers?")

  elseif (en == 2) then
    bubble(en, "Did you meet the old flower lady? She's nuts!")

  elseif (en == 3) then
    bubble(en, "You should go talk to Trezin on the other side of town. He's selling his house, and it's a really good deal.")

  elseif (en == 4) then
    bubble(en, "Oh, how I love to look into your eyes...")

  elseif (en == 5) then
    bubble(en, "Oh, you're so sweet!")

  elseif (en == 6) then
    LOC_buy_house(en)

  elseif (en == 7) then
    LOC_talk_gelik(en)

  elseif (en == 8) then
    bubble(en, "Hello, nice day isn't it?")

  elseif (en == 9) then
    bubble(en, "So the duck says, 200 gp... same as in town.")
    bubble(en, "Ha ha ha ha!")
    bubble(en, "Don't you get it?")
    wait(50)
    bubble(en, "You don't get it.")

  elseif (en == 10) then
    bubble(en, "My roommate tells the worst jokes.")

  elseif (en == 11) then
    bubble(en, "...Huh?",
           "Oh, hi... I'm sick.")

  elseif (en == 12) then
    bubble(en, pick("zzz", "ZZZ", "ZZZ... Long live KQlives!... zzz..."))

  elseif (en == 13) then
    bubble(en, "My son is sick in bed.")

  end
end


function LOC_buy_house(en)
  if (get_progress(P_BOUGHTHOUSE) == 0) then
    bubble(en, "Hey, the name's Trezin, and have I got a deal for you!")
    if (party[0] == CASANDRA) then
      thought(HERO1, "Oh great. This guy sounds like another used-chariot salesman. Sign...")
    end
    if (prompt(en, 2, 0, "You can have this lovely house",
                         "for a mere 7500 GP. Sound good?",
                         "  no",
                         "  yes") == 0) then
      bubble(en, "So be it. You know where I am if you change your mind.")
    else
      if (get_gp() >= 7500) then
        set_gp(get_gp() - 7500)
      else
        bubble(en, "Hmm... looks like you're a bit short on funds. Fear not. I'll be here should you manage to find enough money.")
        if (party[0] == AYLA) then
          thought(HERO1, "Yea, or swipe it out of your purse, you old windbag...")
        elseif (party[0] == CASANDRA) then
          thought(HERO1, "Yea; fear not, maybe I'll even consider buying it!",
                         "Jerk...")
        end
        return
      end
      bubble(en, "Fantastic! You know a good deal when you see one.")
      bubble(en, "Oh, there are some things you should know about the house.")
      bubble(en, "First off, you can sleep here fully recover HP and MP for free!")
      if (party[0] == CASANDRA) then
        thought(HERO1, "No way! Did he say free?! You mean I can sleep in my OWN house, in my OWN bed for free?! Oh wow!")
      end
      bubble(en, "Secondly, there is a save spot right in the house! How's that for convenience?")
      if (party[0] == CASANDRA) then
        thought(HERO1, "Yea, and how's a nice knuckle sandwich sound for convenience? Just LEAVE already you old geezer!")
      end
      bubble(en, "And lastly, I've even left a little something inside for you. It's a very handy spell called Warp.")
      bubble(en, "The Warp spell lets you escape from dungeons and other such places.")
      bubble(en, "As a bonus, you can use Warp from anywhere on the world map and it will bring you back to the house! Great huh?")
      if (party[0] == CASANDRA) then
        thought(HERO1, "Not if you're anywhere around it when I get back, it's not!")
      end
      bubble(en, "Well, enough of my yapping. Go in and see for yourself!, I've got to go put this money somewhere safe!")
      if (party[0] == CASANDRA) then
        thought(HERO1, "Oh, I can tell you where to put it that'll keep it really safe...")
      end
      set_ent_movemode(en, 2)
      set_ent_obsmode(en, 0)
      if (get_ent_facing(en) == 3) then
        set_ent_script(en, "D1R1")
      else
        set_ent_script(en, "R1D1")
      end
      wait_for_entity(en, en)
      set_progress(P_BOUGHTHOUSE, 1)
    end
  else
    bubble(en, "What are you waiting for? Go in and take a look around.")
  end
end


function LOC_lost_partner()
  local a, b
  if (get_progress(P_OLDPARTNER) > 0) then
    bubble(HERO1, "What?")
    set_autoparty(1)
    set_ent_script(HERO1, "F0W25F2W25F1W25F3W25F0")
    wait_for_entity(HERO1, HERO1)
    set_autoparty(0)
    a = get_progress(P_OLDPARTNER) - 1
    b = "Where's "..get_party_name(a).."?"
    bubble(HERO1, b)
    bubble(HERO1, "Just great... how am I supposed to get back?")
    wait(100)
    if (party[0] == CASANDRA) then
      if (get_progress(P_OLDPARTNER) - 1 == TEMMIN) then
        bubble(HERO1, "I have to find him! I'm not letting him get away from me that easily.")
      else
        thought(HERO1, "Oh well...")
      end
    else
      bubble(HERO1, "Sigh. I guess I'll just have to keep going and hope I find a way to get back.")
      wait(50)
    end
  end
  set_progress(P_WARPEDTOT4, 3)
end


function LOC_talk_gelik(en)
  if (get_progress(P_OPALHELMET) == 0) then
    if (get_progress(P_TALKGELIK) == 0) then
      bubble(en, "So, you're looking for Opal armor eh? Well, I know of one piece.")
      bubble(en, "The Opal Helmet belongs to a wealthy man named Dungar who lives in his own private estate east of town.")
      bubble(en, "You should go and talk to him.")
      bubble(en, "If you get the Helmet, come back here and hopefully by then I can find out a little bit more about the other pieces.")
      set_progress(P_TALKGELIK, 1)
    else
      bubble(en, "There isn't anything else I can tell you about Opal Armor right now.")
    end
  end
  if (get_progress(P_OPALSHIELD) == 0 and get_progress(P_OPALHELMET) > 0) then
    if (get_progress(P_TALKGELIK) == 4) then
      bubble(en, "Ah, you are back and you have the Helmet.")
      bubble(en, "Oh, in case you are curious, you can't wear any of the armor until you get all of the pieces and go to the Water Shrine.")
      bubble(HERO1, "Oh... that's why.")
      bubble(en, "Yes, well anyways, I have found out where the next piece is.")
      bubble(en, "You see, it turns out that the Opal Shield is the prize for besting the warrior Trayor at the Coliseum.")
      if (get_progress(P_SEECOLISEUM) == 0) then
        bubble(HERO1, "Coliseum?")
      end
      bubble(en, "The coliseum is west of Sunarin. You can't miss it.")
      if (get_progress(P_SEECOLISEUM) == 1) then
        bubble(HERO1, "But the Coliseum was closed when I went there.")
        bubble(en, "Yes... well I'm sure it's open by now.")
        bubble(HERO1, "Isn't that convenient?")
        bubble(en, "Quite.")
      end
      bubble(HERO1, "So, I have to go there and ask Trayor for the shield?")
      bubble(en, "No, you have to fight him for it.")
      bubble(HERO1, "Fight, fight, fight. That's all I ever do.")
      bubble(en, "Well, that's just how these things work.")
      bubble(HERO1, "Well, I guess I'm off to fight Trayor. Oh by the way, what kind of battle is this? Is magic permitted?")
      bubble(en, "Yes indeed. Anything goes... and Trayor is a very capable warrior. It won't be easy.")
      bubble(HERO1, "Oh... well nothing is ever easy right?")
      bubble(en, "Not if it's worth doing, no.")
      bubble(HERO1, "Thanks. I'll see you after I get the Shield.")
      set_progress(P_TALKGELIK, 5)
      set_progress(P_SEECOLISEUM, 2)
      return
    else
      bubble(en, "Good luck.")
    end
  end
  if (get_progress(P_OPALSHIELD) == 1 and get_progress(P_TALKGELIK) < 6) then
    bubble(en, "Oh... you're back. I'm sorry, but I haven't been able to find out where the other pieces are.")
    bubble(HERO1, "Hey, don't worry about it.")
    bubble(HERO1, "My friend here brought me information about the Band and the Armor. We'll have no trouble finding it now.")
    bubble(HERO1, "I appreciate all your help in finding the Helmet and the Shield. Thanks!")
    bubble(en, "Well, that's great... and you're welcome. I'm glad I was able to help.")
    bubble(HERO1, "I'll see you around then.")
    bubble(en, "Certainly. Good journey.")
    set_progress(P_TALKGELIK, 6)
    return
  end
  if (get_progress(P_TALKGELIK) == 6) then
    bubble(en, "How are things going?")
  end
end
