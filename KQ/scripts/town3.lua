-- town3 - "Andra"

function autoexec()
  set_progress(P_EARLYPROGRESS, 3);
  if (get_progress(P_TALKDERIG) > 2) then
    set_ent_active(1,0);
  end
  if (get_treasure(12) == 1) then
    set_obs(12,41,0);
  end
end

function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    change_map("main",261,30,216,30);

  elseif (zn == 2) then
    door_in(69,9,59,0,79,12);

  elseif (zn == 3) then
    door_out(22,17);

  elseif (zn == 4) then
    door_in(66,51,59,42,73,53);

  elseif (zn == 5) then
    door_out(39,17);

  elseif (zn == 6) then
    door_in(73,60,69,55,77,63);

  elseif (zn == 7) then
    door_out(13,24);

  elseif (zn == 8) then
    door_in(63,37,59,28,67,40);

  elseif (zn == 9) then
    door_out(19,39);

  elseif (zn == 10) then
    door_in(63,60,59,55,67,63);

  elseif (zn == 11) then
    door_out(41,31);

  elseif (zn == 12) then
    door_in(67,23,59,14,75,26);

  elseif (zn == 13) then
    door_out(40,45);

  elseif (zn == 14) then
    bubble(HERO1,"Locked.","","","");

  elseif (zn == 15) then
    inn("Riverside Inn",35,1);

  elseif (zn == 16) then
    shop(6);

  elseif (zn == 17) then
    shop(7);

  elseif (zn == 18) then
    shop(8);

  elseif (zn == 19) then
    shop(9);

  elseif (zn == 20) then
    door_in(73,37,69,28,77,40);

  elseif (zn == 21) then
    door_out(19,52);

  elseif (zn == 22) then
    chest(12,0,1);
    autoexec();

  elseif (zn == 23) then
    chest(14,I_NPOULTICE,1);

  elseif (zn == 24) then
    book_talk(get_pidx(0));

  elseif (zn == 25) then
    chest(13,I_CAP2,1);

  elseif (zn == 26) then
  bubble(HERO1,"Various books about magic.","","","");

  elseif (zn == 27) then
    bubble(HERO1,"The art of battle-magic.","","","");

  elseif (zn == 28) then
    bubble(HERO1,"How magic can make you rich!","","","");

  elseif (zn == 29) then
    bubble(HERO1,"Ten reasons why you should never",
                 "call a wizard 'pencil-neck'.","","");

  elseif (zn == 30) then
    touch_fire(get_pidx(0));

  elseif (zn == 31) then
    bubble(HERO1,"Locked.","","","");

  end
end

