-- Global functions available to all scripts

-- Pick one of the args
-- If arg is a table it can have a pr field which gives
-- its probability of being picked
-- e.g. print(pick(1,2,3))
--      pick({pr = 5, name = "Rare"}, {pr = 95, name = "Common"}).name
function pick(...)
  cumprob = 0

  for i = 1, arg.n do
    if (istable(arg[i]) ) then
      prob = arg[i].pr or 1
    else
      prob = 1
    end

    cumprob = cumprob + prob
  end

  cumprob = krnd(cumprob)

  for i = 1, arg.n do
    if (istable(arg[i]) ) then
      prob = arg[i].pr or 1
    else
      prob = 1
    end

    cumprob = cumprob - prob

    if (cumprob < 0) then
      return arg[i]
    end
  end
end


function Sensar:touch_fire()
  bubble(HERO1, pick("What th..? Ouch! That's hot!",
         "There's no way I'm sticking my hand in that fire!",
         "This feels pretty nice."))
end


function Sarina:touch_fire()
  bubble(HERO1, pick("Mmm, wood smoke.",
         "Smells like burnt hair. Hey wait... that's MY hair!",
         "Ooh, cozy."))
end


function Corin:touch_fire()
  bubble(HERO1, pick("I sure like fire.",
         "Watching this is relaxing.",
         "This is making me sleepy."))
end


function Ajathar:touch_fire()
  bubble(HERO1, pick("Hmm... I want marshmallows.",
         "You call this a fire?!",
         "Ah, relaxing."))
end


function Casandra:touch_fire()
  bubble(HERO1, pick("Something's burning. I hope it's one of those stupid books!",
         "The fire is getting low.",
         "Yessir, this is a fire."))
end


function Temmin:touch_fire()
  bubble(HERO1, pick("Ah, the age-old fire.",
         "This needs more coal.",
         "This would be great to read a book next to."))
end


function Ayla:touch_fire()
  bubble(HERO1, pick("I wonder how hot this is?",
         "Someone should clean all this soot out of here.",
         "Well, my face is warm now, but my butt is still freezing!"))
end


function Noslom:touch_fire()
  bubble(HERO1, pick("I prefer torches.",
         "I love the crackle of a good fire.",
         "I wonder if a spell would make this burn brighter?"))
end


function Sensar:read_book()
  bubble(HERO1, pick("Reading makes me sleepy...",
         "So many books...",
         "Reading is for wimps."))
end


function Sarina:read_book()
  bubble(HERO1, pick("Ugh... this would take me forever to read.",
         "I never liked reading.",
         "Who wrote this trash?"))
end


function Corin:read_book()
  bubble(HERO1, pick("Doesn't anybody leave spellbooks lying around?",
         "Why would I read this?",
         "Can't talk... reading."))
end


function Ajathar:read_book()
  bubble(HERO1, pick("Hmmm... I don't approve of that.",
         "I'm too busy to read now.",
         "How many books can you write that start with 'The Joy of...'?"))
end


function Casandra:read_book()
  bubble(HERO1, pick("Boring.",
         "Somebody should burn these.",
         "Terrible... just terrible."))
end


function Temmin:read_book()
  bubble(HERO1, pick("If only I had more time...",
         "So many books...",
         "Some of these are pretty old."))
end


function Ayla:read_book()
  bubble(HERO1, pick("I don't have time for this.",
         "What language is this written in?",
         "The pages are stuck together!?"))
end


function Noslom:read_book()
  bubble(HERO1, pick("Fascinating.",
         "I have this one.",
         "Romance novels... gack!"))
end


-- This function can be called whenever the hero touches a fire
function touch_fire(ent)
  party[0]:touch_fire()
end


--  Response for reading a book.
function book_talk(ent)
  party[0]:read_book()
end


-- Display bubble text; just concatenate all the args and call the _ex function
-- Args ent  Entity number
--      ...  Variable number of arguments - text to show
function bubble(ent, ...)
  s = ""
  for i = 1, arg.n do
    if (i ~= 1) then
      s = s.."\n"
    end
    s = s..arg[i]
  end
  bubble_ex(ent, s)
end


-- See function bubble()
function thought(ent, ...)
  s = ""
  for i = 1, arg.n do
    if (i ~= 1) then
      s = s.."\n"..arg[i]
    else
      s = s..arg[i]
    end
  end
  thought_ex(ent, s)
end


-- Select from heroes in the manor
-- The available list is stored in eight consecutive P_ constants
-- as 0 for nobody and 1..8 for characters 0..7
function select_manor()
  -- Get the current list
  heroes = {}
  for i = 1, 8 do
    v = get_progress(i + P_MANORPARTY - 1)
    if (v ~= 0) then
      heroes[i] = v - 1
    end
  end
  -- Do the selecting
  heroes = select_team(heroes)
  -- Put back in the list
  for i = 1, 8 do
    if (heroes[i]) then
      v = heroes[i] + 1
    else
      v = 0
    end
    set_progress(i + P_MANORPARTY - 1, v)
  end
end


-- Add this hero to the manor if not already there
-- hero can be a single value or a table
-- returns the number of heroes that were actually added
function add_to_manor(hero)
  local total, i
  if (not hero) then
  return 0
  end
  if (istable(hero)) then
    total = 0
    i = 1
    while (hero[i]) do
      total = total + add_to_manor(hero[i])
      i = i + 1
    end
    return total
  else
    if (hero < 0 or hero > 7) then
      return 0
    end
    for i = 0, 7 do
      if (get_progress(i + P_MANORPARTY) == (hero + 1)) then
        return 0
      end
    end
    for i = 0, 7 do
      if (get_progress(i + P_MANORPARTY) == 0) then
        set_progress(i + P_MANORPARTY, hero + 1)
        return 1
      end
    end
  end
end


-- Checks if this ent is in the party, or in the manor,
-- or has never been recruited.
-- who: hero id
-- returns "manor" if in manor, "party" if in party, nil otherwise
function LOC_manor_or_party(who)
  local a
  if (get_pidx(0) == who) then
    return "party"
  elseif (get_numchrs() > 1 and get_pidx(1) == who) then
    return "party"
  end

  for a = P_MANORPARTY, P_MANORPARTY7 do
    if (get_progress(a) - 1 == who) then
      return "manor"
    end
  end
  return nil
end


function get_quest_info()
   if LOC_add_quest_item then
      LOC_add_quest_item()
   end

   add_quest_item("About...", "This doesn't do much yet")
   add_quest_item("Test1", "Some test info")
   add_quest_item("Sensar", "He rages!")
end

-- backward compat
change_mapm = change_map
