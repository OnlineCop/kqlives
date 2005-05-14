-- town5 - "Sunarin"

-- /*
-- This is where the entrance to the Embers Guild is located.
-- You will meet Ayla (PM #5), and she'll join you if you help her with the
-- quest.
--
-- It is also the start of Ayla's quest - robbing the palace in the inner city.
-- Controlled by P_AYLA_QUEST:
--   0: Nothing done
--   1: Talked to the girl near the gates
--   2: Talked to the folks in the bar
--   3: Talked to both
--   4: Got the disguise and ready to go
--   5: Finished/given up
--*/

--/*
-- Entity table:
-- 0. Townsperson
-- 1. Townsperson
-- 2. Townsperson
-- 3. Townsperson
-- 4. Disgruntled woman in bar
-- 5. Girl near palace gates
-- 6. Townsperson
-- 7. Dungar
-- 8. Soldier
--*/


function autoexec()
  if (get_progress(P_TALKGELIK) ~= 2) then
    set_ent_active(7, 0)
  end
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_desc(0)
  end
  set_ent_active(8, 0)
  refresh()
end


function refresh()
  if (get_treasure(32) == 1) then
    set_obs(21, 27, 0)
  end
end


function postexec()
  if (get_progress(P_FOUGHTGUILD) == 2) then
    set_progress(P_FOUGHTGUILD, 3)
    set_progress(P_EMBERSKEY, 0)
    bubble(HERO1, "Oh no!")
    bubble(HERO1, "I must have dropped that key inside!")
    bubble(HERO1, "Oh well, I got what I came for.")
  end
end


function zone_handler(zn)
  if (zn == 1) then
    LOC_check_costume()
    change_map("main", 247, 115, 247, 115)

  elseif (zn == 2) then
    bubble(HERO1, "Locked.")

  elseif (zn == 3) then
    door_in(65, 16, 61, 5, 77, 19)

  elseif (zn == 4) then
    door_in(73, 16, 61, 5, 77, 19)

  elseif (zn == 5) then
    door_in(65, 36, 61, 25, 75, 39)

  elseif (zn == 6) then
    door_in(71, 36, 61, 25, 75, 39)

  elseif (zn == 7) then
    door_out(24, 12)

  elseif (zn == 8) then
    door_out(28, 12)

  elseif (zn == 9) then
    door_out(37, 34)

  elseif (zn == 10) then
    door_out(42, 34)

  elseif (zn == 11) then
    inn("River's Way Inn", 70, 1)

  elseif (zn == 12) then
    shop(13)

  elseif (zn == 13) then
    shop(14)

  elseif (zn == 14) then
    bubble(HERO1, "Worst books ever.")

  elseif (zn == 15) then
    bubble(HERO1, "What language is this?")

  elseif (zn == 16) then
    chest(32, I_PRIESTESS, 1)
    refresh()

  elseif (zn == 17) then -- Palace gates
    if (party[0] == Ayla) then
      LOC_ayla_gates()
    else
      bubble(HERO1, "The gates are closed and locked.")
    end

  elseif (zn == 18) then
    if (get_progress(P_EMBERSKEY) == 0) then
      bubble(HERO1, "Locked.")
    else
      -- LOC_change_costume()
      bubble(HERO1, "Key goes in...")
      change_map("guild", 0, 0, 0, 0)
    end

  elseif (zn == 19) then
    touch_fire(party[0].id)

  elseif (zn == 20) then
    -- clothes chest in cottage
    if (party[0] == Ayla) then
      if (get_progress(P_AYLA_QUEST) < 4 and get_progress(P_AYLA_QUEST) > 0) then
        sfx(5)
        msg("Maid's outfit procured", 12, 0)
        wait(200)
        bubble(HERO1, "Do you mind? I'm getting undressed!")
        do_fadeout(4)
        set_holdfade(1)
        set_ent_chrx(HERO1, 8)
        wait(200)
        set_holdfade(0)
        do_fadein(4)
        thought(HERO1, "It's a little tight around the arms...")
        set_progress(P_AYLA_QUEST, 4)
      else
        thought(HERO1, "Nothing here worth stealing")
      end
    else
      thought(HERO1, "Nothing in here but old clothes")
    end

  elseif (zn == 21) then
    -- exit cottage
    door_out(37, 17)

  elseif (zn == 22) then
    --enter cottage
    door_in(67, 45, 64, 39, 71, 47)
  end