function entity_handler(en)
  if (en == 0) then
    bubble(0,"This is the town of Andra.","","","");
    if (get_progress(P_TALKDERIG) == 1) then
      bubble(0,"Tsorin? He should be",
               "around here somewhere.","","");
    end

  elseif (en == 1) then
    if (get_progress(P_TALKDERIG) == 1) then
      bubble(1,"Yes, I am Tsorin. I understand",
               "that you've been talking with",
               "Derig?","");
      bubble(HERO1,"If you mean that old man back",
                   "near Ekla, then yes. He told me",
                   "to come here and see you.","");
      bubble(1,"That is because I am the only",
               "one who can get you through the",
               "mountain pass and into the",
               "goblin lands.");
      bubble(1,"We currently have the pass",
               "sealed off to protect Andra.","","");
      bubble(HERO1,"Why do I need to go there?","","","");
      bubble(1,"To find what you seek, you are",
               "going to have to visit the",
               "goblin Oracle. She has the",
               "information you require.");
      bubble(HERO1,"Why would a goblin help me?","","","");
      bubble(1,"She is an Oracle, don't think of",
               "her as a Goblin. She knows how",
               "important this quest is, and she",
               "has information about it.");
      bubble(1,"She will help. I'm sure of it.","","","");
      bubble(HERO1,"And if she doesn't? What do I",
                   "do then? Kill her?","","");
      bubble(1,"No, that would not be wise.",
               "Don't worry, if you can get into",
               "the tower, she will know just",
               "how dire your need is.");
      bubble(HERO1,"What's with this tower? Is it a",
                   "test of some sort, and by",
                   "passing the test, I am allowed",
                   "to talk to her?");
      bubble(1,"Sure, that's a good way of",
               "looking at it. I will tell you",
               "that the first part of the test",
               "is just getting into her tower.");
      bubble(1,"You can't enter without",
               "a goblin token.","","");
      bubble(HERO1,"Token?","","","");
      bubble(1,"Well, I didn't know how else to",
               "word it. Basically, you're going",
               "to need something goblin-made.","");
      bubble(1,"It could be anything, as long",
               "as it was made by goblins.","","");
      bubble(1,"And I think that it has",
               "to be magical.","","");
      bubble(HERO1,"So, how am I supposed to",
                   "find something magical that",
                   "was made by goblins?","");
      bubble(1,"I was getting to that. You'll",
               "have to go to the temple north",
               "of here.","");
      bubble(1,"You see, before the humans",
               "settled these lands, there used",
               "to be goblins here.","");
      bubble(1,"The humans forced the goblins",
               "out of these lands. That's why",
               "things are still really bitter",
               "between our two peoples.");
      bubble(1,"Anyways, the humans went about",
               "conquering and settling, and in",
               "time, they built a temple to the",
               "north of town.");
      bubble(1,"Why they chose to build it there",
               "is beyond me, but unfortunately,",
               "it turns out that the temple was",
               "constructed over an old tomb.");
      bubble(1,"The tomb is a goblin holy place.",
               "The tomb belongs to the first",
               "king of the goblins who died",
               "over six hundred years ago.");
      bubble(1,"Well, after a few months, the",
               "priests at the temple began",
               "disappearing. It was discovered",
               "that spirits were to blame.");
      bubble(1,"There are strange spirits all",
               "over this land, but these were",
               "the enraged spirits of goblins",
               "who called the tomb home.");
      bubble(1,"In short, the temple is now",
               "empty and guarded by goblin",
               "spirits. It is extremely",
               "dangerous inside.");
      bubble(1,"However, the temple is the only",
               "place where you will find the",
               "token that you need.","");
      bubble(HERO1,"Well... if there's no other way",
                   "I guess I'm on my way to the",
                   "temple then.","");
      bubble(1,"By the way... I must tell you",
               "that you can only get to the",
               "lower levels of the temple by",
               "activating a hidden switch.");
      bubble(1,"The switch is hidden on an",
               "altar. The altar room should be",
               "easy to find.","");
      bubble(HERO1,"Okay. Anything else?","","","");
      bubble(1,"Not really, just come back here",
               "afterwards, and I'll get you",
               "through the mountain pass.","");
      set_progress(P_TALKDERIG,2);
      return
    end
    if (get_progress(P_TALKDERIG) == 2 and get_progress(P_GOBLINITEM) == 1) 
then
      set_progress(P_TALKDERIG,3);
      bubble(1,"Hey... you did it! Fantastic!",
               "Let's head to the fort.","","");
      do_fadeout(4);
      set_holdfade(1);
      change_map("fort",38,16,38,16);
    end
    if (get_progress(P_TALKDERIG) == 0) then
      bubble(1,"I am the captain of the guard. I ",
               "should warn you to stay away",
               "from the goblin lands to the",
               "south.");
      bubble(1,"In fact, for everyone's safety,",
               "we've sealed off the pass.","","");
      return
    end
    if (get_progress(P_GOBLINITEM) == 0) then
      bubble(1,"Any luck at the temple yet?","","","");
    end

  elseif (en == 2) then
    bubble(2,"I wish the weapon shop",
             "sold slingshots.","","");

  elseif (en == 3 or en == 4) then
    bubble(en,"We're guarding against goblins.","","","");

  end
end

