"""
====================================================================
This program creates a list for the wheel of all possible geys
depending on the amount of alivers and which asians playing.
    - Dudwen
====================================================================
"""

"""
--------------------------------------------------------------------
Declaration
--------------------------------------------------------------------
"""
#Initializes players
ID = ["cavey", "deeswa", "thien", "dudwen", "deft"]
status = {players: False for players in ID}

#Other Initializations
alivers = 0
quit = False

#The Gey List
geyList = ["Abort",     "Osu",          "Desc men",   "War",         "Halla",   "Business Tour", "Crab Gey",  "Drunk",     "Fps Chess",  "Garry",
           "Terrerer",  "Monster Rise", "Muck",       "Phas",        "Maddog",  "Space War",     "E Od Oder", "Chimpkin",  "Meag",       "Knock",
           "Ark",       "Apex",         "Ranch",      "Hardest Gey", "Diep",    "Road Blocks",   "Over 2",    "Ight",      "Mince",      "Discord/Jklm",
           "Moomoo.io", "Mope.io",      "Shellshock", "Spiltgate",   "Lethal",  "Cuhminme",      "Toms",      "Destiny",   "Bluestacks", "Pixel Gun",
           "Rainworld", "Itchi.io",     "Grabity",    "Astro",       "Rounds",  "Mogus",         "One Arm",   "Sap",       "MCD",        "Runners",
           "Ants",      "Helldivers",   "Thieves",    "Stardew",     "TABG",    "Content",       "CS2",       "Unrailed",  "Stick Fight"]

#The Gey Options
nah = 0
gey = 1
d_u = 2
own = 3

twoPlayer   = [geyList.index("Fps Chess"), geyList.index("Shellshock")]

threePlayer = [geyList.index("Knock"), geyList.index("Apex"), geyList.index("Cuhminme")]

fourPlayer  = [geyList.index("Desc men"), geyList.index("War"), geyList.index("Halla"), geyList.index("Business Tour"), 
               geyList.index("Drunk"), geyList.index("Monster Rise"), geyList.index("Phas"), geyList.index("E Od Oder"), 
               geyList.index("Rainworld"), geyList.index("Grabity"), geyList.index("Rounds"), geyList.index("One Arm"),
              geyList.index("Runners"), geyList.index("Ants"), geyList.index("Helldivers"), geyList.index("Stardew"),
              geyList.index("Content"), geyList.index("MCD")]

fivePlayer  = [geyList.index("Ranch"), geyList.index("Hardest Gey"), geyList.index("Over 2"), geyList.index("Ight"), 
               geyList.index("Toms")]

print("====================================")
print("= ALL POSSIBLE GEY LIST!!! =")
print("====================================")

"""
--------------------------------------------------------------------
QUESTIONS
--------------------------------------------------------------------
"""
#Drafts asians
while quit == False:

    #Checks if everyone in list is already added
    falseCheck = True
    for players in range(len(ID)):
        if status[ID[players]] == False:
            falseCheck = False
    if falseCheck == False:
        quit = False
    else:
        quit = True

    if quit == False:
        print("\n========== MENU ==========")
        print("1. Cavey")
        print("2. Deeswa")
        print("3. Thien")
        print("4. Dudwen")
        print("5. Deft")
        print("6. Who's in the party?")
        print("7. Reset party")
        print("8. Thats all!")
        player = (input("\nWho is playing: "))
        player = str(player)

        if player == "1" or player.lower() == "cavey":
            if status["cavey"] == True:
                print("\n-- Cavey is already added --")
            else:
                status["cavey"] = True
                print("\n-- Cavey is added --")
                alivers += 1
        
        elif player == "2" or player.lower() == "deeswa":
            if status["deeswa"] == True:
                print("\n-- Deeswa is already added --")
            else:
                status["deeswa"] = True
                print("\n-- Deeswa is added --")
                alivers += 1
        
        elif player == "3" or player.lower() == "thien":
            if status["thien"] == True:
                print("\n-- thien is already added --")
            else:
                status["thien"] = True
                print("\n-- Thien is added --")
                alivers += 1
        
        elif player == "4" or player.lower() == "dudwen":
            if status["dudwen"] == True:
                print("\n-- Dudwen is already added --")
            else:
                status["dudwen"] = True
                print("\n-- Dudwen is added --")
                alivers += 1
        
        elif player == "5" or player.lower() == "deft":
            if status["deft"] == True:
                print("\n-- Deft is already added --")
            else:
                status["deft"] = True
                print("\n-- Deft is added --")
                alivers += 1
        
        elif player == "6" or player.lower() == "party":
            print("\n== These asians are in the party ==")
            if alivers == 0:
                print("No one")
            elif alivers > 0:
                for players in range(len(ID)):
                    if status[ID[players]] == True:
                        print(ID[players].capitalize(), end = ' ') 
            else:
                print("\n\nhow did you break the program this bad...")
            print("")
        
        elif player == "7" or player.lower() == "reset":
            for players in range(len(ID)):
                status[ID[players]] = False
            print("\n<< Party is reset >>")
            alivers = 0
        
        elif player == "8" or player.lower() == "quit":
            if alivers < 2:
                print("\n!! Not enough !!")
            else:
                quit = True
        else:
            print("\n!! Not option !!")

