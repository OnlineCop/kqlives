-- dville - "Denorian village southeast of Oracle's tower"

function autoexec()
  return;
end


function postexec()
  return;
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 256, 77, 256, 77);

  elseif (zn == 2) then
    bubble(HERO1, "Locked.");

  elseif (zn == 3) then
    door_in(61, 8, 57, 1, 65, 11);

  elseif (zn == 4) then
    door_in(71, 8, 67, 1, 75, 11);

  elseif (zn == 5) then
    door_in(61, 20, 57, 13, 65, 23);

  elseif (zn == 6) then
    door_in(71, 20, 67, 13, 75, 23);

  elseif (zn == 7) then
    door_in(61, 32, 57, 25, 65, 35);

  elseif (zn == 8) then
    door_in(71, 32, 67, 25, 75, 35);

  elseif (zn == 9) then
    door_out(13, 18);

  elseif (zn == 10) then
    door_out(18, 27);

  elseif (zn == 11) then
    door_out(27, 21);

  elseif (zn == 12) then
    door_out(34, 28);

  elseif (zn == 13) then
    door_out(40, 18);

  elseif (zn == 14) then
    door_out(30, 40);

  elseif (zn == 15) then
    touch_fire(party[0]);

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Who would want to steal from us?");
    else
      bubble(en, "I thought as much.");
    end

  elseif (en == 1) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "This is terrible.");
    else
      bubble(en, "I'm glad that's over.");
    end

  elseif (en == 2) then
    bubble(en, "That statue was given to the village after the evil Kalatos was defeated in the Great War.");

  elseif (en == 3) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "The village council says that it was a troll who stole the statue. What's a troll?");
    else
      bubble(en, "The statue is home.");
    end

  elseif (en == 4) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Apparently, there has not been a troll spotted in these parts since the Great War.");
    else
      bubble(en, "I wonder if our village is really safe?");
    end

  elseif (en == 5) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "I can't believe this has happened to us. We are a peaceful people.");
    else
      bubble(en, "Now, what to make for supper.");
    end

  elseif (en == 6) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Where did our statue go?");
    else
      bubble(en, "Things can get back to normal now.");
    end

  elseif (en == 7) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "The council would like to ask for your help in retrieving our statue. We will tell you what we know.");
      if (prompt(7, 2, 0, "However, I must first ask if",
                       "you are willing to help us?",
                       "  yes",
                       "  no") == 0) then
        bubble(en, "Oh, thank you very much! We will not forget your kindness.");
        bubble(en, "So, here is what we know. The thief is a troll. Our people have tracked the troll to a cave on the other side of the river.");
        bubble(en, "That is where he can be found.");
        bubble(HERO1, "Have your people tried to get the statue back themselves?");
        bubble(en, "Yes, we have tried, but we quickly learned that we were no match for this enemy.");
        bubble(en, "We could not even overcome the denizens of the cave, never mind the troll himself!");
        bubble(HERO1, "Are trolls common to this area? I have seen none in my travels.");
        bubble(en, "That is the strange part of all this. Trolls have not been seen in these parts for nearly a century!");
        bubble(en, "I must say that we suspect that someone is directing the troll in these matters.");
        bubble(HERO1, "Well, don't worry. We will get that statue back for you and get to the bottom of this whole thing.");
        bubble(en, "Oh, and if you haven't seen the cave yet... or even if you have, you'll need to know that there is a trick to opening the door.");
        bubble(en, "There is a set of stones off to the left of the entrace. You have to press them in the right order to gain access.");
        bubble(en, "Press the first stone, then the fourth and lastly the third. Then the door will open. Got it?");
        bubble(HERO1, "Uh... sure.");
        set_progress(P_DENORIAN, 2);
        return;
      else
        bubble(en, "I see. Well, then you should be on your way then. My people will show you out.");
        view_range(0, 0, 0, 0, 0);
        set_progress(P_DENORIAN, 1);
        change_map("main", 256, 77, 256, 77);
      end
    else
      if (get_progress(P_DENORIAN) == 2) then
        bubble(en, "Good luck.");
      end
      if (get_progress(P_DENORIAN) == 3 or get_progress(P_DENORIAN) == 4) then
        bubble(en, "This is incredible... you actually got the statue back. This is wonderful. I wish we had some way of paying you back.");
        bubble(en, "Unfortunately our village has next to nothing.");
        bubble(HERO1, "Don't worry... not everything we adventurers do is for a reward.");
        bubble(en, "What about the troll?");
        if (get_progress(P_DENORIAN) == 3) then
          bubble(HERO1, "Well, there was no sign of a troll. The thief was a Nerakian named Demnas. Have you ever heard of him?");
        else
          bubble(HERO1, "We found the troll. He was dead. He wasn't the real thief. The thief was a Nerakian named Demnas. Ever heard of him?");
        end
        bubble(en, "No, I can't say that I have. Well, I guess I'm a little relieved that we don't have a troll problem.");
        bubble(HERO1, "Well, yeah I guess that's good. And you don't have to worry about any crazed Nerakians for awhile either.");
        bubble(en, "That's good to hear. At the very least I can say that you will always be welcome in our village.");
        bubble(HERO1, "Thank you.");
        set_progress(P_DENORIAN, 5);
        return;
      end
      if (get_progress(P_DENORIAN) == 5) then
        bubble(en, "Good journey friend.");
      end
    end

  elseif (en == 8) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "This fellow sitting with me is the head of the village council. You should speak with him.");
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.");
    else
      bubble(en, "Thanks again.");
    end

  elseif (en == 9) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "You should hear what the head of the village council has to say.");
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.");
    else
      bubble(en, "Thanks again.");
    end

  elseif (en == 10) then
    if (get_progress(P_DENORIAN) == 0) then
      bubble(en, "We hope you will help us.");
    elseif (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Good luck.");
    else
      bubble(en, "Thanks again.");
    end

  elseif (en == 11) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "This is where the statue was taken from.");
    else
      bubble(en, "The statue will be put back soon.");
    end

  elseif (en == 12) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Less council meetings, more cooking!");
    else
      bubble(en, "Let the cookfires burn!");
    end

  elseif (en == 13) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "Geez, I wish I was old enough to go after that smelly old troll! I'd show him a thing or two.");
    else
      bubble(en, "Nerakians? Where are they from?");
    end

  elseif (en == 14) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "The council is having another meeting about what to do to get back our statue.");
    else
      bubble(en, "I think a celebration is in order.");
    end

  elseif (en == 15) then
    if (get_progress(P_DENORIAN) < 5) then
      bubble(en, "You should find the council's hut. They are looking for people to help get the statue back.");
    else
      bubble(en, "The council can relax now. Thank you.");
    end

  end
end