end


function entity_handler(en)
  if (en == 0) then
    bubble(en, "Do you like this shirt?")

  elseif (en == 1) then
    bubble(en, "I really like that guy's shirt.")

  elseif (en == 2) then
    bubble(en, "I have to stop bringing my wife with me.")

  elseif (en == 3) then
    bubble(en, "I must have had too many drinks.",
              "My husband is starting to look handsome.")

  elseif (en == 4) then
    if (party[0] == Ayla) then
      LOC_ayla_bar(en)
    else
      bubble(en, "These two are weird... but at least I don't have to sit alone.")
    end

  elseif (en == 5) then
    if (party[0] == Ayla) then
      LOC_ayla_girl(en)
    else
      bubble(en, "Those gates to the inner city never open. Strangers are not allowed in.")
    end

  elseif (en == 6) then
    bubble(en, "Welcome to Sunarin!",
              "Please feel free to leave at any time.")

  elseif (en == 7) then
    set_progress(P_TALKGELIK, 3)
    if (get_progress(P_AYLA_QUEST) == 4) then
      bubble(en, "Help... go and get me something to drink...")
      LOC_check_costume()
      bubble(HERO1, "...now, what were you saying?")
    end
    bubble(en, "Please... help me... I brought soldiers with me... but the Embers killed them. I barely escaped with my life!")
    bubble(HERO1, "Who are you? And who are the Embers?")
    bubble(en, "My name is Dungar. The Embers is the short name for the Thieves of the Ember Crest. They run this town.")
    bubble(en, "I came to get back something they stole from me...")
    bubble(HERO1, "The Opal Helmet?")
    bubble(en, "Why yes! The Opal Helmet.",
               "Who are you? You're not one of them are you?")
    bubble(HERO1, "No. My name is $0. I am on a quest, and I need to borrow that helmet to traverse the underwater passage.")
    bubble(en, "I see. Well, if you can get it back, I will certainly let you use it.")
    bubble(HERO1, "Well, how do we get it back?")
    bubble(en, "We can discuss that back at my place. It is not safe here.")
    bubble(HERO1, "Okay.")
    set_progress(P_EMBERSKEY, 1)
    change_map("estate", 57, 17, 57, 17)
  end

end


-- Conversations between Ayla and the girl near the palace gates
function LOC_ayla_girl(girl)
  local pp
  pp = get_progress(P_AYLA_QUEST)
  if (pp == 0) then
    bubble(girl, "Those gates to the inner city never open. Strangers are not allowed in.")
    bubble(HERO1, "I suppose ... there must be some valuable stuff in there?")
    bubble(girl, "Oh, yes, this town should be prosperous, but our ruler takes all our gold in taxes and keeps it walled up in there.")
    bubble(girl, "That's why everyone here is so surly.")
    bubble(HERO1, "You said strangers weren't allowed in. Can't locals come and go as they please?")
    bubble(girl, "Only servants, they ... hey, why are you so interested?")
    bubble(HERO1, "No reason. I'm just passing through this town and I was curious, that's all.")
    thought(HERO1, "Hmm... curious about the riches they've got in there. Surely the mission can wait for a while.")
    set_progress(P_AYLA_QUEST, 1)
  elseif (pp == 1) then
    bubble(girl, "You know, I'll call the guards if I see you round here again.")
  elseif (pp == 2) then
    bubble(girl, "Those gates to the inner city never open. Strangers are not allowed in.")
    bubble(HERO1, "Can you show me where the servant's entrance is, then?")
    bubble(girl, "Why would you want to know a thing like that?")
    bubble(HERO1, "Erm... it's for a book I'm writing. That's it, I'm a famous author.")
    bubble(girl, "Well, it won't be a very interesting book, then. Servants just knock on the gates and the guard lets them in.")
    bubble(HERO1, "Thanks very much. I'll be sure to put you in the credits.")
    thought(HERO1, "Every so often I am amazed by the gullibility of my fellow humans.")
    set_progress(P_AYLA_QUEST, 3)
  elseif (pp == 3) then
    bubble(girl, pick("You're always hanging around here. I'm becoming a little suspicious",
           "Have you been talking to those losers in the bar? You stink of beer!",
           "Whatever you want, I'm not interested."))
  elseif (pp == 4) then
    bubble(girl, "Hello. You must be a new employee. But you look strangely familiar...")
  end
