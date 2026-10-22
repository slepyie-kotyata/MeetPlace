package ws

import (
	"log"
	"sync"

	"github.com/gorilla/websocket"
)

type Peer struct {
	id			string
	name		string
	roomID		string
	conn		*websocket.Conn
	outgoing	chan Message
	once		sync.Once
}

func NewPeer(peerId, peerName, roomId string, conn *websocket.Conn) *Peer {
	return &Peer{
		id: peerId,
		name: peerName,
		roomID: roomId,
		conn: conn,
		outgoing: make(chan Message, 16),
	}
}

func (p *Peer) Close() {
	p.once.Do(func(){
		p.Broadcast(Message{
			Type: PeerLeft,
			PeerID: p.id,
		})
		hub.Remove(p)
		p.conn.Close()
		close(p.outgoing)
		log.Println("Connection closed")
	})
}

func (p *Peer) Broadcast(message Message) {
	peers, ok := hub.Rooms[p.roomID]
	if !ok {
		return
	}
	
	for id, peer := range peers {
		if id != p.id {
			peer.outgoing <- message
		}
	}
}

func (p *Peer) Write() {
	defer p.Close()
	
	for message := range p.outgoing {
		if err := p.conn.WriteJSON(message); err != nil {
			log.Printf("Closing connection, : %s", err.Error())
			break
		}
	}
}

func (p *Peer) Read() {
	defer p.Close()

	for {
		var message Message
		err := p.conn.ReadJSON(&message)
		if err != nil {
			log.Printf("Closing connection, : %s", err.Error())
			break
		}
		hub.HandleMessages(p, message)
	}
}