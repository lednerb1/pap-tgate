# pap-tgate
Python Flask Server for a ticket gate

# Running

In order to run this projet you need to install it dependencies

## Installing dependencies

```bash
pip3 install -r requirements.txt
```

## To finally run the server

```bash
export FLASK_APP=server.py
flask run
```

# Viewing on Browser

[Homepage](http://localhost:5000/)

# Default admin

admin / admin

# Feeding the beast

The python application is responsible for registering.

C++ code goes as followed:
* Define new Catraca and type of person allowed
* Get a list from the server of those allowed people
* Input is a bunch of CPF until EOF
* Every 10 new inputs send data back to server (pipe in entradas.txt)

## TODO

* Add OOP: instead of adding CPF to the array, add a new
  instance of a Person or child classes
* Better CSS

## Refer to

[Flask Quickstart](http://flask.pocoo.org/docs/dev/quickstart/)

## Future

* Evolve this into a real project. Já posso falar pt
* Usar esse modelo pro sistema da maratona. vai ficar top