end


-- Conversations between Ayla and the single woman in the bar
function LOC_ayla_bar(woman)
  local pp
  pp = get_progress(P_AYLA_QUEST)
  if (pp == 0) then
    bubble(woman, "Hello there! I'd buy you a drink but I'm broke.")
    bubble(HERO1, "I'd buy you one but it looks like you've had a few already. Goodbye...")
    bubble(woman, "Don't be like that. I've just had to pay out half my gold in taxes.")
    bubble(HERO1, "What did you say?")
    bubble(woman, "Our ruler takes all our wealth and keeps it in the Inner City.",
           "Only the servants get to see what goes on in there.",
           "That woman over there, she's a maid and she gets to live in a nice cottage just opposite Vezdin's shop.")
    bubble(HERO1, "She's certainly got some money to spend on drink.")
    thought(HERO1, "I might just pay a little visit to that cottage while the maid's in here.")
    set_progress(P_AYLA_QUEST, 2)
    bubble(woman, "I know, it's just unfair!")
  elseif (pp == 1) then
    bubble(HERO1, "I don't suppose you work in the palace?")
    bubble(woman, "I wish. That woman over there is a maid. Only a maid and she lives in a lovely cottage opposite Vezdin's shop.")
    bubble(HERO1, "Do  you know anything about the wealth that's supposed to be in there?")
    bubble(woman, "Keep your voice down! I bet you've been talking to that little miss near the palace gates, haven't you?")
    bubble(HERO1, "Yes, but I much prefer the company in here.")
    set_progress(P_AYLA_QUEST, 3)
  elseif (pp == 2) then
    bubble(woman, pick("Y'know, you're my best friend!",
                  "I love this bar - I just wish it would stop spinning.",
                  "My boyfriend used to say: Work is the curse of the drinking classes. That's funny, don't you think?"))
  elseif (pp == 3) then
    bubble(woman, "I always talk too much when I've been drinking. Don't pay any attention to what I said.")
  elseif (pp == 4) then
    bubble(woman, "Oh, I am honoured! Haven't you got better things to do over at the palace?")
  end
end


function LOC_ayla_gates()
  bubble(255, "WHO GOES THERE?!?")
  if (get_ent_chrx(HERO1) ~= 0) then
    bubble(HERO1, "Just a little serving-girl, reporting for duty.")
    bubble(255, "Wait there, I'm coming.")
    set_ent_active(8, 1)
    set_ent_script(8, "L10")
    wait_for_entity(8, 8)
    bubble(8, "Come through.")
    sfx(25)
    warp(50, 23, 8)
    set_ent_facing(HERO1, FACE_DOWN)
    set_ent_facing(8, FACE_UP)
    bubble(8, "Well, you are a cute little thing, aren't you?")
    bubble(HERO1, "Cute??")
    bubble(8, "I'm sorry, miss, I didn't mean to offend. Follow me.")
    ----
    bubble(255, "CUT!")
    bubble(255, "That's a wrap for today.",
                "PH hasn't written the rest of this script yet!")
    set_vfollow(0)
    set_ent_script(HERO1, "R12")
    set_ent_script(8, "R12")
    bubble(255, "Ayla, great work. Sir Alec, you were fabulous, sweetie.")
    bubble(8, "I'm still not sure of my motivation in this scene...")
    wait_for_entity(HERO1, 8)
    LOC_check_costume()
    change_map("main", 247, 115, 247, 115)
    set_vfollow(1)
  else
    bubble(HERO1, "I was wondering if you did tours around the castle?")
    bubble(255, "I will show you the inside of the dungeon if you bother me again. Clear off!")
    bubble(HERO1, "I'll take that as a 'no', then.")
  end
end


function LOC_check_costume()
  if (get_ent_chrx() ~= 0) then
    bubble(HERO1, "Just wait a second whilst I change out of this costume.")
    do_fadeout(4)
    set_holdfade(1)
    set_ent_chrx(HERO1, 0)
    set_progress(P_AYLA_QUEST, 5)
    wait(100)
    set_holdfade(0)
    do_fadein(4)
    return
  end
end
