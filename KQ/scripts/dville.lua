-- dville - "Denorian village southeast of Oracle's tower"

-- /*
-- {
-- P_DENORIAN: Status of the stolen statue
--   (0) You just got there; statue is gone
--   (1) If you refuse after you hear their plea, you are kicked out of town
--   (2) You've agreed to help
--   (3) You found Demnas (but not the troll)
--   (4) You found the troll, too
--   (5) The broken statue is returned to the village
--   (6) The 2nd half of the statue is returned
-- }
-- */

function autoexec()
  return
end


function postexec()
  return
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 256, 77, 256, 77)

  elseif (zn == 2) then
    bubble(HERO1, "Locked.")

  elseif (zn == 3) then
    door_in(61, 8, 57, 1, 65, 11)

  elseif (zn == 4) then
    door_in(71, 8, 67, 1, 75, 11)

  elseif (zn == 5) then
    door_in(61, 20, 57, 13, 65, 23)

  elseif (zn == 6) then
    door_in(71, 20, 67, 13, 75, 23)

  elseif (zn == 7) then
    door_in(61, 32, 57, 25, 65, 35)

  elseif (zn == 8) then
    door_in(71, 32, 67, 25, 75, 35)

  elseif (zn == 9) then
    door_out(13, 18)

  elseif (zn == 10) then
    door_out(18, 27)

  elseif (zn == 11) then
    door_out(27, 21)

  elseif (zn == 12) then
    door_out(34, 28)

  elseif (zn == 13) then
    door_out(40, 18)

  elseif (zn == 14) then
    door_out(30, 40)

  elseif (zn == 15) then
    touch_fire(party[0])

  end
end


