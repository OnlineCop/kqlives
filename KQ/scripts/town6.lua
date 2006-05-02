-- town6 - "Ajantara"

function autoexec()
  if (get_progress(P_OPALSHIELD) == 0) then
    -- Remove the guards from the map
    set_ent_active(5, 0)
    set_ent_active(6, 0)
  end

  if (get_progress(P_OPALSHIELD) == 1 and get_progress(P_SAVEBREANNE) < 3) then
    -- Remove Breanne
    set_ent_active(9, 0)
  end

  refresh()
end


function refresh()
  if (get_treasure(70) == 1) then
    set_obs(15, 39, 0)
  end
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", "town6")

  elseif (zn == 2) then
    door_in(56, 9, 52, 1, 72, 12)

  elseif (zn == 3) then
    door_in(68, 9, 52, 1, 72, 12)

  elseif (zn == 4) then
    door_in(83, 9, 74, 1, 86, 12)

  elseif (zn == 5) then
    door_in(58, 22, 52, 14, 64, 25)

  elseif (zn == 6) then
    door_in(70, 22, 66, 14, 74, 25)

  elseif (zn == 7) then
    door_in(80, 22, 76, 14, 84, 25)

  elseif (zn == 8) then
    door_in(60, 35, 52, 27, 68, 38)

  elseif (zn == 9) then
    door_in(77, 42, 70, 27, 84, 45)

  elseif (zn == 10) then
    door_out(14, 14)

  elseif (zn == 11) then
    door_out(20, 14)

  elseif (zn == 12) then
    door_out(35, 15)

  elseif (zn == 13) then
    door_out(17, 31)

  elseif (zn == 14) then
    door_out(31, 27)

  elseif (zn == 15) then
    door_out(20, 41)

  elseif (zn == 16) then
    door_out(33, 40)

  elseif (zn == 17) then
    door_out(34, 54)

  elseif (zn == 18) then
    bubble(HERO1, "Locked.")

  elseif (zn == 19) then
    shop(15)

  elseif (zn == 20) then
    shop(16)

  elseif (zn == 21) then
    if (get_progress(P_TOWN6INN) > 0) then
      inn("Ajantara Inn", 1, 1)
    else
      inn("Ajantara Inn", 150, 1)
    end

  elseif (zn == 22) then
    shop(17)

  elseif (zn == 23) then
    shop(18)

  elseif (zn == 24) then
    chest(67, I_B_BLIZZARD, 1)

  elseif (zn == 25) then
    chest(68, I_EDAENRA, 1)

  elseif (zn == 26) then
    chest(69, I_NLEAF, 1)

  elseif (zn == 27) then
    chest(70, I_HPUP, 1)
    refresh()

  elseif (zn == 28) then
    book_talk(party[0])

  elseif (zn == 29) then
    if (get_progress(P_SAVEBREANNE) == 1) then
      if (get_numchrs() == 1) then
        bubble(HERO1, "Ugh... I getting real sick of this running around crap! Why is nothing I need where it's supposed to be?")
        bubble(HERO1, "I'll bet that even if I had waited a month and come here for the Band, the Ajantara's daughter would have just been kidnapped.")
        bubble(HERO1, "When will this end?")
      else
        face_each_other(HERO1, HERO2)
        wait(50)
        bubble(HERO1, "You know what? I'm getting real sick of this running around crap!")
        bubble(HERO2, "What are you talking about?")
        bubble(HERO1, "The same thing happened with the Helmet. The minute I show up to get the stupid thing... it's gone.")
        bubble(HERO1, "It's as if someone is purposely trying to make me go crazy.")
        bubble(HERO2, "Don't you think that you're exaggerating just a little?")
        bubble(HERO2, "Sure it may seem that things are out of our control, but what we're doing is of the utmost importance.")
        bubble(HERO2, "This is our chance to do something good. I mean, originally this quest was nothing more than a fetch mission.")
        bubble(HERO2, "However, as we've pursued our quest we've managed to do some good along the way. That's what really counts.")
        bubble(HERO1, "Hmm... I guess you're right. I just don't like being out of control. We still don't really know what's going on!")
        bubble(HERO2, "I'm sure we'll come to understand what this is all about in time. If nothing else, all this running around is making us stronger.")
        bubble(HERO2, "We'll be better prepared for Malkaron and anybody else who gets in our way.")
        bubble(HERO1, "That's true... I have learned a lot since we've started, and I sure feel much better about myself.")
        bubble(HERO2, "So does that mean that you're ready to go?")
        bubble(HERO1, "Yes! This isn't about me... this is about rescuing an innocent little girl and saving the world.")
        bubble(HERO2, "Saving the world?")
        bubble(HERO1, "Stuff it, I'm on a roll here.")
      end
      set_progress(P_SAVEBREANNE, 2)
    end

  elseif (zn == 30) then
    if (get_progress(P_WARPSTONE) == 0) then
      set_progress(P_WARPSTONE, 1)
    end
    change_map("town2", "travelpoint")

  elseif (zn == 31) then
    touch_fire(party[0])

  -- Door in the item shop
  elseif (zn == 32) then
    bubble(HERO1, "Locked.")

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_OPALSHIELD) == 0) then
      bubble(en, "How do you like our little town?")
      return
    end
    if (get_progress(P_SAVEBREANNE) == 0) then
      bubble(en, "You must help save our daughter! We'll give you anything, just please bring her back to us.")
      if (get_progress(P_TALKGRAMPA) == 1) then
        bubble(HERO1, "We were told she was kidnapped, but by who? Where did they take her?")
        bubble(en, "The ones who took her are members of a cult... the Cult of the Shadow Walker. They took her to their tower on Sharia island.")
      else
        bubble(HERO1, "What happened to her? Where did she go?")
        bubble(en, "She was kidnapped by cult members from the Cult of the Shadow Walker. They took her to their tower on Sharia island.")
      end
      bubble(en, "They have the travel stone to the island guarded by a giant!")
      bubble(HERO1, "Why did they take her?")
      bubble(en, "They took her because she is descended from the legendary half-dragon king Elgar.")
      bubble(en, "They are trying to summon an avatar of the Shadow Walker, and they plan on using Breanne to do it!")
      bubble(HERO1, "Doesn't that mean that one of you is also descended from Elgar?")
      bubble(en, "No. We adopted Breanne eight years ago. She doesn't even know that we are not her real parents!")
      bubble(HERO1, "Fear not, we will get your daughter back safely, and we'll put a stop to this summoning as well!")
      bubble(HERO1, "Just one thing.")
      bubble(en, "Yes?")
      bubble(HERO1, "Where is the travel stone to Sharia island?")
      bubble(en, "Oh, it's on the other side of Kragar's Pass.")
      bubble(en, "I'll send word to the guards to let you through because normally only caravans are allowed into the Pass.")
      bubble(HERO1, "Oh. Okay. Thanks!")
      bubble(HERO1, "By the way, do you know anything about an Opal Band? I was told that it was in this town. I need to borrow it.")
      bubble(en, "If you save my daughter, you can have it!")
      bubble(HERO1, "Really? Great! Where is it?")
      bubble(en, "It belongs to my daughter. She is probably wearing it.")
      bubble(HERO1, "Of course. Well, time to go!")
      set_progress(P_SAVEBREANNE, 1)
      return
    end
    if (get_progress(P_SAVEBREANNE) == 4) then
      bubble(en, "Things are finally back to normal.")
      return
    end
    if (get_progress(P_SAVEBREANNE) < 3) then
      bubble(en, "Please be careful.")
    else
      bubble(en, "Breanne told us she gave you the Band. We're glad everything has worked out so great. Thank you very much!")
      bubble(en, "I know it's not much, but to show our appreciation, we want you to feel welcome in our town.")
      bubble(en, "Therefore, any time you need to stay at our inn, you'll only be charged 1 GP!")
      set_progress(P_TOWN6INN, 1)
      bubble(HERO1, "That's very kind of you. Thank you!")
      bubble(en, "Oh, and by the way. You should take this.")
      sfx(5)
      msg("Cave Key procured", 255, 0)
      set_progress(P_CAVEKEY, 1)
      drawmap()
      screen_dump()
      bubble(HERO1, "What's this for?")
      bubble(en, "That will unlock the doors in the mountain pass.")
      bubble(en, "After thinking about this Opal stuff for a while, I remembered a story about the Opal Armor.")
      bubble(en, "Apparently, there is a magical suit of Opal Armor locked away deep in the mountain.")
      bubble(en, "That key will unlock the sealed entrances to the dungeons below.")
      bubble(HERO1, "That's perfect! The next item we were on our way to find was the Opal Armor.")
      bubble(en, "Well, I'm glad we could help. However, I must warn you to be extra cautious on this quest.")
      bubble(en, "A dark race of Larinon live in the dungeons down there and they are most unpleasant.")
      bubble(HERO1, "Thanks for the warning, and thank you again for the key.")
      set_progress(P_SAVEBREANNE, 4)
    end

  elseif (en == 1) then
    if (get_progress(P_OPALSHIELD) == 0) then
      bubble(en, "Welcome... enjoy your stay.")
      return
    end
    if (get_progress(P_SAVEBREANNE) == 0) then
      bubble(en, "Oh poor Breanne!")
    else
      if (get_progress(P_SAVEBREANNE) < 3) then
        bubble(en, "Good luck to you!")
      else
        bubble(en, "Breanne is a dear.")
      end
    end

  elseif (en == 2) then
    bubble(en, "This is a lovely little town. Don't you think so?")

  elseif (en == 3) then
    if (get_progress(P_OPALSHIELD) == 0) then
      bubble(en, "Wanna play tag with me?")
      return
    end
    if (get_progress(P_SAVEBREANNE) < 3) then
      bubble(en, "I miss Breanne.")
    else
      bubble(en, "This town rocks!")
    end

  elseif (en == 4) then
    bubble(en, "Sorry, I'm far too busy and you have terrible breath.")

  elseif (en == 5) then
    if (get_progress(P_SAVEBREANNE) < 3) then
      bubble(en, "There has been a lot of Cult activity around these parts lately.")
    else
      bubble(en, "Has the Cult really been rousted?")
    end

  elseif (en == 6) then
    bubble(en, "Lord and Lady Ajantara have hired us to watch the town.")

  elseif (en == 7) then
    if (get_progress(P_OPALSHIELD) == 0) then
      bubble(en, "Lord Ajantara's grandfather founded this town.")
      return
    end
    if (get_progress(P_SAVEBREANNE) == 0) then
      if (get_progress(P_TALKGRAMPA) == 0) then
        bubble(en, "Lord and Lady Ajantara's daughter, Breanne, was kidnapped last night. They are looking for someone to help find her!")
        set_progress(P_TALKGRAMPA, 1)
      else
        bubble(en, "You should go and speak with the Ajantaras.")
      end
    else
      if (get_progress(P_SAVEBREANNE) < 3) then
        bubble(en, "You truly are kind.")
      else
        bubble(en, "It's a lovely day isn't it?")
      end
    end

  elseif (en == 8) then
    bubble(en, "Everything in this town is owned by Lord and Lady Ajantara.")

  elseif (en == 9) then
    if (get_progress(P_OPALSHIELD) == 0) then
      bubble(en, "Hiya!")
      return
    end
    bubble(en, "Thank you very much. Please feel free to come and visit any time.")

  end
end
