-- town1 - "Ekla"

-- TT todo:
-- Reflect NPCs scripts to accommodate theory that monsters appeared
-- out of nowhere.  Unadium-coin lady will not help until after you find
-- her father/grandfather.
--
-- When we talk to townspeople, they should talk about the monster influx.
-- 
--
-- P_EARLYPROGRESS: Used when talking to Derig in the Grotto.
--   0 - Have not yet entered Ekla
--   1 - Entered Ekla
--   2 - Entered Randen
--   3 - Entered Andra
-- P_START: Monsters will not attack you until you enter and leave Ekla
--   0 - Have not yet entered Ekla
--   1 - Entered Ekla; monsters will now attack randomly on world map
-- P_WARPSTONE: The teleporter from Ajantara <-> Randen
--   0 - Have not used it yet
--   1 - Stepped on the warp stone and created its counterpart in Randen
-- P_EKLAWELCOME: Corny welcome message when you talk to man in Ekla
--   0 - He had not yet given you his corny "Yes! This makes 8!" speech
--   1 - Now he likes cheese
-- P_DARKIMPBOSS: Monster blocking stairway to Randen
--   0 - Still there
--   1 - You defeated it
-- P_PORTALGONE: Whether the portal in the tunnel is still working
--   0 - Still letting monsters through
--   1 - You touched Rod of Cancellation to it
-- P_UCOIN: Received Unadium Coin from old woman in house
--   0 - Have not yet talked to woman
--   1 - Declined her offer to help get rid of Tunnel Portal
--   2 - Accepted her offer to help
--   3 - Received Coin (unnecessary, since it immediately follows 2)
--   4 - Returned Coin; received SunStone
-- P_CANCELROD: Possession of Rod of Cancellation
--   0 - Do not have it
--   1 - Got it


function autoexec()
  set_progress(P_EARLYPROGRESS, 1);
  refresh()
end


function refresh()
  -- Chest in magic shop
  if (get_treasure(0) == 1) then
    set_mtile(63, 27, 265);
  end
  -- Monsters will not attack on the world map until this == 1
  if (get_progress(P_START) == 0) then
    set_progress(P_START, 1);
  end
  if (get_progress(P_WARPSTONE) == 1) then
    set_ent_active(1, 0);
    set_ent_active(3, 0);
  end
end


function postexec()
  return;
end


function zone_handler(zn)
  if (zn == 1) then
    change_map("main", 132, 30, 132, 30);

  elseif (zn == 2) then
    inn("The Blue Boar Inn", 25, 1);

  elseif (zn == 3) then
    shop(0);

  elseif (zn == 4) then
    shop(1);

  elseif (zn == 5) then
    shop(2);

  elseif (zn == 6) then
    bubble(HERO1, "Locked.");

  elseif (zn == 7) then
    change_map("cave1", 0, 0, 0, 0);

  elseif (zn == 8) then
    chest(0, I_B_SCORCH, 1);
    refresh();

  elseif (zn == 9) then
    bubble(255, "I don't have much.");
    shop(3);

  elseif (zn == 10) then
    door_in(60, 20, 52, 11, 64, 22);

  elseif (zn == 11) then
    door_out(20, 19);

  elseif (zn == 12) then
    door_in(81, 33, 76, 24, 88, 35);

  elseif (zn == 13) then
    door_out(18, 40);

  elseif (zn == 14) then
    door_in(56, 33, 52, 24, 60, 35);

  elseif (zn == 15) then
    door_out(14, 45);

  elseif (zn == 16) then
    door_in(73, 19, 66, 11, 79, 22);

  elseif (zn == 17) then
    door_out(34, 37);

  elseif (zn == 18) then
    door_in(68, 31, 62, 24, 74, 33);

  elseif (zn == 19) then
    door_out(31, 27);

  elseif (zn == 20) then
    chest(6, I_NLEAF, 1);

  elseif (zn == 21) then
    book_talk(party[0]);

  elseif (zn == 22) then
    door_in(62, 54, 52, 41, 69, 57);

  elseif (zn == 23) then
    door_out(37, 56);

  elseif (zn == 24) then
    touch_fire(party[0]);

  end