#Asks if alivers are willing to wait for downloads or updates
choice = input("\nAre yall willing to download or update geys: ")
characterCheck = str(choice)
willing = choice.lower().startswith("y")

if willing == True:
    print("<< Unready geys added to list >>")
    d_u = gey

"""
--------------------------------------------------------------------
MATRIX
--------------------------------------------------------------------
"""
# Order: Cavey, Deeswa, Thien, Dudwen, Deft
# Contains asians option on each gey
database = [
    # 1. Abortion
    [gey, gey, gey, gey, d_u],
    # 2. Osu
    [gey, gey, nah, gey, d_u],
    # 3. Descending Men
    [gey, gey, d_u, gey, d_u],
    # 4. Warframe
    [gey, gey, nah, nah, d_u],
    # 5. Brawlhalla
    [nah, gey, d_u, gey, d_u],
    # 6. Business Tour
    [gey, gey, gey, gey, d_u],
    # 7. Crab Gey
    [gey, gey, d_u, gey, nah],
    # 8. Drunk
    [gey, gey, d_u, gey, d_u],
    # 9. Fps Chess
    [gey, gey, d_u, gey, gey],
    # 10. Garry
    [gey, gey, d_u, gey, d_u],
    # 11. Terrerer
    [gey, gey, d_u, gey, gey],
    # 12. Monster Rise
    [gey, gey, d_u, gey, nah],
    # 13. Muck
    [gey, gey, gey, gey, d_u],
    # 14. Phas
    [gey, gey, d_u, gey, d_u],
    # 15. Realm Mad Dog
    [d_u, gey, nah, gey, nah],
    # 16. Space War
    [gey, gey, d_u, d_u, d_u],
    # 17. E Od Oder
    [gey, gey, nah, nah, gey],
    # 18. Chimpkin
    [gey, gey, nah, gey, nah],
    # 19. Meag
    [gey, gey, d_u, d_u, nah],
    # 20. Knock
    [nah, nah, nah, nah, nah],
    # 21. Ark
    [d_u, gey, nah, d_u, nah],
    # 22. Apex
    [d_u, gey, nah, d_u, d_u],
    # 23. Ranch
    [gey, gey, d_u, gey, gey],
    # 24. Hardest Gey
    [gey, gey, d_u, gey, gey],
    # 25. Diep
    [gey, gey, gey, gey, gey],
    # 26. Road Blocks
    [gey, gey, gey, gey, nah],
    # 27. Over 2
    [nah, gey, nah, d_u, nah],
    # 28. Ight
    [gey, gey, nah, gey, nah],
    # 29. Mince
    [gey, gey, gey, gey, nah],
    # 30. Discord/Jklm
    [gey, gey, gey, gey, gey],
    # 31. Moomoo.io
    [gey, gey, gey, gey, gey],
    # 32. Mope.io
    [gey, gey, gey, gey, gey],
    # 33. Shellshock
    [gey, gey, gey, gey, gey],
    # 34. Spiltgate
    [d_u, gey, nah, d_u, d_u],
    # 35. Lethal
    [nah, gey, gey, gey, gey],
    # 36. Cuhminme
    [d_u, gey, nah, d_u, d_u],
    # 37. Toms
    [gey, gey, gey, gey, gey],
    # 38. Destiny 2
    [d_u, d_u, d_u, d_u, d_u],
    # 39. Bluestacks
    [gey, gey, d_u, d_u, d_u],
    # 40. Pixel Gun
    [gey, gey, nah, gey, gey],
    # 41. Rainworld
    [gey, nah, nah, own, gey],
    # 42. Itchi.io
    [gey, gey, gey, gey, gey],
    # 43. Grabity
    [gey, d_u, d_u, gey, gey],
    # 44. Astro
    [gey, gey, nah, gey, nah],
    # 45. Rounds
    [gey, gey, gey, gey, gey],
    # 46. Mogus
    [d_u, gey, gey, gey, gey],
    # 47. One Arm
    [gey, gey, gey, gey, gey],
    # 48. Sap
    [d_u, d_u, d_u, gey, d_u],
    # 49. MCD
    [nah, gey, nah, gey, nah],
    # 50. Speed
    [nah, nah, nah, nah, nah],
    # 51. Ants
    [nah, nah, nah, nah, nah],
    # 52. Helldivers
    [nah, nah, nah, nah, nah],
    # 53. Thieves
    [nah, nah, nah, nah, nah],
    # 54. Stardew
    [nah, nah, gey, gey, nah],
    # 55. TABG
    [d_u, d_u, d_u, gey, d_u],
    # 56. Content
    [nah, nah, nah, gey, gey],
    # 57. CS2
    [nah, nah, nah, gey, gey],
    # 58. Unrailed
    [gey, nah, nah, gey, nah],
    # 59. Stick Fight
    [nah, gey, nah, gey, nah],
]

