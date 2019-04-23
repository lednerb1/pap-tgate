from flask import Flask, request, redirect, session, url_for, escape
from flask import render_template as render
from flask import make_response as respond
import simplejson as json

admins = [['peterbrendel', 'papehlegal']]

app = Flask(__name__)
app.secret_key = b'_9%!@L"9Ylg58zCP]0'

def valid_login(u,p):
    for _ in admins:
        if u == _[0] and p == _[1]:
            return True
    return False

@app.route('/')
def index():
    if 'username' in session:
        return render('index.html', logged=escape(session['username']))
    return render('index.html')

@app.route('/login')
def getLoginView():
    if 'username' in session: # if admin already logged in redirect to dashboard
        return redirect(url_for('dashboard'))
    return render('login.html')

@app.route('/attemptLogin', methods=['POST', 'GET'])
def login():
    error = None
    if request.method == 'POST':
        if valid_login(request.form['username'], request.form['password']):
            resp = respond(render('dashboard.html'))
            resp.set_cookie('username', request.form['username'])
            session['username'] = request.form['username']
            return resp
        else:
            error = "Invalid username or password"
    else:
        error = "Not right method LUL"
    return render('login.html', err=error)

@app.route('/dashboard')
def dashboard():
    return render('dashboard.html')

@app.route('/register', methods=['POST', 'GET'])
def register():
    
    return
