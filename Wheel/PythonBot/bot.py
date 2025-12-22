from typing import Final
import os
from dotenv import load_dotenv
from discord import Intents, Client, Message
from random import choice, randint
from responses import get_response
from botsponses import get_botsponse

# STEP 0: LOAD OUR TOKEN FROM SOMEWHERE SAFE
load_dotenv()
TOKEN: Final[str] = os.getenv('DISCORD_TOKEN')
print(TOKEN)

# STEP 1: BOT SETUP
intents: Intents = Intents.default()
intents.message_content = True  # NOQA
client: Client = Client(intents=intents)


# STEP 2: MESSAGE FUNCTIONALITY
async def send_message(message: Message, user_message: str) -> None:
    # Check if the message content is empty
    if not user_message:
        print('(Message was empty because intents were not enabled probably)')
        return

    # Process messages from other users
    if is_private := user_message[0] == '?':
        user_message = user_message[1:]

    # Check if the message is from the bot
    if message.author == client.user:
        print("I hear myself")
        try:
            print("I cant respond to myself")
            response: str = get_botsponse(user_message)
            await message.author.send(response) if is_private else await message.channel.send(response)
        except Exception as e:
            return
    
    try:
        print("I hear people")
        response: str = get_response(user_message)
        await message.author.send(response) if is_private else await message.channel.send(response)
    except Exception as e:
        return


# STEP 3: HANDLING THE STARTUP FOR OUR BOT
@client.event
async def on_ready() -> None:
    print(f'{client.user} is ready and running!')
    print('-------------------------------------\n')


# STEP 4: HANDLING INCOMING MESSAGES
@client.event
async def on_message(message: Message) -> None:
    if message.author == client.user:
        return

    username: str = str(message.author)
    user_message: str = message.content
    channel: str = str(message.channel)

    print(f'[{channel}] {username}: "{user_message}"')
    await send_message(message, user_message)

# STEP 5: MAIN ENTRY POINT
def main() -> None:
    client.run(token=TOKEN)


if __name__ == '__main__':
    main()