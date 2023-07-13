import mysql
from mysql.connector import connect, Error

class database_client:
    def __init__(self, host, port, user, password, database):
        self.host = host
        self.port = port
        self.user = user
        self.password = password
        self.database = database
        pass
    
    def get_users_id(self):
        try:
            with connect(
                host = self.host,
                user = self.user,
                password = self.password,
                database = self.database
            ) as connection:
                with connection.cursor() as cursor:             
                    cursor.execute("SELECT id FROM Users")
                    return cursor.fetchall()
        except Error as e:
            print(e)

    def get_latest_temerature_data(self):
        try:
            with connect(
                host = self.host,
                user = self.user,
                password = self.password,
                database = self.database
            ) as connection:
                with connection.cursor() as cursor:
                    query = \
                        'SELECT Places_in_locations.name, Locations.name,  date, time, temperature_сelsius '\
                        'FROM( '\
                        'SELECT placeId ,locationId, date, time, temperature_сelsius '\
                        'FROM '\
                        '( '\
                            'SELECT placeId, locationId, date, time, temperature_сelsius '\
                            'FROM Temperature_in_locations T '\
                            'WHERE time = ( '\
                            'SELECT MAX(time) '\
                            'FROM Temperature_in_locations '\
                            'WHERE placeId = T.placeId AND date = T.date '\
                            ') '\
                        ') AS S '\
                        'WHERE date = ( '\
                            'SELECT MAX(date) '\
                            'FROM Temperature_in_locations '\
                            'WHERE placeId = S.placeId '\
                        ') '\
                        ') Y '\
                        'JOIN Places_in_locations ON Y.placeId = Places_in_locations.id '\
                        'JOIN Locations ON Places_in_locations.locationId = Locations.id '\

                    cursor.execute(query)
                    return cursor.fetchall()
        except Error as e:
            print(e)

