from datetime import date
import json

def database_result_to_text(data):
    anser = str()
    try:
        with open("./Telegram_bot_python/location_places_names.json", 'r', encoding="utf-8") as file:
            names = json.load(file)
        loacations_group = {k: list() for k in names['locations']}
        for i in data:
            location_string = names['locations'][i[1]]['places'][i[0]].ljust(11)
            location_string += str(i[4]).rjust(2).ljust(4)
            location_string += ':'.join(str(i[3]).split(':')[:2]).ljust(7)
            location_string += i[2].strftime('%d.%m')
            location_string += "\n"
            loacations_group[i[1]].append(location_string)

        for k, v in loacations_group.items():
            loacations_group[k] = sorted(
                v,
                key = lambda x: x.find("  "),
                reverse = True
            )
            loacations_group[k] ="<pre>" + "".join(loacations_group[k]) + "</pre>" + "\n"

        anser += "<b>" + names['locations']['first floor']['name'] + "</b><code>    </code>🌡️<code>   </code>🕝<code>    </code>📅\n" + loacations_group['first floor']
        anser += "<b>" + names['locations']['second floor']['name'] + "</b>\n" + loacations_group['second floor']
        anser += "<b>" + names['locations']['street']['name'] + "</b>\n" + loacations_group['street']
        anser += "<b>" + names['locations']['basement']['name'] + "</b>\n" + loacations_group['basement']

        return anser
    
    except BaseException as exception:
        print(exception)


def add_format_sapces(string, number):
    string += "<code>" + (" " * number) +"</code>"
    return string



# 1-й эатж  🌡️   🕝     📅
# гостинная 20  13:00  11.06.23
# мастерска 20  13:00  11.06.23
# душевая   12  13:00  11.06.23
# пол       20  13:00  11.06.23

# 2-й эатж  🌡️   🕝     📅
# спалня 1  20  13:00  11.06.23
# спалня 2  1   13:00  11.06.23
# спалня 3  12  13:00  11.06.23
# туалет    20  13:00  11.06.23

# улица     🌡️   🕝     📅
# балкон    20  13:00  11.06.23
# веранда   20  13:00  11.06.23

# Подвал    🌡️   🕝     📅
# подвал 1  22  22:10  12.06.23
# подвал 2  25  11:10  13.06.23
# подвал 3  20  13:00  11.06.23