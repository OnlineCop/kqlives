-- coliseum - coliseum west of Sunarin

function autoexec()
  set_ent_active(12,0);
  if (get_progress(59) > 0) then
    place_ent(6,67,20);
  end
end
  
function postexec()
  return
end

function zone_handler(zn)
  if (zn == 1) then
    set_progress(59,0);
    set_progress(60,0);
    change_map("main",218,123,218,123);
  
  elseif (zn == 2) then
    door_in(72,11,62,0,82,14);
  
  elseif (zn == 3) then
    door_in(66,30,62,16,70,33);
  
  elseif (zn == 4) then
    door_in(79,27,72,16,86,30);
  
  elseif (zn == 5) then
    door_out(20,21);
  
  elseif (zn == 6) then
    door_out(30,22);
  
  elseif (zn == 7) then
    door_out(40,20);
  
  elseif (zn == 8) then
    inn("The Coliseum Inn",250,1);
  
  elseif (zn == 9) then
    shop(22);
  
  elseif (zn == 10) then
    if (get_progress(60) > 0) then
      bubble(200,"I'm not ready yet.","","","");
      return
    end
    view_range(1,86,1,88,3);
    warp(87,2,16);
    set_run(0);
    set_progress(61,1);
    combat(11+get_progress(59));
    set_progress(61,0);
    set_run(1);
    view_range(1,62,16,70,33);
    warp(66,20,16);
    if (get_alldead() == 1) then
      set_progress(60,2);
      set_party_hp(get_pidx(0),1);
      set_alldead(0);
    else
      set_progress(60,1);
    end

  elseif (zn == 11) then
    set_save(0);

  elseif (zn == 12) then
    set_save(1);

  end
end
  
