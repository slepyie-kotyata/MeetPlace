package ws

import (
	"log"

	"github.com/gorilla/websocket"
)

type Peer struct {
	Id			string
	RoomId		string
	conn		*websocket.Conn
	Outgoing	chan Message
}

func NewPeer(conn *websocket.Conn) *Peer {
	return &Peer{
		conn: conn,
		Outgoing: make(chan Message),
	}
}

// Read: читает и смотрит не умерло ли соединение
// если оно умерло, вызывает функцию Close -> в ней:
// закрывается соединение, удаляется peer из Hub (RemovePeer), шлется уведомление об уходе остальным, закрывается канал
// Write: читает из канала, пока он не закрыт, при ошибке отправки тоже вызывает Close

func (p *Peer) Close() {
	//sync.Once ()
	log.Println("Connection closed")
}

func (p *Peer) Read() {
	defer p.Close()

	var message Message
	for {
		err := p.conn.ReadJSON(&message)
		if err != nil {
			log.Printf("Closing connection, : %s", err.Error())
			break
		}

		
	}
}