import os
import discord
from discord import Intents
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Retrieve the tokens from the environment
TOKEN = os.getenv('DISCORD_TOKEN')
GUILD_ID = os.getenv('DISCORD_GUILD')  # Use GUILD_ID to represent the guild ID

# Define and configure intents
intents = Intents.default()
intents.messages = True
intents.reactions = True

# Create a Discord client with specified intents
client = discord.Client(intents=intents)

# Event: Bot is connected
@client.event
async def on_ready():
    # Find the guild by ID
    guild = discord.utils.get(client.guilds, id=int(GUILD_ID))
    
    if guild is not None:
        print(
            f'{client.user} is connected to the following guild:\n'
            f'{guild.name}(id: {guild.id})'
        )
    else:
        print(f'Bot failed to find a guild with ID: {GUILD_ID}')

@commands.Cog.listener()
async def nm_reaction_add(self, reaction, user):
    channel = reaction.message.channel
    await channel.send(user.name + "added: " + reaction.emoji)

"""
# Event: Bot is ready
@client.event
async def on_ready():
    print(f'{client.user} has connected to Discord!')

"""

# Run the bot with the specified token
client.run(TOKEN)
