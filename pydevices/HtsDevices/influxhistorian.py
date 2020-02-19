import MDSplus
import time
from datetime import datetime

try:
    from influxdb import InfluxDBClient
except:
    print("You must install the `influxdb` python package to use the `influxhistorian` device class")
    exit(1)

class INFLUXHISTORIAN(MDSplus.Device):

    DATA_COUNT = 500

    parts = [
        { 'path': ':ADDRESS',       'type': 'text',                                                 'options': ('no_write_shot',) },
        { 'path': ':DATABASE',      'type': 'text',                                                 'options': ('no_write_shot',) },
        { 'path': ':SERIES',        'type': 'text',                                                 'options': ('no_write_shot',) },
        { 'path': ':CREDENTIALS',   'type': 'text',     'value': '/path/to/credentials',            'options': ('write_model',) },
        { 'path': ':DATA_EVENT',    'type': 'text',     'value': 'INFLUXDB_TREND',                  'options': ('no_write_shot',) },
        { 'path': ':START_TIME',    'type': 'numeric',                                              'options': ('write_model', 'write_shot',) },
        { 'path': ':LAST_READ',     'type': 'numeric',  'valueExpr': 'head.start_time',             'options': ('write_model', 'write_shot',) },
        { 'path': ':DATA',          'type': 'text' },
    ]

    for i in range(DATA_COUNT):
        name = ":DATA:D{:03d}".format(i + 1)
        parts.append({ 'path': name,                'type': 'signal',                       'options':('no_write_model', 'write_shot',) })
        parts.append({ 'path': name + ":WHERE",     'type': 'text',                         'options':('no_write_shot',) })
        parts.append({ 'path': name + ":SELECT",    'type': 'text',                         'options':('no_write_shot',) })

    def debugging(self):
        import os
        if self.debug == None:
            self.debug = os.getenv("DEBUG_DEVICES")
        return self.debug

    def trend(self):
        if not self.on:
            return

        new_last_read = int(round(time.time() * 1000))
        self.store(self.last_read.data(), new_last_read)
        self.last_read.record = new_last_read
    TREND=trend

    def store(self, start = 0, end = 0):
        if not self.on:
            return

        address = self.address.data()
        parts = address.split(":", 2)

        address = parts[0]
        port = 8086
        if len(parts) > 1:
            port = int(parts[1])

        username = ''
        password = ''

        try:
            with open(self.credentials.data()) as cred_file:
                lines = cred_file.readlines()
                
                if len(lines) < 2:
                    print("Failed to read credentials from file {}".format(self.credentials.data()))
                
                username = lines[0].strip('\n')
                password = lines[1].strip('\n')

        except IOError as e:
            print("Failed to open credentials file {}".format(self.credentials.data()))

        client = InfluxDBClient(address, port, username, password, self.database.data())

        startTimeQuery = ''
        endTimeQuery = ''

        if start > 0:
            # Convert to nanosecond UNIX timestamp
            startTimeQuery = 'time > {}'.format(start * 1000000)

        if end > 0:
            # Convert to nanosecond UNIX timestamp
            endTimeQuery = 'time < {}'.format(end * 1000000)

        for i in range(self.DATA_COUNT):
            try:
                node  = self.__getattr__("data_d{:03d}".format(i + 1))
                where = self.__getattr__("data_d{:03d}_where".format(i + 1)).data()
                if not node.on:
                    continue

                if where == '':
                    continue

                whereList = [ where ]

                if startTimeQuery != '':
                    whereList.append(startTimeQuery)

                if endTimeQuery != '':
                    whereList.append(endTimeQuery)

                where = ''
                if len(whereList) > 0:
                    where = 'WHERE {}'.format(' AND '.join(whereList))
                
                query = 'SELECT {} AS value FROM "{}" {}'.format(
                    node.SELECT.data(), 
                    self.series.data(), 
                    where
                )

                if self.debugging():
                    print(query)

                result = client.query(query, params={'epoch': 'ms'})
                
                for row in result.get_points():
                    node.putRow(1000, MDSplus.Float32(float(row['value'])), MDSplus.Uint64(row['time']))
                
            except MDSplus.TreeNODATA:
                pass
            except Exception as e:
                print(e)

        MDSplus.Event.setevent(self.data_event.data())
    STORE=store
