from flask import Flask, request, render_template
from flask_sockets import Sockets
import gevent
import time
import json

'''
    Setting up the webserver
'''
app = Flask(__name__)
app.debug = True

'''
    Setting up the websocket
'''
sockets = Sockets(app)

'''
    Multiple clients handling
'''
clients = list()

'''
Client object
'''

class Client:
    def __init__(self):
        self.queue = gevent.queue.Queue()
    def put(self, v):
        self.queue.put_nowait(v)
    def get(self):
        return self.queue.get()
'''
User-defined function for sending data on the websocket to all clients
'''

def send_all(msg):
    for client in clients:
        client.put(msg)

'''
   Reading from websocket
'''

def read_ws(ws, client):
    while not ws.closed:
        gevent.sleep(0)
        try:
            msg = ws.receive()
            print "WS RECEIVED: %s" % msg
            if(msg is None):
                client.put(msg)
            else:
                send_all(msg)
        except:
            print "WS ERROR: read_ws exception"

'''
    Adding a route to the websockets server
    to "subscribe" clients
'''
@sockets.route('/subscribe')
def subscribe_socket(ws):
    client = Client()
    clients.append(client)
    print '# Clients: {}'.format(len(clients))
    g = gevent.spawn( read_ws, ws, client)
    # If data is received by the queue, send it to the websocket
    try:
        while g:
            msg = client.get()
            if msg is not None:
                ws.send(msg)
            else:
                break
    except Exception as e:
        print "WS ERROR: %s" % e
    finally:
        ws.close()
        clients.remove(client)
        gevent.kill(g)

'''
    Adding a route to the webserver
'''
@app.route('/')
def index():
    return render_template("index.html") # rendering index.html in 'templates' dir
