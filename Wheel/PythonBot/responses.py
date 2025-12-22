from random import choice, randint

def get_response(user_input: str) -> str:
    lowered: str = user_input.lower()

    if 'jumco' in lowered:
        return 'Status: Online'
    elif 'help' in lowered:
        return f'''
          Commands
-----------------------------
Jumco: Check if bot is online
Gamble: rolls a dice
'''
    elif 'gamble' in lowered:
        return f'You rolled: {randint(1, 6)}'
    elif 'boi' in lowered:
        return 'https://external-preview.redd.it/eMTVeW8Mty4Ntqzq8Dvzy5zr8rG2jQB7GvMLPWM22wA.jpg?width=1024&auto=webp&s=423be8d9b4eb2be45c32eb1f147b8a63f2d8e29f'      
    elif 'no u' in lowered:        
        return '''
⣿⣿⣿⣿⣿⡿⠿⠿⠿⠿⣿⣿
⣿⣿⣿⣿⣿⠗⠄⠄⠄⠄⣿⣿
⣿⣿⣿⡿⠋⠄⠄⠄⣠⣄⢰⣿
⣿⣿⣿⡇⠄⢀⡴⠚⢿⣿⣿⣿
⣿⣿⡿⣿⣿⠴⠋⠄⠄⢸⣿⣿
⣿⣿⠃⠈⠁⠄⠄⢀⣴⣿⣿⣿
⣿⣿⠄⠄⠄⠄⢶⣿⣿⣿⣿⣿
⣿⣷⣶⣶⣶⣶⣶⣿⣿⣿⣿⣿
'''
    elif 'wheel' in lowered:
        return 'Wheel Time!'