end


function entity_handler(en)
  if (en == 0) then
    if (get_progress(P_WARPSTONE) == 1) then
      if (get_progress(P_EKLAWELCOME) > 0) then
        bubble(0, "Welcome back.");
        return;
      else
        bubble(0, "You're in Ekla, genius. But do you care?");
        bubble(HERO1, "Huh? What's your problem?");
        bubble(0, "Problem? On now I have a problem! First you completely ignore me. Now you come running to me for help.");
        bubble(HERO1, "Whoa, down Rover. Get over yourself. I was probably just busy.");
        bubble(0, "Rover? ROVER?! Oh, am I someone's house pet now?");
        bubble(HERO1, "Well, you're some sort of farm animal. And I'm not just referring to the smell.");
        if (get_numchrs() > 1) then
          bubble(HERO2, "Yea, and if you don't back off, we're going to have to put you down like one. Understand?");
        else
          bubble(HERO1, "And if you don't back off, I'm going to have to put you down like one. Got it?");
        end
        wait(50);
        bubble(0, "...");
        bubble(HERO1, "Good. Glad we have that understanding. Now, do you have anything else to say?");
        set_progress(P_EKLAWELCOME, 1);
        return;
      end
    end
    if (get_progress(P_EKLAWELCOME) == 0) then
      bubble(0, "Welcome to the town of Ekla.");
      bubble(0, "Alright! You make eight. If I welcome enough newcomers to this town, I'll get promoted.");
      bubble(0, "I might get a job sitting in a house all day saying the same thing over and over to anyone who talks to me.");
      bubble(0, "I should start practicing.");
      wait(25);
      set_progress(P_EKLAWELCOME, 1);
      if (party[0] == CASANDRA) then
        thought(HERO1, "Geez! What a loser!");
        thought(HERO1, "Maybe I should put him out of his misery.");
      end
    else
      bubble(0, "I like cheese.")
    end

  elseif (en == 1) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(1, "If you are going underground be warned that there are a great number of beasties roaming around down there as of late.");
      bubble(1, "Before attempting to pass through there make sure you have some good armor and a strong weapon.");
      bubble(1, "As well, if you don't know any magic, I suggest you learn some.");
    else
      if (get_progress(P_PORTALGONE) == 1) then
        bubble(1, "The passage should be safe now.");
      else
        bubble(1, "The tunnel is still dangerous, so be careful.");
      end
    end

  elseif (en == 2) then
    if (get_progress(P_WARPSTONE) == 1) then
      bubble(2, "How are things going?");
      return;
    end
    if (get_progress(P_UCOIN) == 4) then
      if (get_treasure(45) == 0) then
        bubble(2, "Here... you didn't take this before.");
        chest(45, I_SSTONE, 1);
      else
        bubble(2, "Thanks again.");
      end
      return;
    end
    if (get_progress(P_UCOIN) == 3) then
      if (get_progress(P_CANCELROD) > 0) then
        if (get_progress(P_PORTALGONE) > 0) then
          bubble(2, "Excellent! You've done a very good job. And thanks for bringing back my coin!");
          bubble(2, "Here, you should have this.");
          chest(45, I_SSTONE, 1);
          set_progress(P_UCOIN, 4);
        else
          bubble(2, "Thanks for bringing my coin back. Good luck in the tunnel and hopefully that portal will soon be gone.");
          bubble(2, "Here, you should have this.");
          chest(45, I_SSTONE, 1);
          set_progress(P_UCOIN, 4);
        end
      else
        bubble(2, "Please remember to bring back the coin when you're done.",
                 "Good luck.");
      end
    elseif (get_progress(P_UCOIN) == 1) then
      if (prompt(2, 2, 0, "Change your mind?",
                          "  yes",
                          "  no") == 0) then
        set_progress(P_UCOIN, 2);
        LOC_old_lady();
      else
        bubble(2, "Then what are you still doing here?");
      end
    elseif (get_progress(P_UCOIN) == 0) then

      if (prompt(2, 2, 0, "Say, have you been through",
                          "the passage lately?",
                          "  yes",
                          "  no") == 0) then
        if (prompt(2, 2, 0, "Then you saw the portal?",
                            "  yep",
                            "  nope") == 0) then
          bubble(2, "Well, you could do the town a big favour and get rid of it for us.");
          if (prompt(2, 2, 0, "I can tell you how if you're",
                              "interested.",
                              "  sure",
                              "  sorry") == 0) then
            set_progress(P_UCOIN, 2);
            LOC_old_lady();
          else
            bubble(2, "Oh. Then I'd appreciate it if you would get out of my house.");
            set_progress(P_UCOIN, 1);
          end
        else
          bubble(2, "Really? You didn't explore very much of the tunnel then did you?");
          bubble(2, "Regardless, there is a magical portal in the tunnel. The portal is how all of the monsters got there.");
          bubble(2, "Anyways, there is a way to get rid of the monsters and put an end to this mess.");
          if (prompt(2, 2, 0, "Would you be interested in helping",
                              "out our little town?",
                              "  sure",
                              "  sorry") == 0) then
            set_progress(P_UCOIN, 2);
            LOC_old_lady();
          else
            bubble(2, "Oh. Then I'd appreciate it if you would get out of my house.");
            set_progress(P_UCOIN, 1);
          end
        end
      else
        bubble(2, "Well, I guess that's not important. You'll go there eventually, I suppose.");
        bubble(2, "If you've been around town, then you likely know that the tunnel is full of monsters!");
        bubble(2, "The monsters are coming through a magical portal that appeared just recently. It's unknown how it got there, but it has to go.");
        if (prompt(2, 2, 0, "Would you be interested in trying",
                            "to get rid of this portal?",
                            "  yes",
                            "  no") == 0) then
          set_progress(P_UCOIN, 2);
          LOC_old_lady();
        else
          bubble(2, "Oh. Then I'd appreciate it if you would get out of my house.");
          set_progress(P_UCOIN, 1);
        end
      end
    end

  elseif (en == 3) then
    if (get_progress(P_DARKIMPBOSS) == 0) then
      bubble(3, "We're here investigating the recent increase in monsters in the underground passage.");
      if (party[0] == NOSLOM) then
        bubble(HERO1, "And what have you discovered?");
        bubble(3, "Um... that there are more monsters than usual down there.");
        bubble(HERO1, "Oh... good work then.");
        bubble(3, "Thank you!");
        thought(HERO1, "Who hired these guys?");
      end
    else
      if (get_progress(P_PORTALGONE) == 0) then
        bubble(3, "So... it was a dark imp that was blocking the underground passage. Interesting.");
      else
        bubble(3, "A magical portal you say... that sounds very suspicous indeed!");
      end
    end

  end
end


function LOC_old_lady()
  if (get_progress(P_UCOIN) == 2) then
    bubble(2, "Great! Take this.");
    wait(25);
    set_progress(P_UCOIN, 3);
    sfx(5);
    msg("Unadium coin procured", 255, 0);
    bubble(2, "In the grotto north of town there is a metal rune in the ground.");
    bubble(2, "If you are carrying anything made from Unadium, you can teleport into the ruins beyond.");
    bubble(2, "In the ruins you should find a Rod of Cancellation. Use the rod to dispel the portal in the tunnel.");
    bubble(2, "That will stop any new monsters from coming through.");
    wait(25);
    bubble(2, "Oh, and please bring back the coin when you are done.");
    bubble(2, "There are only seven of them in existence and it's very precious to me.");
    if (party[0] == AYLA) then
      thought(HERO1, "Heh, as if!");
    elseif (party[1] == AYLA) then
      thought(HERO2, "Heh, as if!");
    end
  end
end