function entity_handler(en)
  -- Anyone except head of town council kicks you out if you refuse to help
  if (get_progress(P_DENORIAN) == 1 and en ~= 7) then
    bubble(en, "You're not welcome here!")
    change_map("main", 256, 77, 256, 77)
    return
  end

  if (en == 0) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Who would want to steal from us?")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "I thought as much.")
    else
      bubble(en, "We are so happy to have the statue whole once again!")
    end

  elseif (en == 1) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "This is terrible.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "I'm glad the troll isn't a threat to us anymore.")
    else
      bubble(en, "I'm glad that's over.")
    end

  elseif (en == 2) then
    bubble(en, "That statue was given to the village after the evil Kalatos was defeated in the Great War.")

  elseif (en == 3) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "The village council says that it was a troll who stole the statue. What's a troll?")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Mommy told me what a troll is. That's so scary!")
    else
      bubble(en, "The statue is home.")
    end

  elseif (en == 4) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Apparently, there has not been a troll spotted in these parts since the Great War.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "I wonder if our village is really safe?")
    else
      bubble(en, "I feel so much more secure now. Thank you for everything you've done!")
    end

  elseif (en == 5) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "I can't believe this has happened to us. We are a peaceful people.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Now, what to make for supper.")
    else
      bubble(en, "Thank you for restoring peace to our people.")
    end

  elseif (en == 6) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Where did our statue go?")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Things can get back to normal now.")
    else
      bubble(en, "I've lost my hat. I should go ask the statue where to look.")
    end

  elseif (en == 7) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "The council would like to ask for your help in retrieving our statue. We will tell you what we know.")
      if (prompt(en, 2, 0, "However, I must first ask if",
                 "you are willing to help us?",
                 "  yes",
                 "  no") == 0) then
        LOC_trollstory(en)
      else
        bubble(en, "I see. Well, then you should be on your way then. My people will show you out.")
        view_range(0, 0, 0, 0, 0)
        set_progress(P_DENORIAN, 1)
        change_map("main", 256, 77, 256, 77)
      end
    elseif (get_progress(P_DENORIAN) == 1) then
      if (prompt(en, 2, 0, "Have you changed your mind?",
                 "  yes",
                 "  no") == 0) then
        LOC_trollstory(en)
      else
        bubble(en, "I thought as much. Begone!")
        view_range(0, 0, 0, 0, 0)
        change_map("main", 256, 77, 256, 77)
      end
    elseif (get_progress(P_DENORIAN) == 2) then
      bubble(en, "Good luck.")
    elseif (get_progress(P_DENORIAN) == 3 or
            get_progress(P_DENORIAN) == 4) then
      bubble(en, "This is incredible... you actually got the statue back...")
      bubble(HERO1, "...Well, hold on a second. I didn't get the WHOLE statue back; just a part of it. It looks like it had been broken before I got there.")
      bubble(en, "Regardless, this much is wonderful. I wish we had some way of paying you back.")
      bubble(en, "Unfortunately our village has next to nothing.")
      bubble(HERO1, "Don't worry... not everything we adventurers do is for a reward.")
      bubble(en, "What about the troll?")

      msg("You tell the head of the village council what you found.", 0, 0)
      bubble(en, "Demnas... that name is familiar.")
      bubble(en, "I remember that a tribe of Narakians once lived around here somewhere. I thought they were all wiped out by Malkaron's armies.")

      msg("You shrug.", 0, 0)
      bubble(en, "Hmm...")
      bubble(en, "Wait... I think I remember now. Demnas was the name of the Narakian's tribal leader who supposedly went crazy many years ago. It's possible that you found where he has been hiding all of this time.")

      if (get_progress(P_DENORIAN) == 3) then
        bubble(HERO1, "Well, him being crazy explains why he was rambling on about someone trying to steal the statue from him.")
      else
        bubble(HERO1, "He was rambling on about someone trying to seal the statue from him, and it didn't make any sense until we met up with CORIN. I guess Malkaron's armies got to Demnas and the troll first because they were able to get away with half of the statue.")
      end
      bubble(en, "Well, I guess I'm a little relieved that we don't have a troll problem.")
      bubble(HERO1, "Well, yeah I guess that's good. And you don't have to worry about any crazed Nerakians for awhile either.")
      bubble(en, "That's good to hear. At the very least I can say that you will always be welcome in our village.")
      bubble(HERO1, "Thank you.")
      set_progress(P_DENORIAN, 5)
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Good luck in your journey. If you come across the other half of our statue...")
    else
      bubble(en, "Good journey friend.")
    end

  elseif (en == 8) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "This fellow sitting with me is the head of the village council. You should speak with him.")
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "We shall put the statue back as soon as it's whole again!")
    else
      bubble(en, "Thanks again.")
    end

  elseif (en == 9) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "You should hear what the head of the village council has to say.")
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "It's a relief to have our treasured statue in our hands again.")
    else
      bubble(en, "Thanks again.")
    end

  elseif (en == 10) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "We hope you will help us.")
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Although it's broken, the statue still means everything to us.")
    else
      bubble(en, "Oh, it's wonderful to have the whole statue again!")
    end

  elseif (en == 11) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "This is where the statue was taken from.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "We'll put the statue back here once we have the whole thing.")
    else
      bubble(en, "Thank you for getting our statue back!")
    end

  elseif (en == 12) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Less council meetings, more cooking!")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Food always tastes better when it's cooked. Except icecream.")
    else
      bubble(en, "Let the cookfires burn!")
    end

  elseif (en == 13) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Geez, I wish I was old enough to go after that smelly old troll! I'd show him a thing or two.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "Nerakians? Where are they from?")
    else
      bubble(en, "It would be cool to control monsters with your thoughts! ZAAAP!")
    end

  elseif (en == 14) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "The council is having another meeting about what to do to get back our statue.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "I think a celebration is in order.")
    else
      bubble(en, "Ah, even more reason to celebrate!")
    end

  elseif (en == 15) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "You should find the council's hut. They are looking for people to help get the statue back.")
    elseif (get_progress(P_DENORIAN) == 5) then
      bubble(en, "The council can relax now. Thank you.")
    else
      bubble(en, "It sure would be nice to have a little more excitement around here. It's too... boring.")
    end

  end
end


function LOC_trollstory(en)
  bubble(en, "Oh, thank you very much! We will not forget your kindness.")
  bubble(en, "So, here is what we know. The thief is a troll. Our people have tracked the troll to a cave on the other side of the river.")
  bubble(en, "That is where he can be found.")
  bubble(HERO1, "Have your people tried to get the statue back themselves?")
  bubble(en, "Yes, we have tried, but we quickly learned that we were no match for this enemy.")
  bubble(en, "We could not even overcome the denizens of the cave, never mind the troll himself!")
  bubble(HERO1, "Are trolls common to this area? I have seen none in my travels.")
  bubble(en, "That is the strange part of all this. Trolls have not been seen in these parts for nearly a century!")
  bubble(en, "I must say that we suspect that someone is directing the troll in these matters.")
  bubble(HERO1, "Well, don't worry. We will get that statue back for you and get to the bottom of this whole thing.")
  bubble(en, "Oh, and if you haven't seen the cave yet... or even if you have, you'll need to know that there is a trick to opening the door.")
  bubble(en, "There is a set of stones off to the left of the entrance. You have to press them in the right order to gain access.")
  bubble(en, "Press the first stone, then the fourth and lastly the third. Then the door will open. Got it?")
  bubble(HERO1, "Uh... sure.")
  set_progress(P_DENORIAN, 2)
end