"""
--------------------------------------------------------------------
CALCULATIONS
--------------------------------------------------------------------
"""
#Congrats if you can understand this (@_@)
for games in range(len(database)):
    for scan in range(len(database[games])):
        if database[games][scan] == own:
            if status[ID[scan]] == True:
                database[games][scan] = gey

#Removes ded from list
if status["cavey"] == False:
    for remove in range(len(database)):
        database[remove][ID.index("cavey")] = gey
if status["deeswa"] == False:
    for remove in range(len(database)):
        database[remove][ID.index("deeswa")] = gey
if status["thien"] == False:
    for remove in range(len(database)):
        database[remove][ID.index("thien")] = gey
if status["dudwen"] == False:
    for remove in range(len(database)):
        database[remove][ID.index("dudwen")] = gey
if status["deft"] == False:
    for remove in range(len(database)):
        database[remove][ID.index("deft")] = gey

#Removes 2 player geys
if alivers > 2:
    for players in range(len(ID)):
        for remove in range(len(twoPlayer)):
            database[twoPlayer[remove]][players] = nah
"""
#Removes 3 player geys
if alivers > 3:
    for players in range(len(ID)):
        for remove in range(len(threePlayer)):
            database[threePlayer[remove]][players] = nah
"""
#Removes 4 player geys
if alivers > 4:
    for players in range(len(ID)):
        for remove in range(len(fourPlayer)):
            database[fourPlayer[remove]][players] = nah
"""
#Removes 5 player geys
if alivers > 5:
    for players in range(len(ID)):
        for remove in range(len(fivePlayer)):
            database[fivePlayer[remove]][players] = nah
"""

"""
--------------------------------------------------------------------
DISPLAY
--------------------------------------------------------------------
"""
#Prints all possible gey list
print("\nList of possible GEYS")
for games in range(len(database)):
    if (database[games][ID.index("cavey")]  == 
        database[games][ID.index("deeswa")] == 
        database[games][ID.index("thien")]  == 
        database[games][ID.index("dudwen")] == 
        database[games][ID.index("deft")]   == gey):
        print(geyList[games])
