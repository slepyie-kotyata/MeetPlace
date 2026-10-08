package ws

import (
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

//TODO: валидация + ошибка неправильного message
//TODO: функция sendMessage (?)
//TODO: handling client disconnecting
//TODO: leave

var upgrader = websocket.Upgrader{
	ReadBufferSize: 1024,
	WriteBufferSize: 1024,
	CheckOrigin: func(r *http.Request) bool {
		return true
	},
}

func WsHandler(w http.ResponseWriter, r *http.Request) {
	conn, err := upgrader.Upgrade(w, r, nil)
	if err != nil {		
		log.Printf("Upgrade error: %s", err.Error())
		return
	}

	log.Println("client connected")	
}

