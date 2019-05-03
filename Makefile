all:
	@export FLASK_APP=server.py
	@g++ client.cpp -std=c++11 -lcurl -o catraca

server:
	@flask run

client:
	./catraca

clean:
	@rm *.tmp
