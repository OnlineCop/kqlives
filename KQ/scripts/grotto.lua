-- grotto - small forest grotto north of Ekla

function autoexec()
  if (get_treasure(15) == 1) then
    set_obs(24,16,0);
  end
  if (get_treasure(80) == 1) then
    set_obs(26,10,0);
  end
  if (get_progress(P_FELLINPIT) > 1) then
    set_btile(16,14,153);
    set_obs(16,14,1);
  end
  if (get_progress(P_FELLINPIT) == 3) then
    set_ent_active(0,0);
    set_ftile(20,16,154);
    set_zone(20,16,0);
    set_obs(20,16,1);
  end
end

function postexec()
  return
end  

function zone_handler(zn)
  if (zn == 1) then
    if (get_progress(P_FELLINPIT) == 2 and get_progress(P_TALKDERIG) > 0) then
      set_progress(P_FELLINPIT,3);
    end
    change_map("main",129,19,129,19);
  
  elseif (zn == 2) then
    bubble(HERO1,"Mmmm... cozy.","","","");
  
  elseif (zn == 3) then
    if (get_progress(P_FELLINPIT) == 0) then
      set_btile(16,14,153);
      bubble(HERO1,"Uh oh!","","","");
      change_map("cave2",0,0,0,0);
    else
      bubble(HERO1,"I'd rather not go in",
                   "there right now.","","");
    end
  
  elseif (zn == 4) then
    chest(15,I_ERUNE,1);
    set_obs(24,16,0);
  
  elseif (zn == 5) then
    if (get_progress(P_UCOIN) == 0) then
      bubble(HERO1,"Hmm... this seems like it should",
                   "do something. But what?","","");
    else
      if (get_progress(P_UCOIN) == 3) then
        change_map("grotto2",0,0,0,0);
      end
    end
  
  elseif (zn == 6) then
    chest(80,I_MACE1,1);
    set_obs(26,10,0);

  end
end

function entity_handler(en)  
  if (en == 0) then
    if (get_progress(P_TALKDERIG) == 1) then
      if (get_progress(P_FELLINPIT) == 0) then
        bubble(0,"Good luck.","","","");
      else
        bubble(0,"You should be more careful.","","","");
      end
    else
      bubble(0,"Good day $0. I've",
               "been expecting you.","","");
      bubble(HERO1,"How could you have been",
                   "expecting me... and how",
                   "do you know my name?","");
      bubble(0,"I know all about you and the",
               "others and your quest. I know",
               "that Nostik wants you to find",
               "the Staff of Xenarum for him.");
      bubble(0,"I've known for quite",
               "some time now.","","");
      bubble(HERO1,"Are you a friend of his?","","","");
      bubble(0,"No, Nostik doesn't have any",
               "friends. He's a stranger to",
               "these parts.","");
      bubble(0,"Before he was imprisoned, Nostik",
               "had just shown up in this land.",
               "He's a strange and secret man.","");
      bubble(HERO1,"So how do you know",
                   "about the quest then?","","");
      bubble(0,"Well, you might say that I am",
               "somewhat of a prophet. I had a",
               "dream about all of you and",
               "this quest.");
      bubble(0,"So far, everything that I",
               "dreamed about has come to pass.","","");
      bubble(HERO1,"Well, then can you tell me where",
                   "I should start my search?","","");
      bubble(0,"Yes! You must head to the town",
               "of Andra on the mainland. Take",
               "the tunnel from Ekla to Randen.","");
      bubble(0,"From there, go east across the",
               "river to Andra. Once there talk",
               "to a man named Tsornin.","");
      bubble(0,"He's the captain of the city",
               "guard and should be easy to",
               "find. He can guide you from",
               "there.");
      bubble(HERO1,"Can you tell me anything else.","","","");
      bubble(0,"I have nothing further to tell",
               "you except this; be cautious,",
               "especially of Nostik.","");
      bubble(0,"I have yet to figure out why",
               "he wants the Staff, but I",
               "don't trust him.","");
      bubble(HERO1,"Why? What does this Staff do?","","","");
      bubble(0,"I don't know, and that's",
               "the problem.","","");
      if (get_pidx(0) == CASANDRA) then
        bubble(HERO1,"Whatever.","","","");
      else
        bubble(HERO1,"Oh... okay. Well, thanks",
                     "for your help.","","");
      end
      set_progress(P_TALKDERIG,1);
    end

  end
end
