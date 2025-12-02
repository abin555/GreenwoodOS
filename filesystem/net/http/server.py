from flask import Flask

app = Flask(__name__)
count = 0

@app.route("/")
def hello_world():
    global count
    mesg = ""
    for i in range(5):
        mesg += "<p>Aidan Boisvert's HTTP Client</p>\n<p>Count #"+ str(count) +"</p>\n"
    count = count + 1
    return mesg

if __name__ == '__main__':

    # run() method of Flask class runs the application 
    # on the local development server.
    app.run(host="0.0.0.0", port=80)
