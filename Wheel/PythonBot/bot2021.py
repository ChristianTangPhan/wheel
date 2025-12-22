import os
import discord
from discord.ext import commands
from dotenv import load_dotenv

# Load environment variables from .env file
load_dotenv()

# Retrieve the token from the environment
TOKEN = os.getenv('DISCORD_TOKEN')

# Create a Discord bot with specified intents and command prefix
intents = discord.Intents.default()
intents.messages = True
intents.guilds = True
intents.members = True

client = commands.Bot(command_prefix='?', intents=intents)

# Event: Bot is connected and ready
@client.event
async def on_ready():
    print(f"{client.user} is connected to Discord and ready!")
    print(f"Bot is in the following guilds: {[guild.name for guild in client.guilds]}")
    print("---------------------------------------------------")

@client.event
async def on_command_error(ctx, error):
    print(f'An error occurred: {error}')

@client.command()
async def hello(ctx):
    print("Command received: ?hello")  # Debug statement
    await ctx.send("Hello")

# Run the bot with the specified token
client.run(TOKEN)