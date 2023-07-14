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

        for key in loacations_group.keys():
            loacations_group[key] = sorted(
                loacations_group[key],
                key = lambda x: x.find("  "),
                reverse = True
            )
            loacations_group[key] = "<pre>" + "".join(loacations_group[key]) + "</pre>" + "\n"

        anser += "<b>" + names['locations']['first floor']['name']
        anser +="</b><code>    </code>🌡️<code>   </code>🕝<code>    </code>📅\n" + loacations_group['first floor']
        anser += "<b>" + names['locations']['second floor']['name'] + "</b>\n" + loacations_group['second floor']
        anser += "<b>" + names['locations']['street']['name'] + "</b>\n" + loacations_group['street']
        anser += "<b>" + names['locations']['basement']['name'] + "</b>\n" + loacations_group['basement']

        return anser

    except BaseException as exception:
        print(exception)


def add_format_sapces(string, number):
    string += "<code>" + (" " * number) +"</code>"
    return string