function entity_handler(en)
  if (en == 0) then
    bubble(0,"I actually made it to",
             "the second round!","","");
  
  elseif (en == 1) then
    bubble(1,"I'm out of practice.","","","");
  
  elseif (en == 2) then
    bubble(2,"I can't stand waiting like this!","","","");
  
  elseif (en == 3) then
    bubble(3,"There's not enough women",
             "in these competitions.","","");
  
  elseif (en == 4) then
    bubble(4,"I came all this way and then",
             "found out that I didn't have",
             "enough money for the",
             "registration fee!");
  
  elseif (en == 5) then
    bubble(5,"I'm waiting for my next match","","","");
  
  elseif (en == 6) then
    local a,b;
  
    if (get_progress(50) == 1) then
      bubble(6,"This is where you register.","","","");
      bubble(6,"Hey, wait a minute... you're the",
               "one who defeated Trayor. We",
               "can't let you register again...",
               "you're too good for these guys.");
      return
    end
    if (get_progress(59) == 0) then
      bubble(6,"This is where you register.","","","");
      if (get_progress(58) == 0) then
        bubble(6,"Before I explain the rules, I",
                 "should tell you that the", 
                 "registration fee is 2000 gp!","");
        bubble(6,"If you lose even once, you are",
                 "done... and will have to",
                 "register again.","");
        bubble(6,"As well, after you register",
                 "you can't leave the coliseum",
                 "grounds. If you leave, you will",
                 "be forced to register again.");
        bubble(6,"Now, let's talk about",
                 "the actual fights!","","");
        bubble(6,"The first three battles are",
                 "more or less just preliminary",
                 "fights to make sure you have what",
                 "what it takes.");
        bubble(6,"If you win the first three rounds,",
                 "you qualify for more advanced",
                 "matches.","");
        bubble(6,"The more advanced matches are",
                 "against more skilled opponents",
                 "and will determine whether or not",
                 "your skills are balanced enough.");
        bubble(6,"If you manage to defeat the",
                 "advanced warriors, you will be",
                 "given a chance to fight Trayor",
                 "for the grand prize.");
        a = prompt(6,2,0,"So, are you willing to pay the",
                              "2000 gp entrance fee?",
                              "  no",
                              "  yes");
        set_progress(58,1);
      else
        a = prompt(6,2,0,"The registration fee is 2000 gp.",
                              "Are you in?",
                              "  no",
                              "  yes");
      end
      if (a == 0) then
        bubble(6,"Oh well. Come back",
                 "when you're ready.","","");
      else
        if (get_gp() >= 2000) then
        
          set_gp(get_gp()-2000);
          bubble(6,"Consider yourself registered.",
                   "After each battle come back",
                   "and talk to me.","");
          set_ent_script(6,"U2R1F2");
          wait_for_entity(6,6);
          set_progress(59,1);
          set_progress(60,0);
        else
          bubble(6,"Umm... you don't seem to have",
                   "enough. I would suggest selling",
                   "some junk. In any case, you",
                   "can't register at this point.");
        end
      end
    else
      a = get_progress(60);
      if (a == 0) then
        if (get_progress(59) == 7) then
          bubble(6,"Wow! This is it... you're next",
                   "battle is with Trayor. Good",
                   "luck... you'll need it!","");
        else
          b = "Battle number "..get_progress(59).." just";
          bubble(6,b,"head on through that door",
                     "when you are ready.","");
        end
      elseif (a == 1) then
        if (get_progress(59) == 7) then
          bubble(6,"Astounding! Whoever you are,",
                   "you're going to go far!","","");
          bubble(6,"Well, I guess this is yours.","","","");
          sfx(5);
          msg("Opal Shield procured",255,0,xofs,yofs);
          set_progress(50,1);
          set_ent_script(200,"D3F1");
          wait_for_entity(200,200);
          set_ent_script(6,"L1D2");
          wait_for_entity(6,6);
          bubble(6,"Good luck in your endeavours.","","","");
          if (get_progress(40) > 0) then
            set_progress(62,get_progress(40));
          else
            if (get_pidx(0) == 4) then
              if (krnd(10) < 5) then
                set_progress(62,5 + 1);
              end
            end
            if (get_pidx(0) == 5) then
              if (krnd(10) < 5) then
                set_progress(62,4 + 1);
              end
            end
            while (get_progress(62) == 0) do
              z = krnd(8);
              if (z ~= get_pidx(0)) then
                set_progress(62,z+1);
              end
            end
          end
          set_ent_id(12,get_progress(62)-1);
          set_ent_active(12,1);
        else
          bubble(6,"Congratulations! You should",
                   "have a rest or heal up. Come",
                   "back and talk to me again after",
                   "you've prepared.");
          set_progress(60,0);
          set_progress(59,get_progress(59)+1);
        end
      elseif (a == 2) then
        bubble(6,"Ooh... tough luck!","","","");
        set_progress(60,0);
        set_progress(59,0);
        set_ent_script(200,"D3F1");
        wait_for_entity(0,0);
        set_ent_script(6,"L1D2");
        wait_for_entity(6,6);
        play_map_song();
      end
    end
  
  elseif (en == 7) then
    bubble(7,"Losing isn't so bad.","","","");
    bubble(7,"No wait, let me re-phrase that!","","","");
    bubble(7,"Losing sucks!","","","");
  
  elseif (en == 8) then
    bubble(8,"What a waste of money.","","","");
    bubble(200,"How many matches did you fight?","","","");
    bubble(8,"Matches? I haven't",
             "left the bar yet!","","");
  
  elseif (en == 9) then
    bubble(9,"These fights are",
             "quite interesting.","","");
  
  elseif (en == 10) then
    bubble(10,"Wine puts losing in a",
              "whole new perspective.","","");
  
  elseif (en == 11) then
    bubble(11,"The Coliseum hasn't been the",
              "same since they discontinued",
              "betting on the fights.","");
  
  elseif (en == 12) then
    if (get_progress(40) == 0) then
      bubble(12,"Wow! I saw you fighting.",
                "You cleaned up!","","");
      bubble(12,"Oh yeah! I'm here because I ran",
                "into some guy named Derig. Well,",
                "he actually came looking for me.","");
      bubble(12,"Anyways, he wanted me",
                "to come and find you.","","");
      bubble(200,"Derig! What does he want now?","","","");
      bubble(12,"He said that he found out where",
                "all of the Opal armor is and he",
                "sent me to tell you.","");
      bubble(12,"He also figured that if you",
                "didn't have a partner by now,",
                "that I should make my services",
                "available to you.");
      bubble(12,"And let me just say that after",
                "travelling all this way to find",
                "you... I won't take no for an",
                "answer!");
      bubble(200,"Well, in that case, welcome to",
                   "the team.","","");
      bubble(200,"Now, I already found the Opal",
                   "Helmet and the Opal Shield.","","");
      bubble(12,"So what about the other stuff?",
                "Do you already know where the",
                "Band and Armor are?","");
      bubble(200,"No... no, I don't know where",
                   "the other pieces are.","","");
      bubble(12,"Alrighty then, down to business.",
                "The Band belongs to a merchant",
                "in Ajantara south of here.","");
      bubble(12,"The Armor however, is hidden",
                "deep in a cavern east of here.","","");
      bubble(200,"Well, where should we go first?","","","");
      bubble(12,"Derig said to get the Armor last",
                "and since all that leaves is the",
                "Band, then we should go for",
                "that.");
      bubble(200,"Fair enough... let's roll.","","","");
      add_chr(get_progress(62)-1);
      LOC_partner_up();
      set_progress(62,0);
      copy_ent(12,201);
      set_ent_active(12,0);
      orient_heroes();
      drawmap();
      screen_dump();
      msg("$1 joined!",255,0);
    else
      bubble(12,"Hey... long time no see.  I saw",
                "you fighting. You were amazing!","","");
      bubble(12,"Oh yeah! I'm here because I ran",
                "into some guy named Derig. Well,",
                "he actually came looking for me.","");
      bubble(12,"He knew that we were separated",
                "and he gave me some information",
                "to bring back.","");
      bubble(200,"Speaking of that...",
                   "where did you go?","","");
      bubble(12,"Heh... when we hit the",
                "teleporter, it sent me",
                "back to Ekla.","");
      bubble(12,"I was on the way back to the",
                "teleporter when I ran into",
                "Derig... and now I'm here.","");
      bubble(200,"So, what did Derig tell you?","","","");
      bubble(12,"He told me where all the Opal",
                "stuff is... the Shield, the",
                "Band, the Armor, and the Helmet.","");
      bubble(200,"I already snagged the Helmet and",
                   "the Shield... where are the",
                   "other parts?","");
      bubble(12,"You already found the Helmet and",
                "Shield! Wow! Well, the Band is",
                "in a town called Ajantara to the",
                "south.");
      bubble(12,"The Armor is deep in a cavern to",
                "the east. And Derig said to get",
                "the Armor last, so we should go",
                "to Ajantara first. Okay?");
      bubble(200,"No problem... let's go.","","","");
      add_chr(get_progress(62)-1);
      LOC_partner_up();
      set_progress(62,0);
      copy_ent(12,201);
      set_ent_active(12,0);
      orient_heroes();
      drawmap();
      screen_dump();
      msg("$1 joined!",255,0);
    end

  end
end
  
function LOC_partner_up()
  local a;

  a = get_party_xp(get_pidx(0)) - get_party_xp(get_pidx(1));
  if (a > 5000)
    a = a - 5000;
  a = a + krnd(500);
  give_xp(get_pidx(1),a,1);
  a = get_progress(62)-1;
  if (a == 0) then
    set_all_equip(0,6,40,47,69,75,0);
  elseif (a == 1) then
    set_all_equip(1,25,0,47,62,75,0);
  elseif (a == 2) then
    set_all_equip(2,30,40,46,57,75,0);
  elseif (a == 3) then
    set_all_equip(3,6,40,46,57,75,0);
  elseif (a == 4) then
    set_all_equip(4,34,0,46,57,75,0);
  elseif (a == 5) then
    set_all_equip(5,12,40,51,69,75,0);
  elseif (a == 6) then
    set_all_equip(6,12,40,47,62,75,0);
  elseif (a == 7) then
    set_all_equip(7,30,40,46,57,75,0);
  end
end
