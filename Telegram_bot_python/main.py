
import telebot
from telebot import types
from threading import Thread
from database_client import database_client 
from result_formating import database_result_to_text
import json 

is_working = [True]

path_to_bot_config = "./Telegram_bot_python/bot_config.json"
with open(path_to_bot_config, "r", encoding = "utf-8") as bot_config_file:
    bot_config = json.load(bot_config_file)

db_client = database_client(
    bot_config["database"]["host"],
    bot_config["database"]["port"],
    bot_config["database"]["user"],
    bot_config["database"]["password"],
    bot_config["database"]["database"]
)

bot = telebot.TeleBot(bot_config['bot']['token'])

#функция начала
@bot.message_handler(commands=['start'])
def start(message):
    markup = types.ReplyKeyboardMarkup(resize_keyboard=True)
    all_button = types.KeyboardButton('Все послдение данные')
    markup.add(all_button)  
    bot.send_message(message.from_user.id, "Выберите что хотите узнать", reply_markup=markup)

@bot.message_handler(content_types=['text'])
def get_latest_data(message):
    print(message.from_user.id)
    # if  not message.from_user.id in users_id:
    #     bot.send_message(message.from_user.id, "Я тебя не знаю")
    #     return

    markup = types.ReplyKeyboardMarkup(resize_keyboard = True)
    all_button = types.KeyboardButton('Все послдение данные')
    markup.add(all_button)
    if message.text == 'Все послдение данные':
        data = db_client.get_latest_temerature_data()
        anser = database_result_to_text(data)
        bot.send_message(message.from_user.id, anser , parse_mode='HTML', reply_markup=markup)
    else : 
        bot.send_message(message.from_user.id, "ничего не понятно но очень интресно \n попробуйте healp")


def console_input_worker():
    while is_working[0]:
        command = input()
        if command == "stop":
            is_working[0] = False
            bot.stop_polling()
        else:
            print("unknown command")
    print("Console closed. Stoping bot..")


t1 = Thread(target=console_input_worker)
t1.start()
print("Bot started ")
while is_working[0]:
    users_id = db_client.get_users_id()
    try:
        print("Bot polling")
        bot.polling(none_stop=True, interval = 0)
    except Exception as exception:
        print(exception.__traceback__)


print("Bot stopped")
t1.join()



